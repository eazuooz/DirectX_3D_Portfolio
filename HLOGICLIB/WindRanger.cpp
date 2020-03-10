#include "WindRanger.h"

#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "UnicornSkill.h"
#include "ProjectTileLogic.h"
#include "LittleDragonSkill.h"
#include "WindRangerSkill.h"

#include <SOUND.h>

void WindRanger::Update()
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

void WindRanger::MakeProjecTile()
{
	if (pAttTarget != nullptr)
	{
		HPTR<Actor> newActor = GetScene()->CreateActor();
		Vector4 createPos = myActor->GetTransform()->GetLocalPosition();
		//createPos.y += 100.0f;
		newActor->GetTransform()->SetLoclaPosition(createPos);
		newActor->GetTransform()->SetLocalScale({ 1.0F , 1.0f, 1.0f });

		HPTR<StaticFbxRender> staticRender = newActor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Unicorn_Weapon.FBXDATA");
		staticRender->SetShadow(true);

		HPTR<ProjectTileLogic> projectLogic = newActor->AddComponent<ProjectTileLogic>();
		projectLogic->SetTarget(pAttTarget);

		Sound::Play(L"FengXing_Attack_1.wav");
	}
}

void WindRanger::Skill()
{
	CurPosIdxCheck();
	if (isSkillStart == false)
	{
		Vector4 createPos = myActor->GetTransform()->GetLocalPosition();
		//이펙트 테스트용
		chargeActor = GetScene()->CreateActor();
		chargeActor->GetTransform()->SetLocalScale({ 1.0F, 1.0F , 1.0F });
		chargeActor->GetTransform()->SetLoclaPosition(createPos);
		chargeActor->GetTransform()->SetLocalRotX(90.0f);
			   

		HPTR<SpriteRenderer> chargeRender = chargeActor->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		chargeRender->SetSprite(L"chargeEffect.tga", 0);
		chargeRender->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		//battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
		chargeRender->LinkCB(L"VS_CUTDATA", &(chargeRender->GetCutData()));
		chargeRender->LinkTexture(L"PS_MAPTEX", L"chargeEffect.tga");
		chargeRender->LinkSampler(L"PS_LSMP", L"LSMP");
		chargeRender->LinkSampler(L"PS_PSMP", L"PSMP");
		
		chargeRender->SetLoclaPosition({ 0.0F, 60.0F, 0.0f });
		chargeRender->SetLocalScale({ 300.0F, 300.0F, 1.0F, 1.0f });
		//chargeRender->BillBoardOn();

		
		HPTR<SpriteAnimaion> chargeAni = chargeActor->AddComponent<SpriteAnimaion>();
		chargeAni->CreateAniRoll(L"LOOP", L"chargeEffect.tga", 0, 59, 0.07f);

		isSkillStart = true;
	}



	if (pFbxAnimation->GetCurClip()->m_AniEnd == true)
	{
		if (pAttTarget != nullptr)
		{
			Vector4 createPos = myActor->GetTransform()->GetLocalPosition();
			//이펙트 테스트용
			HPTR<Actor> chessPlayer2 = GetScene()->CreateActor();
			chessPlayer2->GetTransform()->SetLocalScale({ 1.0F, 1.0F , 1.0F });
			chessPlayer2->GetTransform()->SetLoclaPosition(createPos);
			chessPlayer2->GetTransform()->SetLocalRotX(90.0f);


			HPTR<SpriteRenderer> skillRender = chessPlayer2->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
			skillRender->SetSprite(L"torEffect.tga", 0);
			skillRender->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
			//battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
			skillRender->LinkCB(L"VS_CUTDATA", &(skillRender->GetCutData()));
			skillRender->LinkTexture(L"PS_MAPTEX", L"torEffect.tga");
			skillRender->LinkSampler(L"PS_LSMP", L"LSMP");
			skillRender->LinkSampler(L"PS_PSMP", L"PSMP");

			skillRender->SetLoclaPosition({ 0.0F, 1.0F, 0.0f });
			skillRender->SetLocalScale({ 800.0F, 600.0F, 1.0F, 1.0f });
			skillRender->SetLocalRotZ(270.0f);

			HPTR<SpriteAnimaion> skillAni = chessPlayer2->AddComponent<SpriteAnimaion>();
			skillAni->CreateAniRoll(L"LOOP", L"torEffect.tga", 0, 143, 0.04f);

			
			HPTR<WindRangerSkill> skillLogic = chessPlayer2->AddComponent<WindRangerSkill>();
			skillLogic->SetTarget(pAttTarget);



			isSkillStart = false;
			statusLogic->InitCurTime();
			statusLogic->SetMp(0.0f);
			if (chargeActor != nullptr)
			{
				chargeActor->Die();
				chargeActor = nullptr;
			}
			Sound::Play(L"FengXingSkill_Launch.wav");
			Sound::Play(L"FengXingSkill_Object.wav");
		}
		pFbxAnimation->ChangeAnimation(L"IDLE");
		m_State = PIECESTATE::IDLE;




		return;
	}

}


WindRanger::WindRanger() : isSkillStart(false)
{
}


WindRanger::~WindRanger()
{
}
