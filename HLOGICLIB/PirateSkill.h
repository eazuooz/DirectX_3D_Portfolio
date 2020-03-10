#pragma once
#include <LOGIC.h>

class PirateSkill :
	public Logic
{
private:
	bool isJumping;
	float particleTime;

public:
	void SetTarget(Actor* _Actor);


protected:
	bool		isRun;
	bool		isJump;
	bool		isRotate;

	BoardIndex  m_Index;
	Actor*		pAttTarget;
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

	float degree;

public:
	void SetDegree(float _Degree)
	{
		degree = _Degree;
	}

public:
	Actor* FindNextTarget();


public:

	void	ParabolaCalculate(float _MaxTime, float _MaxHeight);


	Vector4 JumpCalculate();
	Vector4 Rotate(BoardIndex	_DestIdx, float _Degree);

	void Jump();

public:
	void Init() override;
	void Update() override;
public:
	PirateSkill();
	~PirateSkill();
};

