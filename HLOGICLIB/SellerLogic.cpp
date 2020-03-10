#include "SellerLogic.h"
#include  <RANDOM.h>
#include "LOGICHEADER.h"
#include "Player.h"
#include <SCENE.h>
#include <SPRITE.h>
#include <INPUT.h>
#include <3DANIRENDER.h>
#include "ChessBoard.h"

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

SellerLogic SellerLogic::m_inst = SellerLogic();

SellerLogic::SellerLogic()
{
}


SellerLogic::~SellerLogic()
{
}

void SellerLogic::AllOff()
{
	for (size_t i = 0; i < 6; i++)
	{
		if (sellerActorArr[i].actor != nullptr)
		{
			sellerActorArr[i].actor->UpdateOff();
		}
	}
}

void SellerLogic::AllOn()
{
	for (size_t i = 0; i < 6; i++)
	{
		if (sellerActorArr[i].actor != nullptr)
		{
			sellerActorArr[i].actor->UpdateOn();
		}
	}
}

void SellerLogic::OffIndex(int _Idx)
{
	sellerActorArr[_Idx].actor->UpdateOff();

	sellerActorArr[_Idx].Release();
}
void SellerLogic::OnIndex(int _Idx)
{
	sellerActorArr[_Idx].actor->UpdateOn();
}

bool SellerLogic::GetIdxUpdate(int _Idx)
{
	if (sellerActorArr[_Idx].actor != nullptr)
	{
		return sellerActorArr[_Idx].actor->IsUpdate();
	}
	else
	{
		return false;
	}

	
}

int SellerLogic::GetChessIdx(int _Idx)
{
	if (_Idx >= 0 && _Idx <= 5)
	{
		return sellerActorArr[_Idx].index;
	}

	return -1;
}

void SellerLogic::Init(Scene* _Scene)
{
	scene = _Scene;

	sellerActorArr.resize(6);

	Vector4 pos = { -6.0f, 3.1f, -10.0f, 0.0f };
	for (size_t i = 0; i < 5; i++)
	{
		sellerActorArr[i].actor = _Scene->CreateActor();
		sellerActorArr[i].actor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0f, 0.0f });
		sellerActorArr[i].actor->GetTransform()->SetScale({ 2.5f, 2.5f });

		int idx = Well512Random::Instance().GetValue(1, 21);
		std::wstring spriteName = Player::Inst().GetName(idx);
		sellerActorArr[i].index = idx;
		sellerActorArr[i].spritePtr = sellerActorArr[i].actor->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);

		sellerActorArr[i].spritePtr->SetSprite(spriteName.c_str(), 0);
		sellerActorArr[i].spritePtr->RenderData(L"DTEXMESH", L"UISPRITEPATH");
		sellerActorArr[i].spritePtr->LinkCB(L"UIDATA", &(sellerActorArr[i].spritePtr->GetUIData()));
		sellerActorArr[i].spritePtr->LinkCB(L"VS_UICUTDATA", &(sellerActorArr[i].spritePtr->GetCutData()));
		sellerActorArr[i].spritePtr->LinkTexture(L"PS_MAPTEX", spriteName.c_str());
		sellerActorArr[i].spritePtr->LinkSampler(L"PS_LSMP", L"LSMP");
		sellerActorArr[i].spritePtr->LinkSampler(L"PS_PSMP", L"PSMP");
		
		sellerActorArr[i].spriteAni = sellerActorArr[i].actor->AddComponent<SpriteAnimaion>();
		sellerActorArr[i].spriteAni->CreateAni(L"ANI", spriteName.c_str(), 0, sellerActorArr[i].spritePtr->GetLastIdx(), 0.03f);

		sellerActorArr[i].actor->UpdateOff();
		sellerActorArr[i].actor->GetTransform()->SetLoclaPosition(pos);
		
		pos.x += 3.0f;
		//spriteArr[i].actor->UpdateOff();
	}


