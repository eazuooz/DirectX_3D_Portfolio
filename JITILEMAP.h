#pragma once
#include <RENDER.h>
#include <map>
#include <stack>

class TILEINSTDATA 
{
public:
	Matrix WVP;
	Vector4 SPRITEUV;
	// int SpriteIndex;
};

class HMESH;
class HCB;
class HVTXSHADER;
class HPIXSHADER;
class HSMP;
class Sprite;
class HBLEND;
class HDSSTATE;
class HRSSTATE;
class Instancing;
class JITILEMAP : public Renderer
{
public:

	// 3번 랜더해야 한다.
	// 바닥타일 하나.
	// 왼쪽 타일 하나.
	// 오른쪽 타일 하나.

	// 바이트 패딩
	// 8 + 3 절대로 이렇게 안맞춘다.
	// 64바이트 넘어가면
	// HREF
	class HGAMETILE : public Ref
	{
	public:
		Vector4 Index;
		Vector4 BasePos;
		Vector4 Pos;
		bool IsMove; // 4(이 클래스 내부에서 가장 커다란.)
		bool LeftEnd; // 디아블로에는 없어도 된다. 높이
		bool RightEnd; // 디아블로에는 없어도 된다. 높이 4
		int SpriteIndex; // 나의 텍스처 4
		int Floor; // 디아블로에는 없어도 된다. 높이 4
		int LeftFloorSpriteIndex; // 나의 텍스처 4
		int RightFloorSpriteIndex; // 나의 텍스처 4

		// HGAMETILE*

		// 0이면 1로친다.
	public:
		HGAMETILE() : IsMove(true), LeftEnd(false), RightEnd(false), SpriteIndex(0), Floor(0), LeftFloorSpriteIndex(0), RightFloorSpriteIndex(0)
		{}
	};

	class HPATHNODE : public Ref 
	{
	public:
		// 내가 어디서 부터 와서 여기까지 왔느냐.
		// 역으로 돌아가는 길이 나온다.
		JITILEMAP::HPATHNODE* Parent;
		HGAMETILE* Tile;
		float F; // F
		float G; // G 목적지까지 최단거리로 얼마냐?
		float H; // H 여태까지 얼마나 들었냐?

	public:
		void CalValue(JITILEMAP::HPATHNODE* Parent, HGAMETILE* _Tile, HGAMETILE* End);
	};



public:
	HPTR<Instancing> InstBuffer;

	Vector4 TEXUV;
	HPTR<HMESH> m_HMESH;
	HPTR<HCB> m_HCB;
	HPTR<HCB> m_HCBUV;
	HPTR<HVTXSHADER> m_HVTXSHADER;
	HPTR<HPIXSHADER> m_HPIXSHADER;
	HPTR<HSMP> m_SMP;
	HPTR<HBLEND> m_BLEND;
	HPTR<HDSSTATE> m_DSSTATE;
	HPTR<HRSSTATE> m_RSSTATE;

	HPTR<Sprite> m_TileSprite;
	HPTR<Sprite> m_FloorSprite;

	Vector4 m_FloorScale;
	Vector4 m_TileScale;
	Vector4 m_TileMoveSize;

public:
	std::map<__int64, HPTR<HGAMETILE>>::iterator TileStart;
	std::map<__int64, HPTR<HGAMETILE>>::iterator TileEnd;
	std::map<__int64, HPTR<HGAMETILE>> m_TileMap;

	

public:
	void Init() override;
	void Render(const HPTR<Camera>& _CAM) override;

	void InstRender(const HPTR<Camera>& _CAM);
	void BaseRender(const HPTR<Camera>& _CAM);

public:
	void TileMoveSize(Vector4 _TileMoveSize) 
	{
		m_TileScale = _TileMoveSize;
		m_TileScale *= 2.0f;
		m_TileMoveSize = _TileMoveSize; 
	}

	void FloorSize(Vector4 _FloorSize)
	{
		m_FloorScale = _FloorSize;
	}
	void TileSprite(const wchar_t* _Name);
	void FloorSprite(const wchar_t* _Name);

	// FLOAT 으로 인덱스를 넣어줬을때. 
	// 2, 1 번에 타일 추가하세요.
	void AddTileWorld(Vector4 _Index);
	void AddTileF(Vector4 _Index);
	void AddTile(int _X, int _Y);

	HPTR<HGAMETILE> FindTile(Vector4 _Pos);
	HPTR<HGAMETILE> FindTile(__int64 _Pos);
	void ChangeFloor(Vector4 _Pos, int _Floor);
	void ChangeLeftFloor(Vector4 _Pos, int _Floor);
	void ChangeRightFloor(Vector4 _Pos, int _Floor);

private:
	void AddTile(__int64 _Index);

	

	// 요꼴이 된다.
	// SIZE가 0이면 길이 없다.

public:
	Vector4 PosToIndex(Vector4 _Pos);
	Vector4 IndexToPos(Vector4 _Pos);





	// 
private:
	std::list<HPTR<HPATHNODE>> NodePool;
	// , std::less<float>
	std::set<HGAMETILE*> OpenListSet;
	std::multimap<float, HPTR<HPATHNODE>, std::less<float>> OpenList;
	std::map<__int64, HPTR<HPATHNODE>> CloseList;

public:
	void AStarMaxSetting(int _Count);

private:
	HPTR<HPATHNODE> NewNode(JITILEMAP::HPATHNODE* Parent, HGAMETILE* _Tile, HGAMETILE* End);

	HPTR<HPATHNODE> FirstOpenPop();
	void PathReset();

	void AddOpenList(HPTR<HPATHNODE> _NewNode);
	void PushNode(HPTR<HPATHNODE> _Node);
	HPTR<JITILEMAP::HPATHNODE> OpenNodeFind(HPTR<HPATHNODE> _Node, HGAMETILE* _End);

///////////////// Astar 전용
public:
	std::list<HGAMETILE*> AStarPathFind(Vector4 _Start, Vector4 _End);



public:
	JITILEMAP();
	~JITILEMAP();
};


typedef JITILEMAP::HGAMETILE JITILE;
typedef JITILEMAP::HPATHNODE JINODE;