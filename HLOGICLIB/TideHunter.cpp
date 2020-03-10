#include "TideHunter.h"

#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "UnicornSkill.h"
#include "ProjectTileLogic.h"

#include "EffectRotate.h"
#include "TideHunterSkill.h"
#include <SOUND.h>

void TideHunter::Update()
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

void TideHunter::MakeProjecTile()
{
	Sound::Play(L"ChaoXi_Attack_2.wav");

}

void TideHunter::Skill()
{
	CurPosIdxCheck();

	if (pAttTarget == nullptr || pFbxAnimation->GetCurClip()->m_AniEnd == true)
	{
		skillOn = false;

		statusLogic->SetMp(0.0f);
		pFbxAnimation->ChangeAnimation(L"IDLE");
		m_State = PIECESTATE::IDLE;

		return;
	}

	if (pFbxAnimation->GetCurClip()->m_AniHalf == true && skillOn == false)
	{
		skillOn = true;

		HPTR<Actor> newActor = GetScene()->CreateActor(L"Tentacle");
		Vector4 skillPos = myActor->GetTransform()->GetLocalPosition();

		skillPos.y += 50.0f;
		newActor->GetTransform()->SetPosition(skillPos);

		HPTR<TideHunterSkill> skillLogic = newActor->AddComponent<TideHunterSkill>();
		skillLogic->SetTarget(pAttTarget);

	}

}

TideHunter::TideHunter() : skillOn(false)
{
}


TideHunter::~TideHunter()
{
}
