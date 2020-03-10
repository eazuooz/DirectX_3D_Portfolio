#include "ChessBoard.h"
#include "PieceLogic.h"
#include "StatusLogic.h"

#include <GAMEWIN.h>
#include <3DANIRENDER.h>
#include <SCENE.h>
#include "SellerLogic.h"
#include "LOGICHEADER.h"

//CHARACTER LOGIC
#include "StatusLogic.h"
#include "MpMagicion.h"
#include "BladeKnight.h"
#include "DoomArbiter.h"
#include "Egg.h"
#include "DemonLarva.h"
#include "FireMagician.h"
#include "ThunderSpirit.h"
#include "GodofThunder.h"
#include "GodofWar.h"
#include "TideHunter.h"
#include "LittleDragon.h"
#include "ShiningArcher.h"
#include "PirateCaptain.h"
#include "Siren.h"
#include "ShadowDemon.h"
#include "ToenailDruid.h"
#include "SwordMaster.h"
#include "TortolanElder.h"
#include "Unicorn.h"
#include "WindRanger.h"
#include "Warewolf.h"
#include "WalrusChamp.h"

#include <SOUND.h>

ChessBoard ChessBoard::m_inst = ChessBoard();

ChessBoard::ChessBoard() : curScene(nullptr), wolfEffectEnd(false)
{
	Init();
}


ChessBoard::~ChessBoard()
{

}

BoardIndex ChessBoard::CalculatePickIngIndex()
{
	Vector4 tmp = GameWindow::MainGameWin()->MousePos();
	tmp.x = (tmp.x * 0.01f - 8.0f);
	tmp.y = (tmp.y *-0.01f + 4.5f);
	tmp.z = -99.0f;


	tmp.x *= 100.0f;
	tmp.y *= 100.0f;

	Vector4 xyInfoBase = { -335.0f,315.0f,-220.0f,200.0f };
	Vector4 xyInfoDock = { -364.0f,344.0f,-342.0f,322.0f };


	Vector4 idx;
	idx.z = xyInfoBase.x + (tmp.y + 120.0f) * (xyInfoBase.z - xyInfoBase.x) / 360.0f;
	idx.w = xyInfoBase.y + (tmp.y + 120.0f) * (xyInfoBase.w - xyInfoBase.y) / 360.0f;
	idx.x = (tmp.x - idx.z) / ((idx.w - idx.z) / 8);

	if ((idx.x > 0.0f) && (idx.x < 8.0f))
	{
		if (/*(tmp.x < 315.0f && tmp.x > -335.f) && */(tmp.y < 240.0f && tmp.y > -120.f))
		{
			if (tmp.y < 99.0f)
			{
				if (tmp.y < 4.0f)
				{
					if (tmp.y < -54.0f) { idx.y = 7; }
					else { idx.y = 6; }
				}
				else if (tmp.y < 54.0f) { idx.y = 5; }
				else { idx.y = 4; }
			}
			else if (tmp.y < 177.0f)
			{
				if (tmp.y < 140.0f) { idx.y = 3; }
				else { idx.y = 2; }
			}
			else if (tmp.y < 210.0f) { idx.y = 1; }
			else { idx.y = 0; }
		}
		else if (tmp.y < -150.0f && tmp.y > -210.f) 
		{ 
			idx.y = 9;
		}
		else
		{
			idx.x = idx.y = -1;
		}
	}
	else
	{
		idx.x = idx.y = -1;
	}

	
	return BoardIndex((int)idx.y, (int)idx.x);
}

void ChessBoard::InitPtr()
{
	for (size_t z = 0; z < 8; z++)
	{
		for (size_t x = 0; x < 8; x++)
		{
			m_Board[z][x].m_Actor = nullptr;
			//m_Board[z][x].m_FbxAni = nullptr;
			m_Board[z][x].m_statusLogic = nullptr;
			m_Board[z][x].m_PieceIdx = -1;
			m_Board[z][x].isMove = false;
		}
	}
}

bool ChessBoard::CheckAllyPiece(Actor* _Actor)
{
	if (nullptr == _Actor)
	{
		BOOM;
	}

	m_AllyBegin = m_AllyList.begin();
	m_AllyEnd = m_AllyList.end();
	for (; m_AllyBegin != m_AllyEnd; ++m_AllyBegin)
	{
		if (_Actor == (*m_AllyBegin))
		{
			return true;
		}
	}

	return false;
}

void ChessBoard::PushAllyPiece( Actor* _Actor)
{
	if (nullptr == _Actor)
	{
		BOOM;
	}

	m_AllyList.push_back(_Actor);
}

void ChessBoard::PushEnemyPiece( Actor* _Actor)
{
	if (nullptr == _Actor)
	{
		BOOM;
	}

	m_EnemyList.push_back(_Actor);
}

void ChessBoard::PopAllyPiece(Actor* _Actor)
{
	if (nullptr == _Actor)
	{
		BOOM;
	}

	m_AllyBegin = m_AllyList.begin();
	m_AllyEnd = m_AllyList.end();
	for (; m_AllyBegin != m_AllyEnd; ++m_AllyBegin)
	{
		if (_Actor == (*m_AllyBegin))
		{
			m_AllyBegin = m_AllyList.erase(m_AllyBegin);

			return;
		}
	}
}


void ChessBoard::PopEnemyPiece(Actor* _Actor)
{
	if (nullptr == _Actor)
	{
		BOOM;
	}

	m_EnemyBegin = m_EnemyList.begin();
	m_EnemyEnd = m_EnemyList.end();
	for (; m_EnemyBegin != m_EnemyEnd; ++m_EnemyBegin)
	{
		if (_Actor == (*m_EnemyBegin))
		{
			m_EnemyBegin = m_EnemyList.erase(m_EnemyBegin);

			return;
		}
	}
}

void ChessBoard::PopPiece(Actor* _Actor)
{
	if (nullptr == _Actor)
	{
		BOOM;
	}

	

	m_AllyBegin = m_AllyList.begin();
	m_AllyEnd = m_AllyList.end();
	for (; m_AllyBegin != m_AllyEnd; ++m_AllyBegin)
	{
		if (_Actor == (*m_AllyBegin))
		{
			m_AllyBegin = m_AllyList.erase(m_AllyBegin);

			return;
		}
	}

	m_EnemyBegin = m_EnemyList.begin();
	m_EnemyEnd = m_EnemyList.end();
	for (; m_EnemyBegin != m_EnemyEnd; ++m_EnemyBegin)
	{
		if (_Actor == (*m_EnemyBegin))
		{
			m_EnemyBegin = m_EnemyList.erase(m_EnemyBegin);

			return;
		}
	}
}

void ChessBoard::PushActorInBoard(BoardIndex _Index, Actor* _Actor)
{
	//
	for (size_t z = 0; z < 8; z++)
	{
		for (size_t x = 0; x < 8; x++)
		{
			if (m_Board[z][x].m_Index == _Index)
			{
				m_Board[z][x].m_Actor = _Actor;

				//m_Board[z][x].m_FbxAni = _Actor->GetComponent<FbxAnimation>();
				//
				//
				//m_Board[z][x].m_Actor = _Actor;


				return;
			}
		}
	}	
}

void ChessBoard::PushActorInBoard(BoardIndex _Index, Actor* _Actor, StatusLogic* _Logic)
{
	//
	for (size_t z = 0; z < 8; z++)
	{
		for (size_t x = 0; x < 8; x++)
		{
			if (m_Board[z][x].m_Index == _Index)
			{
				m_Board[z][x].m_Actor = _Actor;
				m_Board[z][x].m_statusLogic = _Logic;

				if (_Logic != nullptr)
				{
					m_Board[z][x].m_PieceIdx = _Logic->GetPieceIdx();
				}
				return;
			}
		}
	}
}

Actor* ChessBoard::GetActor(BoardIndex _Index)
{
	return m_Board[_Index.z][_Index.x].m_Actor;
}
void ChessBoard::SetPosActor(Actor* _Actor, BoardIndex _Index)
{

	if (_Actor != nullptr)
	{
		

		_Actor->GetTransform()->SetLoclaPosition(ChessBoard::Inst().m_Board[_Index.z][_Index.x].m_BoardPos);
	}
	
}

