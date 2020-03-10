#pragma once
#include <LOGIC.h>


class BaseEffect :
	public Logic
{
private:
	float curTime;

public:
	void Init() override;
	void Update() override;

public:
	BaseEffect();
	~BaseEffect();
};

