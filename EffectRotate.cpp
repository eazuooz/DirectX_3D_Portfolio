#include "EffectRotate.h"

#include <JTIME.h>

void EffectRotate::Init()
{

}

void EffectRotate::Update()
{

	curTime += Time::DeltaTime();
	
	if (dir == 1)  //y회전
	{
		GetActor()->GetTransform()->SetLocalRotY(rotSpeed * curTime);
	}
	else if (dir == 2) //x회전
	{
		GetActor()->GetTransform()->SetLocalRotX(rotSpeed * curTime);
	}
	else if (dir == 3) //z 회전
	{
		GetActor()->GetTransform()->SetLocalRotZ(rotSpeed * curTime);
	}
	

}


EffectRotate::EffectRotate() : curTime(0.0f), dir(1), rotSpeed(500.0f)
{
	
}


EffectRotate::~EffectRotate()
{

}
