#pragma once
#include "PieceLogic.h"
class MonsterProjectLogic :
	public PieceLogic
{
	private:
	bool isRock;
	float rockTime;

public:
	void MakeProjecTile() override;


public:
	
	void Update() override;

public:
	MonsterProjectLogic();
	~MonsterProjectLogic();
};

