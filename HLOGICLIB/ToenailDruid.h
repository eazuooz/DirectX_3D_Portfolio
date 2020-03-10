#pragma once
#include "PieceLogic.h"
class ToenailDruid :
	public PieceLogic
{

public:
	void Update() override;

public:
	void MakeProjecTile() override;
	void Skill() override;


public:
	ToenailDruid();
	~ToenailDruid();
};

