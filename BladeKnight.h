#pragma once
#include "PieceLogic.h"



class BladeKnight :
	public PieceLogic
{

public:
	void Update() override;

public:
	void MakeProjecTile() override;
	void Skill() override;
	void Passive() override;
	
public:
	BladeKnight();
	~BladeKnight();
};

