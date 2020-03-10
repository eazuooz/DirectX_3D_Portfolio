#pragma once
#include "PieceLogic.h"
class Siren :
	public PieceLogic
{
private:
	float rockTime;
	bool enemyLock;

	HPTR<Actor> effectActor;
	bool isEffect;

public:
	void Update() override;

public:
	void MakeProjecTile() override;
	void Skill() override;


public:
	Siren();
	~Siren();
};