#pragma region NONE
	Vector4 pos2 = { 20.0f, 3.2f, 10.0f, 0.0f };
	sellerActorArr[5].actor = _Scene->CreateActor();
	sellerActorArr[5].actor->GetTransform()->SetLoclaPosition(pos2);
	sellerActorArr[5].actor->GetTransform()->SetScale({ 2.0f, 2.0f });
	int idx2 = Well512Random::Instance().GetValue(1, 22);
	std::wstring spriteName2 = Player::Inst().GetName(idx2);
	
	sellerActorArr[5].index = idx2;
	sellerActorArr[5].spritePtr = sellerActorArr[5].actor->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
	sellerActorArr[5].spritePtr->SetSprite(spriteName2.c_str(), 0);
	sellerActorArr[5].spritePtr->RenderData(L"DTEXMESH", L"UISPRITEPATH");
	sellerActorArr[5].spritePtr->LinkCB(L"UIDATA", &(sellerActorArr[5].spritePtr->GetUIData()));
	sellerActorArr[5].spritePtr->CreateCB(L"VS_UICUTDATA", Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	sellerActorArr[5].spritePtr->LinkTexture(L"PS_MAPTEX", spriteName2.c_str());
	sellerActorArr[5].spritePtr->LinkSampler(L"PS_LSMP", L"LSMP");
	sellerActorArr[5].spritePtr->LinkSampler(L"PS_PSMP", L"PSMP");

	sellerActorArr[5].spriteAni = sellerActorArr[5].actor->AddComponent<SpriteAnimaion>();
	sellerActorArr[5].spriteAni->CreateAni(L"ANI", spriteName2.c_str(), 0, sellerActorArr[5].spritePtr->GetLastIdx(), 0.03f);

	sellerActorArr[5].actor->UpdateOff();
#pragma endregion


}

void SellerLogic::Update()
{
	if (Input::Up(L"RELOAD") == true)
	{
		Sound::Play(L"commonswitch.wav");
		ReLoad();
	}
}

void SellerLogic::PieceChoice(int _Idx)
{

}

void SellerLogic::ReLoad()
{

	for (size_t i = 0; i < 6; i++)
	{
		if (sellerActorArr[i].actor != nullptr)
		{
			sellerActorArr[i].actor->Die();
		}
		
		sellerActorArr[i].actor = nullptr;
		sellerActorArr[i].index = NULL;
		sellerActorArr[i].spritePtr = nullptr;
		sellerActorArr[i].spriteAni = nullptr;		
	}

	Vector4 pos = { -6.0f, 3.1f, -10.0f, 0.0f };
	for (size_t i = 0; i < 5; i++)
	{
		sellerActorArr[i].actor = scene->CreateActor();
		sellerActorArr[i].actor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0f, 0.0f });
		sellerActorArr[i].actor->GetTransform()->SetScale({ 2.5f, 2.5f });

		int idx = Well512Random::Instance().GetValue(1, 23);
		


		std::wstring spriteName = Player::Inst().GetName(idx);
		sellerActorArr[i].index = idx;
		sellerActorArr[i].spritePtr = sellerActorArr[i].actor->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);

		sellerActorArr[i].spritePtr->SetSprite(spriteName.c_str(), 0);
		sellerActorArr[i].spritePtr->RenderData(L"DTEXMESH", L"UISPRITEPATH");
		sellerActorArr[i].spritePtr->LinkCB(L"UIDATA", &(sellerActorArr[i].spritePtr->GetUIData()));
		sellerActorArr[i].spritePtr->LinkCB(L"VS_UICUTDATA", &(sellerActorArr[i].spritePtr->GetCutData()));
		sellerActorArr[i].spritePtr->LinkTexture(L"PS_MAPTEX", spriteName.c_str());
		sellerActorArr[i].spritePtr->LinkSampler(L"PS_LSMP", L"LSMP");
		sellerActorArr[i].spritePtr->LinkSampler(L"PS_PSMP", L"PSMP");

		sellerActorArr[i].spriteAni = sellerActorArr[i].actor->AddComponent<SpriteAnimaion>();
		sellerActorArr[i].spriteAni->CreateAni(L"ANI", spriteName.c_str(), 0, sellerActorArr[i].spritePtr->GetLastIdx(), 0.03f);

		sellerActorArr[i].actor->UpdateOn();
		sellerActorArr[i].actor->GetTransform()->SetLoclaPosition(pos);
		
		pos.x += 3.0f;
	}

