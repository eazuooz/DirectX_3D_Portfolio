#include "FBX.h"
#include <FILESTREAM.h>
#include <atlstr.h>
#include <algorithm>
#include "ENGINEMESHDATA.h"
#include "HVBBUFFER.h"
#include "HIBBUFFER.h"
#include "HMESH.h"
#include "TEX.h"

thread_local FbxVector4	v1 = { 1.0, 0.0, 0.0, 0.0 };
thread_local FbxVector4	v2 = { 0.0, 0.0, 1.0, 0.0 };
thread_local FbxVector4	v3 = { 0.0, 1.0, 0.0, 0.0 };
thread_local FbxVector4	v4 = { 0.0, 0.0, 0.0, 1.0 };

thread_local FbxAMatrix	matReflect = FbxAMatrix(v1, v2, v3);

thread_local FbxAMatrix	matCluster;
thread_local FbxAMatrix	matClusterLink;

void Fbx::FbxInit() 
{

}


Fbx::Fbx()
{ 
}


Fbx::~Fbx()
{
	for (auto Ptr : m_MeshData)
	{
		if (nullptr != Ptr)
		{
			delete Ptr;
		}
	}

	for (auto Ptr : m_BoneData)
	{
		if (nullptr != Ptr)
		{
			delete Ptr;
		}
	}

	for (auto Arr : m_MaterialData)
	{
		for (auto Ptr : Arr)
		{
			delete Ptr;
		}
	}

	for (auto Ptr : m_AniData)
	{
		if (nullptr != Ptr)
		{
			delete Ptr;
		}
	}

	Release();
}

Matrix Fbx::FbxMatToHMat(const FbxAMatrix& _Mat)
{
	// Y축과 Z축이 뒤집혀져있나 달라
	Matrix RMat;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			RMat.Arr2D[y][x] = (float)_Mat.Get(y, x);
		}
	}

	return RMat;
}

void Fbx::ConvertDataLoad() 
{
	FileStream_RB RStream = FileStream_RB(m_File.GetFullPath());




	//std::vector<HVEC4>	vecPos;
	//std::vector<HVEC4>	vecNormal;
	//std::vector<HVEC4>	vecUV;
	//std::vector<HVEC4>	vecTangent;
	//std::vector<HVEC4>	vecBinormal;
	//std::vector<HVEC4>	vecBlendWeight; // 가중치 // 0이 곱해져서 안나온다.
	//std::vector<HVEC4>	vecBlendIndex;
	//std::vector<std::vector<UINT>>	vecIndices;
	//std::unordered_map<int, std::vector<FbxWeight>>	mapWeights; // 애는 어차피 정렬되서vecBlendWeight에 들어가므로 상관이 없다.
	//bool				bBump;
	//bool				bAnimation;

	int Size;
	RStream >> Size;
	m_MeshData.resize(Size);

	Vector4 ReadVec;

	for (size_t i = 0; i < Size; i++)
	{
		m_MeshData[i] = new FbxMeshData();
		FbxMeshData* pFbxMeshData = m_MeshData[i];

		int VtxSize;
		RStream >> VtxSize;
		pFbxMeshData->vecPos.resize(VtxSize);
		if (0 != VtxSize)
		{
			for (size_t j = 0; j < VtxSize; j++)
			{
				RStream >> ReadVec;
				pFbxMeshData->vecPos[j] = ReadVec;
			}
		}

		RStream >> VtxSize;
		pFbxMeshData->vecNormal.resize(VtxSize);
		if (0 != VtxSize)
		{
			for (size_t j = 0; j < VtxSize; j++)
			{
				RStream >> ReadVec;
				pFbxMeshData->vecNormal[j] = ReadVec;
			}
		}

		RStream >> VtxSize;
		pFbxMeshData->vecUV.resize(VtxSize);
		if (0 != VtxSize)
		{
			for (size_t j = 0; j < VtxSize; j++)
			{
				RStream >> ReadVec;
				pFbxMeshData->vecUV[j] = ReadVec;
			}
		}

		RStream >> VtxSize;
		pFbxMeshData->vecTangent.resize(VtxSize);
		if (0 != VtxSize)
		{
			for (size_t j = 0; j < VtxSize; j++)
			{
				RStream >> ReadVec;
				pFbxMeshData->vecTangent[j] = ReadVec;
			}
		}

		RStream >> VtxSize;
		pFbxMeshData->vecBinormal.resize(VtxSize);
		if (0 != VtxSize)
		{
			for (size_t j = 0; j < VtxSize; j++)
			{
				RStream >> ReadVec;
				pFbxMeshData->vecBinormal[j] = ReadVec;
			}
		}

		RStream >> VtxSize;
		pFbxMeshData->vecBlendWeight.resize(VtxSize);
		if (0 != VtxSize)
		{
			for (size_t j = 0; j < VtxSize; j++)
			{
				RStream >> ReadVec;
				pFbxMeshData->vecBlendWeight[j] = ReadVec;
			}
		}

		RStream >> VtxSize;
		pFbxMeshData->vecBlendIndex.resize(VtxSize);
		if (0 != VtxSize)
		{
			for (size_t j = 0; j < VtxSize; j++)
			{
				RStream >> ReadVec;
				pFbxMeshData->vecBlendIndex[j] = ReadVec;
			}
		}

		RStream >> VtxSize;
		pFbxMeshData->vecIndices.resize(VtxSize);
		for (size_t j = 0; j < VtxSize; j++)
		{
			// pFbxMeshData->vecBlendIndex.pop_back(std::vector<UINT>());

			int IdxCount;

			UINT IDX;
			RStream >> IdxCount;
			pFbxMeshData->vecIndices[j].resize(IdxCount);
			for (size_t k = 0; k < IdxCount; k++)
			{
				RStream >> IDX;
				pFbxMeshData->vecIndices[j][k] = IDX;
			}
		}

		RStream >> pFbxMeshData->bBump;
		RStream >> pFbxMeshData->bAnimation;
	}


	//FbxBoneData*		m_pParent;
	//std::wstring	Name;
	//int				iDepth;
	//int				iIndex;
	//int				iParentIndex;
	//FbxAMatrix		matOffset;
	//FbxAMatrix		matBone;

	RStream >> Size;
	m_BoneData.resize(Size);
	for (FbxBoneData*& RBoneData : m_BoneData)
	{
		RBoneData = new FbxBoneData();
		RStream >> RBoneData->Name;
		RStream >> RBoneData->iDepth;
		RStream >> RBoneData->iIndex;
		RStream >> RBoneData->iParentIndex;
		RStream >> RBoneData->matOffset;
		RStream >> RBoneData->matBone;
		// RStream.Read(&RBoneData.iDepth, sizeof(FbxBoneData) - sizeof(std::wstring));
	}

	//HVEC4				Dif;
	//HVEC4				Amb;
	//HVEC4				Spc;
	//HVEC4				Emv;
	//float				SpecularPower;
	//float				TransparencyFactor;
	//float				Shininess;
	//std::wstring		DifTex;
	//std::wstring		BumpTex;
	//std::wstring		SpcTex;

	RStream >> Size;
	m_MaterialData.resize(Size);
	for (std::vector<FbxMaterialData*>& RMtrlData : m_MaterialData)
	{
		int MtrlCount;
		RStream >> MtrlCount;
		RMtrlData.resize(MtrlCount);
		for (FbxMaterialData*& Mtrl : RMtrlData)
		{
			Mtrl = new FbxMaterialData();
			RStream >> Mtrl->Dif;
			RStream >> Mtrl->Amb;
			RStream >> Mtrl->Spc;
			RStream >> Mtrl->Emv;
			RStream >> Mtrl->SpecularPower;
			RStream >> Mtrl->TransparencyFactor;
			RStream >> Mtrl->Shininess;
			// RStream.Read(&Mtrl->Dif, sizeof(FbxMaterialData) - sizeof(std::wstring) * 3);
			RStream >> Mtrl->DifTex;
			RStream >> Mtrl->BumpTex;
			RStream >> Mtrl->SpcTex;
		}
	}


	//std::wstring Name;
	//FbxTime			StartTime; // 애니메이션 시작시간
	//FbxTime			EndTime; // 애니메이션 
	//FbxLongLong		TimeStartCount; // 그중에서 시작프레임
	//FbxLongLong		TimeEndCount; // 그중에서 끝나는 프레임
	//FbxLongLong		TimeLength; // 그 사이의 길이
	//FbxTime::EMode	TimeMode;
	//std::vector<FbxBoneKeyFrame>	BoneKeyFrame;

	//class FbxBoneKeyFrame
	//{
	//public:
	//	int		iBoneIndex;
	//	std::vector<FbxKeyFrame>	KeyFrame;
	//};

	//class FbxKeyFrame
	//{
	//public:
	//	HMAT		matTransform;
	//	double		dTime;
	//};


	RStream >> Size;
	m_AniData.resize(Size);
	for (FbxAniData*& RAniData : m_AniData)
	{
		RAniData = new FbxAniData();
		RStream >> RAniData->Name;
		// RStream.Read(&RAniData->StartTime, sizeof(FbxAniData) - sizeof(std::wstring) - sizeof(std::vector<FbxBoneKeyFrame>));
		RStream >> RAniData->StartTime; // 애니메이션 시작시간
		RStream >> RAniData->EndTime; // 애니메이션 
		RStream >> RAniData->TimeStartCount; // 그중에서 시작프레임
		RStream >> RAniData->TimeEndCount; // 그중에서 끝나는 프레임
		RStream >> RAniData->TimeLength; // 그 사이의 길이
		RStream >> RAniData->TimeMode;

		int BKFCount;
		RStream >> BKFCount;
		RAniData->BoneKeyFrame.resize(BKFCount);
		for (FbxBoneKeyFrame& RBKF : RAniData->BoneKeyFrame)
		{
			int KFCount;
			RStream >> RBKF.iBoneIndex;
			RStream >> KFCount;
			RBKF.KeyFrame.resize(KFCount);
			for (size_t j = 0; j < KFCount; j++)
			{
				RStream >> RBKF.KeyFrame[j];
			}

				// RStream.Read(&RBKF.KeyFrame[0], sizeof(FbxKeyFrame) * RBKF.KeyFrame.size());
		}
	}

	for (size_t i = 0; i < m_BoneData.size(); i++)
	{
		m_BoneDataToName.insert(std::multimap<std::wstring, FbxBoneData*>::value_type(m_BoneData[i]->Name, m_BoneData[i]));
	}


	m_MeshData;
	m_BoneData;
	m_MaterialData; // 서브셋 개수만큼 있을 것이다.
	m_AniData;

}

