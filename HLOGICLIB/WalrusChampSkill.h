#pragma once
#include <LOGIC.h>


class WalrusChampSkill :
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
	float skillTime;
	int flyAway;

public:
	void Init() override;
	void Update() override;

	void SetTarget(Actor* _Actor);
	void SetTarget(Actor* _Actor, float _Degree);
	HPTR<Actor> PLAYER2;
	HPTR<SpriteRenderer> battleTopFrameSR;


public:
	WalrusChampSkill();
	~WalrusChampSkill();
};


