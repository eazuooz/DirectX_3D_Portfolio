#pragma once
#include "PieceLogic.h"
class SwordMaster :
	public PieceLogic
{
private:
	bool skillStart;

	float curTime;
	int timeCnt;

	HPTR<Actor> effectActor;
public:
	void Update() override;

public:
	void MakeProjecTile() override;
	void Skill() override;


public:
	SwordMaster();
	~SwordMaster();
};