void Fbx::FbxToEngineRes() 
{
	for (size_t i = 0; i < m_MeshData.size(); i++)
	{
		std::vector<EngineVertex::VertexFBX> ArrVtx;
		ArrVtx.resize(m_MeshData[i]->vecPos.size());


		for (size_t j = 0; j < m_MeshData[i]->vecPos.size(); j++)
		{
			ArrVtx[j].Pos = m_MeshData[i]->vecPos[j];
			ArrVtx[j].UV = m_MeshData[i]->vecUV[j];
			ArrVtx[j].Color = Vector4::WHITE;
			ArrVtx[j].Normal = m_MeshData[i]->vecNormal[j];
			ArrVtx[j].Tangent = m_MeshData[i]->vecTangent[j];
			ArrVtx[j].Binormal = m_MeshData[i]->vecBinormal[j];

			if (m_MeshData[i]->vecBlendWeight.size() != 0)
			{
				ArrVtx[j].BlendWeight = m_MeshData[i]->vecBlendWeight[j];
			}
			if (m_MeshData[i]->vecBlendIndex.size() != 0)
			{
				ArrVtx[j].BlendIndex = m_MeshData[i]->vecBlendIndex[j];
			}		
			
		}

		Resources::Create<HVBBUFFER>(m_File.GetFileName(), ArrVtx, D3D11_USAGE_DYNAMIC);
	}

	for (size_t i = 0; i < m_MeshData.size(); i++)
	{
		for (size_t j = 0; j < m_MeshData[i]->vecIndices.size(); j++)
		{
			std::wstring IdxName = m_File.GetFileName();

			wchar_t Number = L'0';

			IdxName += (wchar_t)(Number + j);

			Resources::Create<HIBBUFFER>(IdxName.c_str(), m_MeshData[i]->vecIndices[j], D3D11_USAGE_DYNAMIC);

			HPTR<HMESH> NEWMESH = Resources::Create<HMESH>(IdxName.c_str());
			NEWMESH->AddVB(m_File.GetFileName().c_str());
			NEWMESH->AddIB(IdxName.c_str());
		}
	}
	

	for (size_t i = 0; i < m_MaterialData.size(); i++)
	{
		for (size_t j = 0; j < m_MaterialData[i].size(); j++)
		{
			Directory curDir;
			std::wstring  curDirStr = curDir.GetFullPath();
#pragma region POPBACK
			size_t prevCnt = curDirStr.size();
			size_t endCnt = curDirStr.find_last_of(L"\\");

			for (size_t i = 0; i < prevCnt - endCnt; i++)
			{
				curDirStr.pop_back();
			}
			curDirStr += L"\\BIN\\FBX\\FbxTexture";
#pragma endregion

			std::wstring  materialPathDatas[3] = { m_MaterialData[i][j]->DifTex , 
													m_MaterialData[i][j]->BumpTex ,m_MaterialData[i][j]->SpcTex };
			
			std::wstring  FIleNames[3];			
			std::wstring  copyFIleNames[3];// = { FIleNames[0], FIleNames[1],FIleNames[2] };
			for (size_t i = 0; i < 3; i++)
			{
				if (materialPathDatas[i] == L"")
				{
					continue;
				}

				size_t posFileName = materialPathDatas[i].find_last_of(L"\\");
				FIleNames[i] = materialPathDatas[i].substr(posFileName, materialPathDatas[i].size());

				copyFIleNames[i] = materialPathDatas[i].substr(posFileName+1, materialPathDatas[i].size());

				materialPathDatas[i].clear();
				materialPathDatas[i] = curDirStr + FIleNames[i];
			}
			
					


			//0 Dif 1BUmp 2Spec
			std::wstring emptyTxt = L"";
			if (emptyTxt != materialPathDatas[0])
			{
				if (nullptr == Resources::Find<Texture>(copyFIleNames[0]))
				{
					Resources::Load<Texture>(materialPathDatas[0]);
				}							
			}

			if (emptyTxt != materialPathDatas[1])
			{
				if (nullptr == Resources::Find<Texture>(copyFIleNames[1]))
				{
					Resources::Load<Texture>(materialPathDatas[1]);
				}
			}

			if (emptyTxt != materialPathDatas[2])
			{
				if (nullptr == Resources::Find<Texture>(copyFIleNames[2]))
				{
					Resources::Load<Texture>(materialPathDatas[2]);
				}
			}
		}
	}

}

