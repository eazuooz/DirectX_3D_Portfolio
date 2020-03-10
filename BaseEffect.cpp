#include "BaseEffect.h"
#include <JTIME.h>


void BaseEffect::Init()
{
}

void BaseEffect::Update()
{
	curTime += Time::DeltaTime();

	if (curTime >= 2.5f)
	{
		GetActor()->Die();
	}
}

BaseEffect::BaseEffect() : curTime(0.0f)
{
}


BaseEffect::~BaseEffect()
{
}
