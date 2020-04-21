#include "TERRAIN.h"

// 자기게임 메인컨텐츠의 구조를 잡아라.
// 어떤 어떤 씬 할것인지.
// 대부분 FSM이 존재한다.
// 메인 캐릭터
// 1VS1 싸움을 구현해 보는것이 좋다.

// 스카이박스
// 레스터라이저 스테이트  프론트 백 백으로 구현한다.
// 하늘이 카메라를 따라다닌다.
// 크기 세팅도 필요없다.
// 카메라의 FAR - 10.0F보다 살짝 작게 만든다.
// 맵핑할 택스처 끝.

// 클라이언트만 있는 게임의 대분류는 
// 프레임워크 맵(네비매쉬)과 캐릭터(플레이어 몬스터 등등등등....)

Terrain::Terrain()
{
}


Terrain::~Terrain()
{
}


void Terrain::CreateTerrain(int _X, int _Z, const wchar_t* _Name, const wchar_t* _HeightName, float _HRatio, float _Detail)
{
	HPTR<Texture> HETEX = Resources::Find<Texture>(_HeightName);

	m_Data.m_Size.x = (float)_X;
	m_Data.m_Size.z = (float)_Z;

	std::vector<EngineVertex::Vertex3D> ArrVtx;
	ArrVtx.resize((_X + 1) * (_Z + 1));
	ArrPos.resize((_X + 1) * (_Z + 1));

	float InterX = 1.0f / (float)(_X);
	float InterZ = 1.0f / (float)(_Z);

	EngineVertex::Vertex3D TempVtx;


	float HEY = (float)HETEX->GetHeight() / (float)(_Z + 1);
	float HEX = (float)HETEX->GetWidth() / (float)(_X + 1);

	m_PivotSize.x = InterX;
	m_PivotSize.z = InterZ;

	for (size_t z = 0; z < _X+1 ; z++)
	{
		for (size_t x = 0; x < _Z+1; x++)
		{
			TempVtx.Pos = Vector4(x * InterX - 0.5F, 0.0f, z * InterZ * -1.0f + 0.5F, 1.0f);

			if (nullptr != HETEX)
			{
				float PX = x * HEX;
				float PZ = z * HEY;

				if (PX >= HETEX->GetWidth())
				{
					PX = (float)HETEX->GetWidth() - 1.0F;
				}

				if (PZ >= HETEX->GetHeight())
				{
					PZ = (float)HETEX->GetHeight() - 1.0F;
				}

				Vector4 Pos = HETEX->GetColorToFloat((int)PX, (int)PZ);
				TempVtx.Pos.y = Pos.x * _HRatio;	//높이를 이미지픽셀값으로 설정
			}

			
			TempVtx.Pos.w = 1.0f;
			TempVtx.Color = Vector4::WHITE;
			TempVtx.vUv = Vector2((float)x, (float)z);
			// TempVtx.Normal = HVEC4::UP;
			TempVtx.Normal = Vector4(0, 1.0f, 0, 0.0f);
			TempVtx.Tangent = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
			TempVtx.BiNormal = Vector4(1.0f, 0.0f, 0, 0.0f);

			ArrPos[z * (_Z + 1) + x] = TempVtx.Pos;
			ArrVtx[z * (_Z + 1) + x] = TempVtx;
		}
	}

	

	std::wstring VBNAME = _HeightName;
	VBNAME += L"VB";
	Resources::Create<HVBBUFFER>(VBNAME.c_str(), ArrVtx, D3D11_USAGE_DYNAMIC);


	std::vector<DWORD> ArrIdx;


	// 
	// 0 == z * (_Z) + x
	//ArrIdx.push_back({ z * (_Z) + x,  z * (_Z) + x + 3, z * (_Z) + x + 1 });
	//ArrIdx.push_back({ z * (_Z) + x,  z * (_Z) + x + 2, z * (_Z) + x + 3 });


	for (int z = 0; z < _Z; z++)
	{
		for (int x = 0; x < _X; x++)
		{
			int Pivot = z * (_Z + 1) + x;
			ArrIdx.push_back(Pivot);
			ArrIdx.push_back(Pivot + 1);
			ArrIdx.push_back(Pivot + (_Z + 1) + 1);
			ArrIdx.push_back(Pivot);
			ArrIdx.push_back(Pivot + (_Z + 1) + 1);
			ArrIdx.push_back(Pivot + (_Z + 1));
		}
	}

	std::wstring IBNAME = _HeightName;
	IBNAME += L"IB";

	Resources::Create<HIBBUFFER>(IBNAME.c_str(), ArrIdx, D3D11_USAGE_DYNAMIC);

	
		// 터레인 두개 못만든다.
	NEWMESH = Resources::Create<HMESH>(_HeightName);
	NEWMESH->AddVB(VBNAME.c_str());
	NEWMESH->AddIB(IBNAME.c_str());


	Renderer::HRENDERDATA* RD = RenderData(_HeightName, L"TERRAINDPATH");
	RD->m_bShadow = true;

	LinkTexture(L"F0Tex", _Name);
	LinkSampler(L"PS_LSMP", L"LSMP");
	LinkSampler(L"PS_PSMP", L"PSMP");
	// LinkCB(L"GRIDDATA", &m_Data);

	LinkCB(L"TERRAINDATA", &m_Data);

	m_Data.m_ArrData[0].m_Detail = _Detail;

	for (size_t i = 0; i < 4; i++)
	{
		m_Data.m_ArrData[i].m_Bump = 0.0f;
	}
}

