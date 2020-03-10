#include "DoomArbiterSkill.h"

#include <RANDOM.h>
#include "ChessBoard.h"
#include "StatusLogic.h"
#include <SCENE.h>
#include "LOGICHEADER.h"
#include <2DANICOM.h>
#include "BaseEffect.h"

void DoomArbiterSkill::Init()
{
}

void DoomArbiterSkill::Update()
{
	curTime += Time::DeltaTime();
	divTime += Time::DeltaTime();


	moveDestPos = pMoveTarget->GetTransform()->GetLocalPosition();
	moveDestIdx = ChessBoard::Inst().CalculateIndex(moveDestPos);

	myActor->GetTransform()->SetLoclaPosition(moveDestPos);
	myActor->GetTransform()->SetLocalRotY(curTime * 100.0f);



	if (divTime > 1.0f)
	{
		Actor* enemyActor = ChessBoard::Inst().m_Board[moveDestIdx.z][moveDestIdx.x].m_Actor;
		if (enemyActor != nullptr)
		{
			StatusLogic* enemyStatus = enemyActor->GetComponent<StatusLogic>();
			PIECETYPE type = enemyStatus->GetType();
			if (type == ENEMY)
			{
				enemyStatus->AddHp(-50);
			}
		}

		divTime = 0.0f;
	}



	if (curTime > 10.0f)
	{
		myActor->Die();
	}
}

void DoomArbiterSkill::SetTarget(Actor* _Actor)
{
	myActor = GetActor();

	Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
	m_Index = ChessBoard::Inst().CalculateIndex(myPos);

	pMoveTarget = _Actor;
	moveDestPos = _Actor->GetTransform()->GetLocalPosition();
	moveDestIdx = ChessBoard::Inst().CalculateIndex(moveDestPos);
}

void DoomArbiterSkill::SetTarget(Actor* _Actor, float _Degree)
{
	myActor = GetActor();

	rotateDegree = _Degree;

	Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
	m_Index = ChessBoard::Inst().CalculateIndex(myPos);

	moveDestPos = _Actor->GetTransform()->GetLocalPosition();
	moveDestIdx = ChessBoard::Inst().CalculateIndex(moveDestPos);
}

Vector4 DoomArbiterSkill::Rotate(BoardIndex _DestIdx, float _Degree)
{
	Vector4 standDir = { 1.0f, 0.0f, 0.0f, 0.0f };
	Vector4 resultDir = Vector4((float)_DestIdx.x, 0.0f, (float)_DestIdx.z, 0.0f) - Vector4((float)m_Index.x, 0.0f, (float)m_Index.z, 0.0f);

	//if (resultDir.x < 0.0f)
	//{
	//	standDir *= 1.0f;
	//}

	Vector4 crossVector = Vector4::CrossProduct(standDir, resultDir);

	float cosDegree = Vector4::DotProduct(resultDir.ReturnNormalization(), standDir);
	float resultDegree = acos(cosDegree) * Constant::R2D + 90.0f + _Degree;

	if (_DestIdx.z > m_Index.z)
	{
		resultDegree += 180.0f;
		if (_DestIdx.x < m_Index.x)
		{
			resultDegree -= 90.0f;
		}
		else if (_DestIdx.x > m_Index.x)
		{
			int a = 0;
			resultDegree += 90.0f;
		}
		else
		{

		}
	}
	else if (_DestIdx.z < m_Index.z)
	{
		if (_DestIdx.x < m_Index.x)
		{

		}
		else if (_DestIdx.x > m_Index.x)
		{

		}
		else
		{

		}
	}
	else
	{

	}



	GetTransform()->SetLocalRotY(-resultDegree);

	Vector4 returnDir = { resultDir.x, 0.0f, -resultDir.z };

	return returnDir.ReturnNormalization();
}

DoomArbiterSkill::DoomArbiterSkill() : isRotate(false), myActor(nullptr), curTime(0.0f), divTime(0.0f)
{
}


DoomArbiterSkill::~DoomArbiterSkill()
{
}
