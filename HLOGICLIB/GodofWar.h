#pragma once
#include "PieceLogic.h"


class GodofWar : public PieceLogic
{
private:
	HPTR<Actor> skillActor;
	HPTR<Actor> newActor;

public:
	void Update() override;

public:
	void Skill() override;
	float skillCount;
	bool skillOn;


public:
	GodofWar();
	~GodofWar();
};