void ChessBoard::PushActorInStay(BoardIndex _Start, BoardIndex _Dest, Actor* _Actor)
{
	if (_Actor != nullptr)
	{
		m_BoardStay[_Dest.x].m_PieceIdx = m_BoardStay[_Start.x].m_PieceIdx;
		m_BoardStay[_Dest.x].m_Actor = _Actor;
		_Actor->GetTransform()->SetLoclaPosition(ChessBoard::Inst().m_BoardStay[_Dest.x].m_BoardPos);
	}
}

Actor* ChessBoard::PopActorInStay(int _Idx)
{
	Actor* temp = m_BoardStay[_Idx].m_Actor;
	m_BoardStay[_Idx].m_Actor = nullptr;
	m_BoardStay[_Idx].m_PieceIdx = -1;
	//m_BoardStay[_Idx].in
	return temp;
}

bool ChessBoard::PushActorInStayAutoSort(Actor* _Actor, int _Idx)
{
	for (int i = 0; i < 8; i++)
	{
		if (m_BoardStay[i].m_Actor == nullptr)
		{
			m_BoardStay[i].m_Actor = _Actor;
			m_BoardStay[i].m_Actor->GetTransform()->SetLoclaPosition(m_BoardStay[i].m_BoardPos);

			m_BoardStay[i].m_PieceIdx = _Idx;

			return true;
		}		
	}

	return false;
}

BoardIndex ChessBoard::GetIdxByActor(Actor* _Actor)
{
	BoardIndex temp(-1, -1);

	for (int z = 0; z < 8; z++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (m_Board[z][x].m_Actor == _Actor)
			{
				temp.z = z;
				temp.x = x;

				return temp;
			}
		}
	}

	return temp;
}

size_t ChessBoard::GetBoardStaySize()
{
	size_t value = 0;
	for (int i = 0; i < 8; i++)
	{
		if (m_BoardStay[i].m_Actor != nullptr)
		{
			value++;
		}
	}

	return value;
}

bool ChessBoard::CheckPieceMoving(Actor* _Actor)
{
	for (size_t z = 0; z < 8; z++)
	{
		for (size_t x = 0; x < 8; x++)
		{
			if (m_Board[z][x].m_Actor == _Actor)
			{
				return m_Board[z][x].isMove;
			}
		}
	}

	return false;
}

void ChessBoard::EnemyRockMaterialOn()
{
	m_EnemyBegin = m_EnemyList.begin();
	m_EnemyEnd = m_EnemyList.end();
	for (; m_EnemyBegin != m_EnemyEnd; ++m_EnemyBegin)
	{
		FbxAnimation* fbxAni = (*m_EnemyBegin)->GetComponent<FbxAnimation>();
		fbxAni->ChangeRockMaterialOn();
		fbxAni->LockOn();
	}
}

void ChessBoard::EnemyRockMaterialOff()
{
	m_EnemyBegin = m_EnemyList.begin();
	m_EnemyEnd = m_EnemyList.end();
	for (; m_EnemyBegin != m_EnemyEnd; ++m_EnemyBegin)
	{
		FbxAnimation* fbxAni = (*m_EnemyBegin)->GetComponent<FbxAnimation>();
		fbxAni->ChangeRockMaterialOff();
		fbxAni->LockOff();
	}
}


BoardIndex ChessBoard::CheckUpgrade()
{

	for (size_t y = 0; y < 3; y++)
	{
		for (size_t x = 0; x < 23; x++)
		{
			m_UpgradeCheckIdx[y][x] = 0;
		}
	}
	
	
	bool existEgg = false;
	for (int i = 5; i < 13; i++)
	{
		//m_UpgradeTable[i] = m_BoardStay[i - 5].m_PieceIdx;
		int idx = 0;
		int level = 0;
		if (m_BoardStay[i - 5].m_Actor != nullptr)
		{
			level = m_BoardStay[i - 5].m_Actor->GetLevel();
			idx = m_BoardStay[i - 5].m_PieceIdx;
		}

		if (idx != 0 && idx != -1)
		{
			m_UpgradeCheckIdx[level-1][idx]++;
		}
		if (m_BoardStay[i - 5].m_PieceIdx == 19)
		{
			existEgg = true;
		}
	}
	
	eggCheck = existEgg;
	//에그 계산
	for (int i = 5; i < 13; i++)
	{
		//m_UpgradeTable[i] = m_BoardStay[i - 5].m_PieceIdx;
		int idx = 0;
		int level = 0;
		if (m_BoardStay[i - 5].m_Actor != nullptr)
		{
			level = m_BoardStay[i - 5].m_Actor->GetLevel();
			idx = m_BoardStay[i - 5].m_PieceIdx;
		}

		if (idx != 0 && idx != -1)
		{
			if (existEgg == true)
			{
				m_UpgradeCheckIdx[level - 1][idx]++;
			}
		}

	}


	//넣어줬다
	//체크해서 3개이상 골라내야한다.

	BoardIndex result;
	for (int y = 0; y < 3; y++)
	{
		for (int x = 1; x < 23; x++)
		{
			if (m_UpgradeCheckIdx[y][x] >= 3)
			{
				result.z = y + 1; //레벨
				result.x = x; //피스인덱스

				return result;
			}
		}
	}


	return BoardIndex(-1,-1);
}

void ChessBoard::UpgradePiece(int _Idx, int _Level)
{
	int level = 0;
	//해당 인덱스 유닛 전부 지웓준다

	for (int i = 0; i < 8; i++)
	{
		if (m_BoardStay[i].m_PieceIdx == _Idx)
		{
			level = m_BoardStay[i].m_Actor->GetLevel();
			if (_Level == level)
			{
				//PopAllyPiece(m_BoardStay[i].m_Actor);

				m_BoardStay[i].m_Actor->Die();
				m_BoardStay[i].Release();
			}
		}
	}

	for (int i = 0; i < 8; i++)
	{
		if (m_BoardStay[i].m_PieceIdx == (int)PIECEKINDS::EGG)
		{
			m_BoardStay[i].m_Actor->Die();
			m_BoardStay[i].Release();
		}
	}

	
	HPTR<Actor> newActor = nullptr;
	
	switch (level)
	{
		case 1:
		{
			newActor = CreatePieceLevel2(_Idx);
		}
		break;
		case 2:
		{
			newActor = CreatePieceLevel3(_Idx);
		}
		break;
		default:
		{
			// 일단 남겨둠 예외처리
		}
		break;
	}


	PushActorInStayAutoSort(newActor, _Idx);
	//레벨2 유닛으로 다시 만들어준다.

}

void ChessBoard::CheatUpgradePiece(BoardIndex _Idx)
{
	int level = 0;
	//해당 인덱스 유닛 전부 지웓준다
	int pieceIdx = 0;
	


	if (_Idx.z < 9)
	{
		//체스보드
	}
	else
	{
		//체스 스테이
		level = m_BoardStay[_Idx.x].m_Actor->GetLevel();
		if (level == 3)
		{
			return;
		}
		StatusLogic* status = m_BoardStay[_Idx.x].m_Actor->GetComponent<StatusLogic>();
		pieceIdx  = status->GetPieceIdx();
		

		m_BoardStay[_Idx.x].m_Actor->Die();
		m_BoardStay[_Idx.x].Release();
	}

	HPTR<Actor> newActor = nullptr;
	switch (level)
	{
	case 1:
	{
		newActor = CreatePieceLevel2(pieceIdx);
	}
	break;
	case 2:
	{
		newActor = CreatePieceLevel3(pieceIdx);
	}
	break;
	default:
	{
		// 일단 남겨둠 예외처리
	}
	break;
	}


	PushActorInStayAutoSort(newActor, pieceIdx);

	if (_Idx.z < 9)
	{
		PushAllyPiece(newActor);
	}
	else
	{
		//
	}

}


