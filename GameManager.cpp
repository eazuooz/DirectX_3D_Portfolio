#include "GameManager.h"
#include <2DANICOM.h>
#include <JTIME.h>
#include <SCENE.h>
#include <3DANIRENDER.h>

#include "LOGICHEADER.h"
#include "ChessBoard.h"
#include "MonsterLogic.h"
#include "MonsterProjectLogic.h"
#include "StatusLogic.h"
#include <SOUND.h>
#include <GAMEWIN.h>



GameManager GameManager::m_inst = GameManager();

GameManager::GameManager() : state(MGRSTATE::WAIT), stayTime(0.0f), gameTIme(0.0f), number(0), numberOfTen(0), curTime(0)
, warRound(0), isWarStart(false), bossTime(0.0f), bossOn(false)
{

}


GameManager::~GameManager()
{

}


void GameManager::Init(Scene* _Scene)
{
	curScene = _Scene;

	actor = curScene->CreateActor(L"GameMGR");
	actor->GetTransform()->SetPosition({ -1.5f,4.2f, -98.5f });
	actor->GetTransform()->SetLocalScale({ 0.7f, 0.6f, 0.7f });

	renderer1 = actor->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
	renderer1->SetSprite(L"NumberSprite.png", 0);
	renderer1->RenderData(L"DTEXMESH", L"UISPRITEPATH");
	renderer1->LinkCB(L"UIDATA", &(renderer1->GetUIData()));
	renderer1->LinkCB(L"VS_UICUTDATA", &(renderer1->GetCutData()));
	renderer1->LinkTexture(L"PS_MAPTEX", L"NumberSprite.png");
	renderer1->LinkSampler(L"PS_LSMP", L"LSMP");
	renderer1->LinkSampler(L"PS_PSMP", L"PSMP");

	renderer2 = actor->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
	renderer2->SetSprite(L"NumberSprite.png", 0);
	renderer2->RenderData(L"DTEXMESH", L"UISPRITEPATH");
	renderer2->LinkCB(L"UIDATA", &(renderer2->GetUIData()));
	renderer2->LinkCB(L"VS_UICUTDATA", &(renderer2->GetCutData()));
	renderer2->LinkTexture(L"PS_MAPTEX", L"NumberSprite.png");
	renderer2->LinkSampler(L"PS_LSMP", L"LSMP");
	renderer2->LinkSampler(L"PS_PSMP", L"PSMP");
	renderer2->LocalTranslate(Vector4(0.6f, 0.0f, 0.0f, 0.0f));



	
}



