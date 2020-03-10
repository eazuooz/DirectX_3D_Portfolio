#include "GodofThunder.h"

#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "UnicornSkill.h"
#include "ProjectTileLogic.h"

#include "EffectRotate.h"
#include "GodofThunderSkill.h"
#include <SOUND.h>

void GodofThunder::Update()
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

void GodofThunder::MakeProjecTile()
{
	Sound::Play(L"ZhongShen_Attack_1.wav");
}

void GodofThunder::Skill()
{
	CurPosIdxCheck(); 
	
	if (skillOn == false)
	{
		skillOn = true;

	}

	if (pFbxAnimation->GetCurClip()->m_AniEnd == true)
	{
		skillOn = false;

		HPTR<Actor> newActor = GetScene()->CreateActor();
		Vector4 skillPos = myActor->GetTransform()->GetLocalPosition();
		newActor->GetTransform()->SetPosition(skillPos);

		HPTR<GodofThunderSkill> skillLogic = newActor->AddComponent<GodofThunderSkill>();


		statusLogic->SetMp(0.0f);
		pFbxAnimation->ChangeAnimation(L"IDLE");
		m_State = PIECESTATE::IDLE;

		return;
	}
}

GodofThunder::GodofThunder() : skillOn(false)
{
}


GodofThunder::~GodofThunder()
{
}
