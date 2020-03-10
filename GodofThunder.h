#pragma once
#include "PieceLogic.h"


class GodofThunder :
	public PieceLogic
{

public:
	void Update() override;

public:
	void MakeProjecTile() override;
	void Skill() override;

	bool skillOn;

public:
	GodofThunder();
	~GodofThunder();
};