void Fbx::FbxToEngineRes(const wchar_t* _BumpName)
{
	for (size_t i = 0; i < m_MeshData.size(); i++)
	{
		std::vector<EngineVertex::VertexFBX> ArrVtx;
		ArrVtx.resize(m_MeshData[i]->vecPos.size());


		for (size_t j = 0; j < m_MeshData[i]->vecPos.size(); j++)
		{
			ArrVtx[j].Pos = m_MeshData[i]->vecPos[j];
			ArrVtx[j].UV = m_MeshData[i]->vecUV[j];
			ArrVtx[j].Color = Vector4::WHITE;
			ArrVtx[j].Normal = m_MeshData[i]->vecNormal[j];
			ArrVtx[j].Tangent = m_MeshData[i]->vecTangent[j];
			ArrVtx[j].Binormal = m_MeshData[i]->vecBinormal[j];

			if (m_MeshData[i]->vecBlendWeight.size() != 0)
			{
				ArrVtx[j].BlendWeight = m_MeshData[i]->vecBlendWeight[j];
			}
			if (m_MeshData[i]->vecBlendIndex.size() != 0)
			{
				ArrVtx[j].BlendIndex = m_MeshData[i]->vecBlendIndex[j];
			}

		}

		Resources::Create<HVBBUFFER>(m_File.GetFileName(), ArrVtx, D3D11_USAGE_DYNAMIC);
	}

	for (size_t i = 0; i < m_MeshData.size(); i++)
	{
		for (size_t j = 0; j < m_MeshData[i]->vecIndices.size(); j++)
		{
			std::wstring IdxName = m_File.GetFileName();

			wchar_t Number = L'0';

			IdxName += (wchar_t)(Number + j);

			Resources::Create<HIBBUFFER>(IdxName.c_str(), m_MeshData[i]->vecIndices[j], D3D11_USAGE_DYNAMIC);

			HPTR<HMESH> NEWMESH = Resources::Create<HMESH>(IdxName.c_str());
			NEWMESH->AddVB(m_File.GetFileName().c_str());
			NEWMESH->AddIB(IdxName.c_str());
		}
	}


	for (size_t i = 0; i < m_MaterialData.size(); i++)
	{
		for (size_t j = 0; j < m_MaterialData[i].size(); j++)
		{
			Directory curDir;
			std::wstring  curDirStr = curDir.GetFullPath();
#pragma region POPBACK
			size_t prevCnt = curDirStr.size();
			size_t endCnt = curDirStr.find_last_of(L"\\");

			for (size_t i = 0; i < prevCnt - endCnt; i++)
			{
				curDirStr.pop_back();
			}
			curDirStr += L"\\BIN\\FBX\\FbxTexture";
#pragma endregion

		

			std::wstring  materialPathDatas[3] = { m_MaterialData[i][j]->DifTex ,
													m_MaterialData[i][j]->BumpTex ,m_MaterialData[i][j]->SpcTex };

			std::wstring  FIleNames[3];
			for (size_t i = 0; i < 3; i++)
			{
				if (materialPathDatas[i] == L"")
				{
					continue;
				}

				size_t posFileName = materialPathDatas[i].find_last_of(L"\\");
				FIleNames[i] = materialPathDatas[i].substr(posFileName, materialPathDatas[i].size());

				materialPathDatas[i].clear();
				materialPathDatas[i] = curDirStr + FIleNames[i];
			}
			materialPathDatas[1] = curDirStr + L"\\" +_BumpName;
			


			//0 Dif 1BUmp 2Spec
			std::wstring emptyTxt = L"";
			if (emptyTxt != materialPathDatas[0])
			{
				if (nullptr == Resources::Find<Texture>(materialPathDatas[0]))
				{
					Resources::Load<Texture>(materialPathDatas[0]);
				}
			}
			if (emptyTxt != materialPathDatas[1])
			{
				if (nullptr == Resources::Find<Texture>(_BumpName))
				{
					Resources::Load<Texture>(materialPathDatas[1]);
				}
			}
			if (emptyTxt != materialPathDatas[2])
			{
				if (nullptr == Resources::Find<Texture>(materialPathDatas[2]))
				{
					Resources::Load<Texture>(materialPathDatas[2]);
				}
			}
		}
	}
}

