#include "WalrusChamp.h"
#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "UnicornSkill.h"
#include "ProjectTileLogic.h"

#include "EffectRotate.h"
#include "WalrusChampSkill.h"

#include <SOUND.h>

void WalrusChamp::Update()
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

void WalrusChamp::MakeProjecTile()
{
	Sound::Play(L"JuYa_Attack_2.wav");
}

void WalrusChamp::Skill()
{
	CurPosIdxCheck();

	if (skillOn == false)
	{
		skillOn = true;
		if (pAttTarget != nullptr)
		{
			HPTR<Actor> chessPlayer2 = GetScene()->CreateActor();
			HPTR<WalrusChampSkill> skillLogic = chessPlayer2->AddComponent<WalrusChampSkill>();
			skillLogic->SetTarget(pAttTarget);
		}
		
	}

	if (pFbxAnimation->GetCurClip()->m_AniEnd == true)
	{
		skillOn = false;
		if (pAttTarget != nullptr)
		{
			statusLogic->SetMp(0.0f);
		}
		pFbxAnimation->ChangeAnimation(L"IDLE");
		m_State = PIECESTATE::IDLE;

		Sound::Play(L"JuYa_UnderAttack_1.wav");
		return;
	}
}



WalrusChamp::WalrusChamp() : skillOn(false)
{
}


WalrusChamp::~WalrusChamp()
{
}
