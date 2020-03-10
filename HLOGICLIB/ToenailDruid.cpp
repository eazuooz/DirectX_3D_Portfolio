#include "ToenailDruid.h"
#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "ProjectTileLogic.h"
#include "SoulBear.h"

void ToenailDruid::Update()
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

void ToenailDruid::MakeProjecTile()
{
}

void ToenailDruid::Skill()
{
	CurPosIdxCheck();

	if (pFbxAnimation->GetCurClip()->m_AniEnd == true)
	{
		HPTR<Actor> createdActor = GetScene()->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });

		HPTR <FbxAnimation> createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		int Level = GetActor()->GetLevel();

		if (Level == 1)
		{
			createdActorAni->FbxSetting(L"SoulBear01.FBXDATA");
		}
		else if (Level == 2)
		{
			createdActorAni->FbxSetting(L"SoulBear02.FBXDATA");
		}
		else if (Level == 3)
		{
			createdActorAni->FbxSetting(L"SoulBear03.FBXDATA");
		}

		createdActorAni->CreateAnimation(L"ATTACK01", 0, 40, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 41, 83, false);
		createdActorAni->CreateAnimation(L"BORN", 84, 121, false);
		createdActorAni->CreateAnimation(L"DEATH", 122, 202, false);
		createdActorAni->CreateAnimation(L"DIZZY", 203, 243, true);
		createdActorAni->CreateAnimation(L"IDLE", 244, 284, true);
		createdActorAni->CreateAnimation(L"JUMP", 285, 330, false);
		createdActorAni->CreateAnimation(L"RUN", 331, 359, true);
		createdActorAni->CreateAnimation(L"SKILL", 0, 40, false);
		createdActorAni->CreateAnimation(L"VICTORY", 360, 460, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<SoulBear> moveLogic = createdActor->AddComponent<SoulBear>();
		moveLogic->SetName(L"SoulBear");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();


		if (Level == 1)
		{
			stateLogic->SetStatus(1000, 100, 10.0f, 0.0f, -1);
		}
		else if (Level == 2)
		{
			stateLogic->SetStatus(2000, 150, 10.0f, 0.0f, -1);
		}
		else if (Level == 3)
		{
			stateLogic->SetStatus(3000, 250, 10.0f, 0.0f, -1);
		}

		ChessBoard::Inst().PushAllyPiece(createdActor);

		Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
		m_Index = ChessBoard::Inst().CalculateIndex(myPos);

		BoardIndex myIdx = m_Index;
		m_Index -= 2;

		BoardIndex idx = m_Index;

		for (int z = 4; z >= 0; z--)
		{
			for (int x = 4; x >= 0; x--)
			{
				if ((idx.z + z >= 0 && idx.z + z <= 7) &&
					(idx.x + x >= 0 && idx.x + x <= 7))
				{
					if (myIdx.z == idx.z + z && myIdx.x == idx.x + x)
					{
						continue;
					}

					Actor* enemyActor = ChessBoard::Inst().m_Board[idx.z + z][idx.x + x].m_Actor;
					if (enemyActor == nullptr)
					{
						createdActor->GetTransform()->
							SetLoclaPosition(ChessBoard::Inst().m_Board[idx.z + z][idx.x + x].m_BoardPos);

						statusLogic->InitCurTime();
						statusLogic->SetMp(0.0f);
						pFbxAnimation->ChangeAnimation(L"IDLE");
						m_State = PIECESTATE::IDLE;

						return;
					}
				}
			}
		}

		pFbxAnimation->ChangeAnimation(L"IDLE");
		m_State = PIECESTATE::IDLE;
	}

}

ToenailDruid::ToenailDruid()
{
}


ToenailDruid::~ToenailDruid()
{
}