void GameManager::Update()
{
	UpdateTimeTable();

	if (isWarStart == true)
	{
		switch (warRound)
		{
#pragma region ROUND1
		case 1:
		{
			
			HPTR<Actor> Monster01 = curScene->CreateActor();
			Monster01->GetTransform()->SetLoclaPosition({ 186.0f, 0.0f, 186.0F });
			Monster01->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });


			HPTR<FbxAnimation> Monster01Ani = Monster01->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster01Ani->FbxSetting(L"mon01.FBXDATA");
			Monster01Ani->CreateAnimation(L"ATTACK01", 0, 33, false);
			Monster01Ani->CreateAnimation(L"ATTACK02", 34, 70, false);
			Monster01Ani->CreateAnimation(L"BORN", 71, 116, false);
			Monster01Ani->CreateAnimation(L"DEATH", 117, 168, false);
			Monster01Ani->CreateAnimation(L"DIZZY", 169, 209, true);
			Monster01Ani->CreateAnimation(L"IDLE", 210, 250, true);
			Monster01Ani->CreateAnimation(L"JUMP", 251, 307, 1.2f, false);
			Monster01Ani->CreateAnimation(L"RUN", 308, 332, true);
			Monster01Ani->CreateAnimation(L"VICTORY", 333, 413, true);
			Monster01Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster01Logic = Monster01->AddComponent<MonsterLogic>();
			Monster01Logic->CalculateIndex(Monster01->GetTransform()->GetLocalPosition());
			Monster01Logic->SetName(L"MONSTER01");
			Monster01Logic->SetRange(1);

			HPTR<StatusLogic> Monster01StatLogic = Monster01->AddComponent<StatusLogic>();
			Monster01StatLogic->DefMonster();
			Monster01StatLogic->SetStatus(200, 10, 0.0f, 0.0f);
			ChessBoard::Inst().PushEnemyPiece(Monster01);

			HPTR<Actor> Monster02 = curScene->CreateActor();
			Monster02->GetTransform()->SetLoclaPosition({ -186.0f, 0.0f, 186.0F });
			Monster02->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
			ChessBoard::Inst().PushEnemyPiece(Monster02);

			HPTR<FbxAnimation> Monster02Ani = Monster02->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster02Ani->FbxSetting(L"mon01.FBXDATA");
			Monster02Ani->CreateAnimation(L"ATTACK01", 0, 33, false);
			Monster02Ani->CreateAnimation(L"ATTACK02", 34, 70, false);
			Monster02Ani->CreateAnimation(L"BORN", 71, 116, false);
			Monster02Ani->CreateAnimation(L"DEATH", 117, 168, false);
			Monster02Ani->CreateAnimation(L"DIZZY", 169, 209, true);
			Monster02Ani->CreateAnimation(L"IDLE", 210, 250, true);
			Monster02Ani->CreateAnimation(L"JUMP", 251, 307, 1.2f, false);
			Monster02Ani->CreateAnimation(L"RUN", 308, 332, true);
			Monster02Ani->CreateAnimation(L"VICTORY", 333, 413, true);
			Monster02Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster02Logic = Monster02->AddComponent<MonsterLogic>();
			Monster02Logic->CalculateIndex(Monster02->GetTransform()->GetLocalPosition());
			Monster02Logic->SetName(L"Monster02");
			Monster02Logic->SetRange(1);

			HPTR<StatusLogic> Monster02StatLogic = Monster02->AddComponent<StatusLogic>();
			Monster02StatLogic->DefMonster();
			Monster02StatLogic->SetStatus(200, 10, 0.0f, 0.0f);

			HPTR<Actor> Monster03 = curScene->CreateActor();
			Monster03->GetTransform()->SetLoclaPosition({ -310.0f, 0.0f, 310.0F });
			Monster03->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
			ChessBoard::Inst().PushEnemyPiece(Monster03);

			HPTR<FbxAnimation> Monster03Ani = Monster03->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster03Ani->FbxSetting(L"mon03.FBXDATA");
			Monster03Ani->CreateAnimation(L"ATTACK01", 0, 30, false);
			Monster03Ani->CreateAnimation(L"ATTACK02", 31, 61, false);
			Monster03Ani->CreateAnimation(L"BORN", 62, 102, false);
			Monster03Ani->CreateAnimation(L"DEATH", 103, 177, false);
			Monster03Ani->CreateAnimation(L"DIZZY", 178, 218, true);
			Monster03Ani->CreateAnimation(L"IDLE", 219, 259, true);
			Monster03Ani->CreateAnimation(L"JUMP", 260, 326, 1.2f, false);
			Monster03Ani->CreateAnimation(L"RUN", 327, 347, true);
			Monster03Ani->CreateAnimation(L"VICTORY", 348, 437, true);
			Monster03Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterProjectLogic> Monster03Logic = Monster03->AddComponent<MonsterProjectLogic>();
			Monster03Logic->CalculateIndex(Monster03->GetTransform()->GetLocalPosition());
			Monster03Logic->SetName(L"Monster03");
			Monster03Logic->SetRange(4);

			HPTR<StatusLogic> Monster03StatLogic = Monster03->AddComponent<StatusLogic>();
			Monster03StatLogic->SetStatus(200, 10, 0.0f, 0.0f);
			Monster03StatLogic->DefMonster();

			HPTR<Actor> Monster04 = curScene->CreateActor();
			Monster04->GetTransform()->SetLoclaPosition({ 310.0f, 0.0f, 310.0F });
			Monster04->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
			ChessBoard::Inst().PushEnemyPiece(Monster04);

			HPTR<FbxAnimation> Monster04Ani = Monster04->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster04Ani->FbxSetting(L"mon03.FBXDATA");
			Monster04Ani->CreateAnimation(L"ATTACK01", 0, 30, false);
			Monster04Ani->CreateAnimation(L"ATTACK02", 31, 61, false);
			Monster04Ani->CreateAnimation(L"BORN", 62, 102, false);
			Monster04Ani->CreateAnimation(L"DEATH", 103, 177, false);
			Monster04Ani->CreateAnimation(L"DIZZY", 178, 218, true);
			Monster04Ani->CreateAnimation(L"IDLE", 219, 259, true);
			Monster04Ani->CreateAnimation(L"JUMP", 260, 326, 1.2f, false);
			Monster04Ani->CreateAnimation(L"RUN", 327, 347, true);
			Monster04Ani->CreateAnimation(L"VICTORY", 348, 437, true);
			Monster04Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterProjectLogic> Monster04Logic = Monster04->AddComponent<MonsterProjectLogic>();
			Monster04Logic->CalculateIndex(Monster04->GetTransform()->GetLocalPosition());
			Monster04Logic->SetName(L"Monster04");
			Monster04Logic->SetRange(4);

			HPTR<StatusLogic> Monster04StatLogic = Monster04->AddComponent<StatusLogic>();
			Monster04StatLogic->DefMonster();
			Monster04StatLogic->SetStatus(200, 10, 0.0f, 0.0f);

			///
			HPTR<Actor> Monster05 = curScene->CreateActor();
			Monster05->GetTransform()->SetLoclaPosition({ -62.0f, 0.0f, 62.0F });
			Monster05->GetTransform()->SetLocalScale({ 1.3F, 1.3F, 1.3F });
			ChessBoard::Inst().PushEnemyPiece(Monster05);

			HPTR<FbxAnimation> Monster05Ani = Monster05->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster05Ani->FbxSetting(L"mon02.FBXDATA");
			Monster05Ani->CreateAnimation(L"ATTACK01", 0, 30, false);
			Monster05Ani->CreateAnimation(L"ATTACK02", 31, 61, false);
			Monster05Ani->CreateAnimation(L"BORN", 62, 142, false);
			Monster05Ani->CreateAnimation(L"DEATH", 143, 183, false);
			Monster05Ani->CreateAnimation(L"DIZZY", 184, 254, true);
			Monster05Ani->CreateAnimation(L"IDLE", 255, 305, true);
			Monster05Ani->CreateAnimation(L"JUMP", 306, 351, 1.2f, false);
			Monster05Ani->CreateAnimation(L"RUN", 352, 372, true);
			Monster05Ani->CreateAnimation(L"VICTORY", 373, 425, true);
			Monster05Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster05Logic = Monster05->AddComponent<MonsterLogic>();
			Monster05Logic->CalculateIndex(Monster05->GetTransform()->GetLocalPosition());
			Monster05Logic->SetName(L"Monster05");
			Monster05Logic->SetRange(1);

			HPTR<StatusLogic> Monster05StatLogic = Monster05->AddComponent<StatusLogic>();
			Monster05StatLogic->DefMonster();
			Monster05StatLogic->SetStatus(200, 10, 0.0f, 0.0f);

			HPTR<Actor> Monster06 = curScene->CreateActor();
			Monster06->GetTransform()->SetLoclaPosition({ 62.0f, 0.0f, 62.0F });
			Monster06->GetTransform()->SetLocalScale({ 1.3F, 1.3F, 1.3F });
			ChessBoard::Inst().PushEnemyPiece(Monster06);

			HPTR<FbxAnimation> Monster06Ani = Monster06->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster06Ani->FbxSetting(L"mon02.FBXDATA");
			Monster06Ani->CreateAnimation(L"ATTACK01", 0, 30, false);
			Monster06Ani->CreateAnimation(L"ATTACK02", 31, 61, false);
			Monster06Ani->CreateAnimation(L"BORN", 62, 142, false);
			Monster06Ani->CreateAnimation(L"DEATH", 143, 183, false);
			Monster06Ani->CreateAnimation(L"DIZZY", 184, 254, true);
			Monster06Ani->CreateAnimation(L"IDLE", 255, 305, true);
			Monster06Ani->CreateAnimation(L"JUMP", 306, 351, 1.2f, false);
			Monster06Ani->CreateAnimation(L"RUN", 352, 372, true);
			Monster06Ani->CreateAnimation(L"VICTORY", 373, 425, true);
			Monster06Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster06Logic = Monster06->AddComponent<MonsterLogic>();
			Monster06Logic->CalculateIndex(Monster06->GetTransform()->GetLocalPosition());
			Monster06Logic->SetName(L"Monster06");
			Monster06Logic->SetRange(1);

			HPTR<StatusLogic> Monster06StatLogic = Monster06->AddComponent<StatusLogic>();
			Monster06StatLogic->DefMonster();
			Monster06StatLogic->SetStatus(200, 10, 0.0f, 0.0f);
		}
		break;
#pragma endregion

#pragma region ROUND2
		case 2:
		{
			HPTR<Actor> Monster01 = curScene->CreateActor();
			Monster01->GetTransform()->SetLoclaPosition({ 186.0f, 0.0f, 186.0F });
			Monster01->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });

			HPTR<FbxAnimation> Monster01Ani = Monster01->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster01Ani->FbxSetting(L"mon04.FBXDATA");
			Monster01Ani->CreateAnimation(L"ATTACK01", 0, 53, false);
			Monster01Ani->CreateAnimation(L"ATTACK02", 54, 107, false);
			Monster01Ani->CreateAnimation(L"BORN", 108, 153, false);
			Monster01Ani->CreateAnimation(L"DEATH", 154, 187, false);
			Monster01Ani->CreateAnimation(L"DIZZY", 188, 248, true);
			Monster01Ani->CreateAnimation(L"IDLE", 249, 309, true);
			Monster01Ani->CreateAnimation(L"JUMP", 310, 375, 1.2f, false);
			Monster01Ani->CreateAnimation(L"RUN", 376, 397, true);
			Monster01Ani->CreateAnimation(L"VICTORY", 398, 488, true);
			Monster01Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster01Logic = Monster01->AddComponent<MonsterLogic>();
			Monster01Logic->CalculateIndex(Monster01->GetTransform()->GetLocalPosition());
			Monster01Logic->SetName(L"MONSTER01");
			Monster01Logic->SetRange(1);

			HPTR<StatusLogic> Monster01StatLogic = Monster01->AddComponent<StatusLogic>();
			Monster01StatLogic->DefMonster();
			Monster01StatLogic->SetStatus(300, 10, 0.0f, 0.0f);
			ChessBoard::Inst().PushEnemyPiece(Monster01);

			HPTR<Actor> Monster02 = curScene->CreateActor();
			Monster02->GetTransform()->SetLoclaPosition({ -186.0f, 0.0f, 186.0F });
			Monster02->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
			ChessBoard::Inst().PushEnemyPiece(Monster02);

			HPTR<FbxAnimation> Monster02Ani = Monster02->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster02Ani->FbxSetting(L"mon05.FBXDATA");
			
			Monster02Ani->CreateAnimation(L"ATTACK01", 0, 53, false);
			Monster02Ani->CreateAnimation(L"ATTACK02", 54, 107, false);
			Monster02Ani->CreateAnimation(L"BORN", 108, 153, false);
			Monster02Ani->CreateAnimation(L"DEATH", 154, 187, false);
			Monster02Ani->CreateAnimation(L"DIZZY", 188, 248, true);
			Monster02Ani->CreateAnimation(L"IDLE", 249, 309, true);
			Monster02Ani->CreateAnimation(L"JUMP", 310, 375, 1.2f, false);
			Monster02Ani->CreateAnimation(L"RUN", 376, 397, true);
			Monster02Ani->CreateAnimation(L"VICTORY", 398, 488, true);

			Monster02Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster02Logic = Monster02->AddComponent<MonsterLogic>();
			Monster02Logic->CalculateIndex(Monster02->GetTransform()->GetLocalPosition());
			Monster02Logic->SetName(L"Monster02");
			Monster02Logic->SetRange(1);

			HPTR<StatusLogic> Monster02StatLogic = Monster02->AddComponent<StatusLogic>();
			Monster02StatLogic->DefMonster();
			Monster02StatLogic->SetStatus(300, 10, 0.0f, 0.0f);

			HPTR<Actor> Monster03 = curScene->CreateActor();
			Monster03->GetTransform()->SetLoclaPosition({ -310.0f, 0.0f, 310.0F });
			Monster03->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
			ChessBoard::Inst().PushEnemyPiece(Monster03);

			HPTR<FbxAnimation> Monster03Ani = Monster03->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster03Ani->FbxSetting(L"mon06.FBXDATA");
			Monster03Ani->CreateAnimation(L"ATTACK01", 0, 37, false);
			Monster03Ani->CreateAnimation(L"ATTACK02", 38, 75, false);
			Monster03Ani->CreateAnimation(L"BORN", 76, 125, false);
			Monster03Ani->CreateAnimation(L"DEATH", 126, 196, false);
			Monster03Ani->CreateAnimation(L"DIZZY", 197, 237, true);
			Monster03Ani->CreateAnimation(L"IDLE", 238, 278, true);
			Monster03Ani->CreateAnimation(L"JUMP", 279, 324, 1.2f, false);
			Monster03Ani->CreateAnimation(L"RUN", 325, 345, true);
			Monster03Ani->CreateAnimation(L"VICTORY", 346, 423, true);
			Monster03Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster03Logic = Monster03->AddComponent<MonsterLogic>();
			Monster03Logic->CalculateIndex(Monster03->GetTransform()->GetLocalPosition());
			Monster03Logic->SetName(L"Monster03");
			Monster03Logic->SetRange(1);

			HPTR<StatusLogic> Monster03StatLogic = Monster03->AddComponent<StatusLogic>();
			Monster03StatLogic->DefMonster();
			Monster03StatLogic->SetStatus(300, 10, 0.0f, 0.0f);

			HPTR<Actor> Monster04 = curScene->CreateActor();
			Monster04->GetTransform()->SetLoclaPosition({ 310.0f, 0.0f, 310.0F });
			Monster04->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
			ChessBoard::Inst().PushEnemyPiece(Monster04);

			HPTR<FbxAnimation> Monster04Ani = Monster04->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster04Ani->FbxSetting(L"mon06.FBXDATA");
			Monster04Ani->CreateAnimation(L"ATTACK01", 0, 37, false);
			Monster04Ani->CreateAnimation(L"ATTACK02", 38, 75, false);
			Monster04Ani->CreateAnimation(L"BORN", 76, 125, false);
			Monster04Ani->CreateAnimation(L"DEATH", 126, 196, false);
			Monster04Ani->CreateAnimation(L"DIZZY", 197, 237, true);
			Monster04Ani->CreateAnimation(L"IDLE", 238, 278, true);
			Monster04Ani->CreateAnimation(L"JUMP", 279, 324, 1.2f, false);
			Monster04Ani->CreateAnimation(L"RUN", 325, 345, true);
			Monster04Ani->CreateAnimation(L"VICTORY", 346, 423, true);
			Monster04Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster04Logic = Monster04->AddComponent<MonsterLogic>();
			Monster04Logic->CalculateIndex(Monster04->GetTransform()->GetLocalPosition());
			Monster04Logic->SetName(L"Monster04");
			Monster04Logic->SetRange(1);

			HPTR<StatusLogic> Monster04StatLogic = Monster04->AddComponent<StatusLogic>();
			Monster04StatLogic->DefMonster();
			Monster04StatLogic->SetStatus(300, 10, 0.0f, 0.0f);

			///
			HPTR<Actor> Monster05 = curScene->CreateActor();
			Monster05->GetTransform()->SetLoclaPosition({ -62.0f, 0.0f, 62.0F });
			Monster05->GetTransform()->SetLocalScale({ 1.3F, 1.3F, 1.3F });
			ChessBoard::Inst().PushEnemyPiece(Monster05);

			HPTR<FbxAnimation> Monster05Ani = Monster05->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster05Ani->FbxSetting(L"mon07.FBXDATA");
			Monster05Ani->CreateAnimation(L"ATTACK01", 0, 37, false);
			Monster05Ani->CreateAnimation(L"ATTACK02", 38, 75, false);
			Monster05Ani->CreateAnimation(L"BORN", 76, 125, false);
			Monster05Ani->CreateAnimation(L"DEATH", 126, 196, false);
			Monster05Ani->CreateAnimation(L"DIZZY", 197, 237, true);
			Monster05Ani->CreateAnimation(L"IDLE", 238, 278, true);
			Monster05Ani->CreateAnimation(L"JUMP", 279, 324, 1.2f, false);
			Monster05Ani->CreateAnimation(L"RUN", 325, 345, true);
			Monster05Ani->CreateAnimation(L"VICTORY", 346, 423, true);
			Monster05Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster05Logic = Monster05->AddComponent<MonsterLogic>();
			Monster05Logic->CalculateIndex(Monster05->GetTransform()->GetLocalPosition());
			Monster05Logic->SetName(L"Monster05");
			Monster05Logic->SetRange(1);

			HPTR<StatusLogic> Monster05StatLogic = Monster05->AddComponent<StatusLogic>();
			Monster05StatLogic->DefMonster();
			Monster05StatLogic->SetStatus(300, 10, 0.0f, 0.0f);

			HPTR<Actor> Monster06 = curScene->CreateActor();
			Monster06->GetTransform()->SetLoclaPosition({ 62.0f, 0.0f, 62.0F });
			Monster06->GetTransform()->SetLocalScale({ 1.3F, 1.3F, 1.3F });
			ChessBoard::Inst().PushEnemyPiece(Monster06);

			HPTR<FbxAnimation> Monster06Ani = Monster06->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster06Ani->FbxSetting(L"mon07.FBXDATA");
			Monster06Ani->CreateAnimation(L"ATTACK01", 0, 37, false);
			Monster06Ani->CreateAnimation(L"ATTACK02", 38, 75, false);
			Monster06Ani->CreateAnimation(L"BORN", 76, 125, false);
			Monster06Ani->CreateAnimation(L"DEATH", 126, 196, false);
			Monster06Ani->CreateAnimation(L"DIZZY", 197, 237, true);
			Monster06Ani->CreateAnimation(L"IDLE", 238, 278, true);
			Monster06Ani->CreateAnimation(L"JUMP", 279, 324, 1.2f, false);
			Monster06Ani->CreateAnimation(L"RUN", 325, 345, true);
			Monster06Ani->CreateAnimation(L"VICTORY", 346, 423, true);
			Monster06Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster06Logic = Monster06->AddComponent<MonsterLogic>();
			Monster06Logic->CalculateIndex(Monster06->GetTransform()->GetLocalPosition());
			Monster06Logic->SetName(L"Monster06");
			Monster06Logic->SetRange(1);

			HPTR<StatusLogic> Monster06StatLogic = Monster06->AddComponent<StatusLogic>();
			Monster06StatLogic->DefMonster();
			Monster06StatLogic->SetStatus(300, 10, 0.0f, 0.0f);
		}
		break;
