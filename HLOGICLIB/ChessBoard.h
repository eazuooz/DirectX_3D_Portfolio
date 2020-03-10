#pragma once
#include <vector>
#include <list>

#include <MATHS.h>
#include <ACTOR.h>


class FbxAnimation;
class StatusLogic;
class ChessBoard
{
private:
	static ChessBoard m_inst;

public:
	static ChessBoard& Inst()
	{
		return m_inst;
	}

private:
	bool wolfEffectEnd;

public:
	bool GetWolfEffectEnd()
	{
		return wolfEffectEnd;
	}
	void WolfEffectDie()
	{
		wolfEffectEnd = true;
	}

public:
	class BoardInfo
	{
	public:
		Actor*			m_Actor;			//그 인덱스 값에 피스만넣어준다
		BoardIndex m_Index;		// x, 0, z, 0
		StatusLogic*		m_statusLogic;
		int m_PieceIdx;
		int m_Level;
		Vector4 m_BoardPos;		// 체스맵의 중앙 포지션위치

		bool isMove;

		void Release()
		{
			m_Actor = nullptr;
			m_Index = { -1, -1 };
			m_statusLogic = nullptr;
			m_PieceIdx = -1;
		}


		BoardInfo() : m_Actor(nullptr), m_statusLogic(nullptr), m_PieceIdx(-1), m_Level(0), isMove(false)
		{

		}
	};
	std::vector<std::vector<BoardInfo>> m_Board;	
	std::vector<BoardInfo> m_BoardStay;
	

private:
	Scene* curScene;
	bool eggCheck;

public:
	bool EggCheck()
	{
		return eggCheck;
	}

	BoardIndex  CalculateIndex(Vector4 _Pos)
	{
		BoardIndex index;

		// 124x-496 ~  124x-372
		//-124z+496 ~ -124z+372

		index.x = (int)((_Pos.x + 496.0f) / 124.0f);
		index.z = (int)((_Pos.z - 496.0f) / -124.0f);


		return index;
	}

	BoardIndex  CalculatePickIngIndex();
	


public:
	void SetScene(Scene* _Scene)
	{
		curScene = _Scene;
	}

private:
	std::list<Actor*> m_AllyList;
	std::list<Actor*>::iterator m_AllyBegin;
	std::list<Actor*>::iterator m_AllyEnd;

	std::list<Actor*> m_EnemyList;
	std::list<Actor*>::iterator m_EnemyBegin;
	std::list<Actor*>::iterator m_EnemyEnd;
	

public:
	void InitPtr();
	bool CheckAllyPiece(Actor* _Actor);


	void PushAllyPiece( Actor* _Actor);
	void PushEnemyPiece( Actor* _Actor);

	void PopAllyPiece(Actor* _Actor);
	void PopEnemyPiece(Actor* _Actor);
	void PopPiece(Actor* _Actor);

	void PushActorInBoard(BoardIndex _Index, Actor* _Actor);
	void PushActorInBoard(BoardIndex _Index, Actor* _Actor, StatusLogic* _Logic);

	Actor* GetActor(BoardIndex _Index);
	void SetPosActor(Actor* _Actor, BoardIndex _Index);

	void PushActorInStay(BoardIndex _Start, BoardIndex _Dest, Actor* _Actor);
	Actor* PopActorInStay(int _Idx);
	bool PushActorInStayAutoSort(Actor* _Actor, int _Idx);


	BoardIndex GetIdxByActor(Actor* _Actor);
	size_t GetBoardStaySize();

	bool CheckPieceMoving(Actor* _Actor);

	void EnemyRockMaterialOn();
	void EnemyRockMaterialOff();

private:
	std::vector<int> m_UpgradeTable;
	std::vector<std::vector<int>> m_UpgradeCheckIdx;


	

public:
	BoardIndex CheckUpgrade();
	void UpgradePiece(int _Idx, int _Level);

	void CheatUpgradePiece(BoardIndex _Idx);

public:
	void Init();
	void Update();
	
public:
	void SetActor(BoardIndex _Idx, Actor* _Actor);
	void SetActor(int _X, int _Y, Actor* _Actor);

public:
	HPTR<Actor> CreatePieceLevel2(int _Idx);
	HPTR<Actor> CreatePieceLevel3(int _Idx);


public:
	void MapInfoInit();

public:
	ChessBoard();
	~ChessBoard();
};

