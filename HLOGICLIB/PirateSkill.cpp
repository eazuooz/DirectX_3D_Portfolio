#include "PirateSkill.h"



#include <RANDOM.h>
#include "ChessBoard.h"
#include "StatusLogic.h"
#include <SCENE.h>
#include "LOGICHEADER.h"
#include <2DANICOM.h>
#include "BaseEffect.h"

#include <SOUND.h>

void PirateSkill::SetTarget(Actor* _Actor)
{
	Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
	m_Index = ChessBoard::Inst().CalculateIndex(myPos);

	moveDestPos = _Actor->GetTransform()->GetLocalPosition();
	moveDestIdx = ChessBoard::Inst().CalculateIndex(moveDestPos);

	int flag = 0;
	int check = 0;
	BoardIndex attIdx = m_Index;
	attIdx -= 1;
	for (int z = 0; z < 3; z++)
	{

		if (flag == 1)
		{
			break;
		}
		for (int x = 0; x < 3; x++)
		{
			if ((attIdx.z + z == moveDestIdx.z) && (attIdx.x + x == moveDestIdx.x))
			{
				flag = 1;
				break;
			}
			else
			{
				++check;
			}
		}
	}

	j_StartPos = ChessBoard::Inst().m_Board[m_Index.z][m_Index.x].m_BoardPos;
	float startDis = 1000.0f;
	switch (check)
	{
	case 0:
		j_StartPos.x += startDis;
		j_StartPos.z -= startDis;

		
		break;
	case 1:
		//moveDestPos.x += 500.0f;
		j_StartPos.z -= startDis;
		break;
	case 2:
		j_StartPos.x -= startDis;
		j_StartPos.z -= startDis;
		break;
	case 3:
		j_StartPos.x += startDis;
		//moveDestPos.z -= 500.0f;
		break;
	case 4:
		break;
	case 5:
		j_StartPos.x -= startDis;
		//moveDestPos.z -= 500.0f;
		break;
	case 6:
		j_StartPos.x += startDis;
		j_StartPos.z += startDis;
		break;
	case 7:
		//moveDestPos.x += 500.0f;
		j_StartPos.z += startDis;
		break;

	case 8:
		j_StartPos.x -= startDis;
		j_StartPos.z += startDis;
		break;
	}
	GetActor()->GetTransform()->SetLoclaPosition(moveDestPos);

	j_destPos = moveDestPos;


	
}

Actor* PirateSkill::FindNextTarget()
{
	Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
	m_Index = ChessBoard::Inst().CalculateIndex(myPos);


	BoardIndex myIdx = m_Index;
	m_Index -= 2;

	BoardIndex idx = m_Index;


	for (int z = 0; z < 5; z++)
	{
		for (int x = 0; x < 5; x++)
		{
			if ((idx.z + z >= 0 && idx.z + z <= 7) &&
				(idx.x + x >= 0 && idx.x + x <= 7))
			{
				if (myIdx.z == idx.z + z && myIdx.x == idx.x + x)
				{
					continue;
				}

				Actor* enemyActor = ChessBoard::Inst().m_Board[idx.z + z][idx.x + x].m_Actor;
				if (enemyActor != nullptr)
				{
					StatusLogic* statusLogic = enemyActor->GetComponent<StatusLogic>();
					if (statusLogic->GetType() == PIECETYPE::ENEMY)
					{


						return enemyActor;
					}
				}
			}
		}
	}

	return nullptr;
}


void PirateSkill::ParabolaCalculate(float _MaxTime, float _MaxHeight)
{
	maxHeight = _MaxHeight;
	maxTime = _MaxTime;

	endHeight = j_destPos.y - j_StartPos.y;
	height = maxHeight - j_StartPos.y;

	g = 2 * height / (maxTime * maxTime);
	vY = sqrtf(2 * g * height);

	float a = g;
	float b = -2 * vY;
	float c = 2 * endHeight;

	endTime = (-b + sqrtf(b*b - 4 * a*c)) / (2 * a);

	vX = -(j_StartPos.x - j_destPos.x) / endTime;
	vZ = -(j_StartPos.z - j_destPos.z) / endTime;
}

Vector4 PirateSkill::JumpCalculate()
{
	time += Time::DeltaTime();

	if (time > endTime)
	{

		return j_destPos;
	}
	else
	{
		float x = j_StartPos.x + vX * time;
		float y = j_StartPos.y + (vY * time) - (0.5f * g * time *time);
		float z = j_StartPos.z + vZ * time;

		return Vector4(x, y, z);
	}
}

