#pragma once
#include "PieceLogic.h"


class DemonLarva :
	public PieceLogic
{

public:
	void Update() override;

public:
	void MakeProjecTile() override;
	void Skill() override;
public:
	DemonLarva();
	~DemonLarva();
};

