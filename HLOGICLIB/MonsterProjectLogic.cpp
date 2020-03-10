#include "MonsterProjectLogic.h"
#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "UnicornSkill.h"
#include "ProjectTileLogic.h"
#include "SerpentWard.h"


MonsterProjectLogic::MonsterProjectLogic() : isRock(false), rockTime(0.0f)
{
	isMonster = true;
}


MonsterProjectLogic::~MonsterProjectLogic()
{
}

void MonsterProjectLogic::Update()
{
	if (statusLogic == nullptr)
	{
		statusLogic = GetActor()->GetComponent<StatusLogic>();
	}

	if (statusLogic->GetHp() < 0)
	{
		m_State = PIECESTATE::DIE;
	}
	else if (pFbxAnimation != nullptr && pFbxAnimation->IsLock() == true)
	{

		return;
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
		//pFbxAnimation->LockOn();
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


void MonsterProjectLogic::MakeProjecTile()
{
	HPTR<Actor> newActor = GetScene()->CreateActor();
	Vector4 createPos = myActor->GetTransform()->GetLocalPosition();
	//createPos.y += 100.0f;
	newActor->GetTransform()->SetLoclaPosition(createPos);
	newActor->GetTransform()->SetLocalScale({ 1.0F , 1.0f, 1.0f });


	HPTR<SpriteRenderer> spriteRender = newActor->AddComponent<SpriteRenderer>(RG_PLAYER);
	spriteRender->SetSprite(L"EF_water_03_01.png", 0);
	spriteRender->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
	//battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
	spriteRender->LinkCB(L"VS_CUTDATA", &(spriteRender->GetCutData()));
	spriteRender->LinkTexture(L"PS_MAPTEX", L"EF_water_03_01.png");
	spriteRender->LinkSampler(L"PS_LSMP", L"LSMP");
	spriteRender->LinkSampler(L"PS_PSMP", L"PSMP");

	//battleTopFrameSR->SetLoclaPosition({ 0.0F, 100.0F, 0.0f });
	spriteRender->SetLocalScale({ 80.0F, 80.0F, 1.0F, 1.0f });
	spriteRender->BillBoardOn();

	HPTR<SpriteAnimaion> battleTopFrameSRA = newActor->AddComponent<SpriteAnimaion>();
	battleTopFrameSRA->CreateAni(L"LOOP", L"EF_water_03_01.png", 0, 15, 0.1f);

	HPTR<ProjectTileLogic> projectLogic = newActor->AddComponent<ProjectTileLogic>();
	projectLogic->SetTarget(pAttTarget);
}