void ChessBoard::Init()
{
	m_UpgradeCheckIdx.resize(3);
	for (size_t i = 0; i < 3; i++)
	{
		m_UpgradeCheckIdx[i].resize(23);
	}

	m_UpgradeTable.resize(13);
	m_Board.resize(8);

	Vector4 startPos = {-434.0f, 0.0f, 434.0f, 0.0f};
	for (size_t z = 0; z < 8; z++)
	{
		m_Board[z].resize(8);
		for (size_t x = 0; x < 8; x++)
		{
			m_Board[z][x].m_BoardPos = startPos;
			m_Board[z][x].m_Index.x = (UINT)x;
			m_Board[z][x].m_Index.z = (UINT)z;
			m_Board[z][x].m_Actor = nullptr;

			startPos.x += 124.0f;
		}		
		startPos.x = -434.0f;
		startPos.z -= 124.0f;
	}

	m_BoardStay.resize(8);

	for (size_t i = 0; i < 8; i++)
	{
		m_BoardStay[i].m_BoardPos.z = -600.0f;
		m_BoardStay[i].m_BoardPos.x = -434.0f + (i * 124.0f);
		m_BoardStay[i].m_Index.x = 0;
		m_BoardStay[i].m_Index.z = 0;
		m_BoardStay[i].m_PieceIdx = 0;
		m_BoardStay[i].m_Actor = nullptr;
	}
}

void ChessBoard::Update()
{
	InitPtr();

	m_AllyBegin = m_AllyList.begin();
	m_AllyEnd = m_AllyList.end();
	for (; m_AllyBegin != m_AllyEnd; ++m_AllyBegin)
	{
		StatusLogic* statuslogic = (*m_AllyBegin)->GetComponent<StatusLogic>();	//다운캐스팅을 하지않고 설계할수 있는 방법이 없을까?
		
		Vector4 Pos = (*m_AllyBegin)->GetTransform()->GetLocalPosition();
		BoardIndex tempIndex = CalculateIndex(Pos);

		PushActorInBoard(tempIndex, (*m_AllyBegin), statuslogic);
	}

	m_EnemyBegin = m_EnemyList.begin();
	m_EnemyEnd = m_EnemyList.end();
	for (; m_EnemyBegin != m_EnemyEnd; ++m_EnemyBegin)
	{
		StatusLogic* statuslogic = (*m_EnemyBegin)->GetComponent<StatusLogic>();

		Vector4 Pos = (*m_EnemyBegin)->GetTransform()->GetLocalPosition();
		BoardIndex tempIndex = CalculateIndex(Pos);

		//PushActorInBoard(tempIndex, (*m_EnemyBegin));		
		PushActorInBoard(tempIndex, (*m_EnemyBegin), statuslogic);
	}


}

void ChessBoard::SetActor(BoardIndex _Idx, Actor* _Actor)
{
	m_Board[_Idx.z][_Idx.x].m_Actor = _Actor;
}

void ChessBoard::SetActor(int _X, int _Y, Actor* _Actor)
{
	m_Board[_Y][_X].m_Actor = _Actor;
}

