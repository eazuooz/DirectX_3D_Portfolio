#include "Warewolf.h"
#include "StatusLogic.h"

#include "LOGICHEADER.h"
#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "UnicornSkill.h"
#include "ProjectTileLogic.h"
#include "EffectRotate.h"
#include "WarewolfWard.h"

#include <SOUND.h>

void Warewolf::Update()
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

void Warewolf::MakeProjecTile()
{
	Sound::Play(L"LangRen_S1_Attack_2.wav");
}

void Warewolf::Skill()
{
	CurPosIdxCheck();
	
	if (pFbxAnimation != nullptr)
	{
		if (pFbxAnimation->GetCurClip()->m_AniEnd == true)
		{
			Vector4 createPos = GetActor()->GetTransform()->GetLocalPosition();
			Vector4 createRot = GetActor()->GetTransform()->GetLocalRotation();


			if (isEffect == false)
			{
				HPTR<Actor> createdActor = GetScene()->CreateActor();
				createdActor->GetTransform()->SetLoclaPosition(createPos);
				createdActor->GetTransform()->SetLocalRot(createRot);
				createdActor->GetTransform()->SetLocalScale({ 2.0f, 2.0f, 2.0f });

				HPTR <FbxAnimation> createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
				//int Level = GetActor()->GetLevel();

				createdActorAni->FbxEffectSetting(L"Warewolf_Alt03.FBXDATA");
				createdActorAni->CreateAnimation(L"BORN", 82, 127, false);
				createdActorAni->ChangeAnimation(L"BORN");

				effectLogic = createdActor->AddComponent<WareWolfEffect>();
				effectLogic->SetAniPtr(createdActorAni);


				effectActor = GetScene()->CreateActor();
				effectActor->GetTransform()->SetLoclaPosition(createPos);
				//actor->GetTransform()->SetLocalRotY(180.0f);
				effectActor->GetTransform()->SetLocalScale({ 4.0F, 10.0F , 4.0F });
				HPTR<StaticFbxRender> staticRender = effectActor->AddComponent<StaticFbxRender>(RG_PLAYER);

				staticRender->FbxEffectSetting(L"FireWave.FBXDATA");
				//staticRender->SetShadow(true);
				effectActor->AddComponent<EffectRotate>();




				pFbxAnimation->Die();
				pFbxAnimation = nullptr;
				isEffect = true;

				Sound::Play(L"Langren_Skill.wav");
			}
		}
	}
	else
	{
		if (ChessBoard::Inst().GetWolfEffectEnd() == true)
		{
			effectActor->Die();
			effectActor = nullptr;

			//내가 변신
			pFbxAnimation = GetActor()->AddComponent<FbxAnimation>(RG_PLAYER);
			pFbxAnimation->GetTransform()->SetLocalScale({ 1.5f, 1.5f, 1.5f });

			int Level = GetActor()->GetLevel();
			if (Level == 1)
			{
				pFbxAnimation->FbxSetting(L"Warewolf_Alt01.FBXDATA");
			}
			else if (Level == 2)
			{
				pFbxAnimation->FbxSetting(L"Warewolf_Alt02.FBXDATA");
			}
			else if (Level == 3)
			{
				pFbxAnimation->FbxSetting(L"Warewolf_Alt03.FBXDATA");
			}

			pFbxAnimation->CreateAnimation(L"ATTACK01", 0, 40, false);
			pFbxAnimation->CreateAnimation(L"ATTACK02", 41, 81, false);
			pFbxAnimation->CreateAnimation(L"BORN", 82, 127, false);
			pFbxAnimation->CreateAnimation(L"DEATH", 128, 181, false);
			pFbxAnimation->CreateAnimation(L"DIZZY", 182, 242, true);
			pFbxAnimation->CreateAnimation(L"IDLE", 243, 273, true);
			pFbxAnimation->CreateAnimation(L"JUMP", 274, 349, false);
			pFbxAnimation->CreateAnimation(L"RUN", 350, 368, true);
			pFbxAnimation->CreateAnimation(L"SKILL", 82, 127, false);
			pFbxAnimation->CreateAnimation(L"VICTORY", 369, 476, true);
			pFbxAnimation->ChangeAnimation(L"IDLE");

			statusLogic->SetMp(0.0f);
			statusLogic->InitCurTime();
			m_State = PIECESTATE::IDLE;

			CreateCreature();

			return;
		}
	}
}


void Warewolf::CreateCreature()
{
		Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
		m_Index = ChessBoard::Inst().CalculateIndex(myPos);

		BoardIndex myIdx = m_Index;
		m_Index -= 1;
		BoardIndex idx = m_Index;

		int makeCnt = 0;
		
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

					if (makeCnt >= 2)
					{
						return;
					}

					Actor* enemyActor = ChessBoard::Inst().m_Board[idx.z + z][idx.x + x].m_Actor;
					if (enemyActor == nullptr && makeCnt < 2)
					{
#pragma region CREATEACTOR
						HPTR<Actor> createdActor = nullptr;
						createdActor = GetScene()->CreateActor();
						createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
						createdActor->GetTransform()->SetLocalScale({ 1.2f, 1.2f, 1.2f });

						HPTR <FbxAnimation> createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
						int Level = GetActor()->GetLevel();


						if (Level == 1)
						{
							createdActorAni->FbxSetting(L"Warewolf_Ward01.FBXDATA");
						}
						else if (Level == 2)
						{
							createdActorAni->FbxSetting(L"Warewolf_Ward02.FBXDATA");
						}
						else if (Level == 3)
						{
							createdActorAni->FbxSetting(L"Warewolf_Ward03.FBXDATA");
						}

						createdActorAni->CreateAnimation(L"ATTACK01", 0, 34, false);
						createdActorAni->CreateAnimation(L"ATTACK02", 35, 69, false);
						createdActorAni->CreateAnimation(L"BORN", 70, 110, false);
						createdActorAni->CreateAnimation(L"DEATH", 111, 161, false);
						createdActorAni->CreateAnimation(L"DIZZY", 162, 196, true);
						createdActorAni->CreateAnimation(L"IDLE", 197, 233, true);
						createdActorAni->CreateAnimation(L"JUMP", 234, 274, false);
						createdActorAni->CreateAnimation(L"RUN", 275, 291, true);
						createdActorAni->CreateAnimation(L"SKILL", 0, 34, true);
						createdActorAni->CreateAnimation(L"VICTORY", 292, 368, true);
						createdActorAni->ChangeAnimation(L"BORN");

						HPTR<WarewolfWard> moveLogic = createdActor->AddComponent<WarewolfWard>();
						moveLogic->SetName(L"WarewolfWard");
						moveLogic->SetRange(1);

						HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
						//HP, ATT, COOLTIME, CONTINUETIME


						if (Level == 1)
						{
							stateLogic->SetStatus(300, 60, 10.0f, 0.0f, -1);
						}
						else if (Level == 2)
						{
							stateLogic->SetStatus(500, 100, 10.0f, 0.0f, -1);
						}
						else if (Level == 3)
						{
							stateLogic->SetStatus(700, 140, 10.0f, 0.0f, -1);
						}


						ChessBoard::Inst().PushAllyPiece(createdActor);
#pragma endregion
						createdActor->GetTransform()->
							SetLoclaPosition(ChessBoard::Inst().m_Board[idx.z + z][idx.x + x].m_BoardPos);
						++makeCnt;
					}

				}
			}
		}

}



Warewolf::Warewolf() : isEffect(false), effectLogic(nullptr)
{
}


Warewolf::~Warewolf()
{
}
