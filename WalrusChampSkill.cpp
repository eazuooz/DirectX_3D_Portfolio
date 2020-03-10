#include "WalrusChampSkill.h"

#include <RANDOM.h>
#include "ChessBoard.h"
#include "StatusLogic.h"
#include <SCENE.h>
#include "LOGICHEADER.h"
#include <2DANICOM.h>
#include "BaseEffect.h"
#include <StaticFbxRender.h>

#include "EffectRotate.h"

void WalrusChampSkill::Init()
{
}

void WalrusChampSkill::Update()
{
	curTime += Time::DeltaTime();

	if (curTime > 0.25f)
	{
		skillTime += Time::DeltaTime();

		if (skillTime < 0.75f)
		{
			if (flyAway == 0)
			{
				flyAway = 1;
			}
			pMoveTarget->GetTransform()->SetLoclaPositionY(skillTime * 3000.0f);
		}
		else if (skillTime < 1.5f)
		{
			pMoveTarget->GetTransform()->SetLoclaPositionY((1.5f - skillTime) * 3000.0f);
		}
		else
		{
			pMoveTarget->GetTransform()->SetLoclaPositionY(0.0f);
		}
	}

	if (flyAway == 1)
	{
		flyAway = 2;
		/*StatusLogic* statusLogic = pMoveTarget->GetComponent<StatusLogic>();
		if (statusLogic->GetType() == PIECETYPE::ENEMY)
		{
			statusLogic->AddHp(-150);
		}*/

		PLAYER2 = myActor->GetScene()->CreateActor();

		PLAYER2->GetTransform()->SetLoclaPosition(moveDestPos + Vector4({ 0.0f, 20.0f, 0.0f }));
		PLAYER2->GetTransform()->SetLocalScale({ 200.0F, 200.0F, 200.0F });

		battleTopFrameSR = PLAYER2->AddComponent<SpriteRenderer>(RG_PLAYER);
		battleTopFrameSR->SetSprite(L"Bang.png", 0);
		battleTopFrameSR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		battleTopFrameSR->LinkCB(L"VS_CUTDATA", &(battleTopFrameSR->GetCutData()));
		battleTopFrameSR->LinkTexture(L"PS_MAPTEX", L"Bang.png");
		battleTopFrameSR->LinkSampler(L"PS_LSMP", L"LSMP");
		battleTopFrameSR->LinkSampler(L"PS_PSMP", L"PSMP");
		battleTopFrameSR->BillBoardOn();
	}
	if (flyAway == 2)
	{
		if (skillTime < 0.5f)
			battleTopFrameSR->SetLocalScale({ 300.0F * skillTime, 175.0F * skillTime, 1.0F, 1.0f });
		else if (skillTime > 1.2f)
			PLAYER2->Die();
	}


	if (curTime > 3.0f)
	{
		myActor->Die();
	}
}

void WalrusChampSkill::SetTarget(Actor* _Actor)
{
	myActor = GetActor();

	Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
	m_Index = ChessBoard::Inst().CalculateIndex(myPos);

	pMoveTarget = _Actor;
	moveDestPos = _Actor->GetTransform()->GetLocalPosition();
	moveDestIdx = ChessBoard::Inst().CalculateIndex(moveDestPos);
}

void WalrusChampSkill::SetTarget(Actor* _Actor, float _Degree)
{
	myActor = GetActor();

	rotateDegree = _Degree;

	Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
	m_Index = ChessBoard::Inst().CalculateIndex(myPos);

	moveDestPos = _Actor->GetTransform()->GetLocalPosition();
	moveDestIdx = ChessBoard::Inst().CalculateIndex(moveDestPos);
}

WalrusChampSkill::WalrusChampSkill() : isRotate(false), myActor(nullptr), curTime(0.0f), skillTime(0.0f), C_time(1.0f), flyAway(0)
{
}


WalrusChampSkill::~WalrusChampSkill()
{
}
