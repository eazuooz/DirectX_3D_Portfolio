#include "ShiningArcher.h"

#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "ProjectTileLogic.h"

#include "ShiningArcherSkill.h"

#include <SOUND.h>

void ShiningArcher::Update()
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

void ShiningArcher::MakeProjecTile()
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

		Sound::Play(L"SheFa_Attack_2.wav");
	}
}

void ShiningArcher::Skill()
{
	CurPosIdxCheck();
	if (pFbxAnimation->GetCurClip()->m_AniHalf == true)
	{
		HPTR<Actor> newActor = GetScene()->CreateActor(L"Spark");
		Vector4 skillPos = myActor->GetTransform()->GetLocalPosition();

		skillPos.y += 100.0f;
		newActor->GetTransform()->SetPosition(skillPos);
		newActor->GetTransform()->SetLocalScale({ 1.2f, 1.0f, 1.2f });

		HPTR<SpriteRenderer> skillRender = newActor->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		skillRender->SetLoclaPosition({ 0.0F, 1.0f, 0.0f });
		skillRender->SetLocalScale({ 40.0f, 40.0f, 1.0f, 1.0f });
		skillRender->SetLocalRotX(90.0f);
		skillRender->SetSprite(L"staref.png", 0);
		skillRender->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		skillRender->LinkCB(L"VS_CUTDATA", &(skillRender->GetCutData()));
		skillRender->LinkTexture(L"PS_MAPTEX", L"staref.png");
		skillRender->LinkSampler(L"PS_LSMP", L"LSMP");

		HPTR<SpriteAnimaion> skillAni = newActor->AddComponent<SpriteAnimaion>();
		skillAni->CreateAni(L"LOOP", L"staref.png", 0, 16, 0.02f, false);
	}
	if (pFbxAnimation->GetCurClip()->m_AniEnd == true)
	{
		if (pAttTarget != nullptr)
		{
			Vector4 createPos = myActor->GetTransform()->GetLocalPosition();
			HPTR<Actor> chessPlayer2 = GetScene()->CreateActor();
			chessPlayer2->GetTransform()->SetLocalScale({ 2.0F, 2.0F ,2.0F });
			chessPlayer2->GetTransform()->SetLoclaPosition(createPos);

			HPTR<ShiningArcherSkill> skillLogic = chessPlayer2->AddComponent<ShiningArcherSkill>();
			skillLogic->SetTarget(pAttTarget);

			statusLogic->InitCurTime();
			statusLogic->SetMp(0.0f);
		}

		Sound::Play(L"ShouWei_UnderAttack_2.wav");
		pFbxAnimation->ChangeAnimation(L"IDLE");
		m_State = PIECESTATE::IDLE;
		return;
	}


}

ShiningArcher::ShiningArcher()
{
}


ShiningArcher::~ShiningArcher()
{
}
