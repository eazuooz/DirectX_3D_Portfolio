#include "Unicorn.h"

#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "UnicornSkill.h"
#include "ProjectTileLogic.h"
#include <SOUND.h>

void Unicorn::Update()
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

void Unicorn::MakeProjecTile()
{
	if (pAttTarget != nullptr)
	{
		HPTR<Actor> newActor = GetScene()->CreateActor();
		Vector4 createPos = myActor->GetTransform()->GetLocalPosition();
		//createPos.y += 100.0f;
		newActor->GetTransform()->SetLoclaPosition(createPos);
		newActor->GetTransform()->SetLocalScale({ 1.0F , 1.0f, 1.0f });

		HPTR<StaticFbxRender> staticRender = newActor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Unicorn_Weapon.FBXDATA");
		staticRender->SetShadow(true);

		HPTR<ProjectTileLogic> projectLogic = newActor->AddComponent<ProjectTileLogic>();
		projectLogic->SetTarget(pAttTarget);

		Sound::Play(L"XiaoLu_Attack_2.wav");
	}

}

void Unicorn::Skill()
{
	CurPosIdxCheck();
	if (pFbxAnimation->GetCurClip()->m_AniEnd == true)
	{
		
		HPTR<Actor> newActor = GetScene()->CreateActor(L"BUTTERFLY");
		Vector4 skillPos = myActor->GetTransform()->GetLocalPosition();
		skillPos.y += 0.1f;
		newActor->GetTransform()->SetPosition(skillPos);
		newActor->GetTransform()->SetLocalScale({1.5f, 1.0f, 1.5f});

		HPTR<SpriteRenderer> skillRender = newActor->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		skillRender->SetSprite(L"UnicornHeal.tga", 2);
		skillRender->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		//battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
		skillRender->LinkCB(L"VS_CUTDATA", &(skillRender->GetCutData()));
		skillRender->LinkTexture(L"PS_MAPTEX", L"UnicornHeal.tga");
		skillRender->LinkSampler(L"PS_LSMP", L"LSMP");
		skillRender->LinkSampler(L"PS_PSMP", L"PSMP");

		skillRender->SetLoclaPosition({ 0.0F, 1.0F, 0.0f });
		skillRender->SetLocalScale({ 500.0F, 500.0F, 1.0F, 1.0f });
		skillRender->SetLocalRotX(90.0f);

		HPTR<SpriteAnimaion> skillAni = newActor->AddComponent<SpriteAnimaion>();
		skillAni->CreateAniRoll(L"LOOP", L"UnicornHeal.tga", 2, 77, 0.03f);

		HPTR<UnicornSkill> skillLogic = newActor->AddComponent<UnicornSkill>();
		skillLogic->SetContinueTime(statusLogic->GetContinueTime());

		statusLogic->InitCurTime();
		statusLogic->SetMp(0.0f);
		pFbxAnimation->ChangeAnimation(L"IDLE");
		m_State = PIECESTATE::IDLE;

		Sound::Play(L"XiaoLu_Skill.wav");

		return;
	}
}

Unicorn::Unicorn() : skillSprite(nullptr), skillActor(nullptr)
{
}


Unicorn::~Unicorn()
{
}
