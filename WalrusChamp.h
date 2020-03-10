#pragma once
#include "PieceLogic.h"
class WalrusChamp :
	public PieceLogic
{

public:
	void MakeProjecTile() override;
	void Update() override;

public:
	void Skill() override;
	bool skillOn;

public:
	WalrusChamp();
	~WalrusChamp();
};

