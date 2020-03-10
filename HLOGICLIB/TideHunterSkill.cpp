#include "TideHunterSkill.h"

#include <RANDOM.h>
#include "ChessBoard.h"
#include "StatusLogic.h"
#include <SCENE.h>
#include "LOGICHEADER.h"
#include <2DANICOM.h>
#include "BaseEffect.h"
#include <StaticFbxRender.h>
#include <3DANIRENDER.h>
#include <SOUND.h>

void TideHunterSkill::Init()
{
	myActor = GetActor();
	Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
	m_Index = ChessBoard::Inst().CalculateIndex(myPos);

	tenta.resize(81);
}

void TideHunterSkill::Update()
{
	curTime += Time::DeltaTime();
	skillTime += Time::DeltaTime();

	Vector4 actorPos = GetActor()->GetTransform()->GetLocalPosition();

	if(skillTime > 0.05f && tentacleIdx < 81)
	{
		idxZ = ((tentacleIdx - 1) / 9) - 2;
		idxX = ((tentacleIdx - 1) % 9) - 2;

		if ((moveDestIdx.z + idxZ >= 0 && moveDestIdx.z + idxZ <= 7) && (moveDestIdx.x + idxX >= 0 && moveDestIdx.x + idxX <= 7))
		{
			Actor* target = ChessBoard::Inst().m_Board[moveDestIdx.z + idxZ][moveDestIdx.x + idxX].m_Actor;
			if (target != nullptr)
			{
				StatusLogic* statusLogic = target->GetComponent<StatusLogic>();
				if (statusLogic->GetType() == PIECETYPE::ENEMY)
				{
					tenta[tentacleIdx] = GetScene()->CreateActor();
					tenta[tentacleIdx]->GetTransform()->SetLocalScale({ 3.0F, 3.0F ,3.0F });
					tenta[tentacleIdx]->GetTransform()->SetLoclaPosition(target->GetTransform()->GetLocalPosition());

					HPTR<FbxAnimation> chessPlayerAni2 = tenta[tentacleIdx]->AddComponent<FbxAnimation>(RG_PLAYER);
					chessPlayerAni2->FbxEffectSetting(L"tentacle.FBXDATA");
					chessPlayerAni2->CreateAnimation(L"ATT", 0, 19, 0.3f, true);
					chessPlayerAni2->ChangeAnimation(L"ATT");

					HPTR<TentacleSkill> skillLogic = tenta[tentacleIdx]->AddComponent<TentacleSkill>();
					skillLogic->SetTarget(target);

					skillTime = 0.0f;

					Sound::Play(L"ChaoXi_UnderAttack_2.wav");
				}
			}
		}
		tentacleIdx++;
	}


	if (curTime > 3.0f)
	{
		myActor->Die();
	}
}

void TideHunterSkill::SetTarget(Actor* _Actor)
{
	myActor = GetActor();

	Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
	m_Index = ChessBoard::Inst().CalculateIndex(myPos);


	pAttTarget = _Actor;
	moveDestPos = _Actor->GetTransform()->GetLocalPosition();
	moveDestIdx = ChessBoard::Inst().CalculateIndex(moveDestPos);
}

void TideHunterSkill::SetTarget(Actor* _Actor, float _Degree)
{
	myActor = GetActor();

	rotateDegree = _Degree;

	Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
	m_Index = ChessBoard::Inst().CalculateIndex(myPos);

	moveDestPos = _Actor->GetTransform()->GetLocalPosition();
	moveDestIdx = ChessBoard::Inst().CalculateIndex(moveDestPos);
}


TideHunterSkill::TideHunterSkill() : isRotate(false), myActor(nullptr), curTime(0.0f), skillTime(0.0f), tentacleIdx(0), idxX(0), idxZ(0), skillOn(false)
{
}


TideHunterSkill::~TideHunterSkill()
{
}



void TentacleSkill::Init()
{
	myActor = GetActor();
}

void TentacleSkill::Update()
{
	curTime += Time::DeltaTime();

	if (curTime < 0.75f)
	{
		pAttTarget->GetTransform()->SetLoclaPositionY(curTime * 3000.0f);
	}
	else if (curTime < 1.5f)
	{
		pAttTarget->GetTransform()->SetLoclaPositionY((1.5f - curTime) * 3000.0f);
	}
	else
	{
		pAttTarget->GetTransform()->SetLoclaPositionY(0.0f);
	}

	if (curTime > 1.6f)
	{
		myActor->Die();
	}
}

void TentacleSkill::SetTarget(Actor* _Actor)
{
	myActor = GetActor();

	Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
	m_Index = ChessBoard::Inst().CalculateIndex(myPos);

	pAttTarget = _Actor;
	moveDestPos = _Actor->GetTransform()->GetLocalPosition();
	moveDestIdx = ChessBoard::Inst().CalculateIndex(moveDestPos);
}



TentacleSkill::TentacleSkill() : myActor(nullptr), curTime(0.0f)
{
}


TentacleSkill::~TentacleSkill()
{
}