#pragma region NONE
 	Vector4 pos2 = { 20.0f, 3.2f, 10.0f, 0.0f };
	sellerActorArr[5].actor = scene->CreateActor();
	sellerActorArr[5].actor->GetTransform()->SetLoclaPosition(pos2);
	sellerActorArr[5].actor->GetTransform()->SetScale({ 2.0f, 2.0f });
	int idx2 = Well512Random::Instance().GetValue(1, 21);
	std::wstring spriteName2 = Player::Inst().GetName(idx2);

	sellerActorArr[5].index = idx2;
	sellerActorArr[5].spritePtr = sellerActorArr[5].actor->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
	sellerActorArr[5].spritePtr->SetSprite(spriteName2.c_str(), 0);
	sellerActorArr[5].spritePtr->RenderData(L"DTEXMESH", L"UISPRITEPATH");
	sellerActorArr[5].spritePtr->LinkCB(L"UIDATA", &(sellerActorArr[5].spritePtr->GetUIData()));
	sellerActorArr[5].spritePtr->CreateCB(L"VS_UICUTDATA", Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	sellerActorArr[5].spritePtr->LinkTexture(L"PS_MAPTEX", spriteName2.c_str());
	sellerActorArr[5].spritePtr->LinkSampler(L"PS_LSMP", L"LSMP");
	sellerActorArr[5].spritePtr->LinkSampler(L"PS_PSMP", L"PSMP");

	sellerActorArr[5].spriteAni = sellerActorArr[5].actor->AddComponent<SpriteAnimaion>();
	sellerActorArr[5].spriteAni->CreateAni(L"ANI", spriteName2.c_str(), 0, sellerActorArr[5].spritePtr->GetLastIdx(), 0.1f);
#pragma endregion

}

