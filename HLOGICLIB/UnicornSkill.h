#pragma once
#include <LOGIC.h>


class UnicornSkill :	public Logic
{
private:
	float curTime;
	float continueTime;



	float skillTime;

	bool loopStart;

public:
	void SetContinueTime(float _Time)
	{
		continueTime = _Time;
	}

public:
	void Init() override;
	void Update() override;

public:
	UnicornSkill();
	~UnicornSkill();
};

