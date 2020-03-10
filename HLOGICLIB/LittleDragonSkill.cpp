#include "LittleDragonSkill.h"

#include <RANDOM.h>
#include "ChessBoard.h"
#include "StatusLogic.h"
#include <SCENE.h>
#include "LOGICHEADER.h"
#include <2DANICOM.h>
#include "BaseEffect.h"
#include <SOUND.h>


void LittleDragonSkill::Init()
{
	Sound::Play(L"JingLingLong_Skill.wav");
	Sound::Play(L"JingLingLong_Skill_Object.wav", L"JingLingLong_Skill_Object.wav", 999);
}

void LittleDragonSkill::Update()
{
	Vector4 myPos = myActor->GetTransform()->GetLocalPosition();
	BoardIndex temp = ChessBoard::Inst().CalculateIndex(myPos);

	if (m_Index != temp)
	{
		m_Index = temp;


		if ((m_Index.x >= 0 && m_Index.z >= 0) && 
			(m_Index.x <= 7 && m_Index.z <= 7))
		{
			Actor* enemyActor = ChessBoard::Inst().m_Board[m_Index.z][m_Index.x].m_Actor;
			if (enemyActor != nullptr)
			{
				StatusLogic* enemyStatus = enemyActor->GetComponent<StatusLogic>();
				PIECETYPE type = enemyStatus->GetType();
				if (type == ENEMY)
				{
					enemyStatus->AddHp(-150);
				}
			}
		}
	}
		   
	if (isRotate == false)
	{

			Rotate(moveDestIdx, 0.0f);
		
		isRotate = true;
	}


	curTime += Time::DeltaTime();
	if (myActor == nullptr)
	{
		
	}
	else
	{

			myActor->GetTransform()->LocalTranslate(GetActor()->GetTransform()->LocalLookBack() * Time::SDeltaTime(500.0f));
	
			myActor->GetTransform()->SetLocalRotZ(200.0f * curTime);
	}

	particleTime += Time::DeltaTime();
	
	if (particleTime > 0.1f)
	{
		HPTR<Actor> PLAYER2 = myActor->GetScene()->CreateActor();

		myPos.x += Well512Random::Instance().GetFloatValue(0.0f, 60.0f);
		myPos.z += Well512Random::Instance().GetFloatValue(0.0f, 60.0f);

		PLAYER2->GetTransform()->SetLoclaPosition(myPos);
		PLAYER2->GetTransform()->SetLocalScale({ 20.0F, 20.0F, 20.0F });

		HPTR<SpriteRenderer> battleTopFrameSR = PLAYER2->AddComponent<SpriteRenderer>(RG_PLAYER);
		battleTopFrameSR->SetSprite(L"CircleEffectSprite_Purple.png", 0);
		battleTopFrameSR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		battleTopFrameSR->LinkCB(L"VS_CUTDATA", &(battleTopFrameSR->GetCutData()));
		battleTopFrameSR->LinkTexture(L"PS_MAPTEX", L"CircleEffectSprite_Purple.png");
		battleTopFrameSR->LinkSampler(L"PS_LSMP", L"LSMP");
		battleTopFrameSR->LinkSampler(L"PS_PSMP", L"PSMP");
		battleTopFrameSR->SetLocalScale({ 25.0F, 25.0F, 1.0F, 1.0f });
		battleTopFrameSR->BillBoardOn();
		HPTR<SpriteAnimaion> battleTopFrameSRA = PLAYER2->AddComponent<SpriteAnimaion>();
		battleTopFrameSRA->CreateAni(L"LOOP", L"CircleEffectSprite_Purple.png", 0, 24, 0.06f);

		PLAYER2->AddComponent<BaseEffect>();

		particleTime = 0.0f;
	}

	

	if (curTime > 5.0f)
	{
		Sound::Stop(L"JingLingLong_Skill_Object.wav");
		myActor->Die();
	}
}

void LittleDragonSkill::SetTarget(Actor* _Actor)
{
	myActor = GetActor();

	Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
	m_Index = ChessBoard::Inst().CalculateIndex(myPos);

	moveDestPos = _Actor->GetTransform()->GetLocalPosition();
	moveDestIdx = ChessBoard::Inst().CalculateIndex(moveDestPos);
}

void LittleDragonSkill::SetTarget(Actor* _Actor, float _Degree)
{
	myActor = GetActor();

	rotateDegree = _Degree;

	Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
	m_Index = ChessBoard::Inst().CalculateIndex(myPos);

	moveDestPos = _Actor->GetTransform()->GetLocalPosition();
	moveDestIdx = ChessBoard::Inst().CalculateIndex(moveDestPos);
}

Vector4 LittleDragonSkill::Rotate(BoardIndex _DestIdx, float _Degree)
{
	Vector4 standDir = { 1.0f, 0.0f, 0.0f, 0.0f };
	Vector4 resultDir = Vector4((float)_DestIdx.x, 0.0f, (float)_DestIdx.z, 0.0f) - Vector4((float)m_Index.x, 0.0f, (float)m_Index.z, 0.0f);

	//if (resultDir.x < 0.0f)
	//{
	//	standDir *= 1.0f;
	//}

	Vector4 crossVector = Vector4::CrossProduct(standDir, resultDir);

	float cosDegree = Vector4::DotProduct(resultDir.ReturnNormalization(), standDir);
	float resultDegree = acos(cosDegree) * Constant::R2D + 90.0f + _Degree ;

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

LittleDragonSkill::LittleDragonSkill() : isRotate(false) , myActor(nullptr), curTime(0.0f), particleTime(0.0f)
{
}


LittleDragonSkill::~LittleDragonSkill()
{
}
