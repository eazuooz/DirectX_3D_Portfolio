#include "GodofThunderSkill.h"

#include <RANDOM.h>
#include "ChessBoard.h"
#include "StatusLogic.h"
#include <SCENE.h>
#include "LOGICHEADER.h"
#include <2DANICOM.h>
#include "BaseEffect.h"
#include <SOUND.h>

void GodofThunderSkill::Init()
{
}

void GodofThunderSkill::Update()
{
	myActor = GetActor();

	curTime += Time::DeltaTime();
	skillTime += Time::DeltaTime();

	if (skillTime > 0.25f)
	{
		skillShot = false;

		while (indexZ < 8)
		{

			

			while (indexX < 8)
			{
				Actor* target = ChessBoard::Inst().m_Board[indexZ][indexX].m_Actor;
				if(target != nullptr)
				{
					StatusLogic* statusLogic = target->GetComponent<StatusLogic>();
					
					if (statusLogic->GetType() == PIECETYPE::ENEMY)
					{
						HPTR<Actor> newActor = GetScene()->CreateActor(L"Thunder");
						Vector4 skillPos = target->GetTransform()->GetLocalPosition();

						newActor->GetTransform()->SetPosition({ skillPos.x, 10.0f ,skillPos.z });
						newActor->GetTransform()->SetLocalScale({ 1.2f, 1.2f, 1.2f });

						HPTR<SpriteRenderer> skillRender = newActor->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
						skillRender->SetLoclaPosition({ 0.0F, 0.0f, 0.0f });
						skillRender->SetLocalScale({ 300.0f, 1500.0f, 1.0f, 1.0f });
						skillRender->SetSprite(L"wthunder.png", 0);
						skillRender->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
						skillRender->LinkCB(L"VS_CUTDATA", &(skillRender->GetCutData()));
						skillRender->LinkTexture(L"PS_MAPTEX", L"wthunder.png");
						skillRender->LinkSampler(L"PS_LSMP", L"LSMP");

						HPTR<SpriteAnimaion> skillAni = newActor->AddComponent<SpriteAnimaion>();
						skillAni->CreateAni(L"LOOP", L"wthunder.png", 0, 26, 0.03f, false);

						statusLogic->AddHp(-200);

						skillShot = true;

						Sound::Play(L"ZhongShen_UnderAttack_1.wav");
					}
				}
				indexX++;
				if (skillShot == true)
				{
					break;
				}
			}

			if (skillShot == true)
			{
				skillTime = 0.0f;
				break;
			}

			if (indexX >= 8)
			{
				indexX = 0;
				indexZ++;
			}

		}

	}

	if (skillTime > 4.0f)
	{
		myActor->Die();
	}
}



GodofThunderSkill::GodofThunderSkill() : isRotate(false), myActor(nullptr), curTime(0.0f), skillTime(0.0f), indexX(0), indexZ(0), skillShot(false)
{
}


GodofThunderSkill::~GodofThunderSkill()
{
}