void Fbx::ConvertToSave(const wchar_t* _FileName)
{
	FileStream_WB WStream = FileStream_WB(_FileName);

	m_MeshData;
	m_BoneData;
	m_MaterialData; // 서브셋 개수만큼 있을 것이다.
	m_AniData;


	//std::vector<HVEC4>	vecPos;
	//std::vector<HVEC4>	vecNormal;
	//std::vector<HVEC4>	vecUV;
	//std::vector<HVEC4>	vecTangent;
	//std::vector<HVEC4>	vecBinormal;
	//std::vector<HVEC4>	vecBlendWeight; // 가중치 // 0이 곱해져서 안나온다.
	//std::vector<HVEC4>	vecBlendIndex;
	//std::vector<std::vector<UINT>>	vecIndices;
	//std::unordered_map<int, std::vector<FbxWeight>>	mapWeights; // 애는 어차피 정렬되서vecBlendWeight에 들어가므로 상관이 없다.
	//bool				bBump;
	//bool				bAnimation;

	WStream << (int)m_MeshData.size();
	for (size_t i = 0; i < m_MeshData.size(); i++)
	{
		WStream << (int)m_MeshData[i]->vecPos.size();
		if (0 != m_MeshData[i]->vecPos.size())
		{
			for (size_t j = 0; j < m_MeshData[i]->vecPos.size(); j++)
			{
				WStream << m_MeshData[i]->vecPos[j];
			}
		}
		WStream << (int)m_MeshData[i]->vecNormal.size();
		if (0 != m_MeshData[i]->vecNormal.size())
		{
			for (size_t j = 0; j < m_MeshData[i]->vecNormal.size(); j++)
			{
				WStream << m_MeshData[i]->vecNormal[j];
			}
		}
		WStream << (int)m_MeshData[i]->vecUV.size();
		if (0 != m_MeshData[i]->vecUV.size())
		{
			for (size_t j = 0; j < m_MeshData[i]->vecUV.size(); j++)
			{
				WStream << m_MeshData[i]->vecUV[j];
			}
		}
		WStream << (int)m_MeshData[i]->vecTangent.size();
		if (0 != m_MeshData[i]->vecTangent.size())
		{
			for (size_t j = 0; j < m_MeshData[i]->vecTangent.size(); j++)
			{
				WStream << m_MeshData[i]->vecTangent[j];
			}
		}
		WStream << (int)m_MeshData[i]->vecBinormal.size();
		if (0 != m_MeshData[i]->vecBinormal.size())
		{
			for (size_t j = 0; j < m_MeshData[i]->vecBinormal.size(); j++)
			{
				WStream << m_MeshData[i]->vecBinormal[j];
			}
		}
		WStream << (int)m_MeshData[i]->vecBlendWeight.size();
		if (0 != m_MeshData[i]->vecBlendWeight.size())
		{
			for (size_t j = 0; j < m_MeshData[i]->vecBlendWeight.size(); j++)
			{
				WStream << m_MeshData[i]->vecBlendWeight[j];
			}
		}
		WStream << (int)m_MeshData[i]->vecBlendIndex.size();
		if (0 != m_MeshData[i]->vecBlendIndex.size())
		{
			for (size_t j = 0; j < m_MeshData[i]->vecBlendIndex.size(); j++)
			{
				WStream << m_MeshData[i]->vecBlendIndex[j];
			}
		}

		WStream << (int)m_MeshData[i]->vecIndices.size();
		for (std::vector<UINT>& IdxArr : m_MeshData[i]->vecIndices)
		{
			WStream << (int)IdxArr.size();
			for (size_t j = 0; j < IdxArr.size(); j++)
			{
				WStream << IdxArr[j];
			}
		}

		WStream << m_MeshData[i]->bBump;
		WStream << m_MeshData[i]->bAnimation;
	}


	//std::wstring	Name;
	//int				iDepth;
	//int				iIndex;
	//int				iParentIndex;
	//FbxAMatrix		matOffset;
	//FbxAMatrix		matBone;

	WStream << (int)m_BoneData.size();
	for (FbxBoneData* RBoneData : m_BoneData)
	{
		WStream << RBoneData->Name;
		WStream << RBoneData->iDepth;
		WStream << RBoneData->iIndex;
		WStream << RBoneData->iParentIndex;
		WStream << RBoneData->matOffset;
		WStream << RBoneData->matBone;
		// WStream.Write(&RBoneData.iDepth, sizeof(FbxBoneData) - sizeof(std::wstring));
	}

	//HVEC4				Dif;
	//HVEC4				Amb;
	//HVEC4				Spc;
	//HVEC4				Emv;
	//float				SpecularPower;
	//float				TransparencyFactor;
	//float				Shininess;
	//std::wstring		DifTex;
	//std::wstring		BumpTex;
	//std::wstring		SpcTex;

	WStream << (int)m_MaterialData.size();
	for (std::vector<FbxMaterialData*>& RMtrlData : m_MaterialData)
	{
		WStream << (int)RMtrlData.size();
		for (FbxMaterialData*& Mtrl : RMtrlData)
		{
			WStream << Mtrl->Dif;
			WStream << Mtrl->Amb;
			WStream << Mtrl->Spc;
			WStream << Mtrl->Emv;
			WStream << Mtrl->SpecularPower;
			WStream << Mtrl->TransparencyFactor;
			WStream << Mtrl->Shininess;
			// WStream.Write(&Mtrl->Dif, sizeof(FbxMaterialData) - sizeof(std::wstring) * 3);
			WStream << Mtrl->DifTex;
			WStream << Mtrl->BumpTex;
			WStream << Mtrl->SpcTex;
		}
	}


	//std::wstring Name;
	//FbxTime StartTime; // 애니메이션 시작시간
	//FbxTime EndTime; // 애니메이션 
	//FbxLongLong TimeStartCount; // 그중에서 시작프레임
	//FbxLongLong TimeEndCount; // 그중에서 끝나는 프레임
	//FbxLongLong TimeLength; // 그 사이의 길이
	//FbxTime::EMode TimeMode;
	//std::vector<FbxBoneKeyFrame>	BoneKeyFrame;

	//class FbxBoneKeyFrame
	//{
	//public:
	//	int		iBoneIndex;
	//	std::vector<FbxKeyFrame>	KeyFrame;
	//};

	//class FbxKeyFrame
	//{
	//public:
	//	HMAT		matTransform;
	//	double		dTime;
	//};

	WStream << (int)m_AniData.size();
	for (FbxAniData*& RAniData : m_AniData)
	{
		WStream << RAniData->Name;
		WStream << RAniData->StartTime;
		WStream << RAniData->EndTime;
		WStream << RAniData->TimeStartCount;
		WStream << RAniData->TimeEndCount;
		WStream << RAniData->TimeLength;
		WStream << RAniData->TimeMode;
		// WStream.Write(&RAniData->StartTime, sizeof(FbxAniData) - sizeof(std::wstring) - sizeof(std::vector<FbxBoneKeyFrame>));

		WStream << (int)RAniData->BoneKeyFrame.size();
		for (FbxBoneKeyFrame& RBKF : RAniData->BoneKeyFrame)
		{
			WStream << RBKF.iBoneIndex;
			WStream << (int)RBKF.KeyFrame.size();
			if (0 != RBKF.KeyFrame.size())
			{
				for (size_t i = 0; i < RBKF.KeyFrame.size(); i++)
				{
					WStream << RBKF.KeyFrame[i];
				}
			}
		}
	}

}



bool Fbx::Load(LOADMODE _Mode)
{
	if (_Mode == LOADMODE::CONVERT)
	{
		ConvertDataLoad();
		FbxToEngineRes();
		return true;
	}

	if (false == m_File.CheckExtension(L".FBX") && false == m_File.CheckExtension(L".Fbx"))
	{
		BOOM;
	}

	// Fbx로 로드했다고 하더라도
	// 우리 프로그램에서 보일려면
	// 결국 Mesh
	// 본은 그냥 행렬이다. 자신의 부모를 알고 있는 행렬. 

	// fbx파일에 대한 전체 관리를 맡는 클래스
	m_Mgr = fbxsdk::FbxManager::Create();


	// 바이너리를 읽어들일때의 세팅
	m_Ios = fbxsdk::FbxIOSettings::Create(m_Mgr, IOSROOT);
	m_Mgr->SetIOSettings(m_Ios);
	m_Scene = fbxsdk::FbxScene::Create(m_Mgr, "");

	// 애네들 애네들 하나가 어떤 역할 하는지에 집중하지 말고.
	// 함수에 집중해라.
	m_Imp = fbxsdk::FbxImporter::Create(m_Mgr, "");

	if (false == m_Imp->Initialize(CW2A(m_File.GetFullPath(), CP_UTF8).m_psz,  -1, m_Mgr->GetIOSettings()))
	{
		// UI_Printf("Call to KFbxImporter::Initialize() failed.");
		// UI_Printf("Error returned: %s", m_Imp->GetLastErrorString());

		// getlasterr
		BOOM;
		return false;
	}

	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	// m_Scene은 사실 3d에서 구성한 환경을 의미한다.
	m_Imp->Import(m_Scene);
	// 헤더를 읽어들이는 애다.

	eLocalSpace = m_Scene->GetGlobalSettings().GetAxisSystem();

	if (true == (eLocalSpace != fbxsdk::FbxAxisSystem::eMax))
	{
		m_Scene->GetGlobalSettings().SetAxisSystem(fbxsdk::FbxAxisSystem::eMax);
	}
	// 하나하나 들어있는 의미들이 그때마다 스위치 문을 사용하기 너무 귀찮으니까.

	// 애니메이션이 있는지 체크해 봅시다.
	AniCheck();
	LoadBone(m_Scene->GetRootNode()); // 모든 본을 알아낸다.

	for (size_t i = 0; i < m_BoneData.size(); i++)
	{
		m_BoneDataToName.insert(std::multimap<std::wstring, FbxBoneData*>::value_type(m_BoneData[i]->Name, m_BoneData[i]));
	}

	for (size_t i = 0; i < m_AniData.size(); i++)
	{
		m_AniData[i]->BoneKeyFrame.resize(m_BoneData.size());
	}

	Trangulate(m_Scene->GetRootNode());

	LoadMesh(m_Scene->GetRootNode());

	Release();
	return true;
}

