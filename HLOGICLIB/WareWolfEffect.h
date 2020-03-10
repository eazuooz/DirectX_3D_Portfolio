#pragma once
#include <LOGIC.h>

class FbxAnimation;
class WareWolfEffect : public Logic
{
private:
	float sumTime;

	FbxAnimation*  createdActorAni;

	bool death;

public:

	void SetAniPtr(FbxAnimation* _Ptr)
	{
		createdActorAni = _Ptr;
	}

public:
	void Init() override;
	void Update() override;

public:
	WareWolfEffect();
	~WareWolfEffect();
};

