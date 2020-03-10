#pragma once
#include "PieceLogic.h"
class WindRanger :
	public PieceLogic
{
private:
	bool isSkillStart;
	HPTR<Actor> chargeActor;

public:
	void Update() override;

public:
	void MakeProjecTile() override;
	void Skill() override;

public:
	WindRanger();
	~WindRanger();
};