#pragma endregion

#pragma region ROUND3
		case 3:
		{
			HPTR<Actor> Monster01 = curScene->CreateActor();
			Monster01->GetTransform()->SetLoclaPosition({ 186.0f, 0.0f, 186.0F });
			Monster01->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });


			HPTR<FbxAnimation> Monster01Ani = Monster01->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster01Ani->FbxSetting(L"mon08.FBXDATA");
			Monster01Ani->CreateAnimation(L"ATTACK01", 0, 33, false);
			Monster01Ani->CreateAnimation(L"ATTACK02", 34, 69, false);
			Monster01Ani->CreateAnimation(L"BORN", 70, 113, false);
			Monster01Ani->CreateAnimation(L"DEATH", 114, 161, false);
			Monster01Ani->CreateAnimation(L"DIZZY", 162, 202, true);
			Monster01Ani->CreateAnimation(L"IDLE", 203, 263, true);
			Monster01Ani->CreateAnimation(L"JUMP", 264, 303, 1.2f, false);
			Monster01Ani->CreateAnimation(L"RUN", 304, 328, true);
			Monster01Ani->CreateAnimation(L"VICTORY", 329, 383, true);
			Monster01Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster01Logic = Monster01->AddComponent<MonsterLogic>();
			Monster01Logic->CalculateIndex(Monster01->GetTransform()->GetLocalPosition());
			Monster01Logic->SetName(L"MONSTER01");
			Monster01Logic->SetRange(1);

			HPTR<StatusLogic> Monster01StatLogic = Monster01->AddComponent<StatusLogic>();
			Monster01StatLogic->DefMonster();
			Monster01StatLogic->SetStatus(400, 10, 0.0f, 0.0f);
			ChessBoard::Inst().PushEnemyPiece(Monster01);

			HPTR<Actor> Monster02 = curScene->CreateActor();
			Monster02->GetTransform()->SetLoclaPosition({ -186.0f, 0.0f, 186.0F });
			Monster02->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
			ChessBoard::Inst().PushEnemyPiece(Monster02);

			HPTR<FbxAnimation> Monster02Ani = Monster02->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster02Ani->FbxSetting(L"mon09.FBXDATA");
			Monster02Ani->CreateAnimation(L"ATTACK01", 0, 33, false);
			Monster02Ani->CreateAnimation(L"ATTACK02", 34, 69, false);
			Monster02Ani->CreateAnimation(L"BORN", 70, 113, false);
			Monster02Ani->CreateAnimation(L"DEATH", 114, 161, false);
			Monster02Ani->CreateAnimation(L"DIZZY", 162, 202, true);
			Monster02Ani->CreateAnimation(L"IDLE", 203, 263, true);
			Monster02Ani->CreateAnimation(L"JUMP", 264, 303, 1.2f, false);
			Monster02Ani->CreateAnimation(L"RUN", 304, 328, true);
			Monster02Ani->CreateAnimation(L"VICTORY", 329, 383, true);
			Monster02Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster02Logic = Monster02->AddComponent<MonsterLogic>();
			Monster02Logic->CalculateIndex(Monster02->GetTransform()->GetLocalPosition());
			Monster02Logic->SetName(L"Monster02");
			Monster02Logic->SetRange(1);

			HPTR<StatusLogic> Monster02StatLogic = Monster02->AddComponent<StatusLogic>();
			Monster02StatLogic->DefMonster();
			Monster02StatLogic->SetStatus(400, 10, 0.0f, 0.0f);

			HPTR<Actor> Monster03 = curScene->CreateActor();
			Monster03->GetTransform()->SetLoclaPosition({ -310.0f, 0.0f, 310.0F });
			Monster03->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
			ChessBoard::Inst().PushEnemyPiece(Monster03);

			HPTR<FbxAnimation> Monster03Ani = Monster03->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster03Ani->FbxSetting(L"mon10.FBXDATA");
			Monster03Ani->CreateAnimation(L"ATTACK01", 0, 36, false);
			Monster03Ani->CreateAnimation(L"ATTACK02", 37, 74, false);
			Monster03Ani->CreateAnimation(L"BORN", 75, 123, false);
			Monster03Ani->CreateAnimation(L"DEATH", 124, 182, false);
			Monster03Ani->CreateAnimation(L"DIZZY", 183, 229, true);
			Monster03Ani->CreateAnimation(L"IDLE", 230, 290, true);
			Monster03Ani->CreateAnimation(L"JUMP", 291, 330, 1.2f, false);
			Monster03Ani->CreateAnimation(L"RUN", 331, 355, true);
			Monster03Ani->CreateAnimation(L"VICTORY", 356, 417, true);
			Monster03Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster03Logic = Monster03->AddComponent<MonsterLogic>();
			Monster03Logic->CalculateIndex(Monster03->GetTransform()->GetLocalPosition());
			Monster03Logic->SetName(L"Monster03");
			Monster03Logic->SetRange(1);

			HPTR<StatusLogic> Monster03StatLogic = Monster03->AddComponent<StatusLogic>();
			Monster03StatLogic->DefMonster();
			Monster03StatLogic->SetStatus(400, 10, 0.0f, 0.0f);

			HPTR<Actor> Monster04 = curScene->CreateActor();
			Monster04->GetTransform()->SetLoclaPosition({ 310.0f, 0.0f, 310.0F });
			Monster04->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
			ChessBoard::Inst().PushEnemyPiece(Monster04);

			HPTR<FbxAnimation> Monster04Ani = Monster04->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster04Ani->FbxSetting(L"mon10.FBXDATA");
			Monster04Ani->CreateAnimation(L"ATTACK01", 0, 36, false);
			Monster04Ani->CreateAnimation(L"ATTACK02", 37, 74, false);
			Monster04Ani->CreateAnimation(L"BORN", 75, 123, false);
			Monster04Ani->CreateAnimation(L"DEATH", 124, 182, false);
			Monster04Ani->CreateAnimation(L"DIZZY", 183, 229, true);
			Monster04Ani->CreateAnimation(L"IDLE", 230, 290, true);
			Monster04Ani->CreateAnimation(L"JUMP", 291, 330, 1.2f, false);
			Monster04Ani->CreateAnimation(L"RUN", 331, 355, true);
			Monster04Ani->CreateAnimation(L"VICTORY", 356, 417, true);
			Monster04Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster04Logic = Monster04->AddComponent<MonsterLogic>();
			Monster04Logic->CalculateIndex(Monster04->GetTransform()->GetLocalPosition());
			Monster04Logic->SetName(L"Monster04");
			Monster04Logic->SetRange(1);

			HPTR<StatusLogic> Monster04StatLogic = Monster04->AddComponent<StatusLogic>();
			Monster04StatLogic->DefMonster();
			Monster04StatLogic->SetStatus(400, 10, 0.0f, 0.0f);

			///
			HPTR<Actor> Monster05 = curScene->CreateActor();
			Monster05->GetTransform()->SetLoclaPosition({ -62.0f, 0.0f, 62.0F });
			Monster05->GetTransform()->SetLocalScale({ 1.3F, 1.3F, 1.3F });
			ChessBoard::Inst().PushEnemyPiece(Monster05);

			HPTR<FbxAnimation> Monster05Ani = Monster05->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster05Ani->FbxSetting(L"mon11.FBXDATA");
			Monster05Ani->CreateAnimation(L"ATTACK01", 0, 36, false);
			Monster05Ani->CreateAnimation(L"ATTACK02", 37, 74, false);
			Monster05Ani->CreateAnimation(L"BORN", 75, 123, false);
			Monster05Ani->CreateAnimation(L"DEATH", 124, 182, false);
			Monster05Ani->CreateAnimation(L"DIZZY", 183, 229, true);
			Monster05Ani->CreateAnimation(L"IDLE", 230, 290, true);
			Monster05Ani->CreateAnimation(L"JUMP", 291, 330, 1.2f, false);
			Monster05Ani->CreateAnimation(L"RUN", 331, 355, true);
			Monster05Ani->CreateAnimation(L"VICTORY", 356, 417, true);
			Monster05Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster05Logic = Monster05->AddComponent<MonsterLogic>();
			Monster05Logic->CalculateIndex(Monster05->GetTransform()->GetLocalPosition());
			Monster05Logic->SetName(L"Monster05");
			Monster05Logic->SetRange(1);

			HPTR<StatusLogic> Monster05StatLogic = Monster05->AddComponent<StatusLogic>();
			Monster05StatLogic->DefMonster();
			Monster05StatLogic->SetStatus(400, 10, 0.0f, 0.0f);

			HPTR<Actor> Monster06 = curScene->CreateActor();
			Monster06->GetTransform()->SetLoclaPosition({ 62.0f, 0.0f, 62.0F });
			Monster06->GetTransform()->SetLocalScale({ 1.3F, 1.3F, 1.3F });
			ChessBoard::Inst().PushEnemyPiece(Monster06);

			HPTR<FbxAnimation> Monster06Ani = Monster06->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster06Ani->FbxSetting(L"mon11.FBXDATA");
			Monster06Ani->CreateAnimation(L"ATTACK01", 0, 36, false);
			Monster06Ani->CreateAnimation(L"ATTACK02", 37, 74, false);
			Monster06Ani->CreateAnimation(L"BORN", 75, 123, false);
			Monster06Ani->CreateAnimation(L"DEATH", 124, 182, false);
			Monster06Ani->CreateAnimation(L"DIZZY", 183, 229, true);
			Monster06Ani->CreateAnimation(L"IDLE", 230, 290, true);
			Monster06Ani->CreateAnimation(L"JUMP", 291, 330, 1.2f, false);
			Monster06Ani->CreateAnimation(L"RUN", 331, 355, true);
			Monster06Ani->CreateAnimation(L"VICTORY", 356, 417, true);
			Monster06Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster06Logic = Monster06->AddComponent<MonsterLogic>();
			Monster06Logic->CalculateIndex(Monster06->GetTransform()->GetLocalPosition());
			Monster06Logic->SetName(L"Monster06");
			Monster06Logic->SetRange(1);

			HPTR<StatusLogic> Monster06StatLogic = Monster06->AddComponent<StatusLogic>();
			Monster06StatLogic->DefMonster();
			Monster06StatLogic->SetStatus(400, 10, 0.0f, 0.0f);
		}
		break;
