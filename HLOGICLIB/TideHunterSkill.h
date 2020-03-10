#pragma once
#include <LOGIC.h>


class TideHunterSkill :
	public Logic
{

private:
	BoardIndex  m_Index;
	Actor*		pAttTarget;
	Vector4		attDestPos;
	BoardIndex	attDestIdx;

	Actor*		pMoveTarget;
	Vector4		moveDestPos;
	BoardIndex	moveDestIdx;

	bool isRotate;

	Actor* myActor;

	float rotateDegree;

	float curTime;
	float skillTime;
	bool skillOn;

	std::vector<HPTR<Actor>> tenta;
	int tentacleIdx;
	int idxX;
	int idxZ;

public:
	void Init() override;
	void Update() override;

public:
	void SetTarget(Actor* _Actor);
	void SetTarget(Actor* _Actor, float _Degree);


public:
	TideHunterSkill();
	~TideHunterSkill();
};

class TentacleSkill :
	public Logic
{

private:
	BoardIndex  m_Index;
	Actor*		pAttTarget;
	Vector4		attDestPos;
	BoardIndex	attDestIdx;

	Actor*		pMoveTarget;
	Vector4		moveDestPos;
	BoardIndex	moveDestIdx;

	Actor* myActor;

	float curTime;

public:
	void Init() override;
	void Update() override;

public:
	void SetTarget(Actor* _Actor);


public:
	TentacleSkill();
	~TentacleSkill();
};