bool Fbx::Load(LOADMODE _Mode, const wchar_t* _BumpName)
{
	if (_Mode == LOADMODE::CONVERT)
	{
		ConvertDataLoad();
		if (_BumpName != nullptr)
		{
			FbxToEngineRes(_BumpName);
		}
		else
		{
			FbxToEngineRes();
		}
		
		return true;
	}

	if (false == m_File.CheckExtension(L".FBX") && false == m_File.CheckExtension(L".Fbx"))
	{
		BOOM;
	}

	// Fbx로 로드했다고 하더라도
	// 우리 프로그램에서 보일려면
	// 결국 Mesh
	// 본은 그냥 행렬이다. 자신의 부모를 알고 있는 행렬. 

	// fbx파일에 대한 전체 관리를 맡는 클래스
	m_Mgr = fbxsdk::FbxManager::Create();


	// 바이너리를 읽어들일때의 세팅
	m_Ios = fbxsdk::FbxIOSettings::Create(m_Mgr, IOSROOT);
	m_Mgr->SetIOSettings(m_Ios);
	m_Scene = fbxsdk::FbxScene::Create(m_Mgr, "");

	// 애네들 애네들 하나가 어떤 역할 하는지에 집중하지 말고.
	// 함수에 집중해라.
	m_Imp = fbxsdk::FbxImporter::Create(m_Mgr, "");

	if (false == m_Imp->Initialize(CW2A(m_File.GetFullPath(), CP_UTF8).m_psz, -1, m_Mgr->GetIOSettings()))
	{
		BOOM;
		return false;
	}

	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	// m_Scene은 사실 3d에서 구성한 환경을 의미한다.
	m_Imp->Import(m_Scene);
	// 헤더를 읽어들이는 애다.

	eLocalSpace = m_Scene->GetGlobalSettings().GetAxisSystem();

	if (true == (eLocalSpace != fbxsdk::FbxAxisSystem::eMax))
	{
		m_Scene->GetGlobalSettings().SetAxisSystem(fbxsdk::FbxAxisSystem::eMax);
	}
	// 하나하나 들어있는 의미들이 그때마다 스위치 문을 사용하기 너무 귀찮으니까.

	// 애니메이션이 있는지 체크해 봅시다.
	AniCheck();
	LoadBone(m_Scene->GetRootNode()); // 모든 본을 알아낸다.

	for (size_t i = 0; i < m_BoneData.size(); i++)
	{
		m_BoneDataToName.insert(std::multimap<std::wstring, FbxBoneData*>::value_type(m_BoneData[i]->Name, m_BoneData[i]));
	}

	for (size_t i = 0; i < m_AniData.size(); i++)
	{
		m_AniData[i]->BoneKeyFrame.resize(m_BoneData.size());
	}

	Trangulate(m_Scene->GetRootNode());

	LoadMesh(m_Scene->GetRootNode());

	Release();
	return true;
}

Vector4 Fbx::FbxVecToHVec(const FbxVector4& _Vec) 
{
	Vector4 ReturnVector;

	for (size_t i = 0; i < 4; i++)
	{
		ReturnVector.Arr[i] = (float)_Vec.mData[i];
	}

	return ReturnVector;
}

Vector4 Fbx::FbxQToHVec4(const FbxQuaternion& _Q)
{
	Vector4 ReturnVector;

	for (size_t i = 0; i < 4; i++)
	{
		ReturnVector.Arr[i] = (float)_Q.mData[i];
	}

	return ReturnVector;
}

void Fbx::LoadTimeTransform(FbxNode * pNode, FbxCluster * pCluster, const FbxAMatrix& matTransform, int iBoneIndex) 
{
	for (size_t i = 0; i < m_AniData.size(); ++i)
	{
		FbxLongLong	Start = m_AniData[i]->TimeStartCount;
		FbxLongLong	End = m_AniData[i]->TimeEndCount;

		m_AniData[i]->BoneKeyFrame[iBoneIndex].iBoneIndex = iBoneIndex;
		// 전체 프레임 수만큼 반복한다.
		for (FbxLongLong j = Start; j <= End; ++j)
		{
			FbxTime	tTime = {};

			// 현재 프레임에 해당하는 FbxTime을 만들어낸다.
			tTime.SetFrame(j, m_AniData[i]->TimeMode);

			// EvaluateGlobalTransform
			FbxAMatrix	matOffset = pNode->EvaluateGlobalTransform(tTime) * matTransform;
			FbxAMatrix	matCur = matOffset.Inverse() * pCluster->GetLink()->EvaluateGlobalTransform(tTime);

			matCur = matReflect * matCur * matReflect;

			FbxKeyFrame	tKeyFrame = {};
			tKeyFrame.dTime = tTime.GetSecondDouble();
			tKeyFrame.matTransform = FbxMatToHMat(matCur);

			tKeyFrame.S = FbxVecToHVec(matCur.GetS());
			tKeyFrame.T = FbxVecToHVec(matCur.GetT());
			tKeyFrame.Q = FbxQToHVec4(matCur.GetQ());

			m_AniData[i]->BoneKeyFrame[iBoneIndex].KeyFrame.push_back(tKeyFrame);
		}
	}
}

void Fbx::LoadWeightAndIndex(FbxCluster * pCluster, int iBoneIndex, FbxMeshData* pContainer) {

	int	iControlIndicesCount = pCluster->GetControlPointIndicesCount();

	for (int i = 0; i < iControlIndicesCount; ++i)
	{
		FbxWeight	tWeight = {};

		tWeight.iIndex = iBoneIndex;
		tWeight.dWeight = pCluster->GetControlPointWeights()[i];

		int	iClusterIndex = pCluster->GetControlPointIndices()[i];

		// map의 특징 : 키를 이용해 인덱스처럼 접근할 경우 해당 키가 없다면
		// 만들어준다.
		pContainer->mapWeights[iClusterIndex].push_back(tWeight);
	}

}





void Fbx::LoadOffsetMatrix(FbxCluster* _Cluster
	, const FbxAMatrix & _matTransform
	, int _iBoneIndex, Fbx::FbxMeshData* _Container)
{

	_Cluster->GetTransformMatrix(matCluster);
	_Cluster->GetTransformLinkMatrix(matClusterLink);


	/*
	1 0 0 0   1 2 3 4   1 0 0 0
	0 0 1 0   5 6 7 8   0 0 1 0
	0 1 0 0 * 9 0 1 2 * 0 1 0 0
	0 0 0 1   3 4 5 6   0 0 0 1

	1 2 3 4   1 0 0 0
	9 0 1 2   0 0 1 0
	5 6 7 8 * 0 1 0 0
	3 4 5 6   0 0 0 1

	1 3 2 4
	9 1 0 2
	5 7 6 8
	3 5 4 6
	*/

	// 어떤 포지션이 뷰행렬이 곱해졌다.
	// 
	FbxAMatrix	matOffset;
	matOffset = matClusterLink.Inverse() * matCluster * _matTransform;
	matOffset = matReflect * matOffset * matReflect;

	m_BoneData[_iBoneIndex]->matOffset = FbxMatToHMat(matOffset);
}


