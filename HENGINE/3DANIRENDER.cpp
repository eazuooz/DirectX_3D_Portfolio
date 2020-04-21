#include "3DANIRENDER.h"
#include <JTIME.h>
#include "TEX.h"
#include "FBX.h"
#include <FILESTREAM.h>

FbxAnimation::FbxAnimation() : m_ClipIndex(0), m_CurTime(0.0f), m_UpdateTime(0.0f), isLock(false)
{

}


FbxAnimation::~FbxAnimation()
{

}

bool FbxAnimation::ChangeRockMaterialOn()
{
	seirenData = { 1.0f, 1.0f, 1.0f, 0.0f };
	DATA->m_Path->LinkCB(L"PS_SEIRENDATA", &seirenData);
	
	return true;
}
bool FbxAnimation::ChangeRockMaterialOff()
{
	seirenData = { 0.0f, 0.0f, 0.0f, 0.0f };
	DATA->m_Path->LinkCB(L"PS_SEIRENDATA", &seirenData);

	return true;
}

HPTR<FbxAnimation::UserAniClip> FbxAnimation::FindAni(std::wstring _Name) 
{
	std::map<std::wstring, HPTR<UserAniClip>>::iterator FindIter = m_Ani.find(_Name);

	if (FindIter == m_Ani.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

void FbxAnimation::CreateAnimation(std::wstring _Name, int _Start, int _End, bool _Loop/* = false*/)
{
	if (nullptr != FindAni(_Name.c_str()))
	{
		BOOM;
	}

	UserAniClip* NewClip = new UserAniClip();

	NewClip->Name	= _Name;
	NewClip->m_Start = _Start;
	NewClip->m_End = _End;
	NewClip->Loop = _Loop;
	NewClip->m_AniEnd = false;

	NewClip->m_StartTime = (float)NewClip->m_Start / (float)m_FrameCount;
	NewClip->m_EndTime = (float)NewClip->m_End / (float)m_FrameCount;

	m_Ani[_Name.c_str()] = NewClip;
}

void FbxAnimation::CreateAnimation(std::wstring _Name, int _Start, int _End,  float _Speed ,bool _Loop/* = false*/)
{
	if (nullptr != FindAni(_Name.c_str()))
	{
		BOOM;
	}

	UserAniClip* NewClip = new UserAniClip();

	NewClip->Name = _Name;
	NewClip->m_Start = _Start;
	NewClip->m_End = _End;
	NewClip->Loop = _Loop;
	NewClip->m_Speed = _Speed;
	NewClip->m_AniEnd = false;
	

	NewClip->m_StartTime = (float)NewClip->m_Start / (float)m_FrameCount;
	NewClip->m_EndTime = (float)NewClip->m_End / (float)m_FrameCount;

	
	m_Ani[_Name.c_str()] = NewClip;
	
}

void FbxAnimation::ChangeAnimation(std::wstring _Name) 
{
	if (FindAni(_Name) == CurClip)
	{
		return;
	}

	CurClip = FindAni(_Name);
	if (nullptr == CurClip)
	{
		BOOM;
	}

	m_UpdateTime = CurClip->m_StartTime;
}


void FbxAnimation::Update() 
{
	if (nullptr == CurClip)
	{
		BOOM;
	}

	if (CurClip->m_StartTime > m_UpdateTime)
	{
		m_UpdateTime = CurClip->m_StartTime;
	}

	if (CurClip->m_EndTime < m_UpdateTime)
	{
		m_UpdateTime = CurClip->m_EndTime;
	}

	m_CurTime = 0.0f;
	m_UpdateTime += (Time::DeltaTime() * CurClip->m_Speed);
	//m_UpdateTime *= CurClip->m_Speed;


	if ((CurClip->m_EndTime *0.5f) < m_UpdateTime)
	{
		CurClip->m_AniHalf = true;
	}
	else
	{
		CurClip->m_AniHalf = false;
	}

	if ((CurClip->m_EndTime *0.75f) < m_UpdateTime)
	{
		CurClip->m_AniThreeQuarters = true;
	}
	else
	{
		CurClip->m_AniThreeQuarters = false;
	}

	if (CurClip->m_EndTime < m_UpdateTime) //엔드타임이 업데이트 타임보다 작다면
	{
		if (false == CurClip->Loop)
		{
			m_UpdateTime = CurClip->m_EndTime;
			CurClip->m_AniEnd = true;
		}
		else 
		{
			m_UpdateTime = CurClip->m_StartTime;
		}
	}
	else
	{
		CurClip->m_AniEnd = false;
	}


	if (m_UpdateTime >= CurAniData->TimeLength)	
	{
		m_UpdateTime = 0.0f;
	}

	m_CurTime = ((float)CurAniData->StartTime.GetSecondDouble()) + m_UpdateTime;

	// 1434 프레임이 있었다.
	iFrameIndex = (int)(m_CurTime * m_FrameCount);
	iNextFrameIndex = 0;


	if (iFrameIndex >= CurAniData->TimeLength - 1)
	{
		m_UpdateTime = 0.0f;
		iFrameIndex = 0;
	}

	// 렉이 걸렸으면 
	if (isLock == false)
	{
		iNextFrameIndex = iFrameIndex + 1;
	}
	else
	{
		iNextFrameIndex = iFrameIndex;
	}
}

void FbxAnimation::UpdateBefore()
{
	for (size_t i = 0; i < m_FBXDATA->BoneData().size(); ++i)
	{
		if (CurAniData->BoneKeyFrame[i].KeyFrame.empty())
		{
			m_CalAniBoneData[i] = m_FBXDATA->BoneData()[i]->matBone;
			continue;
		}

		Fbx::FbxKeyFrame& CurFrame = CurAniData->BoneKeyFrame[i].KeyFrame[iFrameIndex];
		Fbx::FbxKeyFrame& NextFrame = CurAniData->BoneKeyFrame[i].KeyFrame[iNextFrameIndex];

		float fFrameTime = (float)CurFrame.dTime;
		float fNextFrameTime = (float)NextFrame.dTime;

		// 프레임 사이의 %를 구했다.
		float fPercent = (float)(m_CurTime - fFrameTime) / (1.0f / (float)m_FrameCount);

		DirectX::XMVECTOR vS = DirectX::XMVectorLerp(CurFrame.S, NextFrame.S, fPercent);
		DirectX::XMVECTOR vT = DirectX::XMVectorLerp(CurFrame.T, NextFrame.T, fPercent);

		// Slerp는 이동속성이 없는 상태에서 이기 때문에 더더욱 정규화 시켜서 하는것이 좋다.
		DirectX::XMVECTOR vQ = DirectX::XMQuaternionSlerp(CurFrame.Q, NextFrame.Q, fPercent);

		DirectX::XMVECTOR vZero = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		// Affine행렬은 우리가 알고 있는 월드 행렬.
		// 1. 크기
		// 2. 원점
		// 3. 
		// 3. 


		m_ANIBONEDATA[i].S = vS;
		m_ANIBONEDATA[i].T = vT;
		m_ANIBONEDATA[i].Q = vQ;
		m_OffsetAniBoneData[i] = DirectX::XMMatrixAffineTransformation(vS, Vector4::ZERO, vQ, vT);

		// 스키닝용 행렬이기 때문에 본래의 월드공간의 행렬이 아니므로
		// offset행렬을 곱해서 우리 공간에 맞게 변형 시켜야 한다.
		// 기본 오프셋과 곱해서 
		m_CalAniBoneData[i] = m_FBXDATA->BoneData()[i]->matOffset * m_OffsetAniBoneData[i];
	}

	m_BoneTex->SetPixel(&m_CalAniBoneData[0], sizeof(Matrix) * (UINT)m_CalAniBoneData.size());
}

void FbxAnimation::RenderDataUpdate(const HPTR<Camera>& _CAM, HRENDERDATA* _Data)
{
	if (nullptr != m_BoneTex)
	{	
		m_BoneTex->Update(HSHADER::SHADERTYPE::SHT_VS, 10);
	}
}

void FbxAnimation::Render(const HPTR<Camera>& _CAM) 
{

}

void FbxAnimation::FbxSetting(const wchar_t* _Name)
{
	m_FBXDATA = nullptr;
	m_FBXDATA = Resources::Find<Fbx>(_Name);

	if (nullptr == m_FBXDATA)
	{
		BOOM;
	}

	m_BoneTex = nullptr;
	m_BoneTex = new Texture();
	m_BoneTex->Create((int)m_FBXDATA->BoneData().size() * 4, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DYNAMIC);

	m_ANIBONEDATA.clear();
	m_OffsetAniBoneData.clear();
	m_CalAniBoneData.clear();

	m_ANIBONEDATA.resize(m_FBXDATA->BoneData().size());
	m_OffsetAniBoneData.resize(m_FBXDATA->BoneData().size());
	m_CalAniBoneData.resize(m_FBXDATA->BoneData().size());

	// RenderData()

	// ANILIGHTMESHDPATH

	for (size_t i = 0; i < m_FBXDATA->MeshData().size(); i++)
	{
		for (size_t j = 0; j < m_FBXDATA->MeshData()[i]->vecIndices.size(); j++)
		{
			std::wstring MeshName = _Name;
			MeshName += L'0' + (wchar_t)j;
			DATA = RenderData(MeshName.c_str(), L"ANILIGHTMESHDPATH");

			DATA->m_bShadow = true;
			DATA->m_bShadowAni = true;


			std::wstring DifTexName = IOPath::PathToFileName(m_FBXDATA->MaterialData()[i][j]->DifTex.c_str());
			if (L"" != DifTexName)
			{
				LinkTexture((int)j, L"PS_MAPTEX", DifTexName.c_str());
				LinkTexture((int)j, L"PS_TESTTEX", DifTexName.c_str());
			}

			std::wstring BumpTexName = IOPath::PathToFileName(m_FBXDATA->MaterialData()[i][j]->BumpTex.c_str());
			if (L"" != BumpTexName)
			{
				LinkTexture((int)j, L"PS_BUMPTEX", BumpTexName.c_str());
			}

			LinkSampler((int)j, L"PS_LSMP", L"LSMP");
			LinkSampler((int)j, L"PS_PSMP", L"PSMP");


			seirenData = { 0.0f, 0.0f, 0.0f, 0.0f };
			DATA->m_Path->LinkCB(L"PS_SEIRENDATA", &seirenData);
			//ChangeRockMaterialOff();
		}
	}

	
	CurAniData = m_FBXDATA->AniData()[m_ClipIndex];
	m_FrameCount = m_FBXDATA->FrameCount(m_ClipIndex);
	// hresmgr
}

void FbxAnimation::FbxEffectSetting(const wchar_t* _Name)
{
	m_FBXDATA = nullptr;
	m_FBXDATA = Resources::Find<Fbx>(_Name);

	if (nullptr == m_FBXDATA)
	{
		BOOM;
	}

	m_BoneTex = nullptr;
	m_BoneTex = new Texture();
	m_BoneTex->Create((int)m_FBXDATA->BoneData().size() * 4, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DYNAMIC);

	m_ANIBONEDATA.clear();
	m_OffsetAniBoneData.clear();
	m_CalAniBoneData.clear();

	m_ANIBONEDATA.resize(m_FBXDATA->BoneData().size());
	m_OffsetAniBoneData.resize(m_FBXDATA->BoneData().size());
	m_CalAniBoneData.resize(m_FBXDATA->BoneData().size());

	// RenderData()

	// ANILIGHTMESHDPATH

	for (size_t i = 0; i < m_FBXDATA->MeshData().size(); i++)
	{
		for (size_t j = 0; j < m_FBXDATA->MeshData()[i]->vecIndices.size(); j++)
		{
			std::wstring MeshName = _Name;
			MeshName += L'0' + (wchar_t)j;
			DATA = RenderData(MeshName.c_str(), L"ANILIGHTEFFECTMESHDPATH");

			DATA->m_bShadow = false;
			DATA->m_bShadowAni = false;


			std::wstring DifTexName = IOPath::PathToFileName(m_FBXDATA->MaterialData()[i][j]->DifTex.c_str());
			if (L"" != DifTexName)
			{
				LinkTexture((int)j, L"PS_MAPTEX", DifTexName.c_str());
				
			}

			std::wstring BumpTexName = IOPath::PathToFileName(m_FBXDATA->MaterialData()[i][j]->BumpTex.c_str());
			if (L"" != BumpTexName)
			{
				LinkTexture((int)j, L"PS_BUMPTEX", BumpTexName.c_str());
			}

			LinkSampler((int)j, L"PS_LSMP", L"LSMP");
			LinkSampler((int)j, L"PS_PSMP", L"PSMP");
		}
	}

	CurAniData = m_FBXDATA->AniData()[m_ClipIndex];
	m_FrameCount = m_FBXDATA->FrameCount(m_ClipIndex);
	// hresmgr
}

FbxAnimation::BONEDATA FbxAnimation::FindBoneData(const wchar_t* _Name)
{
	Fbx::FbxBoneData* Ptr = m_FBXDATA->FindBone(_Name);

	BONEDATA Data;

	if (nullptr == Ptr)
	{
		BOOM;
		return Data;
	}

	Data.LT = m_ANIBONEDATA[Ptr->iIndex].T;
	Data.LS = m_ANIBONEDATA[Ptr->iIndex].S;
	Data.LQ = m_ANIBONEDATA[Ptr->iIndex].Q;
	Data.WT = GetTransform()->GetWorldMatrix().MulOne(m_ANIBONEDATA[Ptr->iIndex].T);
	Data.WS = GetTransform()->GetWorldMatrix().MulZero(m_ANIBONEDATA[Ptr->iIndex].S);
	Data.WQ = m_ANIBONEDATA[Ptr->iIndex].Q;
	Data.LMat = m_OffsetAniBoneData[Ptr->iIndex];
	Data.WMat = Data.LMat * GetTransform()->GetWorldMatrix();


	return Data;
}
