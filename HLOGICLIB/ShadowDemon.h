#pragma once
#include "PieceLogic.h"


class ShadowDemon :
	public PieceLogic
{

public:
	void Update() override;

public:
	void MakeProjecTile() override;
	void Skill() override;
	bool skillOn;

public:
	ShadowDemon();
	~ShadowDemon();
};

