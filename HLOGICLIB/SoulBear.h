#pragma once
#include "PieceLogic.h"
class SoulBear :
	public PieceLogic
{
public:
	void Victory() override;

public:
	void Init() override;
	void Update() override;

public:
	void MakeProjecTile() override;
	void Skill() override;
public:
	void Passive() override;


public:
	SoulBear();
	~SoulBear();
};