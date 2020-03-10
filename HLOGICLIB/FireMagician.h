#pragma once
#include "PieceLogic.h"


class FireMagician :
	public PieceLogic
{

public:
	void Update() override;

public:
	void MakeProjecTile() override;
	void Skill() override;
public:
	FireMagician();
	~FireMagician();
};

