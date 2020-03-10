#include "PieceLogic.h"
#include "StatusLogic.h"
//
#include "ChessBoard.h"
#include "GameManager.h"


std::vector<std::vector<bool>> PieceLogic::m_MapStaticInfo;
std::vector<std::vector<bool>> PieceLogic::m_MapStaticMonster;


PieceLogic::PieceLogic() : m_State(PIECESTATE::BORN), pFbxAnimation(nullptr), m_Range(1), isEnemyFind(false)
,isAttackFind(false) ,isRun(false), isJump(false), attCnt(0) , isDie(false), deadTime(0.0f), isStay(false), isMonster(false)
, myScene(nullptr), makeProjectTile(false), isPassive(false), realDead(false)
{
	
}

PieceLogic::~PieceLogic()
{
}

void PieceLogic::Init()
{
	myActor = GetActor();

}

void PieceLogic::Update()
{
	
}

void PieceLogic::ResetMapStaticInfo()
{
	
	for (int z = 0; z < 8; ++z)
	{
		for (int x = 0; x < 8; ++x)
		{
			m_MapStaticInfo[z][x] = false;
		}
	}


	for (int z = 0; z < 8; ++z)
	{
		for (int x = 0; x < 8; ++x)
		{
			m_MapStaticMonster[z][x] = false;			
		}
	}


	//ChessBoard::Inst().m_Board[][]

}

void PieceLogic::PushPieceInfo(Vector4 _Pos)
{
	BoardIndex index;

	index.x = (int)((_Pos.x + 496.0f) / 124.0f);
	index.z = (int)((_Pos.z - 496.0f) / -124.0f);

	m_MapStaticInfo[index.z][index.x] = true;
	
}

void PieceLogic::PushMonsterInfo(Vector4 _Pos)
{
	BoardIndex index;

	index.x = (int)((_Pos.x + 496.0f) / 124.0f);
	index.z = (int)((_Pos.z - 496.0f) / -124.0f);

	m_MapStaticMonster[index.z][index.x] = true;
	
}

void PieceLogic::ResetMyPath()
{
	if (isMonster != true)
	{
		std::list<PieceNode>::iterator pBegin = m_CloseList.begin();
		std::list<PieceNode>::iterator pEnd	= m_CloseList.end();

		for (; pBegin != pEnd ; ++pBegin)
		{
			m_MapStaticInfo[(*pBegin).pos.z][(*pBegin).pos.x] = false;
		}

	}
	else
	{
		std::list<PieceNode>::iterator pBegin = m_CloseList.begin();
		std::list<PieceNode>::iterator pEnd = m_CloseList.end();

		for (; pBegin != pEnd; ++pBegin)
		{
			m_MapStaticMonster[(*pBegin).pos.z][(*pBegin).pos.x] = false;
		}
	}
}

