#include "SoulBear.h"

#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "ProjectTileLogic.h"
#include "GameManager.h"
#include <SOUND.h>

void SoulBear::Init()
{
	Sound::Play(L"DeLuYi_UnderAttack_1.wav");
}

void SoulBear::Update()
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

void SoulBear::Victory()
{
	CurPosIdxCheck();
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

void SoulBear::MakeProjecTile()
{
	Sound::Play(L"DeLuYi_C_Attack_1.wav");
}


void SoulBear::Skill()
{

}

void SoulBear::Passive()
{
	//
}

SoulBear::SoulBear()
{
	isPassive = true;
}


SoulBear::~SoulBear()
{
}
