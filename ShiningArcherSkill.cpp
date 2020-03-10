#include "ShiningArcherSkill.h"

#include <RANDOM.h>
#include "ChessBoard.h"
#include "StatusLogic.h"
#include <SCENE.h>
#include "LOGICHEADER.h"
#include <2DANICOM.h>
#include "BaseEffect.h"

#include <SOUND.h>

void ShiningArcherSkill::Init()
{
}

void ShiningArcherSkill::Update()
{
	Vector4 myPos = myActor->GetTransform()->GetLocalPosition();
	BoardIndex temp = ChessBoard::Inst().CalculateIndex(myPos);


	if (isRotate == false)
	{
		myActor->GetTransform()->SetLoclaPosition(moveDestPos + Vector4(0.0f, 2500.f, 0.0f));
		Rotate(moveDestIdx, 0.0f);

		isRotate = true;
	}


	curTime += Time::DeltaTime();
	if (myActor == nullptr)
	{

	}
	else
	{


		myActor->GetTransform()->LocalTranslate(Vector4::DOWN * Time::SDeltaTime(2500.0f));

	}

	particleTime += Time::DeltaTime();


	if (particleTime > 0.0005f)
	{
		HPTR<Actor> PLAYER2 = myActor->GetScene()->CreateActor();

		PLAYER2->GetTransform()->SetLoclaPosition(myPos);
		PLAYER2->GetTransform()->SetLocalScale({ 40.0F, 40.0F, 40.0F });

		HPTR<SpriteRenderer> battleTopFrameSR = PLAYER2->AddComponent<SpriteRenderer>(RG_PLAYER);
		battleTopFrameSR->SetSprite(L"starsp.png", 0);
		battleTopFrameSR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		battleTopFrameSR->LinkCB(L"VS_CUTDATA", &(battleTopFrameSR->GetCutData()));
		battleTopFrameSR->LinkTexture(L"PS_MAPTEX", L"starsp.png");
		battleTopFrameSR->LinkSampler(L"PS_LSMP", L"LSMP");
		battleTopFrameSR->SetLocalScale({ 60.0F, 60.0F, 1.0F, 1.0f });
		battleTopFrameSR->BillBoardOn();
		HPTR<SpriteAnimaion> battleTopFrameSRA = PLAYER2->AddComponent<SpriteAnimaion>();
		battleTopFrameSRA->CreateAni(L"LOOP", L"starsp.png", 0, 16, 0.06f, false);

		PLAYER2->AddComponent<BaseEffect>();

		particleTime = 0.0f;
	}

	if (false == falldown)
	{
		Vector4 curpos = myActor->GetTransform()->GetLocalPosition();

		if (curpos.y < 0)
		{
			HPTR<Actor> pl = myActor->GetScene()->CreateActor();

			pl->GetTransform()->SetLoclaPosition(curpos + Vector4(0.0f, 20.0f - curpos.y, 0.0f));
			pl->GetTransform()->SetLocalScale({ 1.0F, 1.0F, 1.0F });

			HPTR<SpriteRenderer> battleTopFrameSR = pl->AddComponent<SpriteRenderer>(RG_PLAYER);
			battleTopFrameSR->SetSprite(L"starfl.png", 0);
			battleTopFrameSR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
			battleTopFrameSR->LinkCB(L"VS_CUTDATA", &(battleTopFrameSR->GetCutData()));
			battleTopFrameSR->LinkTexture(L"PS_MAPTEX", L"starfl.png");
			battleTopFrameSR->LinkSampler(L"PS_LSMP", L"LSMP");
			battleTopFrameSR->SetLocalScale({ 500.0F, 500.0F, 1.0F, 1.0f });
			battleTopFrameSR->SetLocalRotX(90.0f);
			//battleTopFrameSR->BillBoardOn();
			HPTR<SpriteAnimaion> battleTopFrameSRA = pl->AddComponent<SpriteAnimaion>();
			battleTopFrameSRA->CreateAniRoll(L"LOOP", L"starfl.png", 0, 35, 0.025f, false);

			pl->AddComponent<BaseEffect>();
			Sound::Play(L"ShouWei_UnderAttack_1.wav");
			falldown = true;

			Actor* enemyActor = ChessBoard::Inst().m_Board[moveDestIdx.z][moveDestIdx.x].m_Actor;
			if (enemyActor != nullptr)
			{
				StatusLogic* enemyStatus = enemyActor->GetComponent<StatusLogic>();
				PIECETYPE type = enemyStatus->GetType();
				if (type == ENEMY)
				{
					enemyStatus->AddHp(-200);
				}
			}
		}
	}


	if (curTime > 7.0f)
	{
		myActor->Die();
	}
}

void ShiningArcherSkill::SetTarget(Actor* _Actor)
{
	myActor = GetActor();

	Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
	m_Index = ChessBoard::Inst().CalculateIndex(myPos);

	moveDestPos = _Actor->GetTransform()->GetLocalPosition();
	moveDestIdx = ChessBoard::Inst().CalculateIndex(moveDestPos);
}

void ShiningArcherSkill::SetTarget(Actor* _Actor, float _Degree)
{
	myActor = GetActor();

	rotateDegree = _Degree;

	Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
	m_Index = ChessBoard::Inst().CalculateIndex(myPos);

	moveDestPos = _Actor->GetTransform()->GetLocalPosition();
	moveDestIdx = ChessBoard::Inst().CalculateIndex(moveDestPos);
}

Vector4 ShiningArcherSkill::Rotate(BoardIndex _DestIdx, float _Degree)
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

ShiningArcherSkill::ShiningArcherSkill() : falldown(false), isRotate(false), myActor(nullptr), curTime(0.0f), particleTime(0.0f)
{
}


ShiningArcherSkill::~ShiningArcherSkill()
{
}