std::list<PieceLogic::PieceNode> PieceLogic::FindPath( BoardIndex _DestPos)
{
	bool isFind = false;


	BoardIndex myPos;

	myPos.x = m_Index.x;
	myPos.z = m_Index.z;

	BoardIndex destPos = _DestPos;
	//destPos = BoardIndex{ 0,0 };

	m_OpenList.clear();
	m_CloseList.clear();
	m_MapInfo.clear();


	//MAPX 30
	//MAPz 15
	//벡터를 2차원배열처럼 크기에 맞게 초기화해준다
	m_MapInfo.resize(8);	//넉넉하게 50까지 만들어둔다
	for (int z = 0; z < 8; ++z)
	{
		m_MapInfo[z].resize(8);
	}


	//각맵의 모든 위치에 스타트값, 목적지값, 합계, 위치를 모두 구해둔다.
	//그러면 모든 맵의 배열을 돌면서 확인하면 계산을 해주기가 편해진다
	for (int z = 0; z < 8; ++z)
	{
		for (int x = 0; x < 8; ++x)
		{
			BoardIndex nodePos = BoardIndex(z, x);
			m_MapInfo[z][x].pos = nodePos;
			m_MapInfo[z][x].startCost = 0;

			BoardIndex destVector = _DestPos - m_MapInfo[z][x].pos;
			m_MapInfo[z][x].destCost = abs(destVector.x * 10) + abs(destVector.z * 10);
			m_MapInfo[z][x].sumCost = 0;

	
			if (isMonster != true)
			{
				m_MapInfo[z][x].iSWall = false;
				if (m_MapStaticInfo[z][x] == true)	//플레이어경로
				{
					if (_DestPos.z != z || _DestPos.x != x)
					{
						m_MapInfo[z][x].iSWall = true;
					}
				}
			}
			else //몬스터 경로
			{
				m_MapInfo[z][x].iSWall = false;
				if (m_MapStaticMonster[z][x] == true)	//플레이어경로
				{
					if (_DestPos.z != z || _DestPos.x != x)
					{
						m_MapInfo[z][x].iSWall = true;
					}
	
				}
			}
			

		}
	}

	PieceNode mzNode;
	mzNode.pos = myPos;
	m_CloseList.push_back(mzNode);

	//길찾기 시작부분
	while (isFind != true)
	{
		std::list<PieceNode>::iterator curIter = m_CloseList.end();
		--curIter;

		BoardIndex curNodePos = (*curIter).pos;

		PieceNode curNode = (*curIter);

		//m_CloseList.
		bool isExist = false;
		for (int z = 0; z < 3; ++z)
		{
			for (int x = 0; x < 3; ++x)
			{
				if (z == 1 && x == 1)	//현재내위치
				{
					continue;
				}
				
				BoardIndex calPos;
				calPos.z = (curNodePos.z + z - 1);
				calPos.x = (curNodePos.x + x - 1);

				
				if (calPos.z < 0 || calPos.x < 0 || calPos.z >= 8 || calPos.x >= 8)
				{
					continue;
				}
   				if (m_MapInfo[calPos.z][calPos.x].iSWall == true)
				{
					continue;
				}
				
				//클로드 리스트 순회
				bool ExistClosed = false;
				std::list<PieceNode>::iterator Start = m_CloseList.begin();
				std::list<PieceNode>::iterator End = m_CloseList.end();
				for (; Start != End; ++Start)
				{
					if (Start->pos == calPos)
					{
						ExistClosed = true;
					}
				}

				std::multimap<int, PieceNode>::iterator findIter = m_OpenList.begin();

				isExist = false;
				for (; findIter != m_OpenList.end(); ++findIter)
				{
					BoardIndex  checkPos = (*findIter).second.pos;

					if (calPos == checkPos)
					{
						isExist = true;
						break;
					}
				}


				if (ExistClosed == true || isExist == true)
				{
					continue;
				}


				if ((z - 1) == 0 || (x - 1) == 0)	//0이 하나라도 들어있으면 상하좌우이다 0이없으면 대각선 4방향
				{
					m_MapInfo[calPos.z][calPos.x].startCost = curNode.startCost + 10;	//가로세로는 10
				}
				else
				{
					m_MapInfo[calPos.z][calPos.x].startCost = curNode.startCost + 14;	//대각선 14
				}

				//m_MapInfo[calPos.z][calPos.x].destCost = 

				int sum = m_MapInfo[calPos.z][calPos.x].startCost + m_MapInfo[calPos.z][calPos.x].destCost;
				m_MapInfo[calPos.z][calPos.x].sumCost = sum;

				m_OpenList.insert(std::multimap<int, PieceNode>::value_type(sum, m_MapInfo[calPos.z][calPos.x]));
			}
		}
		std::multimap<int /*sumCost*/, PieceNode>::iterator findITer = m_OpenList.begin();
		if (findITer == m_OpenList.end())
		{
			return m_CloseList;
		}
		PieceNode minNode = findITer->second;

		std::list<PieceNode>::iterator endIter = m_CloseList.end();
		--endIter;

		m_CloseList.push_back(minNode);
		m_OpenList.erase(findITer);

		std::list<PieceNode>::iterator posIter = m_CloseList.end();
		--posIter;

		if ((*posIter).pos.x == 7 && (*posIter).pos.z == 1)
		{
			int a = 0;
		}

		if ((*posIter).pos == _DestPos)
		{
			isFind = true;
		}
	}

	//맵에 정보 넘겨주기
	std::list<PieceNode>::iterator closeBegin = m_CloseList.begin();
	std::list<PieceNode>::iterator closeEnd = m_CloseList.end();
	
	std::list<PieceNode>::iterator closeLast = m_CloseList.end();
	--closeLast;

	return m_CloseList;
}



