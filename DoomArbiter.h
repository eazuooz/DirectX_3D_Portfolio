#pragma once
#include "PieceLogic.h"


class DoomArbiter : public PieceLogic
{
private:
	HPTR<Actor> skillActor;
	HPTR<Actor> newActor;

public:
	void Update() override;

public:
	void MakeProjecTile() override;
	void Skill() override;
	float skillCount;
	bool skillOn;


public:
	DoomArbiter();
	~DoomArbiter();
};


