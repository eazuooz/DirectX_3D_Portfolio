#include "JITILEMAP.h"
#include <HMESH.h>
#include <HCB.h>
#include <HVTXSHADER.h>
#include <HPIXSHADER.h>
#include <HSMP.h>
#include <SPRITE.h>
#include <HBLEND.h>
#include <HDSSTATE.h>
#include <HRSSTATE.h>
#include <RENMGR.h>
#include <HINSTBUFFER.h>
#include <RENMGR.h>

void JITILEMAP::HPATHNODE::CalValue(
	JITILEMAP::HPATHNODE* _Parent, HGAMETILE* _Tile, HGAMETILE* _End)
{
	Parent = _Parent;
	Tile = _Tile;

	if (nullptr != _Parent)
	{
		if (_Parent->Tile == _Tile)
		{
			BOOM;
		}
	}

	// 값은 1밖에 안나온다.
	if (nullptr != _Parent)
	{
		H = (float)(abs((_Parent->Tile->Index.ix - Tile->Index.ix)
			+ (_Parent->Tile->Index.iy - Tile->Index.iy)))
			+ _Parent->H;
	}
	else {
		H = 0.0f;
	}
	// 그냥 벡터 빼기 벡터로 길이를 구했어야 한다.
	G = (float)(abs(_End->Index.ix - _Tile->Index.ix) 
		+ abs(_End->Index.iy - _Tile->Index.iy));

	F = H + G;
}

JITILEMAP::JITILEMAP()
{
}


JITILEMAP::~JITILEMAP()
{
}

void JITILEMAP::Init()
{
	// 매쉬를 전용매쉬를 만들어야 한다.
	// 


	if (nullptr == Resources::Find<HVTXSHADER>(L"TEXSHADERINST.fx"))
	{

		// 인스턴싱용 쉐이더
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\TEXSHADERINST.fx", 5, 0, "VS_TEX");
		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0);
		VTXSH->AddLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		// 1
		VTXSH->AddLayOut("WVP", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1/*(요놈이 1이라서 인스턴싱 데이터 인것이다.)*/, D3D11_INPUT_PER_INSTANCE_DATA);
		VTXSH->AddLayOut("WVP", 1, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1/*(요놈이 1이라서 인스턴싱 데이터 인것이다.)*/, D3D11_INPUT_PER_INSTANCE_DATA);
		VTXSH->AddLayOut("WVP", 2, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1/*(요놈이 1이라서 인스턴싱 데이터 인것이다.)*/, D3D11_INPUT_PER_INSTANCE_DATA);
		VTXSH->AddLayOut("WVP", 3, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1/*(요놈이 1이라서 인스턴싱 데이터 인것이다.)*/, D3D11_INPUT_PER_INSTANCE_DATA);
		VTXSH->EndLayOut("SPRITEUV", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1/*(요놈이 1이라서 인스턴싱 데이터 인것이다.)*/, D3D11_INPUT_PER_INSTANCE_DATA);
	}


	if (nullptr == Resources::Find<HINSTBUFFER>(L"TILEINSTBUFFER"))
	{
		Resources::Create<HINSTBUFFER>(L"TILEINSTBUFFER", 1000, &typeid(TILEINSTDATA), sizeof(TILEINSTDATA));
	}

	if (nullptr == Resources::Find<HMESH>(L"TILEINSTMESH"))
	{
		HPTR<HMESH> NEWMESH = Resources::Create<HMESH>(L"TILEINSTMESH");
		NEWMESH->AddVB(L"DTEXVBBUFFER");
		NEWMESH->AddINST(L"TILEINSTBUFFER");
		NEWMESH->AddIB(L"DTEXIBBUFFER");
	}






	//// TILEINSTDATA 기반으로 인스턴싱을 할것이다.
	//InstBuffer = HRENMGR::CreateInstancing(L"TileMapInst", L"TILEINSTMESH");
	//InstBuffer->RenderPath().BLEND(L"DBLEND");
	//InstBuffer->RenderPath().DSSTATE(L"DDSSTATE");
	//InstBuffer->RenderPath().RSSTATE(L"DRSSTATE");
	//InstBuffer->RenderPath().VTXSHADER(L"TEXSHADERINST.fx");
	//InstBuffer->RenderPath().PIXSHADER(L"TEXSHADER.fx");
	//InstBuffer->RenderPath().SMP(HSHADER::SHADERTYPE::SHT_PS, 0, L"DSMP");
	//InstBuffer->RenderPath().DrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//

	Renderer::Init();

	return;

	// 해줘야 pushrENDER가 된다.

	m_SMP = Resources::Find<HSMP>(L"DSMP");
	m_HMESH = Resources::Find<HMESH>(L"DTEXMESH");
	m_HCB = Resources::Find<HCB>(L"MATBUF");
	m_HCBUV = Resources::Find<HCB>(L"CUTBUF");
	m_HVTXSHADER = Resources::Find<HVTXSHADER>(L"TEXSHADER.fx");
	m_HPIXSHADER = Resources::Find<HPIXSHADER>(L"TEXSHADER.fx");
	m_BLEND = Resources::Find<HBLEND>(L"DBLEND");
	m_DSSTATE = Resources::Find<HDSSTATE>(L"DDSSTATE");
	m_RSSTATE = Resources::Find<HRSSTATE>(L"DRSSTATE");
}


