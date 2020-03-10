#include "FireMagician.h"

#include <math.h>
#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "UnicornSkill.h"
#include "ProjectTileLogic.h"

#include "EffectRotate.h"
#include "LittleDragonSkill.h"
#include "FireMagicionSkill.h"
#include <RANDOM.h>

#include <SOUND.h>

void FireMagician::Update()
{
	GetActor()->GetTransform()->SetLoclaPositionY(60.0f);
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

void FireMagician::MakeProjecTile()
{
	if (pAttTarget != nullptr)
	{
		Vector4 createPos = myActor->GetTransform()->GetLocalPosition();
		//createPos.y += 100.0f;
		//newActor->GetTransform()->SetLoclaPosition(createPos);

		HPTR<Actor> PLAYER2 = GetScene()->CreateActor();
		PLAYER2->GetTransform()->SetLoclaPosition(createPos);
		PLAYER2->GetTransform()->SetLocalScale({ 150.0F, 75.0F, 1.0F });
		PLAYER2->GetTransform()->SetLocalRotX(90.0f);

		HPTR<SpriteRenderer> battleTopFrameSR = PLAYER2->AddComponent<SpriteRenderer>(RG_PLAYER);
		battleTopFrameSR->SetSprite(L"FlameMagicionAtt.tga", 0);
		battleTopFrameSR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		//battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
		battleTopFrameSR->LinkCB(L"VS_CUTDATA", &(battleTopFrameSR->GetCutData()));
		battleTopFrameSR->LinkTexture(L"PS_MAPTEX", L"FlameMagicionAtt.tga");
		battleTopFrameSR->LinkSampler(L"PS_LSMP", L"LSMP");
		battleTopFrameSR->LinkSampler(L"PS_PSMP", L"PSMP");

		//battleTopFrameSR->SetLoclaPosition({ 0.0F, 100.0F, 0.0f });
		//battleTopFrameSR->SetLocalScale({ 125.0F, 62.5F, 1.0F, 1.0f });
		//battleTopFrameSR->BillBoardOn();

		HPTR<SpriteAnimaion> battleTopFrameSRA = PLAYER2->AddComponent<SpriteAnimaion>();
		battleTopFrameSRA->CreateAni(L"LOOP", L"FlameMagicionAtt.tga", 0, 31, 0.06f);

		HPTR<ProjectTileLogic> projectLogic2 = PLAYER2->AddComponent<ProjectTileLogic>();
		projectLogic2->SetDegree(180.0f);
		projectLogic2->SetTarget(pAttTarget);

		Sound::Play(L"ThrowFire.wav");
	}
}

void FireMagician::Skill()
{
	CurPosIdxCheck();

	if (pFbxAnimation->GetCurClip()->m_AniEnd == true)
	{
		if (pAttTarget != nullptr)
		{
			Vector4 enemyPos = pAttTarget->GetTransform()->GetLocalPosition();
			Vector4 myPos = myActor->GetTransform()->GetLocalPosition();
			Vector4 disVector = enemyPos - myPos;

			Vector4 createPos = myActor->GetTransform()->GetLocalPosition();
			//createPos.y += 100.0f;
			//newActor->GetTransform()->SetLoclaPosition(createPos);

			BoardIndex myIDx = ChessBoard::Inst().CalculateIndex(createPos);
			BoardIndex enemyIDx = ChessBoard::Inst().CalculateIndex(enemyPos);
			BoardIndex result = enemyIDx - myIDx;

			int dis = (int)sqrt(pow(result.x, 2) + pow(result.z, 2));

			Vector4 minusVec = disVector / (float)dis;



			for (int i = 0; i < dis; i++)
			{
				if (i == 0)
				{
					createPos.x += (minusVec.x / 2.0f);
					createPos.z += (minusVec.z / 2.0f);
				}
				else if (i == (dis - 1))
				{
					createPos.x += (minusVec.x / 1.5f);
					createPos.z += (minusVec.z / 1.5f);
				}
				else
				{
					createPos.x += minusVec.x;
					createPos.z += minusVec.z;
				}

				HPTR<Actor> actor = GetScene()->CreateActor();
				actor->GetTransform()->SetLoclaPosition(createPos);
				actor->GetTransform()->SetLocalRotX(-90.0f);
				actor->GetTransform()->SetLocalScale({ 0.7F, 0.7F , 0.7F });
				HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);

				staticRender->FbxEffectSetting(L"waterWave.FBXDATA");
				//staticRender->SetShadow(true);
				//rotLogic->SetDir(2);
				actor->AddComponent<FireMagicionSkill>();


				for (int j = 0; j < 5; j++)
				{

					Vector4 randomPos = createPos;
					randomPos.z += Well512Random::Instance().GetFloatValue(3.0f, 40.0f);
					HPTR<Actor> PLAYER2 = GetScene()->CreateActor();

					PLAYER2->GetTransform()->SetLoclaPosition(randomPos);
					PLAYER2->GetTransform()->SetLocalScale({ 150.0F, 30.0F, 1.0F });
					PLAYER2->GetTransform()->SetLocalRotX(90.0f);


					HPTR<SpriteRenderer> battleTopFrameSR = PLAYER2->AddComponent<SpriteRenderer>(RG_PLAYER);
					battleTopFrameSR->SetSprite(L"flameMagicionLight.tga", 0);
					battleTopFrameSR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
					//battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
					battleTopFrameSR->LinkCB(L"VS_CUTDATA", &(battleTopFrameSR->GetCutData()));
					battleTopFrameSR->LinkTexture(L"PS_MAPTEX", L"flameMagicionLight.tga");
					battleTopFrameSR->LinkSampler(L"PS_LSMP", L"LSMP");
					battleTopFrameSR->LinkSampler(L"PS_PSMP", L"PSMP");

					//battleTopFrameSR->SetLoclaPosition({ 0.0F, 100.0F, 0.0f });
					//battleTopFrameSR->SetLocalScale({ 125.0F, 62.5F, 1.0F, 1.0f });
					//battleTopFrameSR->BillBoardOn();

					HPTR<SpriteAnimaion> battleTopFrameSRA = PLAYER2->AddComponent<SpriteAnimaion>();
					battleTopFrameSRA->CreateAni(L"LOOP", L"flameMagicionLight.tga", 0, 4, 0.06f);

					HPTR <FireMagicionSkill> skillLogic = PLAYER2->AddComponent<FireMagicionSkill>();
					skillLogic->SetTarget(pAttTarget, myActor);

					HPTR<Actor> PLAYER3 = GetScene()->CreateActor();

					PLAYER3->GetTransform()->SetLoclaPosition(randomPos);
					PLAYER3->GetTransform()->SetLocalScale({ 150.0F, 30.0F, 1.0F });
					PLAYER3->GetTransform()->SetLocalRotX(90.0f);


					HPTR<SpriteRenderer> battleTopFrameSR2 = PLAYER3->AddComponent<SpriteRenderer>(RG_PLAYER);
					battleTopFrameSR2->SetSprite(L"Lightning01.tga", 0);
					battleTopFrameSR2->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
					//battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
					battleTopFrameSR2->LinkCB(L"VS_CUTDATA", &(battleTopFrameSR2->GetCutData()));
					battleTopFrameSR2->LinkTexture(L"PS_MAPTEX", L"Lightning01.tga");
					battleTopFrameSR2->LinkSampler(L"PS_LSMP", L"LSMP");
					battleTopFrameSR2->LinkSampler(L"PS_PSMP", L"PSMP");

					//battleTopFrameSR->SetLoclaPosition({ 0.0F, 100.0F, 0.0f });
					//battleTopFrameSR->SetLocalScale({ 125.0F, 62.5F, 1.0F, 1.0f });
					//battleTopFrameSR->BillBoardOn();

					HPTR<SpriteAnimaion> battleTopFrameSRA2 = PLAYER3->AddComponent<SpriteAnimaion>();
					battleTopFrameSRA2->CreateAni(L"LOOP", L"Lightning01.tga", 0, 3, 0.06f);

					HPTR <FireMagicionSkill> skillLogic2 = PLAYER3->AddComponent<FireMagicionSkill>();
					skillLogic2->SetTarget(pAttTarget, myActor);

					HPTR<Actor> PLAYER5 = GetScene()->CreateActor();

					PLAYER5->GetTransform()->SetLoclaPosition(randomPos);
					PLAYER5->GetTransform()->SetLocalScale({ 50.0F, 50.0F, 1.0F });
					PLAYER5->GetTransform()->SetLocalRotX(90.0f);


					HPTR<SpriteRenderer> battleTopFrameSR5 = PLAYER5->AddComponent<SpriteRenderer>(RG_PLAYER);
					battleTopFrameSR5->SetSprite(L"Lightning05.tga", 0);
					battleTopFrameSR5->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
					//battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
					battleTopFrameSR5->LinkCB(L"VS_CUTDATA", &(battleTopFrameSR5->GetCutData()));
					battleTopFrameSR5->LinkTexture(L"PS_MAPTEX", L"Lightning05.tga");
					battleTopFrameSR5->LinkSampler(L"PS_LSMP", L"LSMP");
					battleTopFrameSR5->LinkSampler(L"PS_PSMP", L"PSMP");
					battleTopFrameSR5->BillBoardOn();
					//battleTopFrameSR->SetLoclaPosition({ 0.0F, 100.0F, 0.0f });
					//battleTopFrameSR->SetLocalScale({ 125.0F, 62.5F, 1.0F, 1.0f });
					//battleTopFrameSR->BillBoardOn();

					HPTR<SpriteAnimaion> battleTopFrameSRA5 = PLAYER5->AddComponent<SpriteAnimaion>();
					battleTopFrameSRA5->CreateAni(L"LOOP", L"Lightning05.tga", 0, 13, 0.07f);

					HPTR <FireMagicionSkill> skillLogic5 = PLAYER3->AddComponent<FireMagicionSkill>();
					skillLogic5->SetTarget(pAttTarget, myActor);
				}

			}

			StatusLogic* enemyStatus = pAttTarget->GetComponent<StatusLogic>();
			if (enemyStatus != nullptr)
			{
				enemyStatus->AddHp(-500);
			}


			statusLogic->InitCurTime();
			statusLogic->SetMp(0.0f);

			Sound::Play(L"HuoNv_UnderAttack_1.wav");
		}
		pFbxAnimation->ChangeAnimation(L"IDLE");
		m_State = PIECESTATE::IDLE;
		return;
	}
}

FireMagician::FireMagician()
{
}


FireMagician::~FireMagician()
{
}
