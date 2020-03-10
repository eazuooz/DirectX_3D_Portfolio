#include "SwordMaster.h"


#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "UnicornSkill.h"
#include "ProjectTileLogic.h"
#include "EffectRotate.h"

#include <SOUND.h>

void SwordMaster::Update()
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

void SwordMaster::MakeProjecTile()
{
	Sound::Play(L"JianSheng_Attack_2.wav");
}

void SwordMaster::Skill()
{
	CurPosIdxCheck();
	if (skillStart == false)
	{
		if (pFbxAnimation->GetCurClip()->m_AniEnd == true)
		{
			pFbxAnimation->ChangeAnimation(L"SKILLLOOP");
			skillStart = true;

			Vector4 createPos = GetActor()->GetTransform()->GetLocalPosition();
			createPos.y += 70.0f;
			effectActor = GetScene()->CreateActor();
			effectActor->GetTransform()->SetLoclaPosition(createPos);
			//actor->GetTransform()->SetLocalRotY(180.0f);
			effectActor->GetTransform()->SetLocalScale({ 4.0F, 4.0F ,4.0F });
			HPTR<StaticFbxRender> staticRender = effectActor->AddComponent<StaticFbxRender>(RG_PLAYER);

			staticRender->FbxEffectSetting(L"circlePlasmaRed.FBXDATA");

			effectActor->AddComponent<EffectRotate>();

			Sound::Play(L"JianSheng_UnderAttack_1.wav");
			Sound::Play(L"JianSheng_Skill.wav", L"JianSheng_Skill.wav", 999);
		} 
	}
	else
	{
		curTime += Time::DeltaTime();
		if (curTime > 1.0f)
		{
			timeCnt++;
			curTime = 0.0f;
			
#pragma region SWORDATTACK
			Vector4 actorPos = GetActor()->GetTransform()->GetLocalPosition();
			BoardIndex idx = ChessBoard::Inst().CalculateIndex(actorPos);
			idx -= 1;

			for (int z = 0; z < 3; z++)
			{
				for (int x = 0; x < 3; x++)
				{
					if ((idx.z + z >= 0 && idx.z + z <= 7) &&
						(idx.x + x >= 0 && idx.x + x <= 7))
					{
						Actor* target = ChessBoard::Inst().m_Board[idx.z + z][idx.x + x].m_Actor;
						if (target != nullptr)
						{
							StatusLogic* statusLogic = target->GetComponent<StatusLogic>();
							if (statusLogic->GetType() == PIECETYPE::ENEMY)
							{
								statusLogic->AddHp(-75);
								Sound::Play(L"JianSheng_Skill_EnemyBuff.wav");
							}
						}
					}
				}
			}
#pragma endregion
			
		}

		if (timeCnt >= 5 )
		{
			skillStart = false;
			timeCnt = 0;
			statusLogic->SetMp(0.0f);
			statusLogic->InitCurTime();
			pAttTarget = nullptr;
			pFbxAnimation->ChangeAnimation(L"IDLE");
			m_State = PIECESTATE::IDLE;

			effectActor->Die();

			Sound::Stop(L"JianSheng_Skill.wav");
			return;
		}
	}
	
}

SwordMaster::SwordMaster() : skillStart(false), timeCnt(0)
{
}


SwordMaster::~SwordMaster()
{
}