void Fbx::ChangeWeightAndIndices(FbxMeshData* _Container)
{
	// 무차별로 집어넣어 놨다.
	std::unordered_map<int, std::vector<FbxWeight>>::iterator	iter;
	std::unordered_map<int, std::vector<FbxWeight>>::iterator	iterEnd = _Container->mapWeights.end();

	for (iter = _Container->mapWeights.begin(); iter != iterEnd; ++iter)
	{
		if (iter->second.size() > 4)
		{
			// [](const FbxWeight& lhs, const FbxWeight& rhs) {	return lhs.dWeight > rhs.dWeight; }

			// 가중치 값에 따라 내림차순 정렬한다.
			std::sort(iter->second.begin(), iter->second.end(), [](const FbxWeight& lhs, const FbxWeight& rhs)
				{
					return lhs.dWeight > rhs.dWeight;
				});

			double	dSum = 0.0;

			for (int i = 0; i < 4; ++i)
			{
				dSum += iter->second[i].dWeight;
			}

			double	dInterpolate = 1.f - dSum;

			std::vector<FbxWeight>::iterator	iterErase = iter->second.begin() + 4;

			iter->second.erase(iterErase, iter->second.end());
			iter->second[0].dWeight += dInterpolate;
		}

		float	fWeight[4] = {};
		int		iIndex[4] = {};

		for (int i = 0; i < iter->second.size(); ++i)
		{
			fWeight[i] = (float)iter->second[i].dWeight;
			iIndex[i] = iter->second[i].iIndex;
		}

		// 버텍스에 

		_Container->vecBlendWeight[iter->first].x = fWeight[0];
		_Container->vecBlendWeight[iter->first].y = fWeight[1];
		_Container->vecBlendWeight[iter->first].z = fWeight[2];
		_Container->vecBlendWeight[iter->first].w = fWeight[3];
		_Container->vecBlendIndex[iter->first].x = (float)iIndex[0];
		_Container->vecBlendIndex[iter->first].y = (float)iIndex[1];
		_Container->vecBlendIndex[iter->first].z = (float)iIndex[2];
		_Container->vecBlendIndex[iter->first].w = (float)iIndex[3];
	}
}


void Fbx::LoadAnimation(FbxMesh * pMesh, FbxMeshData* _pContainer)
{
	int	iSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);

	// 스키닝 정보가 없으므로 애니메이션이 없다는것.
	if (iSkinCount <= 0)
	{
		return;
	}

	// 메쉬의 정점 수를 얻어온다.
	int	iCPCount = pMesh->GetControlPointsCount();

	// 정점의 가중치 정보와 본인덱스 정보는 정점 수만큼
	// 만들어져야 한다.
	_pContainer->vecBlendWeight.resize(iCPCount);
	_pContainer->vecBlendIndex.resize(iCPCount);
	_pContainer->bAnimation = true;

	fbxsdk::FbxNode* Node = pMesh->GetNode();

	const FbxVector4	vT = Node->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
	const FbxVector4	vR = Node->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
	const FbxVector4	vS = Node->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);
	
	FbxAMatrix	matTransform = FbxAMatrix(vT, vR, vS);

	for (int i = 0; i < iSkinCount; ++i)
	{
		FbxSkin*	pSkin = (FbxSkin*)pMesh->GetDeformer(i, FbxDeformer::eSkin);

		if (!pSkin)
		{
			continue;
		}

		FbxSkin::EType	eSkinningType = pSkin->GetSkinningType();

		if (eSkinningType == FbxSkin::eRigid ||
			eSkinningType == FbxSkin::eLinear)
		{
			// Cluster : 관절을 의미한다.
			int	iClusterCount = pSkin->GetClusterCount();

			for (int j = 0; j < iClusterCount; ++j)
			{
				FbxCluster*	pCluster = pSkin->GetCluster(j);

				if (!pCluster->GetLink())
				{
					continue;
				}

				std::wstring	strBoneName = CA2W(pCluster->GetLink()->GetName()).m_psz;

				//if (m_bMixamo)
				//	strBoneName.erase(0, 10);

				FbxBoneData* _Data = FindBone(strBoneName.c_str());

				LoadWeightAndIndex(pCluster, _Data->iIndex, _pContainer);
				LoadOffsetMatrix(pCluster, matTransform, _Data->iIndex, _pContainer);
				m_BoneData[_Data->iIndex]->matBone = FbxMatToHMat(matTransform);
				LoadTimeTransform(pMesh->GetNode(), pCluster, matTransform, _Data->iIndex);
			}
		}
	}

	ChangeWeightAndIndices(_pContainer);
}



void Fbx::LoadNormal(FbxMesh * pMesh, FbxMeshData* pContainer, int iVtxID, int iControlIndex)
{
	// 메쉬로부터 ElementNormal 정보를 얻어온다.
	FbxGeometryElementNormal*	pNormal = pMesh->GetElementNormal();

	int	iNormalIndex = iVtxID;

	// MappingMode와 ReferenceMode에 따라서 인덱스로 사용할 정보가 달라진다.
	if (pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iNormalIndex = pNormal->GetIndexArray().GetAt(iVtxID);
			break;
		}
	}

	else if (pNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iNormalIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iNormalIndex = pNormal->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	FbxVector4	vNormal = pNormal->GetDirectArray().GetAt(iNormalIndex);

	pContainer->vecNormal[iControlIndex].x = (float)vNormal.mData[0];
	pContainer->vecNormal[iControlIndex].y = (float)vNormal.mData[2];
	pContainer->vecNormal[iControlIndex].z = (float)vNormal.mData[1];
}

void Fbx::LoadUV(FbxMesh * pMesh, FbxMeshData* pContainer,	int iUVID, int iControlIndex)
{
	int iCount = pMesh->GetElementUVCount();
	FbxGeometryElementUV*	pUV = pMesh->GetElementUV(0);

	if (!pUV)
		return;

	int	iUVIndex = iUVID;

	if (pUV->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iUVIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iUVIndex = pUV->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	else if (pUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			//iUVIndex = pUV->GetIndexArray().GetAt(iUVIndex);
		}
		break;
		default:
			break; // other reference modes not shown here!
		}
	}

	FbxVector2	vUV = pUV->GetDirectArray().GetAt(iUVIndex);
	// MAX의 UV 저장방식이  정반대이기 때문에 
	// 

	//pContainer->vecUV[iControlIndex].x = (float)(vUV.mData[0] - (int)vUV.mData[0]);
	//pContainer->vecUV[iControlIndex].y = (float)(1.f - (vUV.mData[1] - (int)vUV.mData[1]));

	pContainer->vecUV[iControlIndex].x = (float)(vUV.mData[0]);
	pContainer->vecUV[iControlIndex].y = (float)(1.0f - (vUV.mData[1]));
}

void Fbx::LoadTangent(FbxMesh * pMesh, FbxMeshData* pContainer, int iVtxID, int iControlIndex)
{
	FbxGeometryElementTangent*	pTangent = pMesh->GetElementTangent();

	if (!pTangent)
		return;

	pContainer->bBump = true;

	int	iTangentIndex = iVtxID;

	if (pTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iTangentIndex = pTangent->GetIndexArray().GetAt(iVtxID);
			break;
		}
	}

	else if (pTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iTangentIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iTangentIndex = pTangent->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	FbxVector4	vTangent = pTangent->GetDirectArray().GetAt(iTangentIndex);

	pContainer->vecTangent[iControlIndex].x = (float)vTangent.mData[0];
	pContainer->vecTangent[iControlIndex].y = (float)vTangent.mData[2];
	pContainer->vecTangent[iControlIndex].z = (float)vTangent.mData[1];
}