void PieceLogic::OverlapPathCheck()
{
	std::list<PieceNode>::iterator closeBegin = m_CloseList.begin();
	std::list<PieceNode>::iterator closeEnd = m_CloseList.end();
	int cnt = 0;
	for (; closeBegin != closeEnd; ++closeBegin)
	{
		//if (cnt < 3)
		//{
		if (isMonster != true)
		{
			BoardIndex temp = closeBegin->pos;
			m_MapStaticInfo[temp.z][temp.x] = true;
		}
		else
		{
			BoardIndex temp = closeBegin->pos;
			m_MapStaticMonster[temp.z][temp.x] = true;
		}
		//}
		//++cnt;
	}
}

void PieceLogic::CurPosIdxCheck()
{
	m_Index = CalculateIndex(GetActor()->GetTransform()->GetLocalPosition());

	if (isMonster != true)
	{
		m_MapStaticInfo[m_Index.z][m_Index.x] = true;
	}
	else
	{
		m_MapStaticMonster[m_Index.z][m_Index.x] = true;
	}
	
}

Actor* PieceLogic::FindAttackTarget()
{
	
	std::vector<std::vector<ChessBoard::BoardInfo>> m_Map = ChessBoard::Inst().m_Board;
	
	bool isZ = false;
	int dirZ = 1;
	int dirX = 1;
	int curRange = 1;
	///////////

	while (curRange != (m_Range+1))
	{
		int rotateNum = (2 * curRange);
		//
		dirZ = 1;
		dirX = 1;
		int xCnt = 8 * curRange;
		int idxZ = m_Index.z - curRange;
		int idxX = m_Index.x - curRange;

		isZ = false;
		for (size_t x = 0; x < xCnt; x++)
		{
			if (isZ == false && x != 0)
			{
				idxX = idxX + dirX;
			}
			else if (isZ == true)
			{
				idxZ = idxZ + dirZ;
			}

			if ((x % rotateNum) == 0 && x != 0)
			{
				if (isZ == false)
				{
					isZ = true;
					dirX = -1;
				}
				else
				{
					isZ = false;
					dirZ = -1;
				}
			}

	

			if ((idxZ >= 0 && idxZ < 8) && (idxX >= 0 && idxX < 8))
			{
				//인덱스 체크 부분
	
				

				Actor* pEnemy = m_Map[idxZ][idxX].m_Actor;
				if (nullptr != pEnemy)
				{
					PIECETYPE myType = ChessBoard::Inst().m_Board[m_Index.z][m_Index.x].m_statusLogic->GetType();
					PIECETYPE otherType = ChessBoard::Inst().m_Board[idxZ][idxX].m_statusLogic->GetType();

					if (myType != otherType)
					{
						return pEnemy;
					}
					else
					{
						continue;
					}
					//isAttackFind = true;
					
				}
			}
		}
		curRange += 1;
	}

	return nullptr;	//아무것도 없으면 nullptr 반환
}

Actor* PieceLogic::FindEnemyTarget()
{
	
	std::vector<std::vector<ChessBoard::BoardInfo>> m_Map = ChessBoard::Inst().m_Board;
	
	bool isZ = false;
	int dirZ = 1;
	int dirX = 1;
	int curRange = 1;
	///////////


	while (curRange != (7 + 1))
	{
		int rotateNum = (2 * curRange);
		//
		dirZ = 1;
		dirX = 1;
		int xCnt = 8 * curRange;
		int idxZ = m_Index.z - curRange;
		int idxX = m_Index.x - curRange;

		isZ = false;
		for (size_t x = 0; x < xCnt; x++)
		{
			if (isZ == false && x != 0)
			{
				idxX = idxX + dirX;
			}
			else if (isZ == true)
			{
				idxZ = idxZ + dirZ;
			}

			if ((x % rotateNum) == 0 && x != 0)
			{
				if (isZ == false)
				{
					isZ = true;
					dirX = -1;
				}
				else
				{
					isZ = false;
					dirZ = -1;
				}
			}

			if (idxX == 7 && idxZ == 1)
			{
				int a = 0;
			}

			if ((idxZ >= 0 && idxZ < 8) && (idxX >= 0 && idxX < 8) )
			{
				//인덱스 체크 부분
				Actor* pEnemy = m_Map[idxZ][idxX].m_Actor;
				if (nullptr != pEnemy)
				{
					PIECETYPE myType = ChessBoard::Inst().m_Board[m_Index.z][m_Index.x].m_statusLogic->GetType();
					PIECETYPE otherType = ChessBoard::Inst().m_Board[idxZ][idxX].m_statusLogic->GetType();

					if (myType != otherType)
					{
						return pEnemy;
					}
					else
					{
						continue;
					}
				}
			}
		}
		curRange += 1;
	}

	isEnemyFind = false;
	return nullptr;	//아무것도 없으면 nullptr 반환
}

