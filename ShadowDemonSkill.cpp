#include "ShadowDemonSkill.h"

#include <RANDOM.h>
#include "ChessBoard.h"
#include "StatusLogic.h"
#include <SCENE.h>
#include "LOGICHEADER.h"
#include <2DANICOM.h>
#include "BaseEffect.h"

void ShadowDemonSkill::Init()
{
	myActor = GetActor();
}

void ShadowDemonSkill::Update()
{
	Vector4 myPos = myActor->GetTransform()->GetLocalPosition();

	curTime += Time::DeltaTime();

	particleTime += Time::DeltaTime();

	C_time = 1.5f;

	if (particleTime > 0.25f)
	{
		for (int i = 0; i < 8; ++i)
		{
			Vector4 skillPos = myActor->GetTransform()->GetLocalPosition();

			HPTR<Actor> newActor = GetScene()->CreateActor(L"BlackFlare");
			skillPos.y += 50.0f;
			switch (i)
			{
			case 0:
				skillPos.x += 200.0f * curTime * C_time;
				break;
			case 1:
				skillPos.x -= 200.0f * curTime * C_time;
				break;
			case 2:
				skillPos.z += 200.0f * curTime * C_time;
				break;
			case 3:
				skillPos.z -= 200.0f * curTime * C_time;
				break;
			case 4:
				skillPos.x += 141.0f * curTime * C_time;
				skillPos.z += 141.0f * curTime * C_time;
				break;
			case 5:
				skillPos.x -= 141.0f * curTime * C_time;
				skillPos.z += 141.0f * curTime * C_time;
				break;
			case 6:
				skillPos.x += 141.0f * curTime * C_time;
				skillPos.z -= 141.0f * curTime * C_time;
				break;
			case 7:
				skillPos.x -= 141.0f * curTime * C_time;
				skillPos.z -= 141.0f * curTime * C_time;
				break;
			default:
				break;
			}
			newActor->GetTransform()->SetPosition(skillPos);
			newActor->GetTransform()->SetLocalScale({ 1.2f, 1.0f, 1.2f });

			HPTR<SpriteRenderer> skillRender = newActor->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
			skillRender->SetLoclaPosition({ 0.0F, 1.0f, 0.0f });
			skillRender->SetLocalScale({ 160.0f, 160.0f, 1.0f, 1.0f });
			skillRender->SetSprite(L"burst.png", 0);
			skillRender->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
			skillRender->LinkCB(L"VS_CUTDATA", &(skillRender->GetCutData()));
			skillRender->LinkTexture(L"PS_MAPTEX", L"burst.png");
			skillRender->LinkSampler(L"PS_LSMP", L"LSMP");

			skillRender->BillBoardOn();

			HPTR<SpriteAnimaion> skillAni = newActor->AddComponent<SpriteAnimaion>();
			skillAni->CreateAni(L"LOOP", L"burst.png", 0, 48, 0.01f, false);

			newActor->AddComponent<BaseEffect>();
		}

		particleTime = 0.0f;
	}



	if (curTime > 4.0f)
	{
		myActor->Die();
	}
}


ShadowDemonSkill::ShadowDemonSkill() : isRotate(false), myActor(nullptr), curTime(0.0f), particleTime(0.0f), C_time(1.0f)
{
}


ShadowDemonSkill::~ShadowDemonSkill()
{
}
