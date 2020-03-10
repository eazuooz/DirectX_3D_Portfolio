#include "GodofWar.h"

#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "ProjectTileLogic.h"
#include <SOUND.h>


void GodofWar::Update()
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


void GodofWar::Skill()
{
	CurPosIdxCheck();

	if (skillOn == false)
	{
		skillOn = true;

		HPTR<Actor> newActor = GetScene()->CreateActor(L"ShockWave");
		Vector4 skillPos = myActor->GetTransform()->GetLocalPosition();
		Vector4 skillRot = myActor->GetTransform()->GetLocalRotation();

		skillPos.y += 0.3f;
		newActor->GetTransform()->SetPosition(skillPos);
		newActor->GetTransform()->SetRotation(skillRot);
		newActor->GetTransform()->SetLocalScale({ 1.2f, 1.0f, 1.2f });

		HPTR<SpriteRenderer> skillRender = newActor->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		skillRender->SetLoclaPosition({ 0.0F, 1.0f, 0.0f });
		skillRender->SetLocalScale({ 600.0f, 600.0f, 1.0f, 1.0f });
		skillRender->SetLocalRotX(90.0f);
		skillRender->SetSprite(L"expl.png", 0);
		skillRender->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		skillRender->LinkCB(L"VS_CUTDATA", &(skillRender->GetCutData()));
		skillRender->LinkTexture(L"PS_MAPTEX", L"expl.png");
		skillRender->LinkSampler(L"PS_LSMP", L"LSMP");

		HPTR<SpriteAnimaion> skillAni = newActor->AddComponent<SpriteAnimaion>();
		skillAni->CreateAni(L"LOOP", L"expl.png", 0, 36, 0.01f, false);
	}

	if (pFbxAnimation->GetCurClip()->m_AniEnd == true)
	{
		Sound::Play(L"FaTiao_Skill_Object_2.wav");

		statusLogic->SetMp(0.0f);
		pFbxAnimation->ChangeAnimation(L"IDLE");
		m_State = PIECESTATE::IDLE;

		Vector4 actorPos = GetActor()->GetTransform()->GetLocalPosition();
		BoardIndex idx = ChessBoard::Inst().CalculateIndex(actorPos);
		idx -= 2;

		for (int z = 0; z < 3; z++)
		{
			for (int x = 0; x < 3; x++)
			{
				if ((idx.z + z >= 0 && idx.z + z <= 7) && (idx.x + x >= 0 && idx.x + x <= 7))
				{
					Actor* target = ChessBoard::Inst().m_Board[idx.z + z][idx.x + x].m_Actor;
					if (target != nullptr)
					{
						StatusLogic* statusLogic = target->GetComponent<StatusLogic>();
						if (statusLogic->GetType() == PIECETYPE::ENEMY)
						{
							statusLogic->AddHp(-200);
						}
					}
				}
			}
		}
		skillOn = false;
		return;
	}
}

GodofWar::GodofWar() : skillActor(nullptr), skillCount(0.0f), skillOn(false)
{
}


GodofWar::~GodofWar()
{
}
