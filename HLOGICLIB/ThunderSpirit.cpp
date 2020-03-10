#include "ThunderSpirit.h"

#include <math.h>
#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "ProjectTileLogic.h"

#include "EffectRotate.h"
#include <RANDOM.h>
#include "ThunderSpiritSkill.h"

#include <SOUND.h>

void ThunderSpirit::Update()
{

	if (m_State != PieceLogic::BORN && statusLogic->GetHp() < 0)
	{
		m_State = PIECESTATE::DIE;
	}

	switch (m_State)
	{
	case PieceLogic::BORN:
	{
		Born();
	}
	break;
	case PieceLogic::IDLE:
	{
		Idle();
	}
	break;
	case PieceLogic::FINDATTACKTARGET:
	{
		FindAttTarget();
	}
	break;
	case PieceLogic::ATTACK:
	{
		Attack();
	}
	break;
	case PieceLogic::SKILL:
	{
		Skill();
	}
	break;
	case PieceLogic::FINDMOVETARGET:
	{
		FindTransTarget();
	}
	break;
	case PieceLogic::RUN:
	{
		Run();
	}
	break;
	case PieceLogic::JUMP:
	{
		Jump();
	}
	break;
	case PieceLogic::DIE:
	{
		Die();
	}
	break;
	case PieceLogic::DEAD:
	{
		Dead();
	}
	break;
	case PieceLogic::VICTORY:
	{
		Victory();
	}
	break;
	case PieceLogic::STAY:
	{
		Stay();
	}
	break;
	}

}

void ThunderSpirit::MakeProjecTile()
{
	if (pAttTarget != nullptr)
	{
		Vector4 createPos = myActor->GetTransform()->GetLocalPosition();

		HPTR<Actor> PLAYER2 = GetScene()->CreateActor();
		PLAYER2->GetTransform()->SetLoclaPosition(createPos);
		PLAYER2->GetTransform()->SetLocalScale({ 100.0F, 100.0F, 1.0F });
		PLAYER2->GetTransform()->SetLocalRotX(90.0f);

		HPTR<SpriteRenderer> battleTopFrameSR = PLAYER2->AddComponent<SpriteRenderer>(RG_PLAYER);
		battleTopFrameSR->SetSprite(L"flameMagicionLight.tga", 0);
		battleTopFrameSR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		battleTopFrameSR->LinkCB(L"VS_CUTDATA", &(battleTopFrameSR->GetCutData()));
		battleTopFrameSR->LinkTexture(L"PS_MAPTEX", L"flameMagicionLight.tga");
		battleTopFrameSR->LinkSampler(L"PS_LSMP", L"LSMP");
		battleTopFrameSR->LinkSampler(L"PS_PSMP", L"PSMP");

		HPTR<SpriteAnimaion> battleTopFrameSRA = PLAYER2->AddComponent<SpriteAnimaion>();
		battleTopFrameSRA->CreateAni(L"LOOP", L"flameMagicionLight.tga", 0, 5, 0.01f);

		HPTR<ProjectTileLogic> projectLogic2 = PLAYER2->AddComponent<ProjectTileLogic>();
		projectLogic2->SetDegree(180.0f);
		projectLogic2->SetTarget(pAttTarget);

		Sound::Play(L"DianGun_Attack_1.wav");
	}
}

void ThunderSpirit::Skill()
{
	CurPosIdxCheck();

	if (skillOn == false)
	{
		skillOn = true;

		HPTR<Actor> newActor = GetScene()->CreateActor(L"Shock");
		Vector4 skillPos = myActor->GetTransform()->GetLocalPosition();
		Vector4 skillRot = myActor->GetTransform()->GetLocalRotation();

		skillPos.y += 50.0f;
		newActor->GetTransform()->SetPosition(skillPos);
		newActor->GetTransform()->SetLocalScale({ 1.2f, 1.2f, 1.2f });

		HPTR<SpriteRenderer> skillRender = newActor->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		skillRender->SetLoclaPosition({ 0.0F, 1.0f, 0.0f });
		skillRender->SetLocalScale({ 50.0f, 50.0f, 1.0f, 1.0f });
		skillRender->SetLocalRotX(90.0f);
		skillRender->SetSprite(L"thunderCircleEffect.tga");
		skillRender->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		skillRender->LinkCB(L"VS_CUTDATA", &(skillRender->GetCutData()));
		skillRender->LinkTexture(L"PS_MAPTEX", L"thunderCircleEffect.tga");
		skillRender->LinkSampler(L"PS_LSMP", L"LSMP");

		HPTR<SpriteAnimaion> skillAni = newActor->AddComponent<SpriteAnimaion>();
		skillAni->CreateAni(L"LOOP", L"thunderCircleEffect.tga", 0, 21, 0.01f, true);

		HPTR<ThunderSpiritSkill> skillLogic = newActor->AddComponent<ThunderSpiritSkill>();
		Sound::Play(L"DianGun_Attack_1.wav");
	}

	if (pFbxAnimation->GetCurClip()->m_AniEnd == true)
	{
		skillOn = false;

		statusLogic->SetMp(0.0f);
		pFbxAnimation->ChangeAnimation(L"IDLE");
		m_State = PIECESTATE::IDLE;

		return;
	}

}

ThunderSpirit::ThunderSpirit() : skillOn(false)
{
}


ThunderSpirit::~ThunderSpirit()
{
}
