#include "BladeKnightWeapon.h"

#include "ChessBoard.h"
#include "StatusLogic.h"

#include <SOUND.h>



void BladeKnightWeapon::Init()
{

}

void BladeKnightWeapon::Update()
{
	if (isRotate == false)
	{
		Rotate(moveDestIdx, -90.0f);
		isRotate = true;
	}

	if (attCnt == 1)
	{
		int a = 0;
	}

	if (attCnt == 0)
	{
		j_destPos = moveDestPos;
		j_StartPos = ChessBoard::Inst().m_Board[m_Index.z][m_Index.x].m_BoardPos;
	}
	
	ParabolaCalculate(0.3f, 50.0f);
	Vector4 JumpResult = JumpCalculate();
	GetActor()->GetTransform()->SetLoclaPosition(JumpResult);


	if (JumpResult == j_destPos)
	{
		Vector4	curPos = GetActor()->GetTransform()->GetLocalPosition();
		Actor* target = nullptr;

		Sound::Play(L"YueQi_UnderAttack_2.wav");

		if (attCnt < 3)
		{
			BoardIndex attIdx = ChessBoard::Inst().CalculateIndex(moveDestPos);
			target = ChessBoard::Inst().m_Board[attIdx.z][attIdx.x].m_Actor;

			if (target != nullptr)
			{
				StatusLogic* attLogic = target->GetComponent<StatusLogic>();
				attLogic->AddHp(-55);
			}

			time = 0.0f;
			attCnt++;
			
			j_StartPos = j_destPos;

			target = FindNextTarget();
			if (target != nullptr)
			{


				moveDestPos = target->GetTransform()->GetLocalPosition();
				moveDestIdx = ChessBoard::Inst().CalculateIndex(moveDestPos);

				j_destPos = moveDestPos;				
			}
			else if (target == nullptr)
			{
				GetActor()->Die();
			}
		}
		else
		{
			GetActor()->Die();
		}
	}
}

//void ProjectTileLogic::SetTarget(Actor* _Actor)
//{
//	Vector4 myPos = GetActor()->GetTransform()->GetLocalPosition();
//	m_Index = ChessBoard::Inst().CalculateIndex(myPos);
//
//	moveDestPos = _Actor->GetTransform()->GetLocalPosition();
//	moveDestIdx = ChessBoard::Inst().CalculateIndex(moveDestPos);
//}



BladeKnightWeapon::BladeKnightWeapon() : attCnt(0)
{

}


BladeKnightWeapon::~BladeKnightWeapon()
{
}