HPTR<Actor> SellerLogic::CreatePiece(int _IDX)
{

	HPTR<Actor> createdActor = nullptr;
	HPTR<FbxAnimation> createdActorAni = nullptr;
	switch ((PIECEKINDS)_IDX)
	{
	case PIECEKINDS::UNICORN:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"Unicorn01.FBXDATA");
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
		stateLogic->SetStatus(400, 55, 15.0f, 11.0f, _IDX);

		stateLogic->SetNUMBER(1);		Sound::Play(L"h002_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::GODOFWAR:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 0.8f, 0.8f, 0.8f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
	//	createdActorAni->FbxSetting(L"GodofWar01.FBXDATA");
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
		stateLogic->SetStatus(650, 0, 5.0f, 2.0f, _IDX);
		stateLogic->SetNUMBER(2);		Sound::Play(L"h057_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::SWORDMASTER:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.2F, 1.2F, 1.2F });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"SwordMaster01.FBXDATA");
		createdActorAni->CreateAnimation(L"ATTACK01",  0, 33, false);
		createdActorAni->CreateAnimation(L"ATTACK02", 34, 67, false);
		createdActorAni->CreateAnimation(L"BORN",	68, 113, false);
		createdActorAni->CreateAnimation(L"DEATH", 114, 194, false);
		createdActorAni->CreateAnimation(L"DIZZY", 195, 235, true);
		createdActorAni->CreateAnimation(L"IDLE", 236, 276, true);
		createdActorAni->CreateAnimation(L"JUMP", 277, 335, false);
		createdActorAni->CreateAnimation(L"RUN",  336, 360, true);
		createdActorAni->CreateAnimation(L"SKILL", 361, 376, false);
		createdActorAni->CreateAnimation(L"SKILLLOOP", 377, 385, true);
		createdActorAni->CreateAnimation(L"VICTORY", 386, 483, true);
		createdActorAni->ChangeAnimation(L"BORN");

		HPTR<SwordMaster> moveLogic = createdActor->AddComponent<SwordMaster>();
		moveLogic->SetName(L"SwordMaster01");
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME
		stateLogic->SetStatus(600, 67, 12.0f, 5.0f, _IDX);
		stateLogic->SetNUMBER(3);		Sound::Play(L"h015_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::MANAMAGICIAN:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.2f, 1.2f, 1.2f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"ManaMagician01.FBXDATA");
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
		stateLogic->SetStatus(450, 42, 600.0f, 0.0f,_IDX);
		stateLogic->SetNUMBER(4);		Sound::Play(L"h013_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::LITTLEDRAGON:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.7f, 1.7f, 1.7f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"LittleDragon01.FBXDATA");
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
		stateLogic->SetStatus(4500 , 45, 5.0f ,0.0f, _IDX);
		stateLogic->SetNUMBER(5);		Sound::Play(L"h018_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::SHININGARCHER:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"ShiningArcher01.FBXDATA");
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
		stateLogic->SetStatus(600, 60, 10.0f, 0.0f, _IDX);
		stateLogic->SetNUMBER(6);		Sound::Play(L"h058_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::BLADEKNIGHT:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 0.8f, 0.8f, 0.8f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"BladeKnight01.FBXDATA");
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
		stateLogic->SetStatus(500, 0, 0.0f, 0.0f, _IDX);
		stateLogic->SetNUMBER(7);		Sound::Play(L"h023_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::DEMONLARVA:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"DemonLarva01.FBXDATA");
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
		stateLogic->SetStatus(600, 55, 5.0f, 0.0f, _IDX);
		stateLogic->SetNUMBER(8);		Sound::Play(L"h027_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::WAREWOLF:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"Warewolf01.FBXDATA");
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
		stateLogic->SetStatus(700, 55, 60.0f, 0.0f, _IDX);
		stateLogic->SetNUMBER(9);		Sound::Play(L"h026_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::FIREMAGICIAN:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"FireMagician01.FBXDATA");
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
		stateLogic->SetStatus(600, 52, 10.0f, 0.0f, _IDX);
		stateLogic->SetNUMBER(10);		Sound::Play(L"h038_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::WINDRANGER:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"WindRanger01.FBXDATA");
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
		stateLogic->SetStatus(550, 60, 10.0f, 0.0f, _IDX);
		stateLogic->SetNUMBER(11);		Sound::Play(L"h030_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::THUNDERSPIRIT:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"ThunderSpirit01.FBXDATA");
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
		stateLogic->SetStatus(750, 55, 10.0f, 0.0f, _IDX);
		stateLogic->SetNUMBER(12);		Sound::Play(L"h029_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::TOENAILDRUID:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 0.8f, 0.8f, 0.8f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"ToenailDruid01.FBXDATA");
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
		stateLogic->SetStatus(800, 55, 60.0f, 0.0f, _IDX);
		stateLogic->SetNUMBER(13);		Sound::Play(L"h050_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::SIREN:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"Siren01.FBXDATA");
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
		stateLogic->SetStatus(750, 60, 20.0f, 3.0f, _IDX);
		stateLogic->SetNUMBER(14);		Sound::Play(L"h048_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::PIRATECAPTAIN:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"PirateCaptain01.FBXDATA");
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
		stateLogic->SetStatus(950, 82, 15.0f, 0.0f, _IDX);
		stateLogic->SetNUMBER(15);		Sound::Play(L"h041_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::TORTOLANELDER:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"TortolanElder01.FBXDATA");
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
		stateLogic->SetStatus(650, 42, 10.0f, 0.0f, _IDX);
		stateLogic->SetNUMBER(16);
		Sound::Play(L"h043_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::DOOMARBITER:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"DoomArbiter01.FBXDATA");
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
		stateLogic->SetStatus(950, 115, 10.0f, 15.0f, _IDX);
		stateLogic->SetNUMBER(17);		Sound::Play(L"h040_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::TIDEHUNTER:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.2f, 1.2f, 1.2f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"TideHunter01.FBXDATA");
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
		stateLogic->SetStatus(950, 50, 30.0f, 0.0f, _IDX);
		stateLogic->SetNUMBER(18);		Sound::Play(L"h053_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::EGG:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"Egg01.FBXDATA");
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
		stateLogic->SetStatus(700, 60, 0.0f, 0.0f, _IDX);
		stateLogic->SetNUMBER(19);		Sound::Play(L"h056_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::GODOFTHUNDER:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });


		createdActor->SetLevel(1);
		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"GodofThunder01.FBXDATA");
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
		moveLogic->SetRange(1);

		HPTR<StatusLogic> stateLogic = createdActor->AddComponent<StatusLogic>();
		//HP, ATT, COOLTIME, CONTINUETIME
		stateLogic->SetStatus(950, 60, 15.0f, 0.0f, _IDX);
		stateLogic->SetNUMBER(20);		Sound::Play(L"h060_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::SHADOWDEMON:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0F, 1.0F, 1.0F });
		createdActor->SetLevel(1);

		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"ShadowDemon01.FBXDATA");
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
		stateLogic->SetStatus(450, 80, 15.0f, 0.0f, _IDX);
		stateLogic->SetNUMBER(21);		Sound::Play(L"h037_showup_01_eng.wav");	}
		break;
	case PIECEKINDS::WALRUSCHAMP:
	{
		createdActor = scene->CreateActor();
		createdActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		createdActor->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		createdActor->SetLevel(1);


		createdActorAni = createdActor->AddComponent<FbxAnimation>(RG_PLAYER);
		createdActorAni->FbxSetting(L"WalrusChamp01.FBXDATA");
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
		stateLogic->SetStatus(650, 52, 10.0f, 0.0f, _IDX);
		stateLogic->SetNUMBER(0);		Sound::Play(L"h004_showup_01_eng.wav");	}
		break;
	}
	
	
	//ChessBoard::Inst().PushAllyPiece(createdActor);


	return createdActor;
}