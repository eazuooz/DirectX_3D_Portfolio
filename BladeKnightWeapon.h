#pragma once
#include "ProjectTileLogic.h"

class BladeKnightWeapon :
	public ProjectTileLogic
{
private:
	int attCnt;

public:
	void Init() override;
	void Update() override;

public:
	BladeKnightWeapon();
	~BladeKnightWeapon();
};

