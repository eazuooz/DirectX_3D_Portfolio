#pragma once
#include <LOGIC.h>


class ShiningArcherSkill :
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
	bool falldown;

	Actor* myActor;

	float rotateDegree;

	float curTime;
	float particleTime;

public:
	void Init() override;
	void Update() override;


public:
	void SetTarget(Actor* _Actor);
	void SetTarget(Actor* _Actor, float _Degree);

	Vector4 Rotate(BoardIndex _DestIdx, float _Degree);

public:
	ShiningArcherSkill();
	~ShiningArcherSkill();
};

