#pragma once
#include "PieceLogic.h"


class Egg :
	public PieceLogic
{

public:
	void Update() override;

public:
	void Skill() override;
public:
	Egg();
	~Egg();
};

