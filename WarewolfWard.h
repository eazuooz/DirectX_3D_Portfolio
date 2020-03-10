#pragma once
#include "PieceLogic.h"


class WarewolfWard :
	public PieceLogic
{
public:
	void Victory() override;

public:
	void Update() override;

public:
	void Skill() override;
public:
	void MakeProjecTile() override;
	void Passive() override;

public:
	WarewolfWard();
	~WarewolfWard();
};