#pragma endregion

#pragma region ROUND4
		case 4:
		{
			HPTR<Actor> Monster01 = curScene->CreateActor();
			Monster01->GetTransform()->SetLoclaPosition({ 186.0f, 0.0f, 186.0F });
			Monster01->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });


			HPTR<FbxAnimation> Monster01Ani = Monster01->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster01Ani->FbxSetting(L"mon12.FBXDATA");
			Monster01Ani->CreateAnimation(L"ATTACK01", 0, 35, false);
			Monster01Ani->CreateAnimation(L"ATTACK02", 36, 71, false);
			Monster01Ani->CreateAnimation(L"BORN", 72, 117, false);
			Monster01Ani->CreateAnimation(L"DEATH", 118, 161, false);
			Monster01Ani->CreateAnimation(L"DIZZY", 162, 202, true);
			Monster01Ani->CreateAnimation(L"IDLE", 203, 243, true);
			Monster01Ani->CreateAnimation(L"JUMP", 244, 286, 1.2f, false);
			Monster01Ani->CreateAnimation(L"RUN", 287, 307, true);
			Monster01Ani->CreateAnimation(L"VICTORY", 308, 363, true);
			Monster01Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster01Logic = Monster01->AddComponent<MonsterLogic>();
			Monster01Logic->CalculateIndex(Monster01->GetTransform()->GetLocalPosition());
			Monster01Logic->SetName(L"MONSTER01");
			Monster01Logic->SetRange(1);

			HPTR<StatusLogic> Monster01StatLogic = Monster01->AddComponent<StatusLogic>();
			Monster01StatLogic->DefMonster();
			Monster01StatLogic->SetStatus(500, 10, 0.0f, 0.0f);
			ChessBoard::Inst().PushEnemyPiece(Monster01);

			HPTR<Actor> Monster02 = curScene->CreateActor();
			Monster02->GetTransform()->SetLoclaPosition({ -186.0f, 0.0f, 186.0F });
			Monster02->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
			ChessBoard::Inst().PushEnemyPiece(Monster02);

			HPTR<FbxAnimation> Monster02Ani = Monster02->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster02Ani->FbxSetting(L"mon12.FBXDATA");
			Monster02Ani->CreateAnimation(L"ATTACK01", 0, 35, false);
			Monster02Ani->CreateAnimation(L"ATTACK02", 36, 71, false);
			Monster02Ani->CreateAnimation(L"BORN", 72, 117, false);
			Monster02Ani->CreateAnimation(L"DEATH", 118, 161, false);
			Monster02Ani->CreateAnimation(L"DIZZY", 162, 202, true);
			Monster02Ani->CreateAnimation(L"IDLE", 203, 243, true);
			Monster02Ani->CreateAnimation(L"JUMP", 244, 286, 1.2f, false);
			Monster02Ani->CreateAnimation(L"RUN", 287, 307, true);
			Monster02Ani->CreateAnimation(L"VICTORY", 308, 363, true);
			Monster02Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster02Logic = Monster02->AddComponent<MonsterLogic>();
			Monster02Logic->CalculateIndex(Monster02->GetTransform()->GetLocalPosition());
			Monster02Logic->SetName(L"Monster02");
			Monster02Logic->SetRange(1);

			HPTR<StatusLogic> Monster02StatLogic = Monster02->AddComponent<StatusLogic>();
			Monster02StatLogic->DefMonster();
			Monster02StatLogic->SetStatus(500, 10, 0.0f, 0.0f);

			HPTR<Actor> Monster03 = curScene->CreateActor();
			Monster03->GetTransform()->SetLoclaPosition({ -310.0f, 0.0f, 310.0F });
			Monster03->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
			ChessBoard::Inst().PushEnemyPiece(Monster03);

			HPTR<FbxAnimation> Monster03Ani = Monster03->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster03Ani->FbxSetting(L"mon13.FBXDATA");
			Monster03Ani->CreateAnimation(L"ATTACK01", 0, 35, false);
			Monster03Ani->CreateAnimation(L"ATTACK02", 36, 71, false);
			Monster03Ani->CreateAnimation(L"BORN", 72, 117, false);
			Monster03Ani->CreateAnimation(L"DEATH", 118, 161, false);
			Monster03Ani->CreateAnimation(L"DIZZY", 162, 202, true);
			Monster03Ani->CreateAnimation(L"IDLE", 203, 243, true);
			Monster03Ani->CreateAnimation(L"JUMP", 244, 286, 1.2f, false);
			Monster03Ani->CreateAnimation(L"RUN", 287, 307, true);
			Monster03Ani->CreateAnimation(L"VICTORY", 308, 363, true);
			Monster03Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster03Logic = Monster03->AddComponent<MonsterLogic>();
			Monster03Logic->CalculateIndex(Monster03->GetTransform()->GetLocalPosition());
			Monster03Logic->SetName(L"Monster03");
			Monster03Logic->SetRange(1);

			HPTR<StatusLogic> Monster03StatLogic = Monster03->AddComponent<StatusLogic>();
			Monster03StatLogic->DefMonster();
			Monster03StatLogic->SetStatus(500, 10, 0.0f, 0.0f);

			HPTR<Actor> Monster04 = curScene->CreateActor();
			Monster04->GetTransform()->SetLoclaPosition({ 310.0f, 0.0f, 310.0F });
			Monster04->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
			ChessBoard::Inst().PushEnemyPiece(Monster04);

			HPTR<FbxAnimation> Monster04Ani = Monster04->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster04Ani->FbxSetting(L"mon13.FBXDATA");
			Monster04Ani->CreateAnimation(L"ATTACK01", 0, 35, false);
			Monster04Ani->CreateAnimation(L"ATTACK02", 36, 71, false);
			Monster04Ani->CreateAnimation(L"BORN", 72, 117, false);
			Monster04Ani->CreateAnimation(L"DEATH", 118, 161, false);
			Monster04Ani->CreateAnimation(L"DIZZY", 162, 202, true);
			Monster04Ani->CreateAnimation(L"IDLE", 203, 243, true);
			Monster04Ani->CreateAnimation(L"JUMP", 244, 286, 1.2f, false);
			Monster04Ani->CreateAnimation(L"RUN", 287, 307, true);
			Monster04Ani->CreateAnimation(L"VICTORY", 308, 363, true);
			Monster04Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster04Logic = Monster04->AddComponent<MonsterLogic>();
			Monster04Logic->CalculateIndex(Monster04->GetTransform()->GetLocalPosition());
			Monster04Logic->SetName(L"Monster04");
			Monster04Logic->SetRange(1);

			HPTR<StatusLogic> Monster04StatLogic = Monster04->AddComponent<StatusLogic>();
			Monster04StatLogic->DefMonster();
			Monster04StatLogic->SetStatus(500, 10, 0.0f, 0.0f);

			///
			HPTR<Actor> Monster05 = curScene->CreateActor();
			Monster05->GetTransform()->SetLoclaPosition({ -62.0f, 0.0f, 62.0F });
			Monster05->GetTransform()->SetLocalScale({ 1.3F, 1.3F, 1.3F });
			ChessBoard::Inst().PushEnemyPiece(Monster05);

			HPTR<FbxAnimation> Monster05Ani = Monster05->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster05Ani->FbxSetting(L"mon14.FBXDATA");
			Monster05Ani->CreateAnimation(L"ATTACK01", 0, 45, false);
			Monster05Ani->CreateAnimation(L"ATTACK02", 46, 91, false);
			Monster05Ani->CreateAnimation(L"BORN", 92, 137, false);
			Monster05Ani->CreateAnimation(L"DEATH", 138, 198, false);
			Monster05Ani->CreateAnimation(L"DIZZY", 199, 259, true);
			Monster05Ani->CreateAnimation(L"IDLE", 260, 332, true);
			Monster05Ani->CreateAnimation(L"JUMP", 333, 383, 1.2f, false);
			Monster05Ani->CreateAnimation(L"RUN", 384, 444, true);
			Monster05Ani->CreateAnimation(L"VICTORY", 445, 550, true);
			Monster05Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster05Logic = Monster05->AddComponent<MonsterLogic>();
			Monster05Logic->CalculateIndex(Monster05->GetTransform()->GetLocalPosition());
			Monster05Logic->SetName(L"Monster05");
			Monster05Logic->SetRange(1);

			HPTR<StatusLogic> Monster05StatLogic = Monster05->AddComponent<StatusLogic>();
			Monster05StatLogic->DefMonster();
			Monster05StatLogic->SetStatus(500, 10, 0.0f, 0.0f);

			HPTR<Actor> Monster06 = curScene->CreateActor();
			Monster06->GetTransform()->SetLoclaPosition({ 62.0f, 0.0f, 62.0F });
			Monster06->GetTransform()->SetLocalScale({ 1.3F, 1.3F, 1.3F });
			ChessBoard::Inst().PushEnemyPiece(Monster06);

			HPTR<FbxAnimation> Monster06Ani = Monster06->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster06Ani->FbxSetting(L"mon14.FBXDATA");
			Monster06Ani->CreateAnimation(L"ATTACK01", 0, 45, false);
			Monster06Ani->CreateAnimation(L"ATTACK02", 46, 91, false);
			Monster06Ani->CreateAnimation(L"BORN", 92, 137, false);
			Monster06Ani->CreateAnimation(L"DEATH", 138, 198, false);
			Monster06Ani->CreateAnimation(L"DIZZY", 199, 259, true);
			Monster06Ani->CreateAnimation(L"IDLE", 260, 332, true);
			Monster06Ani->CreateAnimation(L"JUMP", 333, 383, 1.2f, false);
			Monster06Ani->CreateAnimation(L"RUN", 384, 444, true);
			Monster06Ani->CreateAnimation(L"VICTORY", 445, 550, true);
			Monster06Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster06Logic = Monster06->AddComponent<MonsterLogic>();
			Monster06Logic->CalculateIndex(Monster06->GetTransform()->GetLocalPosition());
			Monster06Logic->SetName(L"Monster06");
			Monster06Logic->SetRange(1);

			HPTR<StatusLogic> Monster06StatLogic = Monster06->AddComponent<StatusLogic>();
			Monster06StatLogic->DefMonster();
			Monster06StatLogic->SetStatus(500, 10, 0.0f, 0.0f);
		}
		break;
