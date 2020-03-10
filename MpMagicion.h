#pragma once
#include "PieceLogic.h"


class MpMagicion : public PieceLogic
{

private:
	HPTR<Actor> skillActor;
	HPTR<SpriteRenderer> skillSprite;

public:
	void Update() override;

public:
	void MakeProjecTile()	override;
	void Skill()			override;
	void Passive()			override;

private:
	float skillCount;

public:
	MpMagicion();
	~MpMagicion();
};