HPTR<Actor> ChessBoard::CreatePieceLevel2(int _Idx)
{
	HPTR<Actor> createdActor = nullptr;
	HPTR<FbxAnimation> createdActorAni = nullptr;
	switch ((PIECEKINDS)_Idx)
	{
	case PIECEKINDS::UNICORN:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"Unicorn02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 45, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 46, 91, false);
		createdActorAni->CreateAnimation(L"BORN", 92, 137, false);
		createdActorAni->CreateAnimation(L"DEATH", 138, 192, false);
		createdActorAni->CreateAnimation(L"DIZZY", 193, 253, true);
		createdActorAni->CreateAnimation(L"IDLE", 254, 314, true);
		createdActorAni->CreateAnimation(L"JUMP", 315, 365, false);
		createdActorAni->CreateAnimation(L"RUN", 366, 384, true);
		createdActorAni->CreateAnimation(L"SKILL", 385, 431, false);
		createdActorAni->CreateAnimation(L"VICTORY", 432, 497, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<Unicorn> moveLogic = createdActor->AddComponent<Unicorn>();
		moveLogic->SetName(L"Unicorn");
		moveLogic->SetRange(3);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME
		stateLogic->SetStatus(400, 55, 15.0f, 11.0f, _Idx);
stateLogic->SetNUMBER(1);Sound::Play(L"h002_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::GODOFWAR:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 0.8f, 0.8f, 0.8f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"GodofWar02.FBXDATA");
		createdActorAni->CreateAnimation(L"BORN", 0, 50, false);
		createdActorAni->CreateAnimation(L"ATTACK01", 163, 283, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 163, 283, false);
		createdActorAni->CreateAnimation(L"DEATH", 51, 101, false);
		createdActorAni->CreateAnimation(L"DIZZY", 102, 162, true);
		createdActorAni->CreateAnimation(L"IDLE2", 163, 283, true);
		createdActorAni->CreateAnimation(L"IDLE", 284, 324, true);
		createdActorAni->CreateAnimation(L"JUMP", 325, 365, false);
		createdActorAni->CreateAnimation(L"RUN", 366, 386, true);
		createdActorAni->CreateAnimation(L"SKILL", 387, 427, false);
		createdActorAni->CreateAnimation(L"VICTORY", 428, 503, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<GodofWar> moveLogic = createdActor->AddComponent<GodofWar>();
		moveLogic->SetName(L"GodofWar01");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME
		stateLogic->SetStatus(650, 0, 8.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(2);
		Sound::Play(L"h057_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::SWORDMASTER:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"SwordMaster02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 33, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 34, 67, false);
		createdActorAni->CreateAnimation(L"BORN", 68, 113, false);
		createdActorAni->CreateAnimation(L"DEATH", 114, 194, false);
		createdActorAni->CreateAnimation(L"DIZZY", 195, 235, true);
		createdActorAni->CreateAnimation(L"IDLE", 236, 276, true);
		createdActorAni->CreateAnimation(L"JUMP", 277, 335, false);
		createdActorAni->CreateAnimation(L"RUN", 336, 360, true);
		createdActorAni->CreateAnimation(L"SKILL", 361, 376, false);
		createdActorAni->CreateAnimation(L"SKILLLOOP", 377, 385, true);
		createdActorAni->CreateAnimation(L"VICTORY", 386, 483, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<SwordMaster> moveLogic = createdActor->AddComponent<SwordMaster>();
		moveLogic->SetName(L"SwordMaster01");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME
		stateLogic->SetStatus(600, 67, 12.0f, 5.0f, _Idx);
		stateLogic->SetNUMBER(3);		Sound::Play(L"h015_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::MANAMAGICIAN:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.2f, 1.2f, 1.2f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"ManaMagician02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 44, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 45, 89, false);
		createdActorAni->CreateAnimation(L"BORN", 90, 135, false);
		createdActorAni->CreateAnimation(L"DEATH", 136, 189, false);
		createdActorAni->CreateAnimation(L"DIZZY", 190, 230, true);
		createdActorAni->CreateAnimation(L"IDLE", 231, 271, true);
		createdActorAni->CreateAnimation(L"JUMP", 272, 316, false);
		createdActorAni->CreateAnimation(L"RUN", 317, 335, true);
		createdActorAni->CreateAnimation(L"VICTORY", 336, 390, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<MpMagicion> moveLogic = createdActor->AddComponent<MpMagicion>();
		moveLogic->SetName(L"MPMAGICION");
		moveLogic->SetRange(4);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		stateLogic->SetStatus(450, 42, 0.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(4);		Sound::Play(L"h013_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::LITTLEDRAGON:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.7f, 1.7f, 1.7f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"LittleDragon02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 37, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 38, 76, false);
		createdActorAni->CreateAnimation(L"BORN", 77, 116, false);
		createdActorAni->CreateAnimation(L"DEATH", 117, 159, false);
		createdActorAni->CreateAnimation(L"DIZZY", 160, 192, true);
		createdActorAni->CreateAnimation(L"IDLE", 193, 213, true);
		createdActorAni->CreateAnimation(L"JUMP", 214, 256, false);
		createdActorAni->CreateAnimation(L"RUN", 257, 270, true);
		createdActorAni->CreateAnimation(L"SKILL", 271, 305, false);
		createdActorAni->CreateAnimation(L"VICTORY", 306, 444, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<LittleDragon> moveLogic = createdActor->AddComponent<LittleDragon>();
		moveLogic->SetName(L"LittleDragon01");
		moveLogic->SetRange(3);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME
		stateLogic->SetStatus(450, 45, 5.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(5);		Sound::Play(L"h018_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::SHININGARCHER:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"ShiningArcher02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 30, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 31, 61, false);
		createdActorAni->CreateAnimation(L"BORN", 62, 107, false);
		createdActorAni->CreateAnimation(L"DEATH", 108, 204, false);
		createdActorAni->CreateAnimation(L"DIZZY", 205, 253, true);
		createdActorAni->CreateAnimation(L"IDLE", 254, 294, true);
		createdActorAni->CreateAnimation(L"JUMP", 295, 342, false);
		createdActorAni->CreateAnimation(L"RUN", 343, 363, true);
		createdActorAni->CreateAnimation(L"SKILL", 364, 396, false);
		createdActorAni->CreateAnimation(L"VICTORY", 397, 591, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<ShiningArcher> moveLogic = createdActor->AddComponent<ShiningArcher>();
		moveLogic->SetName(L"ShiningArcher01");
		moveLogic->SetRange(4);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME
		stateLogic->SetStatus(600, 60, 10.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(6);		Sound::Play(L"h058_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::BLADEKNIGHT:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 0.8f, 0.8f, 0.8f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"BladeKnight02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 39, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 40, 79, false);
		createdActorAni->CreateAnimation(L"SKILL", 40, 79, false);
		createdActorAni->CreateAnimation(L"BORN", 80, 143, false);
		createdActorAni->CreateAnimation(L"DEATH", 144, 223, false);
		createdActorAni->CreateAnimation(L"DIZZY", 224, 274, true);
		createdActorAni->CreateAnimation(L"IDLE", 275, 325, true);
		createdActorAni->CreateAnimation(L"JUMP", 326, 370, false);
		createdActorAni->CreateAnimation(L"RUN", 371, 387, true);
		createdActorAni->CreateAnimation(L"VICTORY", 388, 443, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<BladeKnight> moveLogic = createdActor->AddComponent<BladeKnight>();
		moveLogic->SetName(L"BladeKnight01");
		moveLogic->SetRange(3);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME
		stateLogic->SetStatus(500, 0, 0.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(7);		Sound::Play(L"h023_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::DEMONLARVA:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"DemonLarva02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 36, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 37, 73, false);
		createdActorAni->CreateAnimation(L"BORN", 74, 121, false);
		createdActorAni->CreateAnimation(L"DEATH", 122, 169, false);
		createdActorAni->CreateAnimation(L"DIZZY", 170, 210, true);
		createdActorAni->CreateAnimation(L"IDLE", 211, 251, true);
		createdActorAni->CreateAnimation(L"JUMP", 252, 304, false);
		createdActorAni->CreateAnimation(L"RUN", 305, 325, true);
		createdActorAni->CreateAnimation(L"SKILL", 326, 372, false);
		createdActorAni->CreateAnimation(L"VICTORY", 373, 409, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<DemonLarva> moveLogic = createdActor->AddComponent<DemonLarva>();
		moveLogic->SetName(L"DemonLarva01");
		moveLogic->SetRange(3);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME
		stateLogic->SetStatus(600, 55, 5.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(8);		Sound::Play(L"h027_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::WAREWOLF:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"Warewolf02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 33, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 34, 67, false);
		createdActorAni->CreateAnimation(L"BORN", 68, 113, false);
		createdActorAni->CreateAnimation(L"DEATH", 114, 185, false);
		createdActorAni->CreateAnimation(L"DIZZY", 186, 278, true);
		createdActorAni->CreateAnimation(L"IDLE", 279, 329, true);
		createdActorAni->CreateAnimation(L"JUMP", 330, 370, false);
		createdActorAni->CreateAnimation(L"RUN", 371, 392, true);
		createdActorAni->CreateAnimation(L"SKILL", 393, 433, false);
		createdActorAni->CreateAnimation(L"VICTORY", 434, 531, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<Warewolf> moveLogic = createdActor->AddComponent<Warewolf>();
		moveLogic->SetName(L"Warewolf01");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(700, 55, 60.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(9);		Sound::Play(L"h026_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::FIREMAGICIAN:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"FireMagician02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 40, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 41, 81, false);
		createdActorAni->CreateAnimation(L"BORN", 82, 127, false);
		createdActorAni->CreateAnimation(L"DEATH", 128, 199, false);
		createdActorAni->CreateAnimation(L"DIZZY", 200, 240, true);
		createdActorAni->CreateAnimation(L"IDLE", 241, 301, true);
		createdActorAni->CreateAnimation(L"JUMP", 302, 346, false);
		createdActorAni->CreateAnimation(L"RUN", 347, 371, true);
		createdActorAni->CreateAnimation(L"SKILL", 372, 404, false);
		createdActorAni->CreateAnimation(L"VICTORY", 405, 487, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<FireMagician> moveLogic = createdActor->AddComponent<FireMagician>();
		moveLogic->SetName(L"FireMagician01");
		moveLogic->SetRange(4);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(600, 52, 10.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(10);		Sound::Play(L"h038_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::WINDRANGER:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"WindRanger02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 43, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 44, 87, false);
		createdActorAni->CreateAnimation(L"BORN", 88, 134, false);
		createdActorAni->CreateAnimation(L"DEATH", 135, 222, false);
		createdActorAni->CreateAnimation(L"DIZZY", 223, 271, true);
		createdActorAni->CreateAnimation(L"IDLE", 272, 322, true);
		createdActorAni->CreateAnimation(L"JUMP", 323, 373, false);
		createdActorAni->CreateAnimation(L"RUN", 374, 398, true);
		createdActorAni->CreateAnimation(L"SKILL", 399, 492, false);
		createdActorAni->CreateAnimation(L"VICTORY", 493, 575, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<WindRanger> moveLogic = createdActor->AddComponent<WindRanger>();
		moveLogic->SetName(L"WindRanger01");
		moveLogic->SetRange(4);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(550, 60, 10.0f, 0.0f, _Idx);
	stateLogic->SetNUMBER(11);		Sound::Play(L"h030_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::THUNDERSPIRIT:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"ThunderSpirit02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 33, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 34, 67, false);
		createdActorAni->CreateAnimation(L"BORN", 68, 113, false);
		createdActorAni->CreateAnimation(L"DEATH", 114, 203, false);
		createdActorAni->CreateAnimation(L"DIZZY", 204, 248, true);
		createdActorAni->CreateAnimation(L"IDLE", 249, 301, true);
		createdActorAni->CreateAnimation(L"JUMP", 302, 357, false);
		createdActorAni->CreateAnimation(L"RUN", 358, 402, true);
		createdActorAni->CreateAnimation(L"SKILL", 403, 468, false);
		createdActorAni->CreateAnimation(L"VICTORY", 469, 549, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<ThunderSpirit> moveLogic = createdActor->AddComponent<ThunderSpirit>();
		moveLogic->SetName(L"ThunderSpirit01");
		moveLogic->SetRange(3);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(750, 55, 10.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(12);		Sound::Play(L"h029_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::TOENAILDRUID:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 0.8f, 0.8f, 0.8f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"ToenailDruid02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 35, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 36, 71, false);
		createdActorAni->CreateAnimation(L"BORN", 72, 117, false);
		createdActorAni->CreateAnimation(L"DEATH", 118, 182, false);
		createdActorAni->CreateAnimation(L"DIZZY", 183, 223, true);
		createdActorAni->CreateAnimation(L"IDLE", 224, 264, true);
		createdActorAni->CreateAnimation(L"JUMP", 265, 315, false);
		createdActorAni->CreateAnimation(L"RUN", 316, 332, true);
		createdActorAni->CreateAnimation(L"SKILL", 333, 379, false);
		createdActorAni->CreateAnimation(L"VICTORY", 380, 510, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<ToenailDruid> moveLogic = createdActor->AddComponent<ToenailDruid>();
		moveLogic->SetName(L"ToenailDruid01");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(800, 55, 60.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(13);		Sound::Play(L"h050_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::SIREN:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"Siren02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 35, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 36, 76, false);
		createdActorAni->CreateAnimation(L"BORN", 77, 132, false);
		createdActorAni->CreateAnimation(L"DEATH", 133, 192, false);
		createdActorAni->CreateAnimation(L"DIZZY", 193, 233, true);
		createdActorAni->CreateAnimation(L"IDLE", 234, 274, true);
		createdActorAni->CreateAnimation(L"JUMP", 275, 330, false);
		createdActorAni->CreateAnimation(L"RUN", 331, 349, true);
		createdActorAni->CreateAnimation(L"SKILL", 350, 410, false);
		createdActorAni->CreateAnimation(L"VICTORY", 411, 496, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<Siren> moveLogic = createdActor->AddComponent<Siren>();
		moveLogic->SetName(L"Siren01");
		moveLogic->SetRange(4);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(750, 60, 20.0f, 3.0f, _Idx);
		stateLogic->SetNUMBER(14);		Sound::Play(L"h048_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::PIRATECAPTAIN:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"PirateCaptain02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 40, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 41, 86, false);
		createdActorAni->CreateAnimation(L"BORN", 87, 132, false);
		createdActorAni->CreateAnimation(L"DEATH", 133, 203, false);
		createdActorAni->CreateAnimation(L"DIZZY", 204, 264, true);
		createdActorAni->CreateAnimation(L"IDLE", 265, 305, true);
		createdActorAni->CreateAnimation(L"JUMP", 306, 349, false);
		createdActorAni->CreateAnimation(L"RUN", 350, 372, true);
		createdActorAni->CreateAnimation(L"SKILL", 373, 413, false);
		createdActorAni->CreateAnimation(L"VICTORY", 414, 519, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<PirateCaptain> moveLogic = createdActor->AddComponent<PirateCaptain>();
		moveLogic->SetName(L"PirateCaptain01");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(950, 82, 15.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(15);		Sound::Play(L"h041_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::TORTOLANELDER:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"TortolanElder02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 51, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 52, 103, false);
		createdActorAni->CreateAnimation(L"BORN", 104, 159, false);
		createdActorAni->CreateAnimation(L"DEATH", 160, 294, false);
		createdActorAni->CreateAnimation(L"DIZZY", 295, 340, true);
		createdActorAni->CreateAnimation(L"IDLE", 341, 386, true);
		createdActorAni->CreateAnimation(L"JUMP", 387, 442, false);
		createdActorAni->CreateAnimation(L"RUN", 443, 491, true);
		createdActorAni->CreateAnimation(L"SKILL", 492, 602, false);
		createdActorAni->CreateAnimation(L"VICTORY", 603, 703, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<TortolanElder> moveLogic = createdActor->AddComponent<TortolanElder>();
		moveLogic->SetName(L"TortolanElder01");
		moveLogic->SetRange(4);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(650, 42, 10.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(16);		Sound::Play(L"h043_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::DOOMARBITER:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"DoomArbiter02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 40, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 41, 80, false);
		createdActorAni->CreateAnimation(L"BORN", 81, 161, false);
		createdActorAni->CreateAnimation(L"DEATH", 162, 251, false);
		createdActorAni->CreateAnimation(L"DIZZY", 252, 309, true);
		createdActorAni->CreateAnimation(L"IDLE", 310, 350, true);
		createdActorAni->CreateAnimation(L"JUMP", 351, 397, false);
		createdActorAni->CreateAnimation(L"RUN", 398, 424, true);
		createdActorAni->CreateAnimation(L"SKILL", 425, 495, false);
		createdActorAni->CreateAnimation(L"VICTORY", 496, 598, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<DoomArbiter> moveLogic = createdActor->AddComponent<DoomArbiter>();
		moveLogic->SetName(L"DoomArbiter01");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(950, 115, 10.0f, 15.0f, _Idx);
		stateLogic->SetNUMBER(17);		Sound::Play(L"h040_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::TIDEHUNTER:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.2f, 1.2f, 1.2f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"TideHunter02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 37, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 38, 75, false);
		createdActorAni->CreateAnimation(L"BORN", 76, 132, false);
		createdActorAni->CreateAnimation(L"DEATH", 133, 191, false);
		createdActorAni->CreateAnimation(L"DIZZY", 192, 232, true);
		createdActorAni->CreateAnimation(L"IDLE", 233, 273, true);
		createdActorAni->CreateAnimation(L"JUMP", 274, 321, false);
		createdActorAni->CreateAnimation(L"RUN", 322, 346, true);
		createdActorAni->CreateAnimation(L"SKILL", 347, 390, false);
		createdActorAni->CreateAnimation(L"VICTORY", 391, 447, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<TideHunter> moveLogic = createdActor->AddComponent<TideHunter>();
		moveLogic->SetName(L"TideHunter01");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(950, 50, 30.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(18);		Sound::Play(L"h053_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::EGG:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"Egg02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 36, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 37, 73, false);
		createdActorAni->CreateAnimation(L"BORN", 74, 119, false);
		createdActorAni->CreateAnimation(L"DEATH", 120, 152, false);
		createdActorAni->CreateAnimation(L"DIZZY", 153, 213, true);
		createdActorAni->CreateAnimation(L"IDLE", 214, 254, true);
		createdActorAni->CreateAnimation(L"JUMP", 255, 292, false);
		createdActorAni->CreateAnimation(L"RUN", 293, 309, true);
		createdActorAni->CreateAnimation(L"VICTORY", 351, 444, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<Egg> moveLogic = createdActor->AddComponent<Egg>();
		moveLogic->SetName(L"Egg01");
		moveLogic->SetRange(3);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(700, 60, 0.0f, 0.0f, _Idx);

		stateLogic->SetNUMBER(19);		Sound::Play(L"h056_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::GODOFTHUNDER:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"GodofThunder02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 40, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 41, 81, false);
		createdActorAni->CreateAnimation(L"BORN", 82, 127, false);
		createdActorAni->CreateAnimation(L"DEATH", 128, 211, false);
		createdActorAni->CreateAnimation(L"DIZZY", 212, 262, true);
		createdActorAni->CreateAnimation(L"IDLE", 263, 317, true);
		createdActorAni->CreateAnimation(L"JUMP", 318, 374, false);
		createdActorAni->CreateAnimation(L"RUN", 375, 401, true);
		createdActorAni->CreateAnimation(L"SKILL", 402, 438, false);
		createdActorAni->CreateAnimation(L"VICTORY", 439, 564, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<GodofThunder> moveLogic = createdActor->AddComponent<GodofThunder>();
		moveLogic->SetName(L"GodofThunder01");
		moveLogic->SetRange(3);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(950, 60, 15.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(20);		Sound::Play(L"h060_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::SHADOWDEMON:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0F, 1.0F, 1.0F });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"ShadowDemon02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 37, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 38, 77, false);
		createdActorAni->CreateAnimation(L"BORN", 78, 123, false);
		createdActorAni->CreateAnimation(L"DEATH", 124, 197, false);
		createdActorAni->CreateAnimation(L"DIZZY", 198, 260, true);
		createdActorAni->CreateAnimation(L"IDLE", 261, 323, true);
		createdActorAni->CreateAnimation(L"JUMP", 324, 369, false);
		createdActorAni->CreateAnimation(L"RUN", 370, 400, true);
		createdActorAni->CreateAnimation(L"SKILL", 401, 489, false);
		createdActorAni->CreateAnimation(L"VICTORY", 490, 582, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<ShadowDemon> moveLogic = createdActor->AddComponent<ShadowDemon>();
		moveLogic->SetName(L"ShadowDemon01");
		moveLogic->SetRange(4);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(450, 80, 15.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(21);		Sound::Play(L"h037_showup_02_eng.wav");	}
	break;
	case PIECEKINDS::WALRUSCHAMP:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(2);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"WalrusChamp02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 34, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 35, 65, false);
		createdActorAni->CreateAnimation(L"BORN", 66, 106, false);
		createdActorAni->CreateAnimation(L"DEATH", 107, 197, false);
		createdActorAni->CreateAnimation(L"DIZZY", 198, 258, true);
		createdActorAni->CreateAnimation(L"IDLE", 259, 309, true);
		createdActorAni->CreateAnimation(L"JUMP", 310, 370, false);
		createdActorAni->CreateAnimation(L"RUN", 371, 397, true);
		createdActorAni->CreateAnimation(L"SKILL", 398, 448, false);
		createdActorAni->CreateAnimation(L"VICTORY", 449, 519, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<WalrusChamp> moveLogic = createdActor->AddComponent<WalrusChamp>();
		moveLogic->SetName(L"WalrusChamp01");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(650, 52, 10.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(0);		Sound::Play(L"h004_showup_02_eng.wav");	}
	break;
	}

	//ChessBoard::Inst().PushAllyPiece(createdActor);


	return createdActor;
}

HPTR<Actor> ChessBoard::CreatePieceLevel3(int _Idx)
{
	HPTR<Actor> createdActor = nullptr;
	HPTR<FbxAnimation> createdActorAni = nullptr;
	switch ((PIECEKINDS)_Idx)
	{
	case PIECEKINDS::UNICORN:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"Unicorn02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 45, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 46, 91, false);
		createdActorAni->CreateAnimation(L"BORN", 92, 137, false);
		createdActorAni->CreateAnimation(L"DEATH", 138, 192, false);
		createdActorAni->CreateAnimation(L"DIZZY", 193, 253, true);
		createdActorAni->CreateAnimation(L"IDLE", 254, 314, true);
		createdActorAni->CreateAnimation(L"JUMP", 315, 365, false);
		createdActorAni->CreateAnimation(L"RUN", 366, 384, true);
		createdActorAni->CreateAnimation(L"SKILL", 385, 431, false);
		createdActorAni->CreateAnimation(L"VICTORY", 432, 497, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<Unicorn> moveLogic = createdActor->AddComponent<Unicorn>();
		moveLogic->SetName(L"Unicorn");
		moveLogic->SetRange(3);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME
		stateLogic->SetStatus(400, 55, 15.0f, 11.0f, _Idx);
		stateLogic->SetNUMBER(1);		Sound::Play(L"h002_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::GODOFWAR:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 0.8f, 0.8f, 0.8f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"GodofWar02.FBXDATA");
		createdActorAni->CreateAnimation(L"BORN", 0, 50, false);
		createdActorAni->CreateAnimation(L"ATTACK01", 163, 283, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 163, 283, false);
		createdActorAni->CreateAnimation(L"DEATH", 51, 101, false);
		createdActorAni->CreateAnimation(L"DIZZY", 102, 162, true);
		createdActorAni->CreateAnimation(L"IDLE2", 163, 283, true);
		createdActorAni->CreateAnimation(L"IDLE", 284, 324, true);
		createdActorAni->CreateAnimation(L"JUMP", 325, 365, false);
		createdActorAni->CreateAnimation(L"RUN", 366, 386, true);
		createdActorAni->CreateAnimation(L"SKILL", 387, 427, false);
		createdActorAni->CreateAnimation(L"VICTORY", 428, 503, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<GodofWar> moveLogic = createdActor->AddComponent<GodofWar>();
		moveLogic->SetName(L"GodofWar01");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME
		stateLogic->SetStatus(650, 0, 8.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(2);		Sound::Play(L"h057_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::SWORDMASTER:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"SwordMaster02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 33, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 34, 67, false);
		createdActorAni->CreateAnimation(L"BORN", 68, 113, false);
		createdActorAni->CreateAnimation(L"DEATH", 114, 194, false);
		createdActorAni->CreateAnimation(L"DIZZY", 195, 235, true);
		createdActorAni->CreateAnimation(L"IDLE", 236, 276, true);
		createdActorAni->CreateAnimation(L"JUMP", 277, 335, false);
		createdActorAni->CreateAnimation(L"RUN", 336, 360, true);
		createdActorAni->CreateAnimation(L"SKILL", 361, 376, false);
		createdActorAni->CreateAnimation(L"SKILLLOOP", 377, 385, true);
		createdActorAni->CreateAnimation(L"VICTORY", 386, 483, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<SwordMaster> moveLogic = createdActor->AddComponent<SwordMaster>();
		moveLogic->SetName(L"SwordMaster01");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME
		stateLogic->SetStatus(600, 67, 12.0f, 5.0f, _Idx);
		stateLogic->SetNUMBER(3);		Sound::Play(L"h015_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::MANAMAGICIAN:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.2f, 1.2f, 1.2f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"ManaMagician02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 44, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 45, 89, false);
		createdActorAni->CreateAnimation(L"BORN", 90, 135, false);
		createdActorAni->CreateAnimation(L"DEATH", 136, 189, false);
		createdActorAni->CreateAnimation(L"DIZZY", 190, 230, true);
		createdActorAni->CreateAnimation(L"IDLE", 231, 271, true);
		createdActorAni->CreateAnimation(L"JUMP", 272, 316, false);
		createdActorAni->CreateAnimation(L"RUN", 317, 335, true);
		createdActorAni->CreateAnimation(L"VICTORY", 336, 390, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<MpMagicion> moveLogic = createdActor->AddComponent<MpMagicion>();
		moveLogic->SetName(L"MPMAGICION");
		moveLogic->SetRange(4);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		stateLogic->SetStatus(450, 42, 0.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(4);		Sound::Play(L"h013_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::LITTLEDRAGON:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.7f, 1.7f, 1.7f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"LittleDragon02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 37, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 38, 76, false);
		createdActorAni->CreateAnimation(L"BORN", 77, 116, false);
		createdActorAni->CreateAnimation(L"DEATH", 117, 159, false);
		createdActorAni->CreateAnimation(L"DIZZY", 160, 192, true);
		createdActorAni->CreateAnimation(L"IDLE", 193, 213, true);
		createdActorAni->CreateAnimation(L"JUMP", 214, 256, false);
		createdActorAni->CreateAnimation(L"RUN", 257, 270, true);
		createdActorAni->CreateAnimation(L"SKILL", 271, 305, false);
		createdActorAni->CreateAnimation(L"VICTORY", 306, 444, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<LittleDragon> moveLogic = createdActor->AddComponent<LittleDragon>();
		moveLogic->SetName(L"LittleDragon01");
		moveLogic->SetRange(3);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME
		stateLogic->SetStatus(450, 45, 5.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(5);		Sound::Play(L"h018_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::SHININGARCHER:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"ShiningArcher02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 30, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 31, 61, false);
		createdActorAni->CreateAnimation(L"BORN", 62, 107, false);
		createdActorAni->CreateAnimation(L"DEATH", 108, 204, false);
		createdActorAni->CreateAnimation(L"DIZZY", 205, 253, true);
		createdActorAni->CreateAnimation(L"IDLE", 254, 294, true);
		createdActorAni->CreateAnimation(L"JUMP", 295, 342, false);
		createdActorAni->CreateAnimation(L"RUN", 343, 363, true);
		createdActorAni->CreateAnimation(L"SKILL", 364, 396, false);
		createdActorAni->CreateAnimation(L"VICTORY", 397, 591, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<ShiningArcher> moveLogic = createdActor->AddComponent<ShiningArcher>();
		moveLogic->SetName(L"ShiningArcher01");
		moveLogic->SetRange(4);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME
		stateLogic->SetStatus(600, 60, 10.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(6);		Sound::Play(L"h058_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::BLADEKNIGHT:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 0.8f, 0.8f, 0.8f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"BladeKnight02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 39, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 40, 79, false);
		createdActorAni->CreateAnimation(L"SKILL", 40, 79, false);
		createdActorAni->CreateAnimation(L"BORN", 80, 143, false);
		createdActorAni->CreateAnimation(L"DEATH", 144, 223, false);
		createdActorAni->CreateAnimation(L"DIZZY", 224, 274, true);
		createdActorAni->CreateAnimation(L"IDLE", 275, 325, true);
		createdActorAni->CreateAnimation(L"JUMP", 326, 370, false);
		createdActorAni->CreateAnimation(L"RUN", 371, 387, true);
		createdActorAni->CreateAnimation(L"VICTORY", 388, 443, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<BladeKnight> moveLogic = createdActor->AddComponent<BladeKnight>();
		moveLogic->SetName(L"BladeKnight01");
		moveLogic->SetRange(3);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME
		stateLogic->SetStatus(500, 0, 0.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(7);		Sound::Play(L"h023_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::DEMONLARVA:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"DemonLarva02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 36, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 37, 73, false);
		createdActorAni->CreateAnimation(L"BORN", 74, 121, false);
		createdActorAni->CreateAnimation(L"DEATH", 122, 169, false);
		createdActorAni->CreateAnimation(L"DIZZY", 170, 210, true);
		createdActorAni->CreateAnimation(L"IDLE", 211, 251, true);
		createdActorAni->CreateAnimation(L"JUMP", 252, 304, false);
		createdActorAni->CreateAnimation(L"RUN", 305, 325, true);
		createdActorAni->CreateAnimation(L"SKILL", 326, 372, false);
		createdActorAni->CreateAnimation(L"VICTORY", 373, 409, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<DemonLarva> moveLogic = createdActor->AddComponent<DemonLarva>();
		moveLogic->SetName(L"DemonLarva01");
		moveLogic->SetRange(3);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME
		stateLogic->SetStatus(600, 55, 5.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(8);		Sound::Play(L"h027_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::WAREWOLF:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"Warewolf02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 33, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 34, 67, false);
		createdActorAni->CreateAnimation(L"BORN", 68, 113, false);
		createdActorAni->CreateAnimation(L"DEATH", 114, 185, false);
		createdActorAni->CreateAnimation(L"DIZZY", 186, 278, true);
		createdActorAni->CreateAnimation(L"IDLE", 279, 329, true);
		createdActorAni->CreateAnimation(L"JUMP", 330, 370, false);
		createdActorAni->CreateAnimation(L"RUN", 371, 392, true);
		createdActorAni->CreateAnimation(L"SKILL", 393, 433, false);
		createdActorAni->CreateAnimation(L"VICTORY", 434, 531, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<Warewolf> moveLogic = createdActor->AddComponent<Warewolf>();
		moveLogic->SetName(L"Warewolf01");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(700, 55, 60.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(9);		Sound::Play(L"h026_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::FIREMAGICIAN:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"FireMagician02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 40, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 41, 81, false);
		createdActorAni->CreateAnimation(L"BORN", 82, 127, false);
		createdActorAni->CreateAnimation(L"DEATH", 128, 199, false);
		createdActorAni->CreateAnimation(L"DIZZY", 200, 240, true);
		createdActorAni->CreateAnimation(L"IDLE", 241, 301, true);
		createdActorAni->CreateAnimation(L"JUMP", 302, 346, false);
		createdActorAni->CreateAnimation(L"RUN", 347, 371, true);
		createdActorAni->CreateAnimation(L"SKILL", 372, 404, false);
		createdActorAni->CreateAnimation(L"VICTORY", 405, 487, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<FireMagician> moveLogic = createdActor->AddComponent<FireMagician>();
		moveLogic->SetName(L"FireMagician01");
		moveLogic->SetRange(4);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(600, 52, 10.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(10);		Sound::Play(L"h038_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::WINDRANGER:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"WindRanger02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 43, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 44, 87, false);
		createdActorAni->CreateAnimation(L"BORN", 88, 134, false);
		createdActorAni->CreateAnimation(L"DEATH", 135, 222, false);
		createdActorAni->CreateAnimation(L"DIZZY", 223, 271, true);
		createdActorAni->CreateAnimation(L"IDLE", 272, 322, true);
		createdActorAni->CreateAnimation(L"JUMP", 323, 373, false);
		createdActorAni->CreateAnimation(L"RUN", 374, 398, true);
		createdActorAni->CreateAnimation(L"SKILL", 399, 492, false);
		createdActorAni->CreateAnimation(L"VICTORY", 493, 575, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<WindRanger> moveLogic = createdActor->AddComponent<WindRanger>();
		moveLogic->SetName(L"WindRanger01");
		moveLogic->SetRange(4);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(550, 60, 10.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(11);		Sound::Play(L"h030_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::THUNDERSPIRIT:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"ThunderSpirit02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 33, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 34, 67, false);
		createdActorAni->CreateAnimation(L"BORN", 68, 113, false);
		createdActorAni->CreateAnimation(L"DEATH", 114, 203, false);
		createdActorAni->CreateAnimation(L"DIZZY", 204, 248, true);
		createdActorAni->CreateAnimation(L"IDLE", 249, 301, true);
		createdActorAni->CreateAnimation(L"JUMP", 302, 357, false);
		createdActorAni->CreateAnimation(L"RUN", 358, 402, true);
		createdActorAni->CreateAnimation(L"SKILL", 403, 468, false);
		createdActorAni->CreateAnimation(L"VICTORY", 469, 549, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<ThunderSpirit> moveLogic = createdActor->AddComponent<ThunderSpirit>();
		moveLogic->SetName(L"ThunderSpirit01");
		moveLogic->SetRange(3);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(750, 55, 10.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(12);		Sound::Play(L"h029_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::TOENAILDRUID:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 0.8f, 0.8f, 0.8f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"ToenailDruid02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 35, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 36, 71, false);
		createdActorAni->CreateAnimation(L"BORN", 72, 117, false);
		createdActorAni->CreateAnimation(L"DEATH", 118, 182, false);
		createdActorAni->CreateAnimation(L"DIZZY", 183, 223, true);
		createdActorAni->CreateAnimation(L"IDLE", 224, 264, true);
		createdActorAni->CreateAnimation(L"JUMP", 265, 315, false);
		createdActorAni->CreateAnimation(L"RUN", 316, 332, true);
		createdActorAni->CreateAnimation(L"SKILL", 333, 379, false);
		createdActorAni->CreateAnimation(L"VICTORY", 380, 510, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<ToenailDruid> moveLogic = createdActor->AddComponent<ToenailDruid>();
		moveLogic->SetName(L"ToenailDruid01");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(800, 55, 60.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(13);		Sound::Play(L"h050_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::SIREN:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"Siren02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 35, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 36, 76, false);
		createdActorAni->CreateAnimation(L"BORN", 77, 132, false);
		createdActorAni->CreateAnimation(L"DEATH", 133, 192, false);
		createdActorAni->CreateAnimation(L"DIZZY", 193, 233, true);
		createdActorAni->CreateAnimation(L"IDLE", 234, 274, true);
		createdActorAni->CreateAnimation(L"JUMP", 275, 330, false);
		createdActorAni->CreateAnimation(L"RUN", 331, 349, true);
		createdActorAni->CreateAnimation(L"SKILL", 350, 410, false);
		createdActorAni->CreateAnimation(L"VICTORY", 411, 496, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<Siren> moveLogic = createdActor->AddComponent<Siren>();
		moveLogic->SetName(L"Siren01");
		moveLogic->SetRange(4);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(750, 60, 20.0f, 3.0f, _Idx);
		stateLogic->SetNUMBER(14);		Sound::Play(L"h048_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::PIRATECAPTAIN:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"PirateCaptain02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 40, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 41, 86, false);
		createdActorAni->CreateAnimation(L"BORN", 87, 132, false);
		createdActorAni->CreateAnimation(L"DEATH", 133, 203, false);
		createdActorAni->CreateAnimation(L"DIZZY", 204, 264, true);
		createdActorAni->CreateAnimation(L"IDLE", 265, 305, true);
		createdActorAni->CreateAnimation(L"JUMP", 306, 349, false);
		createdActorAni->CreateAnimation(L"RUN", 350, 372, true);
		createdActorAni->CreateAnimation(L"SKILL", 373, 413, false);
		createdActorAni->CreateAnimation(L"VICTORY", 414, 519, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<PirateCaptain> moveLogic = createdActor->AddComponent<PirateCaptain>();
		moveLogic->SetName(L"PirateCaptain01");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(950, 82, 15.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(15);		Sound::Play(L"h041_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::TORTOLANELDER:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"TortolanElder02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 51, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 52, 103, false);
		createdActorAni->CreateAnimation(L"BORN", 104, 159, false);
		createdActorAni->CreateAnimation(L"DEATH", 160, 294, false);
		createdActorAni->CreateAnimation(L"DIZZY", 295, 340, true);
		createdActorAni->CreateAnimation(L"IDLE", 341, 386, true);
		createdActorAni->CreateAnimation(L"JUMP", 387, 442, false);
		createdActorAni->CreateAnimation(L"RUN", 443, 491, true);
		createdActorAni->CreateAnimation(L"SKILL", 492, 602, false);
		createdActorAni->CreateAnimation(L"VICTORY", 603, 703, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<TortolanElder> moveLogic = createdActor->AddComponent<TortolanElder>();
		moveLogic->SetName(L"TortolanElder01");
		moveLogic->SetRange(4);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(650, 42, 10.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(16);		Sound::Play(L"h043_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::DOOMARBITER:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"DoomArbiter02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 40, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 41, 80, false);
		createdActorAni->CreateAnimation(L"BORN", 81, 161, false);
		createdActorAni->CreateAnimation(L"DEATH", 162, 251, false);
		createdActorAni->CreateAnimation(L"DIZZY", 252, 309, true);
		createdActorAni->CreateAnimation(L"IDLE", 310, 350, true);
		createdActorAni->CreateAnimation(L"JUMP", 351, 397, false);
		createdActorAni->CreateAnimation(L"RUN", 398, 424, true);
		createdActorAni->CreateAnimation(L"SKILL", 425, 495, false);
		createdActorAni->CreateAnimation(L"VICTORY", 496, 598, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<DoomArbiter> moveLogic = createdActor->AddComponent<DoomArbiter>();
		moveLogic->SetName(L"DoomArbiter01");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(950, 115, 10.0f, 15.0f, _Idx);
		stateLogic->SetNUMBER(17);		Sound::Play(L"h040_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::TIDEHUNTER:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.2f, 1.2f, 1.2f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"TideHunter02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 37, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 38, 75, false);
		createdActorAni->CreateAnimation(L"BORN", 76, 132, false);
		createdActorAni->CreateAnimation(L"DEATH", 133, 191, false);
		createdActorAni->CreateAnimation(L"DIZZY", 192, 232, true);
		createdActorAni->CreateAnimation(L"IDLE", 233, 273, true);
		createdActorAni->CreateAnimation(L"JUMP", 274, 321, false);
		createdActorAni->CreateAnimation(L"RUN", 322, 346, true);
		createdActorAni->CreateAnimation(L"SKILL", 347, 390, false);
		createdActorAni->CreateAnimation(L"VICTORY", 391, 447, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<TideHunter> moveLogic = createdActor->AddComponent<TideHunter>();
		moveLogic->SetName(L"TideHunter01");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(950, 50, 30.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(18);		Sound::Play(L"h053_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::EGG:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"Egg02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 36, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 37, 73, false);
		createdActorAni->CreateAnimation(L"BORN", 74, 119, false);
		createdActorAni->CreateAnimation(L"DEATH", 120, 152, false);
		createdActorAni->CreateAnimation(L"DIZZY", 153, 213, true);
		createdActorAni->CreateAnimation(L"IDLE", 214, 254, true);
		createdActorAni->CreateAnimation(L"JUMP", 255, 292, false);
		createdActorAni->CreateAnimation(L"RUN", 293, 309, true);
		createdActorAni->CreateAnimation(L"VICTORY", 351, 444, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<Egg> moveLogic = createdActor->AddComponent<Egg>();
		moveLogic->SetName(L"Egg01");
		moveLogic->SetRange(3);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(700, 60, 0.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(19);		Sound::Play(L"h056_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::GODOFTHUNDER:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"GodofThunder02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 40, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 41, 81, false);
		createdActorAni->CreateAnimation(L"BORN", 82, 127, false);
		createdActorAni->CreateAnimation(L"DEATH", 128, 211, false);
		createdActorAni->CreateAnimation(L"DIZZY", 212, 262, true);
		createdActorAni->CreateAnimation(L"IDLE", 263, 317, true);
		createdActorAni->CreateAnimation(L"JUMP", 318, 374, false);
		createdActorAni->CreateAnimation(L"RUN", 375, 401, true);
		createdActorAni->CreateAnimation(L"SKILL", 402, 438, false);
		createdActorAni->CreateAnimation(L"VICTORY", 439, 564, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<GodofThunder> moveLogic = createdActor->AddComponent<GodofThunder>();
		moveLogic->SetName(L"GodofThunder01");
		moveLogic->SetRange(3);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(950, 60, 15.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(20);		Sound::Play(L"h060_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::SHADOWDEMON:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0F, 1.0F, 1.0F });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"ShadowDemon02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 37, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 38, 77, false);
		createdActorAni->CreateAnimation(L"BORN", 78, 123, false);
		createdActorAni->CreateAnimation(L"DEATH", 124, 197, false);
		createdActorAni->CreateAnimation(L"DIZZY", 198, 260, true);
		createdActorAni->CreateAnimation(L"IDLE", 261, 323, true);
		createdActorAni->CreateAnimation(L"JUMP", 324, 369, false);
		createdActorAni->CreateAnimation(L"RUN", 370, 400, true);
		createdActorAni->CreateAnimation(L"SKILL", 401, 489, false);
		createdActorAni->CreateAnimation(L"VICTORY", 490, 582, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<ShadowDemon> moveLogic = createdActor->AddComponent<ShadowDemon>();
		moveLogic->SetName(L"ShadowDemon01");
		moveLogic->SetRange(4);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(450, 80, 15.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(21);		Sound::Play(L"h037_showup_03_eng.wav");	}
	break;
	case PIECEKINDS::WALRUSCHAMP:
	{
		createdActor = curScene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(3);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"WalrusChamp02.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01", 0, 34, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 35, 65, false);
		createdActorAni->CreateAnimation(L"BORN", 66, 106, false);
		createdActorAni->CreateAnimation(L"DEATH", 107, 197, false);
		createdActorAni->CreateAnimation(L"DIZZY", 198, 258, true);
		createdActorAni->CreateAnimation(L"IDLE", 259, 309, true);
		createdActorAni->CreateAnimation(L"JUMP", 310, 370, false);
		createdActorAni->CreateAnimation(L"RUN", 371, 397, true);
		createdActorAni->CreateAnimation(L"SKILL", 398, 448, false);
		createdActorAni->CreateAnimation(L"VICTORY", 449, 519, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<WalrusChamp> moveLogic = createdActor->AddComponent<WalrusChamp>();
		moveLogic->SetName(L"WalrusChamp01");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(650, 52, 10.0f, 0.0f, _Idx);
		stateLogic->SetNUMBER(0);		Sound::Play(L"h004_showup_03_eng.wav");	}
	break;
	}
	
	//ChessBoard::Inst().PushAllyPiece(createdActor);


	return createdActor;
}

void ChessBoard::MapInfoInit()
{
	m_AllyBegin = m_AllyList.begin();
	m_AllyEnd = m_AllyList.end();
	for (; m_AllyBegin != m_AllyEnd; ++m_AllyBegin)
	{
		Vector4 Pos = (*m_AllyBegin)->GetTransform()->GetLocalPosition();
		PieceLogic::PushPieceInfo(Pos);
	}

	m_EnemyBegin = m_EnemyList.begin();
	m_EnemyEnd = m_EnemyList.end();
	for (; m_EnemyBegin != m_EnemyEnd; ++m_EnemyBegin)
	{
		Vector4 Pos = (*m_EnemyBegin)->GetTransform()->GetLocalPosition();
		PieceLogic::PushMonsterInfo(Pos);
	}
}