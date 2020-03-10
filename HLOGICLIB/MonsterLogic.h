#pragma once
#include "PieceLogic.h"

class MonsterLogic :	public PieceLogic
{
private:
	bool isRock;
	float rockTime;

public:
	void MakeProjecTile() override;


public:
	
	void Update() override;


public:
	MonsterLogic();
	~MonsterLogic();
};