#pragma endregion

#pragma region ROUND5
		case 5:
		{
			HPTR<Actor> Monster01 = curScene->CreateActor();
			Monster01->GetTransform()->SetLoclaPosition({ 186.0f, 0.0f, 186.0F });
			Monster01->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });


			HPTR<FbxAnimation> Monster01Ani = Monster01->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster01Ani->FbxSetting(L"mon15.FBXDATA");
			Monster01Ani->CreateAnimation(L"ATTACK01", 0, 30, false);
			Monster01Ani->CreateAnimation(L"ATTACK02", 31, 61, false);
			Monster01Ani->CreateAnimation(L"BORN", 62, 97, false);
			Monster01Ani->CreateAnimation(L"DEATH", 98, 148, false);
			Monster01Ani->CreateAnimation(L"DIZZY", 149, 189, true);
			Monster01Ani->CreateAnimation(L"IDLE", 190, 230, true);
			Monster01Ani->CreateAnimation(L"JUMP", 231, 273, 1.2f, false);
			Monster01Ani->CreateAnimation(L"RUN", 274, 296, true);
			Monster01Ani->CreateAnimation(L"VICTORY", 297, 337, true);
			Monster01Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster01Logic = Monster01->AddComponent<MonsterLogic>();
			Monster01Logic->CalculateIndex(Monster01->GetTransform()->GetLocalPosition());
			Monster01Logic->SetName(L"MONSTER01");
			Monster01Logic->SetRange(1);

			HPTR<StatusLogic> Monster01StatLogic = Monster01->AddComponent<StatusLogic>();
			Monster01StatLogic->DefMonster();
			Monster01StatLogic->SetStatus(600, 10, 0.0f, 0.0f);
			ChessBoard::Inst().PushEnemyPiece(Monster01);

			HPTR<Actor> Monster02 = curScene->CreateActor();
			Monster02->GetTransform()->SetLoclaPosition({ -186.0f, 0.0f, 186.0F });
			Monster02->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
			ChessBoard::Inst().PushEnemyPiece(Monster02);

			HPTR<FbxAnimation> Monster02Ani = Monster02->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster02Ani->FbxSetting(L"mon15.FBXDATA");
			Monster02Ani->CreateAnimation(L"ATTACK01", 0, 30, false);
			Monster02Ani->CreateAnimation(L"ATTACK02", 31, 61, false);
			Monster02Ani->CreateAnimation(L"BORN", 62, 97, false);
			Monster02Ani->CreateAnimation(L"DEATH", 98, 148, false);
			Monster02Ani->CreateAnimation(L"DIZZY", 149, 189, true);
			Monster02Ani->CreateAnimation(L"IDLE", 190, 230, true);
			Monster02Ani->CreateAnimation(L"JUMP", 231, 273, 1.2f, false);
			Monster02Ani->CreateAnimation(L"RUN", 274, 296, true);
			Monster02Ani->CreateAnimation(L"VICTORY", 297, 337, true);
			Monster02Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster02Logic = Monster02->AddComponent<MonsterLogic>();
			Monster02Logic->CalculateIndex(Monster02->GetTransform()->GetLocalPosition());
			Monster02Logic->SetName(L"Monster02");
			Monster02Logic->SetRange(1);

			HPTR<StatusLogic> Monster02StatLogic = Monster02->AddComponent<StatusLogic>();
			Monster02StatLogic->DefMonster();
			Monster02StatLogic->SetStatus(600, 10, 0.0f, 0.0f);

			HPTR<Actor> Monster03 = curScene->CreateActor();
			Monster03->GetTransform()->SetLoclaPosition({ -310.0f, 0.0f, 310.0F });
			Monster03->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
			ChessBoard::Inst().PushEnemyPiece(Monster03);

			HPTR<FbxAnimation> Monster03Ani = Monster03->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster03Ani->FbxSetting(L"mon16.FBXDATA");
			Monster03Ani->CreateAnimation(L"ATTACK01", 0, 30, false);
			Monster03Ani->CreateAnimation(L"ATTACK02", 31, 61, false);
			Monster03Ani->CreateAnimation(L"BORN", 62, 100, false);
			Monster03Ani->CreateAnimation(L"DEATH", 101, 146, false);
			Monster03Ani->CreateAnimation(L"DIZZY", 147, 187, true);
			Monster03Ani->CreateAnimation(L"IDLE", 188, 228, true);
			Monster03Ani->CreateAnimation(L"JUMP", 229, 271, 1.2f, false);
			Monster03Ani->CreateAnimation(L"RUN", 272, 292, true);
			Monster03Ani->CreateAnimation(L"VICTORY", 293, 332, true);
			Monster03Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster03Logic = Monster03->AddComponent<MonsterLogic>();
			Monster03Logic->CalculateIndex(Monster03->GetTransform()->GetLocalPosition());
			Monster03Logic->SetName(L"Monster03");
			Monster03Logic->SetRange(1);

			HPTR<StatusLogic> Monster03StatLogic = Monster03->AddComponent<StatusLogic>();
			Monster03StatLogic->DefMonster();
			Monster03StatLogic->SetStatus(600, 10, 0.0f, 0.0f);

			HPTR<Actor> Monster04 = curScene->CreateActor();
			Monster04->GetTransform()->SetLoclaPosition({ 310.0f, 0.0f, 310.0F });
			Monster04->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
			ChessBoard::Inst().PushEnemyPiece(Monster04);

			HPTR<FbxAnimation> Monster04Ani = Monster04->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster04Ani->FbxSetting(L"mon17.FBXDATA");
			Monster04Ani->CreateAnimation(L"ATTACK01", 0, 30, false);
			Monster04Ani->CreateAnimation(L"ATTACK02", 31, 61, false);
			Monster04Ani->CreateAnimation(L"BORN", 62, 100, false);
			Monster04Ani->CreateAnimation(L"DEATH", 101, 146, false);
			Monster04Ani->CreateAnimation(L"DIZZY", 147, 187, true);
			Monster04Ani->CreateAnimation(L"IDLE", 188, 228, true);
			Monster04Ani->CreateAnimation(L"JUMP", 229, 271, 1.2f, false);
			Monster04Ani->CreateAnimation(L"RUN",	272, 292, true);
			Monster04Ani->CreateAnimation(L"VICTORY", 293, 332, true);
			Monster04Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster04Logic = Monster04->AddComponent<MonsterLogic>();
			Monster04Logic->CalculateIndex(Monster04->GetTransform()->GetLocalPosition());
			Monster04Logic->SetName(L"Monster04");
			Monster04Logic->SetRange(1);

			HPTR<StatusLogic> Monster04StatLogic = Monster04->AddComponent<StatusLogic>();
			Monster04StatLogic->DefMonster();
			Monster04StatLogic->SetStatus(600, 10, 0.0f, 0.0f);

			///
			HPTR<Actor> Monster05 = curScene->CreateActor();
			Monster05->GetTransform()->SetLoclaPosition({ -62.0f, 0.0f, 62.0F });
			Monster05->GetTransform()->SetLocalScale({ 1.0F, 1.0F, 1.0F });
			ChessBoard::Inst().PushEnemyPiece(Monster05);

			HPTR<FbxAnimation> Monster05Ani = Monster05->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster05Ani->FbxSetting(L"mon18.FBXDATA");
			Monster05Ani->CreateAnimation(L"ATTACK01", 0, 44, false);
			Monster05Ani->CreateAnimation(L"ATTACK02", 45, 95, false);
			Monster05Ani->CreateAnimation(L"BORN", 96, 141, false);
			Monster05Ani->CreateAnimation(L"DEATH", 142, 200, false);
			Monster05Ani->CreateAnimation(L"DIZZY", 201, 261, true);
			Monster05Ani->CreateAnimation(L"IDLE", 262, 310, true);
			Monster05Ani->CreateAnimation(L"JUMP", 311, 364, 1.2f, false);
			Monster05Ani->CreateAnimation(L"RUN", 365, 385, true);
			Monster05Ani->CreateAnimation(L"VICTORY", 386, 433, true);
			Monster05Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster05Logic = Monster05->AddComponent<MonsterLogic>();
			Monster05Logic->CalculateIndex(Monster05->GetTransform()->GetLocalPosition());
			Monster05Logic->SetName(L"Monster05");
			Monster05Logic->SetRange(1);

			HPTR<StatusLogic> Monster05StatLogic = Monster05->AddComponent<StatusLogic>();
			Monster05StatLogic->DefMonster();
			Monster05StatLogic->SetStatus(600, 10, 0.0f, 0.0f);

			HPTR<Actor> Monster06 = curScene->CreateActor();
			Monster06->GetTransform()->SetLoclaPosition({ 62.0f, 0.0f, 62.0F });
			Monster06->GetTransform()->SetLocalScale({ 1.0F, 1.0F, 1.0F });
			ChessBoard::Inst().PushEnemyPiece(Monster06);

			HPTR<FbxAnimation> Monster06Ani = Monster06->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster06Ani->FbxSetting(L"mon18.FBXDATA");
			Monster06Ani->CreateAnimation(L"ATTACK01",	0, 44, false);
			Monster06Ani->CreateAnimation(L"ATTACK02",	45, 95, false);
			Monster06Ani->CreateAnimation(L"BORN",		96, 141, false);
			Monster06Ani->CreateAnimation(L"DEATH",		142, 200, false);
			Monster06Ani->CreateAnimation(L"DIZZY",		201, 261, true);
			Monster06Ani->CreateAnimation(L"IDLE",		262, 310, true);
			Monster06Ani->CreateAnimation(L"JUMP",		311, 364, 1.2f, false);
			Monster06Ani->CreateAnimation(L"RUN",		365, 385, true);
			Monster06Ani->CreateAnimation(L"VICTORY",	386, 433, true);
			Monster06Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster06Logic = Monster06->AddComponent<MonsterLogic>();
			Monster06Logic->CalculateIndex(Monster06->GetTransform()->GetLocalPosition());
			Monster06Logic->SetName(L"Monster06");
			Monster06Logic->SetRange(1);

			HPTR<StatusLogic> Monster06StatLogic = Monster06->AddComponent<StatusLogic>();
			Monster06StatLogic->DefMonster();
			Monster06StatLogic->SetStatus(600, 10, 0.0f, 0.0f);
		}
		break;