BoardIndex PieceLogic::CalculateIndex(Vector4 _Pos)
{
	BoardIndex index;

	// 124x-496 ~  124x-372
	//-124z+496 ~ -124z+372

	index.x = (int)((_Pos.x + 496.0f) / 124.0f);
	index.z = (int)((_Pos.z - 496.0f) / -124.0f);
		

	return index;
}

void PieceLogic::Born()
{
	if (myScene == nullptr)
	{
		pFbxAnimation = GetActor()->GetComponent<FbxAnimation>();
		statusLogic = GetActor()->GetComponent<StatusLogic>();
		myScene = GetActor()->GetScene();
	}
	bool isEnd = pFbxAnimation->GetCurClip()->m_AniEnd;

	if (true == isEnd)
	{
		if (isMonster == true)
		{
			m_State = PIECESTATE::IDLE;
		}
		else
		{
			m_State = PIECESTATE::STAY;
		}

		pFbxAnimation->ChangeAnimation(L"IDLE");
	}

	m_Index = CalculateIndex(GetActor()->GetTransform()->GetLocalPosition());
	
	//CurPosIdxCheck();
}

void PieceLogic::Idle()
{
	CurPosIdxCheck();
	m_Index = CalculateIndex(GetActor()->GetTransform()->GetLocalPosition());

	pAttTarget = nullptr;
	pAttTarget = FindAttackTarget();//공격타겟 검색 자신의 사거리만큼
#pragma region SKILL구현부
	/// 여기에 스킬쿨타임 다돌고 mp가 100이상이면 스킬상태로 ㄱㄱ
	float curTime = statusLogic->GetCurTime();
	float coolTime = statusLogic->GetCoolTime();
	float mp = statusLogic->GetMp();

	if (mp > 100.0f && curTime > coolTime && isPassive == false && pAttTarget != nullptr)
	{
		pFbxAnimation->ChangeAnimation(L"SKILL");
		m_State = PIECESTATE::SKILL;

		return;
	}


#pragma endregion

	if (pAttTarget != nullptr)
	{
		m_State = PIECESTATE::FINDATTACKTARGET;

		return;
	}
	   
	pMoveTarget = nullptr;
	pMoveTarget = FindEnemyTarget();
		
	if (nullptr != pMoveTarget)
	{		
		if (ChessBoard::Inst().CheckPieceMoving(pMoveTarget) == true)
		{
			return;
		}

		BoardIndex enemyIndex = CalculateIndex(pMoveTarget->GetTransform()->GetLocalPosition());
		
		m_State = PIECESTATE::FINDMOVETARGET;

		return;
	}

	if (pAttTarget == nullptr && pMoveTarget == nullptr)
	{
		pFbxAnimation->ChangeAnimation(L"VICTORY");

		GetTransform()->SetLocalRotY(0.0f);
		m_State = PIECESTATE::VICTORY;
		return;
	}
}

void PieceLogic::FindAttTarget()
{
	CurPosIdxCheck();
	BoardIndex attackIndex = CalculateIndex(pAttTarget->GetTransform()->GetLocalPosition());
	attDestIdx = attackIndex;
	attDestPos = ChessBoard::Inst().m_Board[attDestIdx.z][attDestIdx.x].m_BoardPos;

	pFbxAnimation->ChangeAnimation(L"ATTACK01");
	Rotate(attDestIdx);
	m_State = PIECESTATE::ATTACK;


	//
	
}


