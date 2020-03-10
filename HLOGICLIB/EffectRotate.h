#pragma once
#include <LOGIC.h>




class EffectRotate :
	public Logic
{


private:
	float curTime;
	int dir;
	float rotSpeed;


public:
	void SetDir(int _Dir)
	{
		dir = _Dir;
	}

	void SetRotSpeed(float _Value)
	{
		rotSpeed = _Value;
	}

public:
	void Init() override;
	void Update() override;

public:
	EffectRotate();
	~EffectRotate();
};

