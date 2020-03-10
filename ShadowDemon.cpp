#include "ShadowDemon.h"
#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "UnicornSkill.h"
#include "ProjectTileLogic.h"

#include "EffectRotate.h"
#include "ShadowDemonSkill.h"

#include <SOUND.h>


void ShadowDemon::Update()
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

void ShadowDemon::MakeProjecTile()
{
	if (pAttTarget != nullptr)
	{
		Vector4 createPos = myActor->GetTransform()->GetLocalPosition();

		HPTR<Actor> PLAYER2 = GetScene()->CreateActor();
		PLAYER2->GetTransform()->SetLoclaPosition(createPos);
		PLAYER2->GetTransform()->SetLocalScale({ 40.0F, 40.0F, 40.0F });

		HPTR<SpriteRenderer> battleTopFrameSR = PLAYER2->AddComponent<SpriteRenderer>(RG_PLAYER);
		battleTopFrameSR->SetSprite(L"fireball.tga", 0);
		battleTopFrameSR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		battleTopFrameSR->LinkCB(L"VS_CUTDATA", &(battleTopFrameSR->GetCutData()));
		battleTopFrameSR->LinkTexture(L"PS_MAPTEX", L"fireball.tga");
		battleTopFrameSR->LinkSampler(L"PS_LSMP", L"LSMP");

		battleTopFrameSR->SetLocalScale({ 50.0F, 50.0F, 1.0F, 1.0f });
		battleTopFrameSR->BillBoardOn();

		HPTR<SpriteAnimaion> battleTopFrameSRA = PLAYER2->AddComponent<SpriteAnimaion>();
		battleTopFrameSRA->CreateAni(L"LOOP", L"fireball.tga", 0, 16, 0.05f);



		PLAYER2->AddComponent<EffectRotate>();

		HPTR<ProjectTileLogic> projectLogic = PLAYER2->AddComponent<ProjectTileLogic>();
		projectLogic->SetTarget(pAttTarget);

		Sound::Play(L"YingMo_Attack_2.wav");
	}
}

void ShadowDemon::Skill()
{
	CurPosIdxCheck();

	

	if (pFbxAnimation->GetCurClip()->m_AniHalf == true && skillOn == false)
	{
		skillOn = true;
		HPTR<Actor> newActor = GetScene()->CreateActor();
		Vector4 skillPos = myActor->GetTransform()->GetLocalPosition();
		newActor->GetTransform()->SetPosition(skillPos);

		HPTR<ShadowDemonSkill> skillLogic = newActor->AddComponent<ShadowDemonSkill>();
		Sound::Play(L"YingMo_Skill_Object.wav");
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


ShadowDemon::ShadowDemon() : skillOn(false)
{
}


ShadowDemon::~ShadowDemon()
{
}