void Fbx::LoadBinormal(FbxMesh * pMesh, FbxMeshData* pContainer, int iVtxID, int iControlIndex)
{
	FbxGeometryElementBinormal*	pBinormal = pMesh->GetElementBinormal();

	if (!pBinormal)
		return;

	pContainer->bBump = true;

	int	iBinormalIndex = iVtxID;

	if (pBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iBinormalIndex = pBinormal->GetIndexArray().GetAt(iVtxID);
			break;
		}
	}

	else if (pBinormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iBinormalIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iBinormalIndex = pBinormal->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	FbxVector4	vBinormal = pBinormal->GetDirectArray().GetAt(iBinormalIndex);

	pContainer->vecBinormal[iControlIndex].x = (float)vBinormal.mData[0];
	pContainer->vecBinormal[iControlIndex].y = (float)vBinormal.mData[2];
	pContainer->vecBinormal[iControlIndex].z = (float)vBinormal.mData[1];
}


void Fbx::LoadMeshData(fbxsdk::FbxMesh* _Mesh) 
{
	FbxMeshData* NewMeshData = new FbxMeshData();
	m_MeshData.push_back(NewMeshData);
	//// ControlPoint 는 위치정보를 담고 있는 배열이다.
	//// 이 배열의 개수는 곧 정점의 개수가 된다.
	int	iVtxCount = _Mesh->GetControlPointsCount();

	//// 내부적으로 FbxVector4타입의 배열로 저장하고 있기 때문에 배열의 
	//// 시작주소를 얻어온다.
	FbxVector4*	pVtxPos = _Mesh->GetControlPoints();

	//// 컨테이너가 가지고 있는 정점 정보들을 정점수만큼 resize 해준다.
	// 17만 2천개가 들어있는데.
	NewMeshData->vecPos.resize(iVtxCount);
	NewMeshData->vecNormal.resize(iVtxCount);
	NewMeshData->vecUV.resize(iVtxCount);
	NewMeshData->vecTangent.resize(iVtxCount);
	NewMeshData->vecBinormal.resize(iVtxCount);

	// 일단 포지션 부터 얻어온다.
	for (int i = 0; i < iVtxCount; ++i)
	{
		// // Max 모드라서 y와 z축이 바뀌어 있기 때문에 변경해준다.
		NewMeshData->vecPos[i].x = (float)pVtxPos[i].mData[0];
		NewMeshData->vecPos[i].y = (float)pVtxPos[i].mData[2];
		NewMeshData->vecPos[i].z = (float)pVtxPos[i].mData[1];
	}

	//// 폴리곤 수를 얻어온다.
	// 인덱스 버퍼 생각하면 된다.
	// 삼각형 수를 얻어온다.
	// 4개로 나누어서 그린다.
	int	iPolygonCount = _Mesh->GetPolygonCount();

	UINT	iVtxID = 0;

	//// 재질 수를 얻어온다.
	// 각 서브셋에 대한 재질의 개수이다.
	int	iMtrlCount = _Mesh->GetNode()->GetMaterialCount();

	// 재질 수는 곧 서브셋 수이기 때문에 재질 수만큼 resize 한다.
	NewMeshData->vecIndices.resize(iMtrlCount);

	//// 재질 정보를 얻어온다.
	FbxGeometryElementMaterial*	pMaterial = _Mesh->GetElementMaterial();
	int iCount = _Mesh->GetElementMaterialCount();
	//// 삼각형 수만큼 반복한다.
	for (int i = 0; i < iPolygonCount; ++i)
	{
		// 이 폴리곤을 구성하는 정점의 수를 얻어온다.
		// 삼각형일 경우 3개를 얻어온다.
		int	iPolygonSize = _Mesh->GetPolygonSize(i);

		//if (iPolygonSize != 3)
		//{
		//	BOOM;
		//}
		int	iIdx[3] = {};

		// 왜만하면 3이다.
		for (int j = 0; j < iPolygonSize; ++j)
		{
			// 현재 삼각형을 구성하고 있는 버텍스정보 내에서의 인덱스를
			// 구한다. 삼각형 구성 인덱스 정보를 얻어온다.
			// i n번째 삼각형의 j는 0번째 정점의 정보를 내놔.
			int	iControlIndex = _Mesh->GetPolygonVertex(i, j);

			iIdx[j] = iControlIndex;

			LoadNormal(_Mesh, NewMeshData, iVtxID, iControlIndex);
			LoadUV(_Mesh, NewMeshData, _Mesh->GetTextureUVIndex(i, j), iControlIndex);
			LoadTangent(_Mesh, NewMeshData, iVtxID, iControlIndex);
			LoadBinormal(_Mesh, NewMeshData, iVtxID, iControlIndex);

			++iVtxID;
		}

		// 면을 이루는게 정점이 3
		// 인덱스 버퍼를 얻어온다.
		int	iMtrlID = pMaterial->GetIndexArray().GetAt(i);
		NewMeshData->vecIndices[iMtrlID].push_back(iIdx[0]);
		NewMeshData->vecIndices[iMtrlID].push_back(iIdx[2]);
		NewMeshData->vecIndices[iMtrlID].push_back(iIdx[1]);
	}

	LoadAnimation(_Mesh, NewMeshData);
}

void Fbx::LoadMesh(fbxsdk::FbxNode* _Node) {
	FbxNodeAttribute*	pAttr = _Node->GetNodeAttribute();

	if (pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh*	_Mesh = _Node->GetMesh();

		if (nullptr != _Mesh) 
		{
			LoadMeshData(_Mesh);
		}
	}


	int	iChildCount = _Node->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
	{
		LoadMesh(_Node->GetChild(i));
	}

}


double Fbx::GetMaterialFactor(FbxSurfaceMaterial * pMtrl,	const char * pPropertyName)
{
	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);
	double	dFactor = 0.0;

	if (tProperty.IsValid())
		dFactor = tProperty.Get<FbxDouble>();

	return dFactor;
}

Vector4 Fbx::GetMaterialColor(FbxSurfaceMaterial * pMtrl, const char * pPropertyName, const char * pPropertyFactorName)
{
	FbxDouble3	vResult(0, 0, 0);
	double		dFactor = 0;

	// 인자로 들어온 재질에서 해당 이름을 가진 프로퍼티가 있는지를 판단한다음
	// 얻어온다. 마찬가지로 해당 이름의 프로퍼티 팩터가 있는지를 판단한다음
	// 얻어온다.
	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);
	FbxProperty	tPropertyFactor = pMtrl->FindProperty(pPropertyFactorName);

	// 유효한지 체크한다.
	if (tProperty.IsValid() && tPropertyFactor.IsValid())
	{
		vResult = tProperty.Get<FbxDouble3>();
		dFactor = tPropertyFactor.Get<FbxDouble>();

		// Factor가 1이 아닐 경우 일종의 비율값으로 사용하여
		// 얻어온 vResult 에 모두 곱해준다.
		if (dFactor != 1)
		{
			vResult[0] *= dFactor;
			vResult[1] *= dFactor;
			vResult[2] *= dFactor;
		}
	}

	return Vector4((float)vResult[0], (float)vResult[1], (float)vResult[2], (float)dFactor);
}


