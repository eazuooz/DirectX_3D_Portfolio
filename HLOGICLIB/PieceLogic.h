#pragma once
#include <LOGIC.h>
#include <map>
#include <list>

//

#include <3DANIRENDER.h>


class StatusLogic;
class PieceLogic : public Logic
{
protected:
	StatusLogic* statusLogic;
	Scene* myScene;

private:
	class PieceNode
	{
	public:
		BoardIndex pos;
		int startCost;  //출발점에서부터 지금위치까지의 비용 가로세로는 10 , 대각선 14
		int sumCost;	//start + dest 비용의 합 최종 길찾기할떄 이용한다
		int destCost;	//도착점으로부터 지금위치까지으 비용 (벽은 무시한다.)
		bool iSWall;

	public:
		PieceNode() : startCost(0), sumCost(0), destCost(0), iSWall(false)
		{
			
		}
	};

private:
	static std::vector<std::vector<bool>> m_MapStaticInfo;
	static std::vector<std::vector<bool>> m_MapStaticMonster;

	

public:
	static void InitMapStaticInfo()
	{
		m_MapStaticInfo.resize(8);	//넉넉하게 50까지 만들어둔다
		for (int z = 0; z < 8; ++z)
		{
			m_MapStaticInfo[z].resize(8);
		}

		for (int z = 0; z < 8; ++z)
		{
			for (int x = 0; x < 8; ++x)
			{
				m_MapStaticInfo[z][x] = false;
			}
		}

		m_MapStaticMonster.resize(8);	//넉넉하게 50까지 만들어둔다
		for (int z = 0; z < 8; ++z)
		{
			m_MapStaticMonster[z].resize(8);
		}

		for (int z = 0; z < 8; ++z)
		{
			for (int x = 0; x < 8; ++x)
			{
				m_MapStaticMonster[z][x] = false;
			}
		}
	}

	static void ResetMapStaticInfo();
		
	static void PushPieceInfo(Vector4 _Pos);
	static void PushMonsterInfo(Vector4 _Pos);

	void ResetMyPath();

protected:
	std::vector<std::vector<PieceNode>> m_MapInfo;
	std::multimap<int /*sumCost*/, PieceNode> m_OpenList;	
	std::list<PieceNode> m_CloseList;

	Actor* myActor;

public:
	std::list<PieceNode> FindPath(const BoardIndex _DestPos);
	void OverlapPathCheck();
	void CurPosIdxCheck();

	Actor* FindAttackTarget();
	Actor* FindEnemyTarget();


public:

	enum PIECESTATE
	{
		BORN,
		IDLE,
		FINDATTACKTARGET,
		ATTACK,
		FINDMOVETARGET,
		RUN,
		JUMP,
		SKILL,
		DIE,		//죽기전
		DEAD,		//완전히 죽은훈
		VICTORY,
		STAY,
	};

protected:
	//Actor* pActor;
	FbxAnimation* pFbxAnimation;
	std::list<PieceNode> path;

	std::wstring m_CharName;
	PIECESTATE m_State;
	BoardIndex m_Index;
	BoardIndex m_StartIndex;
	//PIECETYPE m_Type;

private:
	bool	isEnemyFind;
	bool	isAttackFind;
	int		m_Range;

	bool isStay;
	bool isDie;


	bool realDead;

protected:
	bool isPassive;

protected:
	bool isMonster;

public:
	void SetRange(int _Range)
	{
		m_Range = _Range;
	}

public:
	BoardIndex	CalculateIndex(Vector4 _Pos);
	void		SetName(const wchar_t* _CharName)
	{
		m_CharName = _CharName;
	}

public:
	void Init() override;
	void Update() override;

public:
	void Born();
	void Idle();
	void FindAttTarget();
	void Attack();

	

	void FindTransTarget();
	void Run();
	void Jump();
	void Die();
	void Dead();
	virtual void Victory();
	void Stay();
	
	virtual void MakeProjecTile() {} ;
	virtual void Skill() {};
	virtual void Passive() {};
	
	void SkillCheck();


private:
	bool makeProjectTile;
protected:
	Actor*		pAttTarget;

private:
	bool		isRun;
	bool		isJump;
	bool		isRotate;

	Vector4		attDestPos;
	BoardIndex	attDestIdx;
	int attCnt;

	Actor*		pMoveTarget;
	Vector4		moveDestPos;
	BoardIndex	moveDestIdx;

	//
#pragma region MOVE
	Vector4 j_destPos;
	Vector4 j_StartPos;

	float vX; // x축방향의속도
	float vY; // y축방향의속도
	float vZ; // z축방향의속도
	float g; // y축방향의 중력가속도
	float endTime;// 도착점 도달시간
	float maxHeight; //최고점 높이
	float height; // 최고점 - 시작점(높이)
	float endHeight;//도착점 높이
	float time; //진행시간
	float maxTime; //최고점 도달시간
#pragma endregion

	float deadTime;


public:
	void	ParabolaCalculate(float _MaxTime, float _MaxHeight);
	Vector4 JumpCalculate();
	Vector4 Rotate(BoardIndex	_DestIdx);


//private:
//	int hp;
//	int mp;
//	int att;
//
//	//스킬 쿨타임 현재타임, 최대타임
//	float curTime;
//	float maxTime;

public:
	PieceLogic();
	~PieceLogic();
};

