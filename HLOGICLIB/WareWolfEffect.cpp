#include "WareWolfEffect.h"

#include <JTIME.h>
#include <3DANIRENDER.h>
#include "ChessBoard.h"

void WareWolfEffect::Init()
{
}

void WareWolfEffect::Update()
{
	sumTime += Time::DeltaTime();

	GetActor()->GetTransform()->LocalScaleSum((sumTime / 5.0f) * Time::DeltaTime());

	if (createdActorAni->GetCurClip()->m_AniEnd == true)
	{
		GetActor()->Die();
		ChessBoard::Inst().WolfEffectDie();
	}
}

WareWolfEffect::WareWolfEffect() : sumTime(0.0f), death(false)
{
}


WareWolfEffect::~WareWolfEffect()
{
}
