#include "BladeKnight.h"
#include "StatusLogic.h"
#include <2DANICOM.h>
#include <SCENE.h>
#include <StaticFbxRender.h>

#include "StatusLogic.h"
#include "ChessBoard.h"
#include "LOGICHEADER.h"
#include "UnicornSkill.h"
#include "ProjectTileLogic.h"
#include "EffectRotate.h"
#include "BladeKnightWeapon.h"

#include <SOUND.h>

BladeKnight::BladeKnight()
{
	isPassive = true;
}


BladeKnight::~BladeKnight()
{
}

void BladeKnight::Update()
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

	if (isPassive == true)
	{
		Passive();
	}
}

void BladeKnight::MakeProjecTile()
{
	if (pAttTarget != nullptr)
	{
	HPTR<Actor> newActor = GetScene()->CreateActor();
	Vector4 createPos = myActor->GetTransform()->GetLocalPosition();
	//createPos.y += 100.0f;
	newActor->GetTransform()->SetLoclaPosition(createPos);
	newActor->GetTransform()->SetLocalScale({ 0.7F , 0.7f, 0.7f });


	HPTR<StaticFbxRender> staticRender = newActor->AddComponent<StaticFbxRender>(RG_PLAYER);
	staticRender->FbxSetting(L"BladeKnight_Weapon.FBXDATA");
	staticRender->SetShadow(true);


		HPTR<BladeKnightWeapon> projectLogic = newActor->AddComponent<BladeKnightWeapon>();
		projectLogic->SetTarget(pAttTarget);

		HPTR<EffectRotate> rotateLogic = newActor->AddComponent<EffectRotate>();
		rotateLogic->SetRotSpeed(800.0f);

		Sound::Play(L"YueQi_Attack_2.wav");
	}
}

void BladeKnight::Skill()
 {

}
void BladeKnight::Passive()
{

}