std::wstring Fbx::GetMaterialTexture(FbxSurfaceMaterial * pMtrl,	const char * pPropertyName)
{
	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);

	std::wstring	str = L"";

	if (tProperty.IsValid())
	{
		// FbxFileTexture 이 타입이 몇개가 있는지를 얻어오는 함수이다.
		int	iTexCount = tProperty.GetSrcObjectCount<FbxFileTexture>();

		if (iTexCount > 0)
		{
			FbxFileTexture*	pFileTex = tProperty.GetSrcObject<FbxFileTexture>(0);

			if (pFileTex)
				str = CA2W(pFileTex->GetFileName());
		}
	}

	return str;
}

void Fbx::LoadMaterial(FbxSurfaceMaterial * pMtrl) {
	// 재질 정보를 저장할 구조체를 생성한다.
	FbxMaterialData*	pMtrlInfo = new FbxMaterialData;

	// 가장 마지막 컨테이너에 재질 정보를 추가한다.
	m_MaterialData[m_MaterialData.size() - 1].push_back(pMtrlInfo);

	// Diffuse 정보를 읽어온다.
	pMtrlInfo->Dif = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sDiffuse,
		FbxSurfaceMaterial::sDiffuseFactor);

	pMtrlInfo->Amb = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sAmbient,
		FbxSurfaceMaterial::sAmbientFactor);

	pMtrlInfo->Spc = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sSpecular,
		FbxSurfaceMaterial::sSpecularFactor);

	pMtrlInfo->Emv = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sEmissive,
		FbxSurfaceMaterial::sEmissiveFactor);

	pMtrlInfo->SpecularPower = (float)GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sSpecularFactor);
	pMtrlInfo->Shininess = (float)GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sShininess);
	pMtrlInfo->TransparencyFactor = (float)GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sTransparencyFactor);

	pMtrlInfo->DifTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sDiffuse);
	pMtrlInfo->BumpTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sNormalMap);
	pMtrlInfo->SpcTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sSpecular);

	//FbxSurfaceMaterial::s

	//FbxSurfaceMaterial::s

	//if (pMtrlInfo->BumpTex.empty())
	//	pMtrlInfo->BumpTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sBump);
}

void Fbx::Trangulate(fbxsdk::FbxNode* _Node) 
{
	// 그 노드안에 만약 매쉬가 있다면
	// 그 매쉬를 MAX에 맞게 정보를 버텍스와 인덱스 버퍼 형식으로 변환 해주는 작업.
	fbxsdk::FbxNodeAttribute* _Attr = _Node->GetNodeAttribute();

	if (nullptr != _Attr && 
		(_Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh ||
			_Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNurbs ||
			_Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNurbsSurface))
	{
		FbxGeometryConverter	converter(m_Mgr);
		converter.Triangulate(_Attr, true);
	}


	// 재질정보를 읽는다.
	int	iMtrlCount = _Node->GetMaterialCount();

	if (iMtrlCount > 0)
	{
		std::vector<FbxMaterialData*>	vecMtrl;

		m_MaterialData.push_back(vecMtrl);

		for (int i = 0; i < iMtrlCount; ++i)
		{
			// FbxSurfaceMaterial : Fbx 가 가지고 있는 재질 정보를
			// 담고있다.
			FbxSurfaceMaterial*	pMtrl = _Node->GetMaterial(i);

			if (!pMtrl)
				continue;

			LoadMaterial(pMtrl);
		}
	}

	int ChildCount = _Node->GetChildCount();

	for (int i = 0; i < ChildCount; i++)
	{
		Trangulate(_Node->GetChild(i));
	}
}

Fbx::FbxBoneData* Fbx::FindBone(const wchar_t* _Name)
{
	std::multimap<std::wstring, FbxBoneData*>::iterator FindBoneIter = m_BoneDataToName.find(_Name);

	if (FindBoneIter == m_BoneDataToName.end())
	{
		return nullptr;
	}

	return FindBoneIter->second;
}

void Fbx::LoadBone(fbxsdk::FbxNode* _Node, int iDepth, int _iIndex, int _iParent)
{
	fbxsdk::FbxNodeAttribute* _Attr = _Node->GetNodeAttribute();

	if (nullptr != _Attr && _Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
	{
		m_BoneData.push_back(new FbxBoneData());
		m_BoneData[_iIndex]->Name = CA2W(_Node->GetName()).m_psz;
		// 막시모
		// if(막시모라혐ㄴ)
		// m_BoneData[iIndex]->Name.erase(0, 10)
		// 혹시나 부모가 
		m_BoneData[_iIndex]->iIndex = _iIndex;
		m_BoneData[_iIndex]->iParentIndex = _iParent;
		m_BoneData[_iIndex]->iDepth = iDepth;

	}

	int ChildCount = _Node->GetChildCount();

	for (int i = 0; i < ChildCount; i++)
	{
		LoadBone(_Node->GetChild(i), iDepth + 1, (int)m_BoneData.size(), _iIndex);
	}
}

bool Fbx::AniCheck() 
{
	fbxsdk::FbxArray<FbxString*> AniNameArray;
	m_Scene->FillAnimStackNameArray(AniNameArray);

	if (0 == AniNameArray.Size())
	{
		return false;
	}

	// m_AniData.resize(AniNameArray.Size());


	for (int i = 0; i < AniNameArray.GetCount(); ++i)
	{
		FbxAnimStack* pAniStack = m_Scene->FindMember<FbxAnimStack>(AniNameArray[i]->Buffer());

		if (nullptr == pAniStack)
		{
			continue;
		}

		m_AniData.push_back(new FbxAniData());

		m_AniData[i]->Name = CA2W(pAniStack->GetName(), CP_UTF8).m_psz;

		FbxTakeInfo* TakeInfo = m_Scene->GetTakeInfo(pAniStack->GetName());
		m_AniData[i]->StartTime = TakeInfo->mLocalTimeSpan.GetStart();
		m_AniData[i]->EndTime = TakeInfo->mLocalTimeSpan.GetStop();
		m_AniData[i]->TimeMode = m_Scene->GetGlobalSettings().GetTimeMode();
		m_AniData[i]->TimeStartCount = m_AniData[i]->StartTime.GetFrameCount(m_AniData[i]->TimeMode);
		m_AniData[i]->TimeEndCount = m_AniData[i]->EndTime.GetFrameCount(m_AniData[i]->TimeMode);
		m_AniData[i]->TimeLength = m_AniData[i]->TimeEndCount - m_AniData[i]->TimeStartCount;

		// pAniStack->Destroy();
	}

	for (int i = 0; i < AniNameArray.GetCount(); i++)
	{
		delete AniNameArray[i];
	}

	return true;

}

void Fbx::Release()
{

	if (nullptr != m_Ios)
	{
		m_Ios->Destroy();
		m_Ios = nullptr;
	}


	if (nullptr != m_Imp)
	{
		m_Imp->Destroy();
		m_Imp = nullptr;
	}

	if (nullptr != m_Scene)
	{
		m_Scene->Destroy();
		m_Scene = nullptr;
	}


	if (nullptr != m_Mgr)
	{
		m_Mgr->Destroy();
		m_Mgr = nullptr;
	}

}

int Fbx::FrameCount(int _AniData) 
{
	switch (m_AniData[_AniData]->TimeMode)
	{
	case FbxTime::EMode::eFrames120:
		return 120;
	case FbxTime::EMode::eFrames100:
		return 100;
	case FbxTime::EMode::eFrames60:
		return 60;
	case FbxTime::EMode::eFrames30:
		return 30;
	default:
		break;
	}

	return 0;

}