void PieceLogic::Attack()
{


	CurPosIdxCheck();

	StatusLogic* enemyLogic = pAttTarget->GetComponent<StatusLogic>();//ChessBoard::Inst().m_Board[attDestIdx.z][attDestIdx.x].m_statusLogic;
	StatusLogic* myLogic = GetActor()->GetComponent<StatusLogic>();

	if (enemyLogic != nullptr && enemyLogic->IsLogicDeath() == true)
	{
		m_State = PIECESTATE::IDLE;
		pAttTarget = nullptr;

		return;
	}

	if (pAttTarget != nullptr)
	{
		attDestIdx = CalculateIndex(pAttTarget->GetTransform()->GetLocalPosition());
		attDestPos = ChessBoard::Inst().m_Board[attDestIdx.z][attDestIdx.x].m_BoardPos;
	}
	else
	{
		m_State = PIECESTATE::IDLE;
	}

	Rotate(attDestIdx);
	   
	if (pFbxAnimation->GetCurClip()->m_AniThreeQuarters == true && makeProjectTile == false)
	{
		MakeProjecTile();
		makeProjectTile = true;
	}

	if (pFbxAnimation->GetCurClip()->m_AniEnd == true)
	{
		makeProjectTile = false;
		float curTime = statusLogic->GetCurTime();
		float coolTime = statusLogic->GetCoolTime();
		float mp = statusLogic->GetMp();


		if (mp > 100.0f && curTime > coolTime && isPassive == false)
		{
			pFbxAnimation->ChangeAnimation(L"SKILL");
			m_State = PIECESTATE::SKILL;

			return;
		}



		if (nullptr == enemyLogic)
		{
			m_State = PIECESTATE::IDLE;
			return;
		}

		enemyLogic->TakeAwayHp(myLogic->GetAtt());
		if (isMonster != true)
		{
			statusLogic->AddMp(20.0f);
		}

		if (attCnt == 0)
		{
			pFbxAnimation->ChangeAnimation(L"ATTACK02");
			attCnt = 1;
		}
		else if (attCnt == 1)
		{
			pFbxAnimation->ChangeAnimation(L"ATTACK01");
			attCnt = 0;
		}
	}
}

void PieceLogic::FindTransTarget()
{
	CurPosIdxCheck();
	ChessBoard::Inst().m_Board[m_Index.z][m_Index.x].isMove = true;
	path.clear();


	BoardIndex enemyIndex = CalculateIndex(pMoveTarget->GetTransform()->GetLocalPosition());
	path = FindPath(enemyIndex);

	if (path.size() <= 1)
	{
		pFbxAnimation->ChangeAnimation(L"IDLE");
		m_State = PIECESTATE::IDLE;
		path.clear();
		path = FindPath(enemyIndex);

		OverlapPathCheck();

		return;
	}
	
	std::list<PieceNode>::iterator runDest = path.begin();
	++runDest;
	std::list<PieceNode>::iterator jumpDest = runDest;
	if (path.size() > 3)
	{
		++jumpDest;
	}


	if (isMonster == true)
	{
		if (path.size() > 3)
		{
			moveDestIdx = jumpDest->pos;
			moveDestPos = ChessBoard::Inst().m_Board[moveDestIdx.z][moveDestIdx.x].m_BoardPos;

			pFbxAnimation->ChangeAnimation(L"JUMP");
			Rotate(moveDestIdx);
			m_State = PIECESTATE::JUMP;

			
			OverlapPathCheck();
			return;
		}
		else if (path.size() <= 3 && path.size() > 1)
		{
			moveDestIdx = runDest->pos;
			moveDestPos = ChessBoard::Inst().m_Board[moveDestIdx.z][moveDestIdx.x].m_BoardPos;

			pFbxAnimation->ChangeAnimation(L"RUN");
			Rotate(moveDestIdx);
			m_State = PIECESTATE::RUN;

			
			OverlapPathCheck();
			return;
		}
		else //1이면
		{
			m_State = PIECESTATE::IDLE;
			return;
		}
	}
	else
	{
		if (path.size() > 3)
		{
			moveDestIdx = jumpDest->pos;
			moveDestPos = ChessBoard::Inst().m_Board[moveDestIdx.z][moveDestIdx.x].m_BoardPos;

			pFbxAnimation->ChangeAnimation(L"JUMP");
			Rotate(moveDestIdx);
			m_State = PIECESTATE::JUMP;
			OverlapPathCheck();
			return;
		}
		else if (path.size() <= 3 && path.size() > 1)
		{
			moveDestIdx = runDest->pos;
			moveDestPos = ChessBoard::Inst().m_Board[moveDestIdx.z][moveDestIdx.x].m_BoardPos;

			pFbxAnimation->ChangeAnimation(L"RUN");
			Rotate(moveDestIdx);
			m_State = PIECESTATE::RUN;
			OverlapPathCheck();
			return;
		}
		else //1이면
		{

			m_State = PIECESTATE::IDLE;
			OverlapPathCheck();
			return;
		}
	}


}

