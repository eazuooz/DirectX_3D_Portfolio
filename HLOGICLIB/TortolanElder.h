#pragma once
#include "PieceLogic.h"
class TortolanElder :
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
	TortolanElder();
	~TortolanElder();
};

