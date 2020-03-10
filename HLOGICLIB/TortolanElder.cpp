#include "TortolanElder.h"

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



void TortolanElder::Update()
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

void TortolanElder::MakeProjecTile()
{
	HPTR<Actor> newActor = GetScene()->CreateActor();
	Vector4 createPos = myActor->GetTransform()->GetLocalPosition();
	//createPos.y += 100.0f;
	newActor->GetTransform()->SetLoclaPosition(createPos);
	newActor->GetTransform()->SetLocalScale({ 1.0F , 1.0f, 1.0f });


	HPTR<SpriteRenderer> spriteRender = newActor->AddComponent<SpriteRenderer>(RG_PLAYER);
	spriteRender->SetSprite(L"T_GPP_MagicBall.tga", 0);
	spriteRender->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
	//battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
	spriteRender->LinkCB(L"VS_CUTDATA", &(spriteRender->GetCutData()));
	spriteRender->LinkTexture(L"PS_MAPTEX", L"T_GPP_MagicBall.tga");
	spriteRender->LinkSampler(L"PS_LSMP", L"LSMP");
	spriteRender->LinkSampler(L"PS_PSMP", L"PSMP");

	//battleTopFrameSR->SetLoclaPosition({ 0.0F, 100.0F, 0.0f });
	spriteRender->SetLocalScale({ 40.0F, 40.0F, 1.0F, 1.0f });
	spriteRender->BillBoardOn();

	HPTR<SpriteAnimaion> battleTopFrameSRA = newActor->AddComponent<SpriteAnimaion>();
	battleTopFrameSRA->CreateAni(L"LOOP", L"T_GPP_MagicBall.tga", 0, 15, 0.1f);

	HPTR<ProjectTileLogic> projectLogic = newActor->AddComponent<ProjectTileLogic>();
	projectLogic->SetTarget(pAttTarget);

	Sound::Play(L"GuangFa_Attack_1.wav");
}

void TortolanElder::Skill()
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

		chargeRender->SetLoclaPosition({ 0.0F, 0.0F, 0.0f });
		chargeRender->SetLocalScale({ 400.0F, 400.0F, 1.0F, 1.0f });
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
			skillRender->SetSprite(L"windRangerEffect.tga", 0);
			skillRender->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
			//battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
			skillRender->LinkCB(L"VS_CUTDATA", &(skillRender->GetCutData()));
			skillRender->LinkTexture(L"PS_MAPTEX", L"windRangerEffect.tga");
			skillRender->LinkSampler(L"PS_LSMP", L"LSMP");
			skillRender->LinkSampler(L"PS_PSMP", L"PSMP");

			skillRender->SetLoclaPosition({ 0.0F, 1.0F, 0.0f });
			skillRender->SetLocalScale({ 700.0F, 600.0F, 1.0F, 1.0f });
			skillRender->SetLocalRotZ(90.0f);

			HPTR<SpriteAnimaion> skillAni = chessPlayer2->AddComponent<SpriteAnimaion>();
			skillAni->CreateAniRoll(L"LOOP", L"windRangerEffect.tga", 0, 15, 0.05f);


			HPTR<WindRangerSkill> skillLogic = chessPlayer2->AddComponent<WindRangerSkill>();
			skillLogic->SetTarget(pAttTarget);


			Sound::Play(L"GuangFa_UnderAttack_2.wav");

			isSkillStart = false;
			statusLogic->InitCurTime();
			statusLogic->SetMp(0.0f);
			chargeActor->Die();
			chargeActor = nullptr;
		}
		pFbxAnimation->ChangeAnimation(L"IDLE");
		m_State = PIECESTATE::IDLE;

		

		return;
	}
}

TortolanElder::TortolanElder()
{
}


TortolanElder::~TortolanElder()
{
}
