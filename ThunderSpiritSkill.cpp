#include "ThunderSpiritSkill.h"

#include <RANDOM.h>
#include "ChessBoard.h"
#include "StatusLogic.h"
#include <SCENE.h>
#include "LOGICHEADER.h"
#include <2DANICOM.h>
#include "BaseEffect.h"

#include <SOUND.h>

void ThunderSpiritSkill::Init()
{
	myActor = GetActor();

	Sound::Play(L"DianGun_Skill.wav");

	Sound::Play(L"DianGun_Skill_Object.wav", L"DianGun_Skill_Object.wav", 999);
}

void ThunderSpiritSkill::Update()
{
	Vector4 myPos = myActor->GetTransform()->GetLocalPosition();


	curTime += Time::DeltaTime();

	float curTimeAlt = curTime;


	if (curTime > 1.25f)
	{
		if (attCnt == 1)
		{
			attCnt = 2;
		}

		curTimeAlt = 2.5f - curTime;
	}

	myActor->GetTransform()->SetLocalScale({curTimeAlt * 12.0f, 1.0f, curTimeAlt * 12.0f, 1.0f });

	if(attCnt == 0)
	{
		attCnt = 1;

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
							statusLogic->AddHp(-100);
						}
					}
				}
			}
		}
	}
	else if (attCnt == 2)
	{
		attCnt = 3;

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
							statusLogic->AddHp(-100);
						}
					}
				}
			}
		}
	}


	if (curTime > 2.5f)
	{


		Sound::Stop(L"DianGun_Skill_Object.wav");
		myActor->Die();
	}
}

ThunderSpiritSkill::ThunderSpiritSkill() : isRotate(false), myActor(nullptr), curTime(0.0f), particleTime(0.0f), attCnt(0)
{
}


ThunderSpiritSkill::~ThunderSpiritSkill()
{
}
