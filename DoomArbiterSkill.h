#pragma once
#include <LOGIC.h>


class DoomArbiterSkill :
	public Logic
{

private:
	BoardIndex  m_Index;
	Actor*		pAttTarget;
	Vector4		attDestPos;
	BoardIndex	attDestIdx;
	int attCnt;

	Actor*		pMoveTarget;
	Vector4		moveDestPos;
	BoardIndex	moveDestIdx;

	bool isRotate;

	Actor* myActor;

	float rotateDegree;

	float curTime;
	float divTime;

public:
	void Init() override;
	void Update() override;


public:
	void SetTarget(Actor* _Actor);
	void SetTarget(Actor* _Actor, float _Degree);

	Vector4 Rotate(BoardIndex _DestIdx, float _Degree);

public:
	DoomArbiterSkill();
	~DoomArbiterSkill();
};