#pragma endregion

#pragma region ROUND6 BOSS
		case 6:
		{
			
			HPTR<Actor> Monster01 = curScene->CreateActor();
			Monster01->GetTransform()->SetLoclaPosition({ 186.0f, 0.0f, 186.0F });
			Monster01->GetTransform()->SetLocalScale({ 2.0F, 2.0F, 2.0F });


			HPTR<FbxAnimation> Monster01Ani = Monster01->AddComponent<FbxAnimation>(RG_PLAYER);
			Monster01Ani->FbxSetting(L"mon19.FBXDATA");
			Monster01Ani->CreateAnimation(L"ATTACK01", 0, 45, false);
			Monster01Ani->CreateAnimation(L"ATTACK02", 46, 91, false);
			Monster01Ani->CreateAnimation(L"BORN", 92, 137, false);
			Monster01Ani->CreateAnimation(L"DEATH", 138, 203, false);
			Monster01Ani->CreateAnimation(L"DIZZY", 204, 254, true);
			Monster01Ani->CreateAnimation(L"IDLE", 255, 305, true);
			Monster01Ani->CreateAnimation(L"JUMP", 306, 360, 1.2f, false);
			Monster01Ani->CreateAnimation(L"RUN", 361, 391, true);
			Monster01Ani->CreateAnimation(L"VICTORY", 392, 502, true);
			Monster01Ani->ChangeAnimation(L"BORN");

			HPTR<MonsterLogic> Monster01Logic = Monster01->AddComponent<MonsterLogic>();
			Monster01Logic->CalculateIndex(Monster01->GetTransform()->GetLocalPosition());
			Monster01Logic->SetName(L"MONSTER01");
			Monster01Logic->SetRange(1);

			HPTR<StatusLogic> Monster01StatLogic = Monster01->AddComponent<StatusLogic>();
			Monster01StatLogic->DefMonster();
			Monster01StatLogic->SetStatus(100000, 10, 0.0f, 0.0f);
			ChessBoard::Inst().PushEnemyPiece(Monster01);

			bossOn = true;
		}
		break;