void PieceLogic::Run()
{
	ChessBoard::Inst().m_Board[m_Index.z][m_Index.x].isMove = true;
	
	OverlapPathCheck();


	j_destPos = moveDestPos;
	j_StartPos = ChessBoard::Inst().m_Board[m_Index.z][m_Index.x].m_BoardPos;
	


	ParabolaCalculate(0.4f, 0.01f);
	Vector4 RunResult = JumpCalculate();

	GetTransform()->SetLoclaPosition(RunResult);

	if (RunResult == j_destPos)
	{
		time = 0.0f;
		m_Index = CalculateIndex(j_destPos);
		isEnemyFind = false;
		m_State = PIECESTATE::IDLE;
	}
}

void PieceLogic::Jump()
{
	ChessBoard::Inst().m_Board[m_Index.z][m_Index.x].isMove = true;
	
	OverlapPathCheck();


	j_destPos = moveDestPos;
	j_StartPos = ChessBoard::Inst().m_Board[m_Index.z][m_Index.x].m_BoardPos;

	ParabolaCalculate(0.4f, 20.0f);
	Vector4 JumpResult = JumpCalculate();

	GetTransform()->SetLoclaPosition(JumpResult);

	if (JumpResult == j_destPos)
	{
		time = 0.0f;
		m_Index = CalculateIndex(j_destPos);
		isEnemyFind = false;
		m_State = PIECESTATE::IDLE;
	}
}

void PieceLogic::Die()
{
	if (isDie == false)
	{
		pFbxAnimation->ChangeAnimation(L"DEATH");
		isDie = true;


		statusLogic->StatusAllOff();
		statusLogic->LogicOff();
		statusLogic->UpdateOff();
		

		ChessBoard::Inst().PopPiece(this->GetActor());
	}


	deadTime += Time::DeltaTime();
	if (deadTime > 5.0f)
	{
		GetActor()->GetTransform()->WMove(Vector4::DOWN * Time::SDeltaTime(20.0f));
	}
	
	Vector4 curPos = GetActor()->GetTransform()->GetLocalPosition();

	if (curPos.y < -300.0f)
	{
		m_State = PIECESTATE::DEAD;

	}
}

void PieceLogic::Dead()
{
	if (realDead == false)
	{
		GameManager::Inst().PushDeadList(GetActor());
		realDead = true;
	}
}

void PieceLogic::Victory()
{
	CurPosIdxCheck();
	//OverlapPathCheck();
	if ((int)GameManager::Inst().GetState() == 0)
	{
		if (isMonster == true)
		{
			GetActor()->Die();
			ChessBoard::Inst().PopEnemyPiece(GetActor());
		}
		else
		{
			m_State = PIECESTATE::STAY;
			statusLogic->InitStatus();
		}

		pFbxAnimation->ChangeAnimation(L"IDLE");
		Vector4 startPos = ChessBoard().Inst().m_Board[m_StartIndex.z][m_StartIndex.x].m_BoardPos;
		
		myActor->GetTransform()->SetLoclaPosition(startPos);
		m_Index = CalculateIndex(myActor->GetTransform()->GetLocalPosition());
	}
}

void PieceLogic::Stay()
{
	bool checkThis = ChessBoard::Inst().CheckAllyPiece(GetActor());
	
	if ((int)GameManager::Inst().GetState() == 1 && checkThis == true)
	{
		m_Index = CalculateIndex(GetActor()->GetTransform()->GetLocalPosition());
		m_StartIndex = m_Index;

		CurPosIdxCheck();

		m_State = PIECESTATE::IDLE;

		return;
	}
}


void PieceLogic::SkillCheck()
{

}




void PieceLogic::ParabolaCalculate(float _MaxTime, float _MaxHeight)
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

Vector4 PieceLogic::JumpCalculate()
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

Vector4 PieceLogic::Rotate(BoardIndex _DestIdx)
{
	Vector4 standDir = { 1.0f, 0.0f, 0.0f, 0.0f };
	Vector4 resultDir = Vector4((float)_DestIdx.x, 0.0f, (float)_DestIdx.z, 0.0f) - Vector4((float)m_Index.x, 0.0f, (float)m_Index.z, 0.0f);

	//if (resultDir.x < 0.0f)
	//{
	//	standDir *= 1.0f;
	//}

	Vector4 crossVector = Vector4::CrossProduct(standDir, resultDir);

	float cosDegree = Vector4::DotProduct(resultDir.ReturnNormalization(), standDir);
	float resultDegree = acos(cosDegree) * Constant::R2D + 90.0f; 
	
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
	else if(_DestIdx.z < m_Index.z)
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