Vector4 PirateSkill::Rotate(BoardIndex _DestIdx, float _Degree)
{
	Vector4 standDir = { 1.0f, 0.0f, 0.0f, 0.0f };
	Vector4 resultDir = Vector4((float)_DestIdx.x, 0.0f, (float)_DestIdx.z, 0.0f) - Vector4((float)m_Index.x, 0.0f, (float)m_Index.z, 0.0f);

	Vector4 crossVector = Vector4::CrossProduct(standDir, resultDir);

	float cosDegree = Vector4::DotProduct(resultDir.ReturnNormalization(), standDir);
	float resultDegree = acos(cosDegree) * Constant::R2D + 90.0f + _Degree + degree;

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


void PirateSkill::Jump()
{


}

void PirateSkill::Init()
{
	Sound::Play(L"ChuanZhang_Skill.wav");
	Sound::Play(L"ChuanZhang_Skill_Object.wav", L"ChuanZhang_Skill_Object.wav", 999);
}

void PirateSkill::Update()
{
	if (isRotate == false)
	{
		Rotate(moveDestIdx, -90.0f);
		isRotate = true;
	}

	Vector4 JumpResult;
	if (isJumping == false)
	{
		ParabolaCalculate(3.0f, 30.0f);
		JumpResult = JumpCalculate();

		GetActor()->GetTransform()->SetLoclaPosition(JumpResult);
	}
	else
	{
		GetActor()->GetTransform()->LocalTranslateY(-50.0f * Time::DeltaTime());
		GetActor()->GetTransform()->LocalRotXSum(-50.0f * Time::DeltaTime());

		Vector4 dieCheck = GetActor()->GetTransform()->GetLocalPosition();

		if (dieCheck.y < -300.0f)
		{
			GetActor()->Die();
		}
	}

	if (JumpResult == j_destPos)
	{
		//GetActor()->Die();
		isJumping = true;
		//여기서 딜을 넣어준다
		Sound::Stop(L"ChuanZhang_Skill_Object.wav");
		Sound::Play(L"ChuanZhang_Skill_End.wav");
		Vector4 shipPos = GetActor()->GetTransform()->GetLocalPosition();
		BoardIndex shipIdx = ChessBoard::Inst().CalculateIndex(shipPos);

		shipIdx -= 2;

		for (int z = 0; z < 5; z++)
		{
			for (int x = 0; x < 5; x++)
			{
				if ((shipIdx.z + z >= 0 && shipIdx.z + z <= 7) &&
					(shipIdx.x + x >= 0 && shipIdx.x + x <= 7))
				{
					Actor* target = ChessBoard::Inst().m_Board[shipIdx.z + z][shipIdx.x + x].m_Actor;
					if (target != nullptr)
					{
						StatusLogic* statusLogic = target->GetComponent<StatusLogic>();
						if (statusLogic->GetType() == PIECETYPE::ALLY)
						{
							statusLogic->AddHp(-150);
						}
					}
				}
			}
		}

	}

	particleTime += Time::DeltaTime();

	if (particleTime > 0.1f)
	{
		for (int i = 0; i < 5; i++)
		{
			HPTR<Actor> PLAYER2 = GetActor()->GetScene()->CreateActor();

			Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();

			myPos.x += Well512Random::Instance().GetFloatValue(-160.0f, 160.0f);
			myPos.z += Well512Random::Instance().GetFloatValue(-160.0f, 160.0f);

			PLAYER2->GetTransform()->SetLoclaPosition(myPos);
			PLAYER2->GetTransform()->SetLocalScale({ 20.0F, 20.0F, 20.0F });

			HPTR<SpriteRenderer> battleTopFrameSR = PLAYER2->AddComponent<SpriteRenderer>(RG_PLAYER);
			battleTopFrameSR->SetSprite(L"EF_shape_95_01_z.tga", 0);
			battleTopFrameSR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
			battleTopFrameSR->LinkCB(L"VS_CUTDATA", &(battleTopFrameSR->GetCutData()));
			battleTopFrameSR->LinkTexture(L"PS_MAPTEX", L"EF_shape_95_01_z.tga");
			battleTopFrameSR->LinkSampler(L"PS_LSMP", L"LSMP");
			battleTopFrameSR->LinkSampler(L"PS_PSMP", L"PSMP");
			battleTopFrameSR->SetLocalScale({ 55.0F, 55.0F, 1.0F, 1.0f });
			battleTopFrameSR->BillBoardOn();
			HPTR<SpriteAnimaion> battleTopFrameSRA = PLAYER2->AddComponent<SpriteAnimaion>();
			battleTopFrameSRA->CreateAni(L"LOOP", L"EF_shape_95_01_z.tga", 0, 15, 0.06f);

			PLAYER2->AddComponent<BaseEffect>();
		}

		particleTime = 0.0f;
	}
}

PirateSkill::PirateSkill() : isRotate(false), degree(0.0f), isJumping(false)
{
}


PirateSkill::~PirateSkill()
{
}
