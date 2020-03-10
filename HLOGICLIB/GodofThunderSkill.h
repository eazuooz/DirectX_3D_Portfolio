#pragma once
#include <LOGIC.h>


class GodofThunderSkill :
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

	int indexZ;
	int indexX;
	bool skillShot;

	bool isRotate;

	Actor* myActor;

	float rotateDegree;

	float curTime;
	float skillTime;

public:
	void Init() override;
	void Update() override;

public:
	GodofThunderSkill();
	~GodofThunderSkill();
};