//void Terrain::Update()
//{
//
//}
void Terrain::FloorTextureSetting(int _X, const wchar_t* _Name, const wchar_t* _SpName, float _Detail)
{
	if (4 <= _X)
	{
		BOOM;
	}

	m_Data.m_ArrData[_X].m_OnOff = 1.0f;
	m_Data.m_ArrData[_X].m_Detail = _Detail;

	std::wstring Name = L"F";

	std::wstring TexName = Name;
	// std::to_string()
	TexName += (wchar_t)(L'0' + _X);
	TexName += L"Tex";
	LinkTexture(TexName.c_str(), _Name);
	

	if (0 == _X)
	{
		return;
	}

	std::wstring SpName = Name;
	SpName += (wchar_t)(L'0' + _X);
	SpName += L"Sp";
	LinkTexture(SpName.c_str(), _SpName);

}

void Terrain::FloorBumpSetting(int _X, const wchar_t* _BumpImgName)
{
	if (nullptr == _BumpImgName)
	{
		BOOM;

	}

	std::wstring Name = L"F";
	std::wstring SpName = Name;
	SpName += (wchar_t)(L'0' + _X);
	SpName += L"Bump";
	LinkTexture(SpName.c_str(), _BumpImgName);

	m_Data.m_ArrData[_X].m_Bump= 1.0f;
}

void Terrain::RenderUpdate()
{

}

void Terrain::RenderDataUpdate(const HPTR<Camera>& _CAM, HRENDERDATA* _Data)
{

}

float Terrain::WPosToY(Vector4 _Pos)
{
	Vector4 Index = WPosToIndex(_Pos);

	int X = (int)std::floor(Index.x);
	int Z = (int)std::floor(Index.z);

	if (0 > X || (int)m_Data.m_Size.x <= X)
	{
		BOOM;
	}

	if (0 > Z || (int)m_Data.m_Size.z <= Z)
	{
		BOOM;
	}

	int PosIndex = (((int)m_Data.m_Size.x + 1) * Z) + X;

	Vector4 LeftTop = ArrPos[PosIndex] * GetTransform()->GetScale();
	Vector4 RightTop = ArrPos[PosIndex + 1] * GetTransform()->GetScale();
	Vector4 LeftBot = ArrPos[PosIndex + ((int)m_Data.m_Size.x + 1)] * GetTransform()->GetScale();
	Vector4 RightBot = ArrPos[PosIndex + ((int)m_Data.m_Size.x + 1) + 1] * GetTransform()->GetScale();

	_Pos.y = 0.0f;
	float Result = FLT_MAX;
	DirectX::TriangleTests::Intersects(_Pos, Vector4::UP, LeftTop, RightTop, RightBot, Result);
	if (Result != FLT_MAX && Result != 0.0f)
	{
		return Result;
	}
	DirectX::TriangleTests::Intersects(_Pos, Vector4::UP, LeftTop, RightBot, LeftBot, Result);
	if (Result != FLT_MAX && Result != 0.0f)
	{
		return Result;
	}
	/*
		DirectX::TriangleTests::Intersects(_Pos, HVEC4::DOWN, LeftTop, RightTop, RightBot, Result);
		if (Result != FLT_MAX)
		{
			return Result - (HVEC4::UP * 100.0f).y;
		}
		DirectX::TriangleTests::Intersects(_Pos, HVEC4::DOWN, LeftTop, RightBot, LeftBot, Result);
		if (Result != FLT_MAX)
		{
			return Result - (HVEC4::UP * 100.0f).y;
		}*/

	return 0.0f;
}

Vector4 Terrain::WPosToIndex(Vector4 _Pos)
{

	// 어느 사각형에 있는지 구해와야 한다.
// 1차
// WSCALE -> 확되어있으므로
// 몇칸인지도 필요하다.
// WPos ->  맵을 이동시켜놓았을수도 있으므로

	//m_Data.m_Size;
	//GetTransform()->GetScale();

	Vector4 RectSize;
	RectSize.x = m_PivotSize.x * GetTransform()->GetScale().x;
	RectSize.z = m_PivotSize.z * GetTransform()->GetScale().z;

	
	_Pos.x += RectSize.x * m_Data.m_Size.x * 0.5f;
	_Pos.z -= RectSize.x * m_Data.m_Size.x * 0.5f;

	Vector4 Index;
	Index.x = _Pos.x / RectSize.x;
	Index.z = _Pos.z / RectSize.z * -1.0f;

	return Index;
	
	//return Vector4::ZERO;
}
//void Terrain::Render(const HPTR<Camera>& _CAM)
//{
//
//}