void JITILEMAP::TileSprite(const wchar_t* _Name) 
{
	m_TileSprite = Resources::Find<Sprite>(_Name);

	if (nullptr == m_TileSprite)
	{
		BOOM;
	}

	// InstBuffer->RenderPath().TEX(_Name, HSHADER::SHADERTYPE::SHT_PS, 0);

}

void JITILEMAP::FloorSprite(const wchar_t* _Name) 
{
	m_FloorSprite = Resources::Find<Sprite>(_Name);
}

void JITILEMAP::AddTileWorld(Vector4 _Pos) 
{
	//_Pos.x = _Pos.x / m_TileScale.x;
	//_Pos.y = _Pos.y / m_TileScale.y;

	Vector4 Idx;
	Idx.x = (_Pos.x / m_TileMoveSize.x + _Pos.y / m_TileMoveSize.y) / 2.0f;
	Idx.y = (_Pos.y / m_TileMoveSize.y + -(_Pos.x / m_TileMoveSize.x)) / 2.0f;

	Idx.ix = std::lroundf( Idx.x);
	Idx.iy = std::lroundf(Idx.y);
	AddTile(Idx.KeyPos);
}

Vector4 JITILEMAP::PosToIndex(Vector4 _Pos) 
{
	Vector4 Idx;
	Idx.x = (_Pos.x / m_TileMoveSize.x + _Pos.y / m_TileMoveSize.y) / 2.0f;
	Idx.y = (_Pos.y / m_TileMoveSize.y + -(_Pos.x / m_TileMoveSize.x)) / 2.0f;

	Idx.ix = std::lroundf(Idx.x);
	Idx.iy = std::lroundf(Idx.y);

	return Idx;
}


