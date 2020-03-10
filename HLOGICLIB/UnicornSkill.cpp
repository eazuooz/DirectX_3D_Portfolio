#include "UnicornSkill.h"

#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "StatusLogic.h"
#include <SOUND.h>


void UnicornSkill::Init()
{

}

void UnicornSkill::Update()
{
	curTime += Time::DeltaTime();
	skillTime += Time::DeltaTime();

	if (curTime > 4.0f && loopStart == false)
	{
		Sound::Play(L"XiaoLu_Skill_Loop.wav", L"XiaoLu_Skill_Loop.wav", 999);
		loopStart = true;
	}


	if (skillTime > 1.5f)
	{
		//여기서 피를 채워준다
		Vector4 actorPos = GetActor()->GetTransform()->GetLocalPosition();
		BoardIndex idx = ChessBoard::Inst().CalculateIndex(actorPos);
		idx -= 2;

		for (int z = 0; z < 5; z++)
		{
			for (int x = 0; x < 5; x++)
			{
				if ((idx.z + z >= 0 && idx.z + z <= 7) && 
					(idx.x + x >= 0 && idx.x + x <= 7))
				{
					Actor* target = ChessBoard::Inst().m_Board[idx.z + z][idx.x + x].m_Actor;
					if (target != nullptr)
					{
						StatusLogic* statusLogic = target->GetComponent<StatusLogic>();
						if (statusLogic->GetType() == PIECETYPE::ALLY)
						{
							statusLogic->AddHp(20);
						}
					}
				}
			}
		}

		skillTime = 0.0f;
	}


	if (curTime > continueTime)
	{
		Sound::Stop(L"XiaoLu_Skill_Loop.wav");
		GetActor()->Die();
	}
}

UnicornSkill::UnicornSkill() : continueTime(0.0f) , curTime(0.0f), skillTime(0.0f), loopStart(false)
{
}


UnicornSkill::~UnicornSkill()
{
}
