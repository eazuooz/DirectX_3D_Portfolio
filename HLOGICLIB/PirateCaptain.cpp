#include "PirateCaptain.h"

#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "UnicornSkill.h"
#include "ProjectTileLogic.h"
#include "EffectRotate.h"
#include "PirateSkill.h"
#include "DoomArbiterSkill.h"

#include <SOUND.h>

void PirateCaptain::Update()
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

void PirateCaptain::MakeProjecTile()
{
	Sound::Play(L"ChuanZhang_Attack_2.wav");
}

void PirateCaptain::Skill()
{
	CurPosIdxCheck();



	if (pFbxAnimation->GetCurClip()->m_AniEnd == true)
	{
		//pFbxAnimation->ChangeAnimation(L"SKILLLOOP");

		if (pAttTarget != nullptr)
		{
			Sound::Play(L"ChuanZhang_UnderAttack_1.wav");

			Vector4 createPos = GetActor()->GetTransform()->GetLocalPosition();
			//createPos.y += 70.0f;
			HPTR<Actor> effectActor = GetScene()->CreateActor();
			effectActor->GetTransform()->SetLoclaPosition(createPos);
			//actor->GetTransform()->SetLocalRotY(180.0f);
			effectActor->GetTransform()->SetLocalScale({ 2.0F, 2.0F ,2.0F });
			HPTR<StaticFbxRender> staticRender = effectActor->AddComponent<StaticFbxRender>(RG_PLAYER);

			staticRender->FbxEffectSetting(L"PriateShip.FBXDATA");

			HPTR<PirateSkill> skillLogic = effectActor->AddComponent<PirateSkill>();

			skillLogic->SetTarget(pAttTarget);




			HPTR<Actor> newActor = GetScene()->CreateActor(L"Curse");

			newActor->GetTransform()->SetLocalScale({ 1.2f, 1.2f, 1.2f });

			HPTR<SpriteRenderer> skillRender = newActor->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
			skillRender->SetLoclaPosition({ 0.0F, 1.0f, 0.0f });
			skillRender->SetLocalScale({ 200.0f, 200.0f, 1.0f, 1.0f });
			skillRender->SetLocalRotX(90.0f);
			skillRender->SetSprite(L"MagicRuneBlue.png", 0);
			skillRender->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
			skillRender->LinkCB(L"VS_CUTDATA", &(skillRender->GetCutData()));
			skillRender->LinkTexture(L"PS_MAPTEX", L"MagicRuneBlue.png");
			skillRender->LinkSampler(L"PS_LSMP", L"LSMP");


			HPTR<DoomArbiterSkill> projectLogic2 = newActor->AddComponent<DoomArbiterSkill>();
			projectLogic2->SetTarget(pAttTarget);


			statusLogic->InitCurTime();
			statusLogic->SetMp(0.0f);
		}

		pFbxAnimation->ChangeAnimation(L"IDLE");
		m_State = PIECESTATE::IDLE;

		return;
	}


}

PirateCaptain::PirateCaptain()
{
}


PirateCaptain::~PirateCaptain()
{
}
