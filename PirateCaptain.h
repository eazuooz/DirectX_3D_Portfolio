#pragma once
#include "PieceLogic.h"


class PirateCaptain :
	public PieceLogic
{

public:
	void Update() override;

public:
	void MakeProjecTile() override;
	void Skill() override;


public:
	PirateCaptain();
	~PirateCaptain();
};