HPTR<JITILEMAP::HGAMETILE> JITILEMAP::FindTile(Vector4 _Pos) 
{
	std::map<__int64, HPTR<HGAMETILE>>::iterator FindIter = m_TileMap.find(PosToIndex(_Pos).KeyPos);

	if (FindIter == m_TileMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

HPTR<JITILEMAP::HGAMETILE> JITILEMAP::FindTile(__int64 _KeyPos)
{
	std::map<__int64, HPTR<HGAMETILE>>::iterator FindIter = m_TileMap.find(_KeyPos);

	if (FindIter == m_TileMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

void JITILEMAP::ChangeFloor(Vector4 _Pos, int _Floor)
{
 	HPTR<HGAMETILE> Tile = FindTile(_Pos);

	if (nullptr != Tile)
	{
		Tile->Floor = _Floor;
		Tile->Pos = Tile->BasePos;
		Tile->Pos.y = Tile->BasePos.y + Tile->Floor * m_TileMoveSize.y;
		// Value 값형 복사생성자 혹은 대입 연산자가 자동으로 호출될 것이다.
		// 특히나 내가 만들어주지 않았으니까.
	}
}

void JITILEMAP::ChangeLeftFloor(Vector4 _Pos, int _Floor)
{
	HPTR<HGAMETILE> Tile = FindTile(_Pos);

	if (nullptr != Tile)
	{
		Tile->LeftFloorSpriteIndex = _Floor;
		// Value 값형 복사생성자 혹은 대입 연산자가 자동으로 호출될 것이다.
		// 특히나 내가 만들어주지 않았으니까.
	}
}

void JITILEMAP::ChangeRightFloor(Vector4 _Pos, int _Floor)
{
	HPTR<HGAMETILE> Tile = FindTile(_Pos);

	if (nullptr != Tile)
	{
		Tile->RightFloorSpriteIndex = _Floor;
		// Value 값형 복사생성자 혹은 대입 연산자가 자동으로 호출될 것이다.
		// 특히나 내가 만들어주지 않았으니까.
	}
}

void JITILEMAP::AddTileF(Vector4 _Index) 
{
	_Index.ConToInt();

	_Index.ix, _Index.iy;

	AddTile(_Index.KeyPos);
}

void JITILEMAP::AddTile(int _X, int _Y) 
{
	__int64 Key = 0;
	int* pKey = (int*)&Key;
	(*pKey) = _X;
	++pKey;
	(*pKey) = _Y;

	//HVEC4 Key = {0,};
	//Key.ix = _X;
	//Key.iy = _Y;

	AddTile(Key);
}

void JITILEMAP::AddTile(__int64 _Index)
{
	if (nullptr == m_TileSprite)
	{
		BOOM;
	}

	if (m_TileMap.end() != m_TileMap.find(_Index))
	{
		return;
	}

	HPTR<HGAMETILE> Value = new HGAMETILE();

	Vector4 Idx;
	Idx.KeyPos = _Index;
	Vector4 Pos;
	// floor과 상관없는 z값이다.
	Pos.x = (Idx.ix - Idx.iy) * m_TileMoveSize.x;
	Pos.y = (Idx.ix + Idx.iy) * m_TileMoveSize.y;
	// Pos.y += 0 * m_TileMoveSize.y;
	Pos.z = Pos.y;

	Value->Index.KeyPos = _Index;
	Value->BasePos = Pos;
	Value->Pos = Pos;

	// Value 값형 복사생성자 혹은 대입 연산자가 자동으로 호출될 것이다.
	// 특히나 내가 만들어주지 않았으니까.
	m_TileMap.insert(std::map<__int64, HPTR<HGAMETILE>>::value_type(_Index, Value));
}

void JITILEMAP::Render(const HPTR<Camera>& _CAM)
{
	InstRender(_CAM);
}

// char Arr[10000000];
// 메모리 풀과 오브젝트풀은 완전히 다른개념이다.
// 관리가 편한것
// 연산 new 
void JITILEMAP::PushNode(HPTR<JITILEMAP::HPATHNODE> _Node) 
{
	if (nullptr == _Node)
	{
		BOOM;
	}

	NodePool.push_back(_Node);
}

HPTR<JITILEMAP::HPATHNODE> JITILEMAP::NewNode(JITILEMAP::HPATHNODE* _Parent, HGAMETILE* _Tile, HGAMETILE* _End)
{
	// 2000개 이상 찾아봤는데도 길이 없었다.
	// 길을 못찾았다라고 생각할것.
	if (0 == NodePool.size())
	{
		return nullptr;
	}

	std::list<HPTR<HPATHNODE>>::iterator Start = NodePool.begin();
	HPTR<JITILEMAP::HPATHNODE> Node = (*Start);
	Node->CalValue(_Parent, _Tile, _End);
	NodePool.pop_front();
	return Node;
}

void JITILEMAP::AStarMaxSetting(int _Count)
{
	for (size_t i = 0; i < _Count; i++)
	{
		NodePool.push_back(new JITILEMAP::HPATHNODE());
	}
}

HPTR<JITILEMAP::HPATHNODE> JITILEMAP::FirstOpenPop()
{
	HPTR<JITILEMAP::HPATHNODE> FirstNode = OpenList.begin()->second;
	if (nullptr == FirstNode->Tile)
	{
		BOOM;
	}

	OpenListSet.erase(FirstNode->Tile);
	OpenList.erase(OpenList.begin());
	CloseList.insert(std::map<__int64, HPTR<HPATHNODE>>::value_type(FirstNode->Tile->Index.KeyPos, FirstNode));
	return FirstNode;
}

void JITILEMAP::AddOpenList(HPTR<HPATHNODE> _NewNode) 
{
	if (nullptr == _NewNode->Tile)
	{
		BOOM;
	}

	OpenListSet.insert(_NewNode->Tile);
	OpenList.insert(std::multimap<float, HPTR<HPATHNODE>>::value_type(_NewNode->F, _NewNode));
}

// 4방향
// 리턴되게 바꿈.
HPTR<JITILEMAP::HPATHNODE> JITILEMAP::OpenNodeFind(HPTR<HPATHNODE> _Node, HGAMETILE* _End)
{
	for (size_t i = 0; i < 4; i++)
	{
		Vector4 Idx;
		Idx.ix = _Node->Tile->Index.ix + Vector4::ARRIDIR[i].ix;
		Idx.iy = _Node->Tile->Index.iy + Vector4::ARRIDIR[i].iy;

		HGAMETILE* _Tile = FindTile(Idx.KeyPos);

		// 높이가 2이상인 경우
		// 벽타일인 경우
		// 타일이 없는 경우
		if (nullptr == _Tile)
		{
			continue;
		}

		// 이 타일은 오픈 리스트에 들어있는 타일이다.
		if (OpenListSet.end() != OpenListSet.find(_Tile))
		{
			continue;
		}

		// 이타일은 클로즈 리스트에 들어있는 타일이다.
		if (CloseList.end() != CloseList.find(_Tile->Index.KeyPos))
		{
			continue;
		}

		// std::find()
		// 아직 멀었네.
		// 여기도 바꿈.
		// 만약 앤드 타일이면 리턴함.
		HPTR<JINODE> Node = NewNode(_Node, _Tile, _End);
		AddOpenList(Node);

		if (Node->Tile == _End)
		{
			return Node;
		}
	}

	return nullptr;
}

void JITILEMAP::PathReset() 
{
	OpenListSet.clear();
	{
		std::multimap<float, HPTR<HPATHNODE>, std::less<float>>::iterator Start = OpenList.begin();
		std::multimap<float, HPTR<HPATHNODE>, std::less<float>>::iterator End = OpenList.end();

		for (; Start != End; ++Start)
		{
			NodePool.push_back(Start->second);
		}

		OpenList.clear();
	}
	{
		std::map<__int64, HPTR<HPATHNODE>>::iterator Start = CloseList.begin();
		std::map<__int64, HPTR<HPATHNODE>>::iterator End = CloseList.end();

		for (; Start != End; ++Start)
		{
			NodePool.push_back(Start->second);
		}
		CloseList.clear();
	}

}

std::list<JITILEMAP::HGAMETILE*> JITILEMAP::AStarPathFind(Vector4 _Start, Vector4 _End) 
{
	HPTR<JITILEMAP::HGAMETILE> StartTile = FindTile(_Start);
	HPTR<JITILEMAP::HGAMETILE> EndTile = FindTile(_End);

	// 이것도 추가 예외처리
	if (nullptr == StartTile || nullptr == EndTile)
	{
		return std::list<HGAMETILE*>();
	}

	if (StartTile == EndTile)
	{
		return std::list<HGAMETILE*>();
	}

	// 여기서 터졌음 마지막에
	// JINODE* Node = NewNode(nullptr, StartTile, EndTile);
	HPTR<JINODE> Node = NewNode(nullptr, StartTile, EndTile);
	AddOpenList(Node);

	HPTR<JINODE> LastNode = nullptr;

	while (0 != OpenList.size())
	{
		LastNode = OpenNodeFind(FirstOpenPop(), EndTile);

		if (0 == NodePool.size())
		{
			break;
		}

		if (nullptr != LastNode)
		{
			break;
		}
	}

	std::list<HGAMETILE*> NewList;

	if (nullptr != LastNode)
	{
		while (nullptr != LastNode->Parent)
		{
			NewList.push_front(LastNode->Tile);
			LastNode = LastNode->Parent;
		}
	}

	// 다 풀로 다시 돌려줘야 한다.
	PathReset();

	return NewList;
	// 역으로 돌아가는 길이 나온다.
	// 

}

void JITILEMAP::BaseRender(const HPTR<Camera>& _CAM)
{


	TileStart = m_TileMap.begin();
	TileEnd = m_TileMap.end();

	// 모든 타일의 부모역할
	UpdateTransform();
	Matrix W = GetWorldMatrix();
	Matrix V = _CAM->GetViewMatrix();
	Matrix P = _CAM->PMAT();
	Matrix WVP = W * V * P;

	m_HVTXSHADER->Update();
	m_HPIXSHADER->Update();
	GraphicDevice::MainContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_SMP->Update(HSHADER::SHADERTYPE::SHT_PS, 0);
	m_DSSTATE->Update();
	m_RSSTATE->Update();
	m_HMESH->Update(0, 0);
	m_BLEND->Update();

	Vector4 Idx;
	Vector4 Pos;
	Vector4 LFPos;
	Vector4 RFPos;
	Matrix ScaleMAT;
	Matrix PosMAT;
	Matrix ResultMat;

	for (; TileStart != TileEnd; ++TileStart)
	{
		m_TileSprite->Update(HSHADER::SHADERTYPE::SHT_PS, 0);
		ScaleMAT.Scaling(m_TileScale);
		PosMAT.Translation(TileStart->second->Pos);
		ResultMat = ScaleMAT * PosMAT * WVP;
		ResultMat.TransPose();

		m_HCB->DataSetting(&ResultMat, sizeof(Matrix));
		m_HCB->Update();
		TEXUV = m_TileSprite->GetCutData(TileStart->second->SpriteIndex);
		m_HCBUV->DataSetting(&TEXUV, sizeof(TEXUV));
		m_HCBUV->Update();
		m_HMESH->Render(0);

		// InstBuffer->PushData<>

		// TILEINSTDATA Data;
		// Data.MAT = ResultMat; // 트랜스 포즈 하면 안된다.
		// Data.SPRITEUV = TEXUV; // 트랜스 포즈 하면 안된다.
		// HRENMGR::PushInstData<TILEINSTDATA>(L"TileMapInst", Data);

		// 왼쪽
		m_FloorSprite->Update(HSHADER::SHADERTYPE::SHT_PS, 0);
		ScaleMAT.Scaling(m_FloorScale);


		LFPos = TileStart->second->Pos;
		LFPos.x -= m_FloorScale.x * 0.5f;
		LFPos.y -= (m_FloorScale.y * 0.5f)/* - (m_TileScale.y * 0.5f)*/;
		PosMAT.Translation(LFPos);
		ResultMat = ScaleMAT * PosMAT * WVP;
		ResultMat.TransPose();

		TEXUV = m_FloorSprite->GetCutData(TileStart->second->LeftFloorSpriteIndex);
		m_HCBUV->DataSetting(&TEXUV, sizeof(TEXUV));
		m_HCBUV->Update();

		m_HCB->DataSetting(&ResultMat, sizeof(Matrix));
		m_HCB->Update();
		m_HMESH->Render(0);

		//m_HCB->DataSetting(&ResultMat, sizeof(HMAT));
		//m_HCB->Update();

		// 오른쪽

		RFPos = TileStart->second->Pos;
		RFPos.x += m_FloorScale.x * 0.5f;
		RFPos.y -= (m_FloorScale.y * 0.5f)/* - (m_TileScale.y * 0.5f)*/;
		PosMAT.Translation(RFPos);
		ResultMat = ScaleMAT * PosMAT * WVP;
		ResultMat.TransPose();

		TEXUV = m_FloorSprite->GetCutData(TileStart->second->RightFloorSpriteIndex);
		m_HCBUV->DataSetting(&TEXUV, sizeof(TEXUV));
		m_HCBUV->Update();

		m_HCB->DataSetting(&ResultMat, sizeof(Matrix));
		m_HCB->Update();
		m_HMESH->Render(0);
	}

}


void JITILEMAP::InstRender(const HPTR<Camera>& _CAM)
{
	TileStart = m_TileMap.begin();
	TileEnd = m_TileMap.end();

	// 모든 타일의 부모역할
	UpdateTransform();
	Matrix W = GetWorldMatrix();
	Matrix V = _CAM->GetViewMatrix();
	Matrix P = _CAM->PMAT();
	Matrix WVP = W * V * P;

	Vector4 Idx;
	Vector4 Pos;
	Vector4 LFPos;
	Vector4 RFPos;
	Matrix ScaleMAT;
	Matrix PosMAT;
	Matrix ResultMat;

	TILEINSTDATA Data;

	for (; TileStart != TileEnd; ++TileStart)
	{
		m_TileSprite->Update(HSHADER::SHADERTYPE::SHT_PS, 0);
		ScaleMAT.Scaling(m_TileScale);
		PosMAT.Translation(TileStart->second->Pos);
		ResultMat = ScaleMAT * PosMAT * WVP;
		TEXUV = m_TileSprite->GetCutData(TileStart->second->SpriteIndex);

		Data.WVP = ResultMat;
		Data.SPRITEUV = TEXUV;

		InstBuffer->PushData(Data);
	}

}