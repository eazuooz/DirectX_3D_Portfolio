#include "LittleDragon.h"

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

#include <SOUND.h>

void LittleDragon::Update()
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

void LittleDragon::MakeProjecTile()
{
	if (pAttTarget != nullptr)
	{


		Vector4 createPos = myActor->GetTransform()->GetLocalPosition();
		//createPos.y += 100.0f;
		//newActor->GetTransform()->SetLoclaPosition(createPos);

		HPTR<Actor> PLAYER2 = GetScene()->CreateActor();
		PLAYER2->GetTransform()->SetLoclaPosition(createPos);
		PLAYER2->GetTransform()->SetLocalScale({ 20.0F, 20.0F, 20.0F });

		HPTR<Renderer> SR2 = PLAYER2->AddComponent<Renderer>(RG_PLAYER);
		Renderer::HRENDERDATA* Data = SR2->RenderData(L"SPHEREMESH", L"EFFECTDPATH");
		//Data->m_Path->m_BUMP = BUMPOK;
		//Data->m_bShadow = false;

		SR2->CreateCB(L"VS_CUTDATA", Vector4(0.0f, 0.0f, 1.0f, 1.0f));
		// 직접 내가 상수값을 들고 있는 경우.
		SR2->LinkTexture(L"PS_MAPTEX", L"T_GPP_Plasma_A.tga");
		//SR2->LinkTexture(L"PS_BUMPTEX", L"T_GPP_Trail_Plasma_A.tga");

		SR2->LinkSampler(L"PS_LSMP", L"LSMP");
		SR2->LinkSampler(L"PS_PSMP", L"PSMP");

		HPTR<SpriteRenderer> battleTopFrameSR = PLAYER2->AddComponent<SpriteRenderer>(RG_PLAYER);
		battleTopFrameSR->SetSprite(L"CircleEffectSprite.png", 0);
		battleTopFrameSR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		//battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
		battleTopFrameSR->LinkCB(L"VS_CUTDATA", &(battleTopFrameSR->GetCutData()));
		battleTopFrameSR->LinkTexture(L"PS_MAPTEX", L"CircleEffectSprite.png");
		battleTopFrameSR->LinkSampler(L"PS_LSMP", L"LSMP");
		battleTopFrameSR->LinkSampler(L"PS_PSMP", L"PSMP");

		//battleTopFrameSR->SetLoclaPosition({ 0.0F, 100.0F, 0.0f });
		battleTopFrameSR->SetLocalScale({ 25.0F, 25.0F, 1.0F, 1.0f });
		battleTopFrameSR->BillBoardOn();

		HPTR<SpriteAnimaion> battleTopFrameSRA = PLAYER2->AddComponent<SpriteAnimaion>();
		battleTopFrameSRA->CreateAni(L"LOOP", L"CircleEffectSprite.png", 0, 24, 0.06f);



		PLAYER2->AddComponent<EffectRotate>();

		HPTR<ProjectTileLogic> projectLogic = PLAYER2->AddComponent<ProjectTileLogic>();
		projectLogic->SetTarget(pAttTarget);


		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition(createPos);
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 0.5F, 0.5F ,0.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);

		staticRender->FbxEffectSetting(L"circlePlasma.FBXDATA");
		//staticRender->SetShadow(true);
		actor->AddComponent<EffectRotate>();

		HPTR<ProjectTileLogic> projectLogic2 = actor->AddComponent<ProjectTileLogic>();
		projectLogic2->SetTarget(pAttTarget);
	
		Sound::Play(L"JingLingLong_Attack_1.wav");
	
	}


}

void LittleDragon::Skill()
{
	CurPosIdxCheck();
	if (pFbxAnimation->GetCurClip()->m_AniEnd == true)
	{
		if (pAttTarget != nullptr)
		{
			Vector4 createPos = myActor->GetTransform()->GetLocalPosition();
			//이펙트 테스트용
			HPTR<Actor> chessPlayer2 = GetScene()->CreateActor();
			chessPlayer2->GetTransform()->SetLocalScale({ 2.0F, 2.0F ,2.0F });
			chessPlayer2->GetTransform()->SetLoclaPosition(createPos);

			HPTR<FbxAnimation> chessPlayerAni2 = chessPlayer2->AddComponent<FbxAnimation>(RG_PLAYER);
			chessPlayerAni2->FbxEffectSetting(L"LittleDragonSkill.FBXDATA");
			chessPlayerAni2->CreateAnimation(L"ATT", 0, 20, true);
			//SR->CreateAn2imation(L"ATT", 886, 909, true);
			chessPlayerAni2->ChangeAnimation(L"ATT");


			HPTR<LittleDragonSkill> skillLogic = chessPlayer2->AddComponent<LittleDragonSkill>();
			skillLogic->SetTarget(pAttTarget);

			Sound::Play(L"JingLingLong_UnderAttack_1.wav");

			statusLogic->InitCurTime();
			statusLogic->SetMp(0.0f);
		}
		pFbxAnimation->ChangeAnimation(L"IDLE");
		m_State = PIECESTATE::IDLE;
		return;
	}


}



LittleDragon::LittleDragon()
{
}


LittleDragon::~LittleDragon()
{
}
