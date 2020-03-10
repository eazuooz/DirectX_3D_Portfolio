#pragma once
#include "PieceLogic.h"
#include "WareWolfEffect.h"


class Warewolf :public PieceLogic
{
private:
	HPTR<WareWolfEffect> effectLogic;

	bool isEffect;

	HPTR<Actor> effectActor;

public:
	void Update() override;



public:
	void MakeProjecTile() override;
	void Skill() override;
	void CreateCreature();

public:
	Warewolf();
	~Warewolf();
};

