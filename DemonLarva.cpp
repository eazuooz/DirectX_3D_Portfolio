#include "DemonLarva.h"

#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "UnicornSkill.h"
#include "ProjectTileLogic.h"
#include "SerpentWard.h"
#include <SOUND.h>


void DemonLarva::Update()
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

void DemonLarva::MakeProjecTile()
{
	if (pAttTarget != nullptr)
	{
		HPTR<Actor> newActor = GetScene()->CreateActor();
		Vector4 createPos = myActor->GetTransform()->GetLocalPosition();
		//createPos.y += 100.0f;
		newActor->GetTransform()->SetLoclaPosition(createPos);
		newActor->GetTransform()->SetLocalScale({ 1.0F , 1.0f, 1.0f });


		HPTR<SpriteRenderer> spriteRender = newActor->AddComponent<SpriteRenderer>(RG_PLAYER);
		spriteRender->SetSprite(L"EF_water_03_01.png", 0);
		spriteRender->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		//battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
		spriteRender->LinkCB(L"VS_CUTDATA", &(spriteRender->GetCutData()));
		spriteRender->LinkTexture(L"PS_MAPTEX", L"EF_water_03_01.png");
		spriteRender->LinkSampler(L"PS_LSMP", L"LSMP");
		spriteRender->LinkSampler(L"PS_PSMP", L"PSMP");

		//battleTopFrameSR->SetLoclaPosition({ 0.0F, 100.0F, 0.0f });
		spriteRender->SetLocalScale({ 80.0F, 80.0F, 1.0F, 1.0f });
		spriteRender->BillBoardOn();

		HPTR<SpriteAnimaion> battleTopFrameSRA = newActor->AddComponent<SpriteAnimaion>();
		battleTopFrameSRA->CreateAni(L"LOOP", L"EF_water_03_01.png", 0, 15, 0.1f);

		HPTR<ProjectTileLogic> projectLogic = newActor->AddComponent<ProjectTileLogic>();
		projectLogic->SetTarget(pAttTarget);

		Sound::Play(L"JuDu_Attack_2.wav");
	}
}

void DemonLarva::Skill()
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
			createdActorAni->FbxSetting(L"SerpentWard01.FBXDATA");
		}else if (Level == 2)
		{
			createdActorAni->FbxSetting(L"SerpentWard02.FBXDATA");
		}
		else if (Level == 3)
		{
			createdActorAni->FbxSetting(L"SerpentWard03.FBXDATA");
		}
			
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 30, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 31, 61, false);
		createdActorAni->CreateAnimation(L"BORN", 62, 94, false);
		createdActorAni->CreateAnimation(L"DEATH", 95, 129, false);
		createdActorAni->CreateAnimation(L"DIZZY", 130, 170, true);
		createdActorAni->CreateAnimation(L"IDLE", 171, 211, true);
		createdActorAni->CreateAnimation(L"JUMP", 212, 248, false);
		createdActorAni->CreateAnimation(L"RUN", 249, 269, true);
		createdActorAni->CreateAnimation(L"SKILL", 0, 31, true);
		createdActorAni->CreateAnimation(L"VICTORY", 270, 318, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<SerpentWard> moveLogic = createdActor->AddComponent<SerpentWard>();
		moveLogic->SetName(L"SerpentWard");
		moveLogic->SetRange(7);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME


		if (Level == 1)
		{
			stateLogic->SetStatus(250, 75, 10.0f, 0.0f, -1);
		}
		else if (Level == 2)
		{
			stateLogic->SetStatus(400, 125, 10.0f, 0.0f, -1);
		}
		else if (Level == 3)
		{
			stateLogic->SetStatus(550, 175, 10.0f, 0.0f, -1);
		}

		ChessBoard::Inst().PushAllyPiece(createdActor);

		Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
		m_Index = ChessBoard::Inst().CalculateIndex(myPos);

		BoardIndex myIdx = m_Index;
		m_Index -= 2;

		BoardIndex idx = m_Index;

		for (int z = 0; z < 5; z++)
		{
			for (int x = 0; x < 5; x++)
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
		Sound::Play(L"JuDu_UnderAttack_2.wav");
	}

}

DemonLarva::DemonLarva()
{

}


DemonLarva::~DemonLarva()
{
}
