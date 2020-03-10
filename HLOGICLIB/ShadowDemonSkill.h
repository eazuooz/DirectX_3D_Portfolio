#pragma once
#include <LOGIC.h>


class ShadowDemonSkill :
	public Logic
{

private:
	BoardIndex  m_Index;
	Actor*		pAttTarget;
	Vector4		attDestPos;
	BoardIndex	attDestIdx;
	int attCnt;
	float C_time;

	Actor*		pMoveTarget;
	Vector4		moveDestPos;
	BoardIndex	moveDestIdx;

	bool isRotate;

	Actor* myActor;

	float rotateDegree;

	float curTime;
	float particleTime;

public:
	void Init() override;
	void Update() override;


public:
	ShadowDemonSkill();
	~ShadowDemonSkill();
};

