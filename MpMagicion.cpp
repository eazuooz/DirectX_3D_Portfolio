#include "MpMagicion.h"


#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "UnicornSkill.h"
#include "ProjectTileLogic.h"
#include "GameManager.h"
#include "EffectRotate.h"
#include <SOUND.h>

MpMagicion::MpMagicion() : skillSprite(nullptr), skillActor(nullptr), skillCount(0.0f)
{
}


MpMagicion::~MpMagicion()
{
}

void MpMagicion::Update()
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
	statusLogic->SetMp(0.0f);
	Passive();
}

void MpMagicion::MakeProjecTile()
{
	if (pAttTarget != nullptr)
	{
		Vector4 createPos = myActor->GetTransform()->GetLocalPosition();
		//createPos.y += 100.0f;
		//newActor->GetTransform()->SetLoclaPosition(createPos);

		HPTR<Actor> PLAYER2 = GetScene()->CreateActor();
		PLAYER2->GetTransform()->SetLoclaPosition(createPos);
		PLAYER2->GetTransform()->SetLocalScale({ 20.0F, 20.0F, 20.0F });

		HPTR<Renderer> SR2 = PLAYER2->AddComponent<Renderer>(RG_PLAYER);
		Renderer::HRENDERDATA* Data = SR2->RenderData(L"SPHEREMESH", L"EFFECTDPATH");

		SR2->CreateCB(L"VS_CUTDATA", Vector4(0.0f, 0.0f, 1.0f, 1.0f));
		SR2->LinkTexture(L"PS_MAPTEX", L"T_GPP_Plasma_A.tga");

		SR2->LinkSampler(L"PS_LSMP", L"LSMP");

		HPTR<SpriteRenderer> battleTopFrameSR = PLAYER2->AddComponent<SpriteRenderer>(RG_PLAYER);
		battleTopFrameSR->SetSprite(L"CircleEffectSprite.png", 0);
		battleTopFrameSR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		battleTopFrameSR->LinkCB(L"VS_CUTDATA", &(battleTopFrameSR->GetCutData()));
		battleTopFrameSR->LinkTexture(L"PS_MAPTEX", L"CircleEffectSprite.png");
		battleTopFrameSR->LinkSampler(L"PS_LSMP", L"LSMP");

		battleTopFrameSR->SetLocalScale({ 25.0F, 25.0F, 1.0F, 1.0f });
		battleTopFrameSR->BillBoardOn();

		HPTR<SpriteAnimaion> battleTopFrameSRA = PLAYER2->AddComponent<SpriteAnimaion>();
		battleTopFrameSRA->CreateAni(L"LOOP", L"CircleEffectSprite.png", 0, 24, 0.06f);
		PLAYER2->AddComponent<EffectRotate>();

		HPTR<ProjectTileLogic> projectLogic = PLAYER2->AddComponent<ProjectTileLogic>();
		projectLogic->SetTarget(pAttTarget);

		Sound::Play(L"BingNv_Attack_1.wav");
	}
}

void MpMagicion::Skill()
{
	CurPosIdxCheck();
	return;
}

void MpMagicion::Passive()
{
	if(0 == GameManager::Inst().GetState())
		return;

	if (m_State == PIECESTATE::DIE)
	{
		return;
	}

	skillCount += Time::DeltaTime();

	if (skillCount > 3.0f)
	{
		for (int z = 0; z < 8; z++)
		{
			for (int x = 0; x < 8; x++)
			{
				Actor* target = ChessBoard::Inst().m_Board[z][x].m_Actor;

				if (target != nullptr)
				{
					StatusLogic* statusLogic = target->GetComponent<StatusLogic>();

					if (statusLogic->GetType() == PIECETYPE::ALLY)
					{
						statusLogic->AddMp(15);
					}
				}
			}
		}

		skillCount = 0.0f;
	}
}