#pragma endregion
		}

		isWarStart = false;
	}

}


int GameManager::GetCurTime()
{
	return curTime;
}

void GameManager::UpdateTimeTable()
{
	if (bossTime > 40.0f)
	{
		Sound::Stop(L"bgm_Battle_4.wav");
		//Ã¼ÀÎÁö¾À
		GameWindow::MainGameWin()->ChangeScene(L"EndingScene");
	}


	if (bossOn == true)
	{
		bossTime += Time::DeltaTime();
	}

	if (curTime == 31 && state == MGRSTATE::WAIT)
	{
		state = MGRSTATE::WAR;

		number = 10;
		numberOfTen = 10;

		renderer1->SetSpriteIdx(numberOfTen);
		renderer2->SetSpriteIdx(number);
		curTime = 0;

		warRound++;


		switch (warRound)
		{
		case 1:
			Sound::Stop(L"bgm_battle_embattle.wav");
			Sound::Play(L"bgm_Battle_1.wav", L"bgm_Battle_1.wav");
			break;
		case 2:
			Sound::Stop(L"bgm_battle_embattle.wav");
			Sound::Play(L"bgm_battle_2.wav", L"bgm_battle_2.wav");
			break;
		case 3:
			Sound::Stop(L"bgm_battle_embattle.wav");
			Sound::Play(L"bgm_battle_3.wav", L"bgm_battle_3.wav");
			break;
		case 4:
			Sound::Stop(L"bgm_battle_embattle.wav");
			Sound::Play(L"bgm_battle_2.wav", L"bgm_battle_2.wav");
			break;
		case 5:
			Sound::Stop(L"bgm_battle_embattle.wav");
			Sound::Play(L"bgm_battle_3.wav", L"bgm_battle_3.wav");
			break;
		case 6:
			Sound::Stop(L"bgm_battle_embattle.wav");
			Sound::Play(L"bgm_Battle_4.wav", L"bgm_Battle_4.wav");
			break;
		}

		isWarStart = true;
	}
	else if (curTime == 61 && state == MGRSTATE::WAR)
	{
		state = MGRSTATE::WAIT;

		switch (warRound)
		{
		case 1:
			Sound::Stop(L"bgm_Battle_1.wav");
			Sound::Play(L"roundstart.wav");
			DeadListAllDie();
			break;
		case 2:
			Sound::Stop(L"bgm_battle_2.wav");
			Sound::Play(L"roundstart.wav");
			DeadListAllDie();
			break;
		case 3:
			Sound::Stop(L"bgm_battle_3.wav");
			Sound::Play(L"roundstart.wav");
			DeadListAllDie();
			break;
		case 4:
			Sound::Stop(L"bgm_battle_2.wav");
			Sound::Play(L"roundstart.wav");
			DeadListAllDie();
			break;
		case 5:
			Sound::Stop(L"bgm_battle_3.wav");
			Sound::Play(L"roundstart.wav");
			DeadListAllDie();
			break;
		case 6:
			//Sound::Stop(L"bgm_battle_4.wav");
			break;
		}

		Sound::Play(L"bgm_battle_embattle.wav", L"bgm_battle_embattle.wav");
		number = 0;
		numberOfTen = 0;

		renderer1->SetSpriteIdx(numberOfTen);
		renderer2->SetSpriteIdx(number);
		curTime = 0;
	}

	stayTime += Time::DeltaTime();

	if (state == WAIT)
	{
		if (stayTime >= 1.0f)
		{
			Sound::Play(L"countdown.wav");
			number += 1;
			++curTime;
			if (number == 10)
			{
				numberOfTen += 1;
				if (numberOfTen == 10)
				{
					numberOfTen = 0;
				}
				
				renderer1->SetSpriteIdx(numberOfTen);
				number = 0;
			}
			renderer2->SetSpriteIdx(number);
			stayTime = 0.0f;
		}
	}
	else //WAR
	{
		if (stayTime >= 1.0f)
		{
			number += 1;
			++curTime;
			if (number == 20)
			{
				numberOfTen += 1;
				if (numberOfTen == 20)
				{
					numberOfTen = 10;
				}
				renderer1->SetSpriteIdx(numberOfTen);
				number = 10;
			}
			renderer2->SetSpriteIdx(number);
			stayTime = 0.0f;
		}
	}


}

void GameManager::SkipState()
{
	if (state == MGRSTATE::WAIT)
	{
		curTime = 30;
	}
	else if (state == MGRSTATE::WAR)
	{
		curTime = 60;
	}
}
