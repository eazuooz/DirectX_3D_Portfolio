#pragma once
#include "PieceLogic.h"
class TideHunter :
	public PieceLogic
{

public:
	void Update() override;

public:
	void MakeProjecTile() override;
	void Skill() override;

	bool skillOn;
	float skillTime;

	int indexX;
	int indexZ;
	int indexNum;

	BoardIndex targetIdx;
	std::vector<HPTR<Actor>> tentacle;


public:
	TideHunter();
	~TideHunter();
};

