#pragma once
#include "PieceLogic.h"


class LittleDragon :
	public PieceLogic
{

public:
	void Update() override;

public:
	void MakeProjecTile() override;
	void Skill() override;



public:
	LittleDragon();
	~LittleDragon();
};

