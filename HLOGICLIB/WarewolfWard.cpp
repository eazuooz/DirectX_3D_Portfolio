#include "WarewolfWard.h"
#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "UnicornSkill.h"
#include "ProjectTileLogic.h"
#include "GameManager.h"

#include <SOUND.h>

void WarewolfWard::Update()
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

void WarewolfWard::Victory()
{
	CurPosIdxCheck();
	//OverlapPathCheck();
	if ((int)GameManager::Inst().GetState() == 0)
	{
		if (isMonster == true)
		{

		}
		else
		{
			ChessBoard::Inst().PopAllyPiece(GetActor());

			GetActor()->Die();
		}


	}
}

void WarewolfWard::MakeProjecTile()
{
	Sound::Play(L"LangRen_S1_Attack_1.wav");
	
}



void WarewolfWard::Skill()
{

}

void WarewolfWard::Passive()
{
	//
}

WarewolfWard::WarewolfWard()
{
	isPassive = true;
}


WarewolfWard::~WarewolfWard()
{
}
