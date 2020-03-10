#include "MonsterLogic.h"
#include "StatusLogic.h"


MonsterLogic::MonsterLogic() : isRock(false), rockTime(0.0f)
{
	isMonster = true;
}


MonsterLogic::~MonsterLogic()
{
}

void MonsterLogic::Update()
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

void MonsterLogic::MakeProjecTile()
{

}

