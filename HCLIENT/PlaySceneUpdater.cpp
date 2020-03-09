#include "PlaySceneUpdater.h"


//BASE
#include <INPUT.h>
#include <GAMEWIN.h>
#include <RANDOM.h>
#include <JTIME.h>

#include <DirectXMath.h>
//ENGINE
#include <GRAPHICDEBUG.h>
#include <GRIDRENDER.h>
#include <CAMERA.h>
#include <FREECAM.h>
#include <3DANIRENDER.h>
#include <StaticFbxRender.h>
#include <BOUNDINGBOX.h>


//LOGIC
#include <LOGICHEADER.h>
#include <ChessBoard.h>
#include <StatusLogic.h>
#include <SellerLogic.h>
#include <GameManager.h>

//CLIENT
#include "LOGICHEADER.h"
#include <Player.h>
#include <GAMEWIN.h>
#include <FREECAM.h>
#include <SPRITERENDER.h>
#include <RECTRENDER.h>
#include <TEX.h>
#include <SPRITE.h>
#include <2DANICOM.h>
#include <INPUT.h>
#include <SOUND.h>
#include <RENDER.h>
#include <GRIDRENDER.h>
#include <TEX.h>

#include <BoneChaser.h>
#include <BOUNDINGRECT.h>
#include <BOUNDINGCIRCLE.h>
#include <BOUNDINGBOX.h>
#include <GRAPHICDEBUG.h>
#include <DEBUG.h>
#include <GAMETHREAD.h>
#include <HMULTIRENDERTarget.h>
#include <HRENDERTARGET.h>
#include <BloomFilter.h>


#include <GRAPHICDEBUG.h>
#include <3DANIRENDER.h>


#include <TERRAIN.h>
#include <FREECAM.h>

#include <GAMETHREAD.h>


//
#include <ChessBoard.h>
#include <MpMagicion.h>
#include <MonsterLogic.h>


//

#include <EffectRotate.h>
#include <LittleDragonSkill.h>
#include <SOUND.h>


PlaySceneUpdater::PlaySceneUpdater() : rotSkyBox(0.0f)
{

}
PlaySceneUpdater::~PlaySceneUpdater()
{

}

void PlaySceneUpdater::Loading()
{
#pragma region LOADING
#pragma endregion

	Font = Resources::Find<HEFONT>(L"1훈떡볶이");

	Player::Inst().Init();
	ChessBoard::Inst().SetScene(GetScene());

#pragma region CAMERA
	{
		m_MainCam = GetScene()->CreateActor(L"MAINCAM");
		m_MainCam->GetTransform()->SetLoclaPosition({ 4.47530842f, 1027.91724f, -1142.71094f });
		m_MainCam->GetTransform()->SetLocalRot({ 49.1068573f, 0.586260557f, 0.0f });

		camCom = m_MainCam->AddComponent<Camera>(RG_GRID, RG_PLAYER);
		camCom->Order(0);
		camCom->SetFarValue(100000.0f);
		camCom->SetProjMode(PROJMODE::PM_PERS);
		camCom->SetArea((WIN_SIZE_X / 100), (WIN_SIZE_Y / 100));

		HPTR<FreeCamera> freeCamCom = m_MainCam->AddComponent<FreeCamera>();
	}

	//UICAMERA
	HPTR<Actor> mainCam = GetScene()->CreateActor(L"LOADINGCAM");
	mainCam->GetTransform()->SetPosition({ 0.0f, 0.0f, -100.0f });
	HPTR<Camera> camera = mainCam->AddComponent<Camera>(RENDERGROUP::RG_UI);
	camera->SetProjMode(PROJMODE::PM_ORTH);
	camera->SetFarValue(100000.0f);
	camera->SetArea((WIN_SIZE_X / 100), (WIN_SIZE_Y / 100));


	Camera::SetUICam(camera);


#pragma endregion

#pragma region LIGHT
	HPTR<Actor> Light01 = GetScene()->CreateActor();
	Light01->GetTransform()->SetLocalRotX(45.0f);
	Light01->GetTransform()->SetLocalRotY(45.0f);
	Light01->GetTransform()->SetLoclaPosition({ 0.0f, 1000.0f, -1000.0f });
	LightCheck01 = Light01->AddComponent<Light>(RG_PLAYER, RG_UI);
	LightCheck01->SetLightType(Light::LT_DIR);
	LightCheck01->SetSpecPow(1);
	LightCheck01->SetDiffPow(1.1f);
	LightCheck01->SpecPower(1.0f);
#pragma endregion





	HPTR<Actor> chessPlayer = GetScene()->CreateActor();
	chessPlayer->GetTransform()->SetLoclaPosition({ 550.0f, 120.0f, -480.0F });

	HPTR<FbxAnimation> chessPlayerAni = chessPlayer->AddComponent<FbxAnimation>(RG_PLAYER);
	chessPlayerAni->FbxSetting(L"EatingDragon.FBXDATA");
	chessPlayerAni->CreateAnimation(L"ATT", 0, 919, true);
	//SR->CreateAnimation(L"ATT", 886, 909, true);
	chessPlayerAni->ChangeAnimation(L"ATT");
	//chessPlayerAni->ChangeRockMaterialOff();


	HPTR<SpriteRenderer> chessPlayerSprite = chessPlayer->AddComponent<SpriteRenderer>(RG_PLAYER);
	chessPlayerSprite->SetSprite(L"HP_Prograss.png");
	chessPlayerSprite->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
	chessPlayerSprite->LinkCB(L"VS_CUTDATA", &(chessPlayerSprite->GetCutData()));
	chessPlayerSprite->LinkTexture(L"PS_MAPTEX", L"HP_Prograss.png");
	chessPlayerSprite->LinkSampler(L"PS_LSMP", L"LSMP");
	chessPlayerSprite->LinkSampler(L"PS_PSMP", L"PSMP");

	chessPlayerSprite->SetLoclaPosition({ 0.0F, 150.0F, 0.0f });
	chessPlayerSprite->SetLocalScale({ 50.0F, 20.0F, 1.0F, 1.0f });
	chessPlayerSprite->BillBoardOn();

	ScenryInit();
	UIInit();

	//TEST
	//{
	//	HPTR<Actor> CHESSBOARD3 = GetScene()->CreateActor();
	//	CHESSBOARD3->GetTransform()->SetLoclaPosition({ 0.0f, 100.0f, 0.0F });
	//	CHESSBOARD3->GetTransform()->SetLocalScale({ 5.5F, 5.5F ,5.5F });
	//	HPTR<StaticFbxRender> BOARDSR3 = CHESSBOARD3->AddComponent<StaticFbxRender>(RG_PLAYER);
	//	BOARDSR3->FbxSetting(L"Unicorn_Weapon.FBXDATA");
	//	BOARDSR3->SetShadow(true);
	//}



	PieceLogic::InitMapStaticInfo();
}


void PlaySceneUpdater::SceneUpdate()
{
	Vector4 tmp = GameWindow::MainGameWin()->MousePos();
	tmp.x = (tmp.x * 0.01f - 8.0f);
	tmp.y = (tmp.y *-0.01f + 4.5f);
	tmp.z = -99.0f;
	Mouse->GetTransform()->SetLoclaPosition(tmp);
	tmp.x *= 100.0f;
	tmp.y *= 100.0f;

	ChessBoard::Inst().Update();

	if (GameManager::Inst().GetState() == 0)
	{
		if (Input::Press(L"CHEAT") == true && Input::Down(L"CLICK") == true)
		{
			CheatUpgradePiece();
		}
		else
		{
			MoveChessPiece();
		}
	}

	PurchaseChessPiece();

	CombinationPiece();

	BoardIndex upCheck = ChessBoard::Inst().CheckUpgrade();
	if (upCheck.x != -1 && upCheck.z != -1)
	{
		ChessBoard::Inst().UpgradePiece(upCheck.x, upCheck.z);
	}

	GameManager::Inst().Update();

	if (Input::Down(L"SKIP") == true)
	{
		GameManager::Inst().SkipState();
	}



	//CB SETTING 
	waterTime.x += Time::DeltaTime();
	WATERRENDER->LinkCB(L"VS_TIMEDATA", &waterTime);

	rotSkyBox += 3.0f * Time::DeltaTime();
	skyShphere->GetTransform()->SetLocalRotY(rotSkyBox);


	PieceLogic::ResetMapStaticInfo();
	ChessBoard::Inst().MapInfoInit(); 
}
void PlaySceneUpdater::ChangeEnd()
{

}
void PlaySceneUpdater::ChangeStart()
{

}
void PlaySceneUpdater::DebugRender()
{
#pragma region DEBUG
	int frt = (int)(1.0f / Time::DeltaTime());


	//std::wstring frame = L"FRAME : ";

	//frame += std::to_wstring(frt);

	//GraphicDebug::DrawFontUI(frame.c_str(), Vector4::RED);
	//{
	//	float my = Mouse->GetTransform()->GetLocalPosition().z;
	//	float mx = Mouse->GetTransform()->GetLocalPosition().x;

	//	std::wstring mouse = L"MOUSE POS : ";
	//	mouse += std::to_wstring(my);
	//	mouse += L"  ";
	//	mouse += std::to_wstring(mx);

	//	GraphicDebug::DrawFontUI(mouse.c_str(), Vector4::RED);
	//}

	//{
	//	Vector4 temp = GameWindow::MainGameWin()->MousePos();
	//	temp.x = (temp.x * 0.01f) * 100.0f;
	//	//temp.x *= 2.0f;
	//	temp.y = (temp.y * 0.01f) * 100.0f;

	//	std::wstring mouse = L"MOUSE POS : ";
	//	mouse += std::to_wstring(temp.x);
	//	mouse += L"  ";
	//	mouse += std::to_wstring(temp.y);

	//	GraphicDebug::DrawFontUI(mouse.c_str(), Vector4::RED);
	//}

	//{
	//	std::wstring mouse = L"MOUSE INDEX : ";
	//	mouse += std::to_wstring(ChessBoard::Inst().CalculatePickIngIndex().z);
	//	mouse += L"  ";
	//	mouse += std::to_wstring(ChessBoard::Inst().CalculatePickIngIndex().x);

	//	GraphicDebug::DrawFontUI(mouse.c_str(), Vector4::RED);
	//}
	//{
	//	std::wstring mouse = L"MOUSEposInt : ";

	//	mouse += std::to_wstring(GameWindow::MainGameWin()->MousePosInt().x);
	//	mouse += L"  ";
	//	mouse += std::to_wstring(GameWindow::MainGameWin()->MousePosInt().y);
	//	GraphicDebug::DrawFontUI(mouse.c_str(), Vector4::BLACK);
	//}


	CombinationLetter();


	if (isGraphicDebug == false)
	{
		if (Input::Down(L"GRAPHICDEBUG") == true)
		{
			isGraphicDebug = true;
		}
	}
	else
	{
		if (Input::Down(L"GRAPHICDEBUG") == true)
		{
			isGraphicDebug = false;
		}
	}
#pragma region DEBUGRENDER
	//if (isGraphicDebug == true)
	//{
	//	Vector4 Pos;
	//	Pos = { -700 , 350 };
	//	Vector4 Size;
	//	Size = { 200, 150 };

	//	GraphicDebug::DrawTexture(camCom->GetDeferredTarget()->Target(0)->GetTex(), Pos, Size);
	//	Pos.x += Size.x;
	//	GraphicDebug::DrawTexture(camCom->GetDeferredTarget()->Target(1)->GetTex(), Pos, Size);
	//	Pos.x += Size.x;
	//	GraphicDebug::DrawTexture(camCom->GetDeferredTarget()->Target(2)->GetTex(), Pos, Size);
	//	Pos.x += Size.x;
	//	GraphicDebug::DrawTexture(camCom->GetDeferredTarget()->Target(3)->GetTex(), Pos, Size);

	//	//Pos = { -650 , 450 - Size.y };
	//	Pos.x += Size.x;
	//	GraphicDebug::DrawTexture(camCom->GetLightTarget()->Target(0)->GetTex(), Pos, Size);
	//	Pos.x += Size.x;
	//	GraphicDebug::DrawTexture(camCom->GetLightTarget()->Target(1)->GetTex(), Pos, Size);

	//	if (nullptr != BFilter)
	//	{
	//		Pos.x += Size.x;
	//		GraphicDebug::DrawTexture(BFilter->SmallTaregt->GetTex(), Pos, Size/*, HVEC4{128, 128}*/);
	//		Pos.x += Size.x;
	//		GraphicDebug::DrawTexture(BFilter->SSmallTaregt->GetTex(), Pos, Size/*, HVEC4{128, 128}*/);
	//	}

	//	if (nullptr != LightCheck01)
	//	{
	//		Pos.x += Size.x;
	//		GraphicDebug::DrawTexture(LightCheck01->m_ShadowTarget->GetTex(), Pos, Size/*, HVEC4{128, 128}*/);
	//	}
	//}
#pragma endregion
}

void PlaySceneUpdater::ScenryInit()
{
#pragma region SCENRY
#pragma region CHESSBOARD

	chessBoardActor = GetScene()->CreateActor(L"ChessBoard");
	chessBoardActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
	// 맵의 크기
	chessBoardActor->GetTransform()->SetLocalScale({ 1000.0F, 1.0F, 1000.0F });
	chessBoardActor->GetTransform()->LocalScaleMul(1.0f);

	chessBoardTerrain = chessBoardActor->AddComponent<Terrain>(RG_PLAYER);
	chessBoardTerrain->CreateTerrain(1, 1, L"Brick_D_02.tga", L"Brick_H.tga", 1000.0f, 1.0f);
	//MAPCOM->FloorTextureSetting(1, L"BUMPTEST.png", L"TILE02_S.png", 0.1F);

	chessBoardTerrain->FloorBumpSetting(0, L"Brick_N.tga");


	{


		chessStayActor = GetScene()->CreateActor();	//체스 유닛 대기판
		chessStayActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, -600.0F });
		// 맵의 크기
		chessStayActor->GetTransform()->SetLocalScale({ 1000.0F, 1.0F, 100.0F });
		chessStayActor->GetTransform()->LocalScaleMul(1.0f);

		chessStayTerrain = chessStayActor->AddComponent<Terrain>(RG_PLAYER);
		chessStayTerrain->CreateTerrain(1, 1, L"ChessStay_D.tga", L"ChessStay_H.tga", 10.0f, 1.0f);
		//MAPCOM->FloorTextureSetting(1, L"BUMPTEST.png", L"TILE02_S.png", 0.1F);

		chessStayTerrain->FloorBumpSetting(0, L"ChessStay_N.tga");
	}

	{
		//밑에 돌판원형 큰거
		HPTR<Actor> CHESSBOARD3 = GetScene()->CreateActor();
		CHESSBOARD3->GetTransform()->SetLoclaPosition({ -290.0f, -9.5f, 300.0F });
		CHESSBOARD3->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> BOARDSR3 = CHESSBOARD3->AddComponent<StaticFbxRender>(RG_PLAYER);
		BOARDSR3->FbxSetting(L"Grass.FBXDATA");
		BOARDSR3->SetShadow(true);
	}

#pragma endregion

#pragma region TREES
	{
		HPTR<Actor> CHESSBOARD3 = GetScene()->CreateActor();
		CHESSBOARD3->GetTransform()->SetLoclaPosition({ 650.0f, 0.0f, 580.0F });
		CHESSBOARD3->GetTransform()->SetLocalScale({ 0.5F, 0.5F ,0.5F });
		HPTR<StaticFbxRender> BOARDSR3 = CHESSBOARD3->AddComponent<StaticFbxRender>(RG_PLAYER);
		BOARDSR3->FbxSetting(L"Palm_Tree_01.FBXDATA");
		BOARDSR3->SetShadow(true);
	}
	{

		HPTR<Actor> CHESSBOARD3 = GetScene()->CreateActor();
		CHESSBOARD3->GetTransform()->SetLoclaPosition({ 500.0f, 0.0f, 580.0F });
		CHESSBOARD3->GetTransform()->SetLocalScale({ 0.5F, 0.5F ,0.5F });
		HPTR<StaticFbxRender> BOARDSR3 = CHESSBOARD3->AddComponent<StaticFbxRender>(RG_PLAYER);
		BOARDSR3->FbxSetting(L"Palm_Tree_02.FBXDATA");
		BOARDSR3->SetShadow(true);
	}
	{
		HPTR<Actor> CHESSBOARD3 = GetScene()->CreateActor();
		CHESSBOARD3->GetTransform()->SetLoclaPosition({ 630.0f, 0.0f, 580.0F });
		CHESSBOARD3->GetTransform()->SetLocalScale({ 0.5F, 0.5F ,0.5F });
		HPTR<StaticFbxRender> BOARDSR3 = CHESSBOARD3->AddComponent<StaticFbxRender>(RG_PLAYER);
		BOARDSR3->FbxSetting(L"Palm_Tree_03.FBXDATA");
		BOARDSR3->SetShadow(true);
	}
	{
		HPTR<Actor> CHESSBOARD3 = GetScene()->CreateActor();
		CHESSBOARD3->GetTransform()->SetLoclaPosition({ -630.0f, 0.0f, 580.0F });
		CHESSBOARD3->GetTransform()->SetLocalRotY(180.0f);
		CHESSBOARD3->GetTransform()->SetLocalScale({ 0.5F, 0.5F ,0.5F });
		HPTR<StaticFbxRender> BOARDSR3 = CHESSBOARD3->AddComponent<StaticFbxRender>(RG_PLAYER);
		BOARDSR3->FbxSetting(L"Palm_Tree_01.FBXDATA");
		BOARDSR3->SetShadow(true);
	}
	{
		HPTR<Actor> CHESSBOARD3 = GetScene()->CreateActor();
		CHESSBOARD3->GetTransform()->SetLoclaPosition({ -630.0f, 0.0f, 80.0F });
		CHESSBOARD3->GetTransform()->SetLocalScale({ 0.5F, 0.5F ,0.5F });
		HPTR<StaticFbxRender> BOARDSR3 = CHESSBOARD3->AddComponent<StaticFbxRender>(RG_PLAYER);
		BOARDSR3->FbxSetting(L"Palm_Tree_02.FBXDATA");
		BOARDSR3->SetShadow(true);
	}
	{
		HPTR<Actor> CHESSBOARD3 = GetScene()->CreateActor();
		CHESSBOARD3->GetTransform()->SetLoclaPosition({ -500.0f, 0.0f, -580.0F });
		CHESSBOARD3->GetTransform()->SetLocalScale({ 0.5F, 0.4F ,0.5F });
		HPTR<StaticFbxRender> BOARDSR3 = CHESSBOARD3->AddComponent<StaticFbxRender>(RG_PLAYER);
		BOARDSR3->FbxSetting(L"Palm_Tree_03.FBXDATA");
		BOARDSR3->SetShadow(true);
	}
	{
		HPTR<Actor> CHESSBOARD3 = GetScene()->CreateActor();
		CHESSBOARD3->GetTransform()->SetLoclaPosition({ -750.0f, 0.0f, -550.0F });
		CHESSBOARD3->GetTransform()->SetLocalRotY(150.0f);
		CHESSBOARD3->GetTransform()->SetLocalScale({ 0.5F, 0.4F ,0.5F });
		HPTR<StaticFbxRender> BOARDSR3 = CHESSBOARD3->AddComponent<StaticFbxRender>(RG_PLAYER);
		BOARDSR3->FbxSetting(L"Palm_Tree_01.FBXDATA");
		BOARDSR3->SetShadow(true);
	}
	{
		HPTR<Actor> CHESSBOARD3 = GetScene()->CreateActor();
		CHESSBOARD3->GetTransform()->SetLoclaPosition({ -600.0f, 0.0f, -650.0F });
		CHESSBOARD3->GetTransform()->SetLocalRot({ 340.0f, 0.0f, 0.0F });
		CHESSBOARD3->GetTransform()->SetLocalScale({ 0.5F, 0.4F ,0.5F });
		HPTR<StaticFbxRender> BOARDSR3 = CHESSBOARD3->AddComponent<StaticFbxRender>(RG_PLAYER);
		BOARDSR3->FbxSetting(L"Palm_Tree_02.FBXDATA");
		BOARDSR3->SetShadow(true);
	}
	{
		HPTR<Actor> CHESSBOARD3 = GetScene()->CreateActor();
		CHESSBOARD3->GetTransform()->SetLoclaPosition({ 750.0f, 0.0f, -500.0F });
		CHESSBOARD3->GetTransform()->SetLocalScale({ 0.5F, 0.5F ,0.5F });
		HPTR<StaticFbxRender> BOARDSR3 = CHESSBOARD3->AddComponent<StaticFbxRender>(RG_PLAYER);
		BOARDSR3->FbxSetting(L"Palm_Tree_02.FBXDATA");
		BOARDSR3->SetShadow(true);
	}
	{
		HPTR<Actor> CHESSBOARD3 = GetScene()->CreateActor();
		CHESSBOARD3->GetTransform()->SetLoclaPosition({ 530.0f, 0.0f, -700.0F });
		CHESSBOARD3->GetTransform()->SetLocalScale({ 0.5F, 0.5F ,0.5F });
		HPTR<StaticFbxRender> BOARDSR3 = CHESSBOARD3->AddComponent<StaticFbxRender>(RG_PLAYER);
		BOARDSR3->FbxSetting(L"Palm_Tree_03.FBXDATA");
		BOARDSR3->SetShadow(true);
	}
#pragma endregion

#pragma region BUSH

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -260.0f,0.0f,0.0f });
		actor->GetTransform()->SetLocalRotY(135.0f);
		actor->GetTransform()->SetLocalScale({ 0.5F, 0.5F ,0.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Grass_01_B.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -380.0f,0.0f,0.0f });
		actor->GetTransform()->SetLocalRotY(135.0f);
		actor->GetTransform()->SetLocalScale({ 0.5F, 0.5F ,0.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Grass_01_B.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -380.0f,0.0f,-120.0f });
		actor->GetTransform()->SetLocalRotY(135.0f);
		actor->GetTransform()->SetLocalScale({ 0.5F, 0.5F ,0.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Grass_01_B.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -380.0f,0.0f,-370.0f });
		actor->GetTransform()->SetLocalRotY(135.0f);
		actor->GetTransform()->SetLocalScale({ 0.5F, 0.5F ,0.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Grass_01_B.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -260.0f,0.0f,-370.0f });
		actor->GetTransform()->SetLocalRotY(135.0f);
		actor->GetTransform()->SetLocalScale({ 0.5F, 0.5F ,0.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Grass_01_B.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 120.0f,0.0f, 0.0f });
		actor->GetTransform()->SetLocalRotY(135.0f);
		actor->GetTransform()->SetLocalScale({ 0.5F, 0.5F ,0.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Grass_01_B.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 120.0f,0.0f, -260.0f });
		actor->GetTransform()->SetLocalRotY(135.0f);
		actor->GetTransform()->SetLocalScale({ 0.5F, 0.5F ,0.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Grass_01_B.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 120.0f,0.0f, -390.0f });
		actor->GetTransform()->SetLocalRotY(135.0f);
		actor->GetTransform()->SetLocalScale({ 0.5F, 0.5F ,0.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Grass_01_A.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 0.0f,0.0f, -380.0f });
		actor->GetTransform()->SetLocalRotY(135.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Grass_01_D.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 540.0f,0.0f, 0.0f });
		actor->GetTransform()->SetLocalRotY(135.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_01.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 540.0f,0.0f, 0.0f });
		actor->GetTransform()->SetLocalRotY(135.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_PlantGrassV2.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 250.0f,0.0f, 380.0f });
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_PlantGrassBush.FBXDATA");
		staticRender->SetShadow(true);
	}

#pragma endregion

#pragma region ROCK

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -610.0f,0.0f, -580.0f });
		//actor->GetTransform()->SetLocalRotY(135.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_forest_rock_3.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 700.0f,0.0f, -400.0f });
		//actor->GetTransform()->SetLocalRotY(135.0f);
		actor->GetTransform()->SetLocalScale({ 1.2F, 1.2F ,1.2F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_forest_rock_4.FBXDATA");
		staticRender->SetShadow(true);
	}

#pragma endregion

#pragma region FLOWER
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -550.0f,0.0f, -200.0f });
		//actor->GetTransform()->SetLocalRotY(135.0f);
		actor->GetTransform()->SetLocalScale({ 1.2F, 1.2F ,1.2F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_PlantBush.FBXDATA");
		staticRender->SetShadow(true);
	}


#pragma endregion

#pragma region SIDEROCK
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -600.0f,-170.0f, -750.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalScale({ 2.5F, 2.5F ,2.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_forest_ground_rock_26.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 0.0f,-200.0f, -750.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalScale({ 2.5F, 2.5F ,2.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_forest_ground_rock_26.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 600.0f,-140.0f, -750.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalScale({ 2.5F, 2.5F ,2.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_forest_ground_rock_26.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 600.0f,-200.0f, -600.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_01_grey.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 100.0f,-140.0f, -650.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -700.0f,-130.0f, -650.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -400.0f,-140.0f, -650.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}



	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -700.0f,-180.0f, -750.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_forest_rock_4.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 800.0f,-140.0f, -650.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalRotY(-90.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 800.0f,-140.0f, -350.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalRotY(-90.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 800.0f,-140.0f, -50.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalRotY(-90.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 800.0f,-140.0f, 350.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalRotY(-90.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 800.0f,-140.0f, 650.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalRotY(-90.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}

	//
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -800.0f,-140.0f, -650.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalRotY(90.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -800.0f,-140.0f, -350.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalRotY(90.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -800.0f,-140.0f, -50.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalRotY(90.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -800.0f,-140.0f, 350.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalRotY(90.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -800.0f,-140.0f, 650.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalRotY(90.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}

	//
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -800.0f,-140.0f, 650.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -500.0f,-140.0f, 650.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -200.0f,-140.0f, 650.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 100.0f,-140.0f, 650.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 400.0f,-140.0f, 650.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 700.0f,-140.0f, 650.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_rock_03_flower.FBXDATA");
		staticRender->SetShadow(true);
	}

	//

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -730.0f,-140.0f, -800.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.5F, 1.5F ,1.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_forest_rock_4.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 730.0f,-120.0f, -800.0f });
		actor->GetTransform()->SetLocalRotX(-90.0f);
		actor->GetTransform()->SetLocalRotY(45.0f);

		actor->GetTransform()->SetLocalScale({ 1.5F, 1.5F ,1.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_forest_rock_4.FBXDATA");
		staticRender->SetShadow(true);
	}
#pragma endregion

#pragma region FLOWER LOG GRASS BUSH
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -450.0f, 0.0f,-700.0f });

		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_Plant.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -480.0f, 0.0f, 600.0f });

		actor->GetTransform()->SetLocalScale({ 3.0F, 3.0F ,3.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_Plant.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 700.0f, 0.0f, 0.0f });

		actor->GetTransform()->SetLocalScale({ 2.5F, 2.5F ,2.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_Plant.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -290.0f, 0.0f,-700.0f });
		actor->GetTransform()->SetLocalRotY(-100.0f);
		actor->GetTransform()->SetLocalScale({ 2.0F, 1.5F ,1.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_Log_Var03.FBXDATA");
		staticRender->SetShadow(true);
	}


	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -340.0f, 50.0f,-700.0f });
		actor->GetTransform()->SetLocalScale({ 0.8F, 0.8F ,0.8F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_Mushroom_Var03.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -240.0f, 60.0f,-700.0f });
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_Mushroom_Var02.FBXDATA");
		staticRender->SetShadow(true);
	}
	//

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 710.0f, 0.0f, -170.0f });
		actor->GetTransform()->SetLocalRotY(30.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.5F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_Log_Var04.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 710.0f, 80.0f, -140.0f });
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_Mushroom_Var02.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 710.0f, 90.0f, -200.0f });
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_Mushroom_Var03.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 600.0f, 0.0f, -240.0f });
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_Mushroom_Var04.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 800.0f, 0.0f, 300.0f });
		actor->GetTransform()->SetLocalRotY(90.0f);
		actor->GetTransform()->SetLocalRotX(-45.0f);

		actor->GetTransform()->SetLocalScale({ 0.7F, 0.7F ,0.7F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_Boat.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 570.0f, 0.0f, -230.0f });
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_Flower_Var01.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 570.0f, 0.0f, -200.0f });
		actor->GetTransform()->SetLocalScale({ 2.0F, 2.0F ,2.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_Flower_Var01.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 500.0f, 0.0f, -200.0f });
		actor->GetTransform()->SetLocalRotY(90.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_Flower_Var01.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 500.0f, 0.0f, -150.0f });
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_Flower_Var01.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 550.0f, 0.0f, -180.0f });
		actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.5F, 1.5F ,1.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_Flower_Var01.FBXDATA");
		staticRender->SetShadow(true);
	}




	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 550.0f, 0.0f, -330.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.5F, 1.5F ,1.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_03.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 600.0f, 0.0f, -380.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.3F, 1.3F ,1.3F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_03.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 500.0f, 0.0f, -420.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.5F, 1.5F ,1.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_03.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 550.0f, 0.0f, -470.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_03.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 600.0f, 0.0f, -520.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.5F, 1.5F ,1.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_03.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 520.0f, 0.0f, -570.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 2.5F, 2.5F ,2.5F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_03.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 620.0f, 0.0f, -570.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_05.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 600.0f, 0.0f, -600.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_05.FBXDATA");
		staticRender->SetShadow(true);
	}

	//GRASS
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 600.0f, 0.0f, -700.0f });
		actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,0.6F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_02.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 600.0f, 0.0f, -700.0f });
		actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,0.6F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_02.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 600.0f, 0.0f, -700.0f });
		actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,0.6F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_02.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 400.0f, 0.0f, -700.0f });
		actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,0.6F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_02.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 200.0f, 0.0f, -700.0f });
		actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,0.6F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_02.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, -700.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,0.6F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_02.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -200.0f, 0.0f, -700.0f });
		actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,0.6F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_02.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -400.0f, 0.0f, -700.0f });
		actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,0.6F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_02.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -600.0f, 0.0f, -700.0f });
		actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,0.6F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"Ground_Plant_02.FBXDATA");
		staticRender->SetShadow(true);
	}



	////FLOWER LEFT

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -700.0f, 0.0f, 400.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 3.0F, 3.0F ,3.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_Fern2.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -700.0f, 0.0f, -200.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 3.0F, 3.0F ,3.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_Fern2.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 550.0f,0.0f, 400.0f });
		//actor->GetTransform()->SetLocalRotY(135.0f);
		actor->GetTransform()->SetLocalScale({ 1.2F, 1.2F ,1.2F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_PlantBush.FBXDATA");
		staticRender->SetShadow(true);
	}

	//
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -660.0f, 0.0f, 300.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_PlantGrass1.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -600.0f, 0.0f, 270.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_PlantGrassBush.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -660.0f, 0.0f, 250.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_PlantGrass1.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -600.0f, 0.0f, 230.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_PlantGrassBush.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -660.0f, 0.0f, 200.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_PlantGrassV3.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -600.0f, 0.0f, 180.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_PlantGrassV2.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -660.0f, 0.0f, 180.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_PlantGrassV3.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -600.0f, 0.0f, 150.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_PlantGrassV2.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -630.0f, 0.0f, 270.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_PlantGrass1.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -630.0f, 0.0f, 240.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_PlantGrass1.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -630.0f, 0.0f, 210.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_PlantGrass1.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -630.0f, 0.0f, 180.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_PlantGrass1.FBXDATA");
		staticRender->SetShadow(true);
	}
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -630.0f, 0.0f, 150.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SmallPlant_PlantGrass1.FBXDATA");
		staticRender->SetShadow(true);
	}

	//MINIFLOWER
	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -550.0f, 0.0f, 150.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 3.0F, 3.0F ,3.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_FlowerA.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -550.0f, 0.0f, 0.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 3.0F, 3.0F ,3.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_FlowerB.FBXDATA");
		staticRender->SetShadow(true);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ -550.0f, 0.0f, -150.0f });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor->GetTransform()->SetLocalScale({ 3.0F, 3.0F ,3.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_FlowerC.FBXDATA");
		staticRender->SetShadow(true);
	}

	Vector4 flowerPos = { 550.0f, 0.0f, 350.0f };
	for (size_t i = 0; i < 10; i++)
	{
		HPTR<Actor> actor1 = GetScene()->CreateActor();
		actor1->GetTransform()->SetLoclaPosition({ Well512Random::Instance().GetFloatValue(550.0f, 650.0f), 0.0f, flowerPos.z });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor1->GetTransform()->SetLocalScale({ 3.0F, 3.0F ,3.0F });
		HPTR<StaticFbxRender> staticRender1 = actor1->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender1->FbxSetting(L"SM_FlowerA.FBXDATA");
		staticRender1->SetShadow(true);

		HPTR<Actor> actor2 = GetScene()->CreateActor();
		actor2->GetTransform()->SetLoclaPosition({ Well512Random::Instance().GetFloatValue(550.0f, 650.0f), 0.0f, flowerPos.z });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor2->GetTransform()->SetLocalScale({ 3.0F, 3.0F ,3.0F });
		HPTR<StaticFbxRender> staticRender2 = actor2->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender2->FbxSetting(L"SM_FlowerB.FBXDATA");
		staticRender2->SetShadow(true);

		HPTR<Actor> actor3 = GetScene()->CreateActor();
		actor3->GetTransform()->SetLoclaPosition({ Well512Random::Instance().GetFloatValue(550.0f, 650.0f) , 0.0f, flowerPos.z });
		//actor->GetTransform()->SetLocalRotY(180.0f);
		actor3->GetTransform()->SetLocalScale({ 3.0F, 3.0F ,3.0F });
		HPTR<StaticFbxRender> staticRender3 = actor3->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender3->FbxSetting(L"SM_FlowerC.FBXDATA");
		staticRender3->SetShadow(true);


		flowerPos.z -= Well512Random::Instance().GetFloatValue(15.0f, 30.0f);
	}

	{
		HPTR<Actor> actor = GetScene()->CreateActor();
		actor->GetTransform()->SetLoclaPosition({ 0.0f, -40.0f, 700.0f });
		actor->GetTransform()->SetLocalRotY(45.0f);
		actor->GetTransform()->SetLocalScale({ 1.0F, 1.0F ,1.0F });
		HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
		staticRender->FbxSetting(L"SM_Arch_Alley_Rounded_05.FBXDATA");
		staticRender->SetShadow(true);
	}

	HPTR<Actor> actor3 = GetScene()->CreateActor();
	actor3->GetTransform()->SetLoclaPosition({ 62.0F , 0.0f, 62.0f });
	//actor->GetTransform()->SetLocalRotY(180.0f);
	actor3->GetTransform()->SetLocalScale({ 5.0F, 5.0F ,5.0F });
	HPTR<StaticFbxRender> staticRender3 = actor3->AddComponent<StaticFbxRender>(RG_PLAYER);
	staticRender3->FbxSetting(L"SM_FlowerC.FBXDATA");
	staticRender3->SetShadow(true);



	{
		HPTR<Actor> battleTopFrame = GetScene()->CreateActor(L"BUTTERFLY");
		battleTopFrame->GetTransform()->SetPosition({ 520.0f, 70.0f, 200.0f });
		HPTR<SpriteRenderer> battleTopFrameSR = battleTopFrame->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		battleTopFrameSR->SetSprite(L"Butterfly4X4.png", 0);
		battleTopFrameSR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		//battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
		battleTopFrameSR->LinkCB(L"VS_CUTDATA", &(battleTopFrameSR->GetCutData()));
		battleTopFrameSR->LinkTexture(L"PS_MAPTEX", L"Butterfly4X4.png");
		battleTopFrameSR->LinkSampler(L"PS_LSMP", L"LSMP");
		battleTopFrameSR->LinkSampler(L"PS_PSMP", L"PSMP");
	
		battleTopFrameSR->SetLoclaPosition({ 0.0F, 100.0F, 0.0f });
		battleTopFrameSR->SetLocalScale({ 50.0F, 50.0F, 1.0F, 1.0f });
		battleTopFrameSR->BillBoardOn();

		HPTR<SpriteAnimaion> battleTopFrameSRA = battleTopFrame->AddComponent<SpriteAnimaion>();
		battleTopFrameSRA->CreateAni(L"LOOP", L"Butterfly4X4.png", 0, 15, 0.06f);
	}

	{
		HPTR<Actor> battleTopFrame = GetScene()->CreateActor(L"BUTTERFLY");
		battleTopFrame->GetTransform()->SetPosition({ 550.0f, 100.0f, 300.0f });
		HPTR<SpriteRenderer> battleTopFrameSR = battleTopFrame->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		battleTopFrameSR->SetSprite(L"Butterfly4X4.png", 0);
		battleTopFrameSR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		//battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
		battleTopFrameSR->LinkCB(L"VS_CUTDATA", &(battleTopFrameSR->GetCutData()));
		battleTopFrameSR->LinkTexture(L"PS_MAPTEX", L"Butterfly4X4.png");
		battleTopFrameSR->LinkSampler(L"PS_LSMP", L"LSMP");
		battleTopFrameSR->LinkSampler(L"PS_PSMP", L"PSMP");

		battleTopFrameSR->SetLoclaPosition({ 0.0F, 100.0F, 0.0f });
		battleTopFrameSR->SetLocalScale({ 50.0F, 50.0F, 1.0F, 1.0f });
		battleTopFrameSR->BillBoardOn();

		HPTR<SpriteAnimaion> battleTopFrameSRA = battleTopFrame->AddComponent<SpriteAnimaion>();
		battleTopFrameSRA->CreateAni(L"LOOP", L"Butterfly4X4.png", 0, 15, 0.06f);
	}


	//HPTR<Actor> newActor = GetScene()->CreateActor(L"BUTTERFLY");
	//newActor->GetTransform()->SetPosition({ 0.0f, 0.0f, 0.0f });
	//HPTR<SpriteRenderer> skillRender = newActor->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
	//skillRender->SetSprite(L"unicornwave.png", 0);
	//skillRender->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
	////battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
	//skillRender->LinkCB(L"VS_CUTDATA", &(skillRender->GetCutData()));
	//skillRender->LinkTexture(L"PS_MAPTEX", L"unicornwave.png");
	//skillRender->LinkSampler(L"PS_LSMP", L"LSMP");
	//skillRender->LinkSampler(L"PS_PSMP", L"PSMP");

	//skillRender->SetLoclaPosition({ 0.0F, 1.0F, 0.0f });
	//skillRender->SetLocalScale({ 500.0F, 500.0F, 1.0F, 1.0f });
	//skillRender->SetLocalRotX(90.0f);

	//HPTR<SpriteAnimaion> skillAni = newActor->AddComponent<SpriteAnimaion>();
	//skillAni->CreateAni(L"LOOP", L"unicornwave.png", 0, 15, 0.1f);

#pragma endregion
	//모래
	{
		HPTR<Actor> PLAYER2 = GetScene()->CreateActor();
		PLAYER2->GetTransform()->SetLoclaPosition({ 0.0f, -550.0f, 2000.0F });
		PLAYER2->GetTransform()->SetLocalScale({ 8000.0F, 1.0F, 7000.0F });

		HPTR<Renderer> SR2 = PLAYER2->AddComponent<Renderer>(RG_PLAYER);
		Renderer::HRENDERDATA* Data = SR2->RenderData(L"WATERMESH", L"LIGHTMESHDPATH");
		//Data->m_Path->m_BUMP = BUMPOK;
		//Data->m_bShadow = true;

		HPTR<BoundingBox> Col = PLAYER2->AddComponent<BoundingBox>(RG_PLAYER);
		Col->SetLocalScale(1.0f);

		SR2->CreateCB(L"VS_CUTDATA", Vector4(0.0f, 0.0f, 10.0f, 10.0f));
		// 직접 내가 상수값을 들고 있는 경우.
		SR2->LinkTexture(L"PS_MAPTEX", L"SAND_D.png");
		SR2->LinkTexture(L"PS_BUMPTEX", L"SAND_D.png");

		SR2->LinkSampler(L"PS_LSMP", L"LSMP");
		SR2->LinkSampler(L"PS_PSMP", L"PSMP");
	}
	//WATER
	{
		HPTR<Actor> PLAYER2 = GetScene()->CreateActor();
		PLAYER2->GetTransform()->SetLoclaPosition({ 0.0f, -500.0f, 2000.0F });
		PLAYER2->GetTransform()->SetLocalScale({ 8000.0F, 1.0F, 7000.0F });

		WATERRENDER = PLAYER2->AddComponent<Renderer>(RG_PLAYER);
		Renderer::HRENDERDATA* Data = WATERRENDER->RenderData(L"WATERMESH", L"WATERMESHDPATH");
		//Data->m_Path->m_BUMP = BUMPOK;
		//Data->m_bShadow = true;

		HPTR<BoundingBox> Col = PLAYER2->AddComponent<BoundingBox>(RG_PLAYER);
		Col->SetLocalScale(1.0f);

		WATERRENDER->CreateCB(L"VS_CUTDATA", Vector4(0.0f, 0.0f, 10.0f, 10.0f));
		//WATERRENDER->CreateCB(L"PS_TIMEDATA", Vector4(0.0f, 0.0f, 0.0f, 0.0f));

		// 직접 내가 상수값을 들고 있는 경우.
		WATERRENDER->LinkTexture(L"PS_MAPTEX", L"EF_water_33_03_w.png");
		//WATERRENDER->LinkTexture(L"PS_BUMPTEX", L"Water_N2.png");

		WATERRENDER->LinkSampler(L"PS_LSMP", L"LSMP");
		WATERRENDER->LinkSampler(L"PS_PSMP", L"PSMP");
	}



	//스카이박스
	{
		skyShphere = GetScene()->CreateActor();
		skyShphere->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		skyShphere->GetTransform()->SetLocalScale({ 9000.0F, 9000.0F, 9000.0F });

		HPTR<Renderer> SR2 = skyShphere->AddComponent<Renderer>(RG_PLAYER);
		Renderer::HRENDERDATA* Data = SR2->RenderData(L"SPHEREMESH", L"SKYBOXPATH");
		//Data->m_Path->m_BUMP = BUMPOK;


		SR2->CreateCB(L"VS_CUTDATA", Vector4(0.0f, 0.0f, 1.0f, 1.0f));
		// 직접 내가 상수값을 들고 있는 경우.
		SR2->LinkTexture(L"PS_MAPTEX", L"sky_PNG5464.png");
		//SR2->LinkTexture(L"PS_BUMPTEX", L"SAND_D.png");

		SR2->LinkSampler(L"PS_LSMP", L"LSMP");
		SR2->LinkSampler(L"PS_PSMP", L"PSMP");
	}

	//돌 //Part_01_D //Part_01_N
	{
		HPTR<Actor> PLAYER2 = GetScene()->CreateActor();
		PLAYER2->GetTransform()->SetLoclaPosition({ 0.0f, -250.0f, 0.0F });
		PLAYER2->GetTransform()->SetLocalScale({ 8000.0F, 8000.0F, 8000.0F });
		PLAYER2->GetTransform()->SetLocalRotY(90.0f);


		HPTR<Renderer> SR2 = PLAYER2->AddComponent<Renderer>(RG_PLAYER);
		Renderer::HRENDERDATA* Data = SR2->RenderData(L"SPHEREMESH", L"ROCKMESHPATH");
		Data->m_Path->m_BUMP = BUMPOK;
		//Data->m_bShadow = false;

		SR2->CreateCB(L"VS_CUTDATA", Vector4(0.0f, 0.0f, 1.0f, 1.0f));
		// 직접 내가 상수값을 들고 있는 경우.
		SR2->LinkTexture(L"PS_MAPTEX", L"Part_01_D.tga");
		SR2->LinkTexture(L"PS_BUMPTEX", L"Part_01_N.tga");

		SR2->LinkSampler(L"PS_LSMP", L"LSMP");
		SR2->LinkSampler(L"PS_PSMP", L"PSMP");
	}

	////이펙트 테스트용
	//HPTR<Actor> chessPlayer2 = GetScene()->CreateActor();
	//chessPlayer2->GetTransform()->SetLocalScale({ 2.0F, 2.0F ,2.0F });
	//chessPlayer2->GetTransform()->SetLoclaPosition({ 0.0f, 120.0f, 0.0F });

	//HPTR<FbxAnimation> chessPlayerAni2 = chessPlayer2->AddComponent<FbxAnimation>(RG_PLAYER);
	//chessPlayerAni2->FbxEffectSetting(L"LittleDragonSkill.FBXDATA");
	//chessPlayerAni2->CreateAnimation(L"ATT", 0, 20, true);
	////SR->CreateAn2imation(L"ATT", 886, 909, true);
	//chessPlayerAni2->ChangeAnimation(L"ATT");


	//{
	//HPTR<Actor> actor = GetScene()->CreateActor();
	//actor->GetTransform()->SetLoclaPosition({ 0.0f, 250.0f, 0.0f });
	////actor->GetTransform()->SetLocalRotY(180.0f);
	//actor->GetTransform()->SetLocalScale({ 1.2F, 1.2F ,1.2F });
	//HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
	//
	//staticRender->FbxEffectSetting(L"twistMesh.FBXDATA");
	////staticRender->SetShadow(true);
	//
	//}

	//이펙트 테스트용
	//{
	//	HPTR<Actor> PLAYER2 = GetScene()->CreateActor();
	//	PLAYER2->GetTransform()->SetLoclaPosition({ 0.0f, 250.0f, 0.0F });
	//	PLAYER2->GetTransform()->SetLocalScale({ 30.0F, 30.0F, 30.0F });
	//	PLAYER2->GetTransform()->SetLocalRotY(90.0f);


	//	HPTR<Renderer> SR2 = PLAYER2->AddComponent<Renderer>(RG_PLAYER);
	//	Renderer::HRENDERDATA* Data = SR2->RenderData(L"SPHEREMESH", L"EFFECTDPATH");
	//	//Data->m_Path->m_BUMP = BUMPOK;
	//	//Data->m_bShadow = false;

	//	SR2->CreateCB(L"VS_CUTDATA", Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	//	// 직접 내가 상수값을 들고 있는 경우.
	//	SR2->LinkTexture(L"PS_MAPTEX", L"T_GPP_Plasma_A.tga");
	//	//SR2->LinkTexture(L"PS_BUMPTEX", L"T_GPP_Trail_Plasma_A.tga");

	//	SR2->LinkSampler(L"PS_LSMP", L"LSMP");
	//	SR2->LinkSampler(L"PS_PSMP", L"PSMP");

	//	PLAYER2->AddComponent<EffectRotate>();

	//	HPTR<SpriteRenderer> battleTopFrameSR = PLAYER2->AddComponent<SpriteRenderer>(RG_PLAYER);
	//	battleTopFrameSR->SetSprite(L"CircleEffectSprite.png", 0);
	//	battleTopFrameSR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
	//	//battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
	//	battleTopFrameSR->LinkCB(L"VS_CUTDATA", &(battleTopFrameSR->GetCutData()));
	//	battleTopFrameSR->LinkTexture(L"PS_MAPTEX", L"CircleEffectSprite.png");
	//	battleTopFrameSR->LinkSampler(L"PS_LSMP", L"LSMP");
	//	battleTopFrameSR->LinkSampler(L"PS_PSMP", L"PSMP");

	//	//battleTopFrameSR->SetLoclaPosition({ 0.0F, 100.0F, 0.0f });
	//	battleTopFrameSR->SetLocalScale({ 50.0F, 50.0F, 1.0F, 1.0f });
	//	battleTopFrameSR->BillBoardOn();

	//	HPTR<SpriteAnimaion> battleTopFrameSRA = PLAYER2->AddComponent<SpriteAnimaion>();
	//	battleTopFrameSRA->CreateAni(L"LOOP", L"CircleEffectSprite.png", 0, 24, 0.06f);

	//}

	//{
	//	HPTR<Actor> actor = GetScene()->CreateActor();
	//	actor->GetTransform()->SetLoclaPosition({ 0.0f, 250.0f, 0.0f });
	//	//actor->GetTransform()->SetLocalRotY(180.0f);
	//	actor->GetTransform()->SetLocalScale({ 0.7F, 0.7F ,0.7F });
	//	HPTR<StaticFbxRender> staticRender = actor->AddComponent<StaticFbxRender>(RG_PLAYER);
	//	
	//	staticRender->FbxEffectSetting(L"circlePlasma.FBXDATA");
	//	//staticRender->SetShadow(true);
	//	actor->AddComponent<EffectRotate>();
	//}
#pragma endregion




	Sound::Play(L"bgm_battle_embattle.wav", L"bgm_battle_embattle.wav");

}

void PlaySceneUpdater::UIInit()
{
#pragma region UI
	CombineRACE.resize(5);
	CombineCLASS.resize(5);
	CombineFIRE.resize(10);
	PieceCheacker.resize(22);
	CombineCheacker.resize(10);

{
		{
			CombineRACE[0] = GetScene()->CreateActor(L"BEAST");
			CombineRACE[0]->GetTransform()->SetScale({ 0.35f, 0.35f });
			HPTR<SpriteRenderer> SR = CombineRACE[0]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
			SR->SetSprite(L"Race.png", 2);
			SR->RenderData(L"DTEXMESH", L"UISPRITEPATH");
			SR->LinkCB(L"UIDATA", &(SR->GetUIData()));
			SR->LinkCB(L"VS_UICUTDATA", &(SR->GetCutData()));
			SR->LinkTexture(L"PS_MAPTEX", L"Race.png");
			SR->LinkSampler(L"PS_LSMP", L"LSMP");
		}
		{
			CombineRACE[1] = GetScene()->CreateActor(L"OCEAN");
			CombineRACE[1]->GetTransform()->SetScale({ 0.35f, 0.35f });
			HPTR<SpriteRenderer> SR = CombineRACE[1]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
			SR->SetSprite(L"Race.png", 5);
			SR->RenderData(L"DTEXMESH", L"UISPRITEPATH");
			SR->LinkCB(L"UIDATA", &(SR->GetUIData()));
			SR->LinkCB(L"VS_UICUTDATA", &(SR->GetCutData()));
			SR->LinkTexture(L"PS_MAPTEX", L"Race.png");
			SR->LinkSampler(L"PS_LSMP", L"LSMP");
		}
		{
			CombineRACE[2] = GetScene()->CreateActor(L"HUMAN");
			CombineRACE[2]->GetTransform()->SetScale({ 0.35f, 0.35f });
			HPTR<SpriteRenderer> SR = CombineRACE[2]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
			SR->SetSprite(L"Race.png", 6);
			SR->RenderData(L"DTEXMESH", L"UISPRITEPATH");
			SR->LinkCB(L"UIDATA", &(SR->GetUIData()));
			SR->LinkCB(L"VS_UICUTDATA", &(SR->GetCutData()));
			SR->LinkTexture(L"PS_MAPTEX", L"Race.png");
			SR->LinkSampler(L"PS_LSMP", L"LSMP");
		}
		{
			CombineRACE[3] = GetScene()->CreateActor(L"DEVIL");
			CombineRACE[3]->GetTransform()->SetScale({ 0.35f, 0.35f });
			HPTR<SpriteRenderer> SR = CombineRACE[3]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
			SR->SetSprite(L"Race.png", 10);
			SR->RenderData(L"DTEXMESH", L"UISPRITEPATH");
			SR->LinkCB(L"UIDATA", &(SR->GetUIData()));
			SR->LinkCB(L"VS_UICUTDATA", &(SR->GetCutData()));
			SR->LinkTexture(L"PS_MAPTEX", L"Race.png");
			SR->LinkSampler(L"PS_LSMP", L"LSMP");
		}
		{
			CombineRACE[4] = GetScene()->CreateActor(L"GOD");
			CombineRACE[4]->GetTransform()->SetScale({ 0.35f, 0.35f });
			HPTR<SpriteRenderer> SR = CombineRACE[4]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
			SR->SetSprite(L"Race.png", 14);
			SR->RenderData(L"DTEXMESH", L"UISPRITEPATH");
			SR->LinkCB(L"UIDATA", &(SR->GetUIData()));
			SR->LinkCB(L"VS_UICUTDATA", &(SR->GetCutData()));
			SR->LinkTexture(L"PS_MAPTEX", L"Race.png");
			SR->LinkSampler(L"PS_LSMP", L"LSMP");
		}
		{
			CombineCLASS[0] = GetScene()->CreateActor(L"WARRIOR");
			CombineCLASS[0]->GetTransform()->SetScale({ 0.35f, 0.35f });
			HPTR<SpriteRenderer> SR = CombineCLASS[0]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
			SR->SetSprite(L"Class.png", 2);
			SR->RenderData(L"DTEXMESH", L"UISPRITEPATH");
			SR->LinkCB(L"UIDATA", &(SR->GetUIData()));
			SR->LinkCB(L"VS_UICUTDATA", &(SR->GetCutData()));
			SR->LinkTexture(L"PS_MAPTEX", L"Class.png");
			SR->LinkSampler(L"PS_LSMP", L"LSMP");
		}
		{
			CombineCLASS[1] = GetScene()->CreateActor(L"HUNTER");
			CombineCLASS[1]->GetTransform()->SetScale({ 0.35f, 0.35f });
			HPTR<SpriteRenderer> SR = CombineCLASS[1]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
			SR->SetSprite(L"Class.png", 3);
			SR->RenderData(L"DTEXMESH", L"UISPRITEPATH");
			SR->LinkCB(L"UIDATA", &(SR->GetUIData()));
			SR->LinkCB(L"VS_UICUTDATA", &(SR->GetCutData()));
			SR->LinkTexture(L"PS_MAPTEX", L"Class.png");
			SR->LinkSampler(L"PS_LSMP", L"LSMP");
		}
		{
			CombineCLASS[2] = GetScene()->CreateActor(L"WARLOCK");
			CombineCLASS[2]->GetTransform()->SetScale({ 0.35f, 0.35f });
			HPTR<SpriteRenderer> SR = CombineCLASS[2]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
			SR->SetSprite(L"Class.png", 6);
			SR->RenderData(L"DTEXMESH", L"UISPRITEPATH");
			SR->LinkCB(L"UIDATA", &(SR->GetUIData()));
			SR->LinkCB(L"VS_UICUTDATA", &(SR->GetCutData()));
			SR->LinkTexture(L"PS_MAPTEX", L"Class.png");
			SR->LinkSampler(L"PS_LSMP", L"LSMP");
		}
		{
			CombineCLASS[3] = GetScene()->CreateActor(L"MAGICIAN");
			CombineCLASS[3]->GetTransform()->SetScale({ 0.35f, 0.35f });
			HPTR<SpriteRenderer> SR = CombineCLASS[3]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
			SR->SetSprite(L"Class.png", 7);
			SR->RenderData(L"DTEXMESH", L"UISPRITEPATH");
			SR->LinkCB(L"UIDATA", &(SR->GetUIData()));
			SR->LinkCB(L"VS_UICUTDATA", &(SR->GetCutData()));
			SR->LinkTexture(L"PS_MAPTEX", L"Class.png");
			SR->LinkSampler(L"PS_LSMP", L"LSMP");
		}
		{
			CombineCLASS[4] = GetScene()->CreateActor(L"DRUID");
			CombineCLASS[4]->GetTransform()->SetScale({ 0.35f, 0.35f });
			HPTR<SpriteRenderer> SR = CombineCLASS[4]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
			SR->SetSprite(L"Class.png", 9);
			SR->RenderData(L"DTEXMESH", L"UISPRITEPATH");
			SR->LinkCB(L"UIDATA", &(SR->GetUIData()));
			SR->LinkCB(L"VS_UICUTDATA", &(SR->GetCutData()));
			SR->LinkTexture(L"PS_MAPTEX", L"Class.png");
			SR->LinkSampler(L"PS_LSMP", L"LSMP");
		}
		{
			CombineBP = GetScene()->CreateActor();
			CombineBP->GetTransform()->SetScale({ 2.0f, 0.5f });
			CombineBP->GetTransform()->SetPosition({ -6.6f,1.6f,1.0f });
			HPTR<SpriteRenderer> SR = CombineBP->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
			SR->SetSprite(L"buyBg.png");
			SR->RenderData(L"DTEXMESH", L"UISPRITEPATH");
			SR->LinkCB(L"UIDATA", &(SR->GetUIData()));
			SR->LinkCB(L"VS_UICUTDATA", &(SR->GetCutData()));
			SR->LinkTexture(L"PS_MAPTEX", L"buyBg.png");
			SR->LinkSampler(L"PS_LSMP", L"LSMP");
			CombineBP->UpdateOff();
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		CombineFIRE[i] = GetScene()->CreateActor();
		CombineFIRE[i]->GetTransform()->SetScale({ 0.8f, 0.8f });

		HPTR<SpriteRenderer> SR = CombineFIRE[i]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
		SR->SetSprite(L"FireRing.png");
		SR->RenderData(L"DTEXMESH", L"UISPRITEPATH");
		SR->LinkCB(L"UIDATA", &(SR->GetUIData()));
		SR->LinkCB(L"VS_UICUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"FireRing.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");

		HPTR<SpriteAnimaion> SA = CombineFIRE[i]->AddComponent<SpriteAnimaion>();
		SA->CreateAni(L"LOOP", L"FireRing.png", 0, 12, 0.08f);


		CombineFIRE[i]->UpdateOff();
	}

	for (int i = 0; i < 5; ++i)
	{
		CombineRACE[i]->GetTransform()->SetPosition({ -7.4f + (i * 0.4f),3.0f,1.0f });
		CombineCLASS[i]->GetTransform()->SetPosition({ -7.4f + (i * 0.4f),2.4f,1.0f });

		CombineFIRE[i]->GetTransform()->SetPosition({ -7.4f + (i * 0.4f),3.0f,2.0f });
		CombineFIRE[i + 5]->GetTransform()->SetPosition({ -7.4f + (i * 0.4f),2.4f,2.0f });
	}


	{
		unitSeller = GetScene()->CreateActor(L"buyBg");
		unitSeller->GetTransform()->SetScale({ 16.0f, 5.0f });
		//BGIMG->GetTransform()->LocalScaleMul(10.0f);
		unitSeller->GetTransform()->SetPosition({ 0.0f,4.2f,0.0f });
		//monster->GetTransform()->SetScale({ 640.0f / GameWindow::Inst()->GetWindowSize().y , 480.0f / GameWindow::Inst()->GetWindowSize().y });
		//monster->GetTransform()->LocalScaleMultiple(GameWindow::Inst()->GetWindowSize().y / 100.0f);
		HPTR<SpriteRenderer>  battleTopFrameSR = unitSeller->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
		battleTopFrameSR->SetSprite(L"buyBg.png", 0);
		battleTopFrameSR->RenderData(L"DTEXMESH", L"UISPRITEPATH");
		battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
		battleTopFrameSR->LinkCB(L"VS_UICUTDATA", &(battleTopFrameSR->GetCutData()));
		battleTopFrameSR->LinkTexture(L"PS_MAPTEX", L"buyBg.png");
		battleTopFrameSR->LinkSampler(L"PS_LSMP", L"LSMP");
		battleTopFrameSR->LinkSampler(L"PS_PSMP", L"PSMP");
		unitSeller->UpdateOff();
	}

	{
		HPTR<Actor> battleTopFrame = GetScene()->CreateActor(L"EatDragonSprite");
		battleTopFrame->GetTransform()->SetScale({ 2.0f, 2.0f });
		//BGIMG->GetTransform()->LocalScaleMul(10.0f);
		battleTopFrame->GetTransform()->SetPosition({ 7.0f,-3.2f,0.0f });
		//monster->GetTransform()->SetScale({ 640.0f / GameWindow::Inst()->GetWindowSize().y , 480.0f / GameWindow::Inst()->GetWindowSize().y });
		//monster->GetTransform()->LocalScaleMultiple(GameWindow::Inst()->GetWindowSize().y / 100.0f);
		HPTR<SpriteRenderer> battleTopFrameSR = battleTopFrame->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
		battleTopFrameSR->SetSprite(L"EatDragonSprite2.png", 0);
		battleTopFrameSR->RenderData(L"DTEXMESH", L"UISPRITEPATH");
		battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
		battleTopFrameSR->LinkCB(L"VS_UICUTDATA", &(battleTopFrameSR->GetCutData()));
		battleTopFrameSR->LinkTexture(L"PS_MAPTEX", L"EatDragonSprite2.png");
		battleTopFrameSR->LinkSampler(L"PS_LSMP", L"LSMP");
		battleTopFrameSR->LinkSampler(L"PS_PSMP", L"PSMP");

		HPTR<SpriteAnimaion> battleTopFrameSRA = battleTopFrame->AddComponent<SpriteAnimaion>();
		battleTopFrameSRA->CreateAni(L"LOOP", L"EatDragonSprite2.png", 0, 60, 0.03f);
	}

	GameManager::Inst().Init(GetScene());

	{

		Mouse = GetScene()->CreateActor(L"Mouse");
		Mouse->GetTransform()->SetPosition({ 0.0f,0.0f,-99.0f });
		Mouse->GetTransform()->SetScale({ 1.0f, 1.0f });

		HPTR<SpriteRenderer> SR = Mouse->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
		SR->SetSprite(L"MouseCursor.png", 0);
		SR->RenderData(L"DTEXMESH", L"UISPRITEPATH");
		SR->LinkCB(L"UIDATA", &(SR->GetUIData()));
		SR->LinkCB(L"VS_UICUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"MouseCursor.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");


		HPTR<BoundingCircle> Col = Mouse->AddComponent<BoundingCircle>(RG_UI);
		Col->SetLocalScale(10.0f);

		UIMouse = GetScene()->CreateActor(L"UIMouse");
		UIMouse->GetTransform()->SetPosition({ 0.0f,0.0f, 0.0f });

		/*HPTR<BoudingRect> uiCol = UIMouse->AddComponent<BoudingRect>(RG_UI);
		uiCol->SetLocalScale(100.0f);*/
	}

	//BattleTopFrame
	{
		HPTR<Actor> battleTopFrame = GetScene()->CreateActor(L"BattleTopFrame");
		battleTopFrame->GetTransform()->SetScale({ 6.0f, 1.0f });
		//BGIMG->GetTransform()->LocalScaleMul(10.0f);
		battleTopFrame->GetTransform()->SetPosition({ 0.0f,4.2f, -98.0f });
		//monster->GetTransform()->SetScale({ 640.0f / GameWindow::Inst()->GetWindowSize().y , 480.0f / GameWindow::Inst()->GetWindowSize().y });
		//monster->GetTransform()->LocalScaleMultiple(GameWindow::Inst()->GetWindowSize().y / 100.0f);
		HPTR<SpriteRenderer> battleTopFrameSR = battleTopFrame->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
		battleTopFrameSR->SetSprite(L"BattleTopFrame.png", 0);
		battleTopFrameSR->RenderData(L"DTEXMESH", L"UISPRITEPATH");
		battleTopFrameSR->LinkCB(L"UIDATA", &(battleTopFrameSR->GetUIData()));
		battleTopFrameSR->LinkCB(L"VS_UICUTDATA", &(battleTopFrameSR->GetCutData()));
		battleTopFrameSR->LinkTexture(L"PS_MAPTEX", L"BattleTopFrame.png");
		battleTopFrameSR->LinkSampler(L"PS_LSMP", L"LSMP");
		battleTopFrameSR->LinkSampler(L"PS_PSMP", L"PSMP");
	}


	SellerLogic::Inst().Init(GetScene());

#pragma endregion



}

void PlaySceneUpdater::MoveChessPiece()
{
	destIdx = { 0, 0 };
	BoardIndex startIdx = ChessBoard::Inst().CalculatePickIngIndex();

	//if ((startIdx.x >= 0 && startIdx.x <= 7 && startIdx.z >= 0 && startIdx.z <= 9))
	//{

	if (Input::Down(L"CLICK") == true)
	{
		Sound::Play(L"click.wav");
	}


		if (Input::Press(L"CLICK") == true)
		{
			if (isPick == false)
			{
				startIdx = ChessBoard::Inst().CalculatePickIngIndex();

				if (startIdx.z >= 0 && startIdx.z < 8 && startIdx.x >= 0 && startIdx.x < 8)
				{
					pickActor = ChessBoard::Inst().GetActor(startIdx);
					if (pickActor == nullptr)
					{
						return;
					}
					ChessBoard::Inst().PopAllyPiece(pickActor);
				}
				else if (startIdx.z == -1 && startIdx.x == -1)
				{

				}
				else //여기가 보유유닛판
				{
					pickActor = ChessBoard::Inst().PopActorInStay(startIdx.x);
					if (pickActor == nullptr)
					{
						return;
					}
				}
				isPick = true;
			}
			if (pickActor != nullptr)
			{
				prevSoundIdx = destIdx;
				destIdx = ChessBoard::Inst().CalculatePickIngIndex();
				if (destIdx.z != -1 || destIdx.x != -1)
				{
					if (destIdx.z != 9)
					{
						pickActor->GetTransform()->SetLoclaPosition(ChessBoard::Inst().m_Board[destIdx.z][destIdx.x].m_BoardPos);
					}
					else
					{
						pickActor->GetTransform()->SetLoclaPosition(ChessBoard::Inst().m_BoardStay[destIdx.x].m_BoardPos);
					}
					
				}
				else if (destIdx.z == -1 && destIdx.x == -1)
				{

				}
			}
		}
		else if (Input::Up(L"CLICK") == true)
		{
			destIdx = ChessBoard::Inst().CalculatePickIngIndex();
			if (pickActor != nullptr)
			{
				ChessBoard::Inst().PopPiece(pickActor);
				if (destIdx.z >= 0 && destIdx.z < 8 && destIdx.x >= 0 && destIdx.x < 8)
				{
					Sound::Play(L"chessmove.wav");
					ChessBoard::Inst().PushAllyPiece(pickActor);
					ChessBoard::Inst().SetPosActor(pickActor, destIdx);
				}
				else if (destIdx.z == -1 || destIdx.x == -1)
				{
					//ChessBoard::Inst().PushAllyPiece(pickActor);
					//ChessBoard::Inst().SetPosActor(pickActor, destIdx);
					if (pickActor != nullptr)
					{
						ChessBoard::Inst().PopAllyPiece(pickActor);
						pickActor->Die();
						pickActor = nullptr;

						Sound::Play(L"chessleave.wav");
					}
				}
				else if (destIdx.z == 9)
				{
					ChessBoard::Inst().PushActorInStay(startIdx, destIdx, pickActor);
				}
				pickActor = nullptr;

			}
			isPick = false;
		}
	//}
	//else	// -1, -1
	//{

	//}
}

void PlaySceneUpdater::CheatUpgradePiece()
{
	BoardIndex pickIdx = ChessBoard::Inst().CalculatePickIngIndex();

	if (pickIdx.x != -1 && pickIdx.z != -1)
	{
		ChessBoard::Inst().CheatUpgradePiece(pickIdx);
		Sound::Play(L"compose.wav");
	}
}

void PlaySceneUpdater::PurchaseChessPiece()
{
	if (Input::Down(L"SPACE") == true)
	{
		Sound::Play(L"commonbtn.wav");
		if (unitSeller->IsUpdate() == true)
		{
			unitSeller->UpdateOff();
			SellerLogic::Inst().AllOff();
		}
		else
		{
			unitSeller->UpdateOn();
			SellerLogic::Inst().AllOn();
		}
	}

	if (unitSeller->IsUpdate() == true)
	{
		SellerLogic::Inst().Update();

		Vector4 UIMousePos = Mouse->GetTransform()->GetLocalPosition();

		if (Input::Down(L"CLICK") == true)
		{
			Sound::Play(L"chessbuy.wav");
			//Sound::Play(L"click.wav");
			if (UIMousePos.x > -6.5f && UIMousePos.x <-5.5f
				&& UIMousePos.y > 2.1f && UIMousePos.y < 4.1f)
			{
				if (SellerLogic::Inst().GetIdxUpdate(0) != false && ChessBoard::Inst().GetBoardStaySize() < 8)
				{
					std::wcout << L"1";



					int idx = SellerLogic::Inst().GetChessIdx(0);
					Actor* newActor = SellerLogic::Inst().CreatePiece(idx);

					ChessBoard::Inst().PushActorInStayAutoSort(newActor, idx);

					SellerLogic::Inst().OffIndex(0);
				}
			}
			else if (UIMousePos.x > -3.5f && UIMousePos.x <-2.5f
				&& UIMousePos.y > 2.1f && UIMousePos.y < 4.1f)
			{
				if (SellerLogic::Inst().GetIdxUpdate(1) != false && ChessBoard::Inst().GetBoardStaySize() < 8)
				{
					int idx = SellerLogic::Inst().GetChessIdx(1);
					Actor* newActor = SellerLogic::Inst().CreatePiece(idx);
					ChessBoard::Inst().PushActorInStayAutoSort(newActor, idx);

					std::wcout << L"2";
					SellerLogic::Inst().OffIndex(1);
				}
			}
			else if (UIMousePos.x > -0.5f && UIMousePos.x < 0.5f
				&& UIMousePos.y > 2.1f && UIMousePos.y < 4.1f)
			{
				if (SellerLogic::Inst().GetIdxUpdate(2) != false && ChessBoard::Inst().GetBoardStaySize() < 8)
				{

					int idx = SellerLogic::Inst().GetChessIdx(2);
					Actor* newActor = SellerLogic::Inst().CreatePiece(idx);

					ChessBoard::Inst().PushActorInStayAutoSort(newActor, idx);

					std::wcout << L"3";

					SellerLogic::Inst().OffIndex(2);
				}
			}
			else if (UIMousePos.x > 2.5f && UIMousePos.x < 3.5f
				&& UIMousePos.y > 2.1f && UIMousePos.y < 4.1f)
			{
				if (SellerLogic::Inst().GetIdxUpdate(3) != false && ChessBoard::Inst().GetBoardStaySize() < 8)
				{
					int idx = SellerLogic::Inst().GetChessIdx(3);
					Actor* newActor = SellerLogic::Inst().CreatePiece(idx);
					ChessBoard::Inst().PushActorInStayAutoSort(newActor, idx);
					SellerLogic::Inst().OffIndex(3);
					std::wcout << L"4";
				}
			}
			else if (UIMousePos.x > 5.5f && UIMousePos.x <6.5f
				&& UIMousePos.y > 2.1f && UIMousePos.y < 4.1f)
			{
				if (SellerLogic::Inst().GetIdxUpdate(4) != false && ChessBoard::Inst().GetBoardStaySize() < 8)
				{
					int idx = SellerLogic::Inst().GetChessIdx(4);
					Actor* newActor = SellerLogic::Inst().CreatePiece(idx);
					ChessBoard::Inst().PushActorInStayAutoSort(newActor, idx);
					SellerLogic::Inst().OffIndex(4);
					std::wcout << L"5";
				}
			}
		}
	}
}

void PlaySceneUpdater::CombinationPiece()
{
	
	for (int i = 0; i < 22; ++i)
	{
		PieceCheacker[i] = false;
	}
	for (int i = 0; i < 10; ++i)
	{
		CombineCheacker[i] = 0;
	}

	CombineBP->UpdateOff(); 
	
	if (unitSeller->IsUpdate() == true)
	{
		for (int i = 0; i < 5; i++)
		{
			CombineRACE[i]->UpdateOff();
			CombineCLASS[i]->UpdateOff();
			CombineFIRE[i]->UpdateOff();
			CombineFIRE[i + 5]->UpdateOff();
		}
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			CombineRACE[i]->UpdateOn();
			CombineCLASS[i]->UpdateOn();
		}

		for (int z = 0; z < 8; z++)
		{
			for (int x = 0; x < 8; x++)
			{
				Actor* target = ChessBoard::Inst().m_Board[z][x].m_Actor;
				if (target != nullptr)
				{
					StatusLogic* statusLogic = target->GetComponent<StatusLogic>();
					if (statusLogic->GetType() == PIECETYPE::ALLY)
					{
						if (-1 != statusLogic->GetNUMBER())
						{
							PieceCheacker[statusLogic->GetNUMBER()] = true;
						}
					}
				}
			}
		}



		if (PieceCheacker[0] == true)
		{
			CombineCheacker[1]++;
			CombineCheacker[5]++;
		}
		if (PieceCheacker[1] == true)
		{
			CombineCheacker[0]++;
			CombineCheacker[9]++;
		}
		if (PieceCheacker[2] == true)
		{
			CombineCheacker[4]++;
			CombineCheacker[5]++;
		}
		if (PieceCheacker[3] == true)
		{
			CombineCheacker[2]++;
			CombineCheacker[5]++;
		}
		if (PieceCheacker[4] == true)
		{
			CombineCheacker[2]++;
			CombineCheacker[8]++;
		}
		if (PieceCheacker[5] == true)
		{
			CombineCheacker[0]++;
			CombineCheacker[8]++;
		}
		if (PieceCheacker[6] == true)
		{
			CombineCheacker[2]++;
			CombineCheacker[6]++;
		}
		if (PieceCheacker[7] == true)
		{
			CombineCheacker[0]++;
			CombineCheacker[5]++;
		}
		if (PieceCheacker[8] == true)
		{
			CombineCheacker[0]++;
			CombineCheacker[7]++;
		}
		if (PieceCheacker[9] == true)
		{
			CombineCheacker[0]++;
			CombineCheacker[5]++;
		}
		if (PieceCheacker[10] == true)
		{
			CombineCheacker[2]++;
			CombineCheacker[8]++;
		}
		if (PieceCheacker[11] == true)
		{
			CombineCheacker[2]++;
			CombineCheacker[6]++;
		}
		if (PieceCheacker[12] == true)
		{
			CombineCheacker[3]++;
			CombineCheacker[8]++;
		}
		if (PieceCheacker[13] == true)
		{
			CombineCheacker[0]++;
			CombineCheacker[9]++;
		}
		if (PieceCheacker[14] == true)
		{
			CombineCheacker[1]++;
			CombineCheacker[6]++;
		}
		if (PieceCheacker[15] == true)
		{
			CombineCheacker[1]++;
			CombineCheacker[5]++;
		}
		if (PieceCheacker[16] == true)
		{
			CombineCheacker[2]++;
			CombineCheacker[8]++;
		}
		if (PieceCheacker[17] == true)
		{
			CombineCheacker[3]++;
			CombineCheacker[7]++;
		}
		if (PieceCheacker[18] == true)
		{
			CombineCheacker[1]++;
			CombineCheacker[6]++;
		}
		if (PieceCheacker[19] == true)
		{

		}
		if (PieceCheacker[20] == true)
		{
			CombineCheacker[4]++;
			CombineCheacker[8]++;
		}
		if (PieceCheacker[21] == true)
		{
			CombineCheacker[3]++;
			CombineCheacker[7]++;
		}

		for (int i = 0; i < 5; ++i)
		{
			if (CombineCheacker[i] == 0)
			{
				CombineRACE[i]->GetComponent<SpriteRenderer>()->LinkTexture(L"PS_MAPTEX", L"Race_HF.png");
			}
			else
			{
				CombineRACE[i]->GetComponent<SpriteRenderer>()->LinkTexture(L"PS_MAPTEX", L"Race.png");
			}
		}

		for (int i = 0; i < 5; ++i)
		{
			if (CombineCheacker[i + 5] == 0)
			{
				CombineCLASS[i]->GetComponent<SpriteRenderer>()->LinkTexture(L"PS_MAPTEX", L"Class_HF.png");
			}
			else
			{
				CombineCLASS[i]->GetComponent<SpriteRenderer>()->LinkTexture(L"PS_MAPTEX", L"Class.png");
			}
		}



		if (CombineCheacker[0] >= 6)
		{
			if (CombineFIRE[0]->IsUpdate() == false)
			{
				CombineFIRE[0]->UpdateOn();
			}
		}
		else
		{
			CombineFIRE[0]->UpdateOff();
		}

		if (CombineCheacker[1] >= 4)
		{
			if (CombineFIRE[1]->IsUpdate() == false)
			{
				CombineFIRE[1]->UpdateOn();
			}
		}
		else
		{
			CombineFIRE[1]->UpdateOff();
		}

		if (CombineCheacker[2] >= 6)
		{
			if (CombineFIRE[2]->IsUpdate() == false)
			{
				CombineFIRE[2]->UpdateOn();
			}
		}
		else
		{
			CombineFIRE[2]->UpdateOff();
		}

		if (CombineCheacker[3] >= 3)
		{
			if (CombineFIRE[3]->IsUpdate() == false)
			{
				CombineFIRE[3]->UpdateOn();
			}
		}
		else
		{
			CombineFIRE[3]->UpdateOff();
		}

		if (CombineCheacker[4] >= 2)
		{
			if (CombineFIRE[4]->IsUpdate() == false)
			{
				CombineFIRE[4]->UpdateOn();
			}
		}
		else
		{
			CombineFIRE[4]->UpdateOff();
		}

		if (CombineCheacker[5] >= 6)
		{
			if (CombineFIRE[5]->IsUpdate() == false)
			{
				CombineFIRE[5]->UpdateOn();
			}
		}
		else
		{
			CombineFIRE[5]->UpdateOff();
		}

		if (CombineCheacker[6] >= 4)
		{
			if (CombineFIRE[6]->IsUpdate() == false)
			{
				CombineFIRE[6]->UpdateOn();
			}
		}
		else
		{
			CombineFIRE[6]->UpdateOff();
		}

		if (CombineCheacker[7] >= 3)
		{
			if (CombineFIRE[7]->IsUpdate() == false)
			{
				CombineFIRE[7]->UpdateOn();
			}
		}
		else
		{
			CombineFIRE[7]->UpdateOff();
		}

		if (CombineCheacker[8] >= 6)
		{
			if (CombineFIRE[8]->IsUpdate() == false)
			{
				CombineFIRE[8]->UpdateOn();
			}
		}
		else
		{
			CombineFIRE[8]->UpdateOff();
		}

		if (CombineCheacker[9] >= 2)
		{
			if (CombineFIRE[9]->IsUpdate() == false)
			{
				CombineFIRE[9]->UpdateOn();
			}
		}
		else
		{
			CombineFIRE[9]->UpdateOff();
		}

		if (Input::Press(L"LCLICK") == true)
		{
			if (MouseInTheBox(CombineRACE[0]) ||
				MouseInTheBox(CombineRACE[1]) ||
				MouseInTheBox(CombineRACE[2]) ||
				MouseInTheBox(CombineRACE[3]) ||
				MouseInTheBox(CombineRACE[4]) ||
				MouseInTheBox(CombineCLASS[0]) ||
				MouseInTheBox(CombineCLASS[1]) ||
				MouseInTheBox(CombineCLASS[2]) ||
				MouseInTheBox(CombineCLASS[3]) ||
				MouseInTheBox(CombineCLASS[4]))
			{
				CombineBP->UpdateOn();
			}
		}

	}
}

void PlaySceneUpdater::CombinationLetter()
{
	if (unitSeller->IsUpdate() == false)
	{
		{
			std::wstring mouse;
			mouse += std::to_wstring(CombineCheacker[0]);
			mouse += L"/6";
			Font->DrawUI(mouse.c_str(), 17.0f, { 60.0f - 13.0f, 150.f + 20.0f }, Vector4::WHITE);
		}
		{
			std::wstring mouse;
			mouse += std::to_wstring(CombineCheacker[1]);
			mouse += L"/4";
			Font->DrawUI(mouse.c_str(), 17.0f, { 60.0f - 13.0f + 40.0f  , 150.f + 20.0f }, Vector4::WHITE);
		}
		{
			std::wstring mouse;
			mouse += std::to_wstring(CombineCheacker[2]);
			mouse += L"/6";
			Font->DrawUI(mouse.c_str(), 17.0f, { 60.0f - 13.0f + 40.0f + 40.0f  , 150.f + 20.0f }, Vector4::WHITE);
		}
		{
			std::wstring mouse;
			mouse += std::to_wstring(CombineCheacker[3]);
			mouse += L"/3";
			Font->DrawUI(mouse.c_str(), 17.0f, { 60.0f - 13.0f + 40.0f + 40.0f + 40.0f  , 150.f + 20.0f }, Vector4::WHITE);
		}
		{
			std::wstring mouse;
			mouse += std::to_wstring(CombineCheacker[4]);
			mouse += L"/2";
			Font->DrawUI(mouse.c_str(), 17.0f, { 60.0f - 13.0f + 40.0f + 40.0f + 40.0f + 40.0f  , 150.f + 20.0f }, Vector4::WHITE);
		}
		{
			std::wstring mouse;
			mouse += std::to_wstring(CombineCheacker[5]);
			mouse += L"/6";
			Font->DrawUI(mouse.c_str(), 17.0f, { 60.0f - 13.0f , 150.f + 20.0f + 60.0f }, Vector4::WHITE);
		}
		{
			std::wstring mouse;
			mouse += std::to_wstring(CombineCheacker[6]);
			mouse += L"/4";
			Font->DrawUI(mouse.c_str(), 17.0f, { 60.0f - 13.0f + 40.0f  , 150.f + 20.0f + 60.0f }, Vector4::WHITE);
		}
		{
			std::wstring mouse;
			mouse += std::to_wstring(CombineCheacker[7]);
			mouse += L"/3";
			Font->DrawUI(mouse.c_str(), 17.0f, { 60.0f - 13.0f + 40.0f + 40.0f  , 150.f + 20.0f + 60.0f }, Vector4::WHITE);
		}
		{
			std::wstring mouse;
			mouse += std::to_wstring(CombineCheacker[8]);
			mouse += L"/6";
			Font->DrawUI(mouse.c_str(), 17.0f, { 60.0f - 13.0f + 40.0f + 40.0f + 40.0f , 150.f + 20.0f + 60.0f }, Vector4::WHITE);
		}
		{
			std::wstring mouse;
			mouse += std::to_wstring(CombineCheacker[9]);
			mouse += L"/2";
			Font->DrawUI(mouse.c_str(), 17.0f, { 60.0f - 13.0f + 40.0f + 40.0f + 40.0f + 40.0f  , 150.f + 20.0f + 60.0f }, Vector4::WHITE);
		}

		if (CombineBP->IsUpdate() == true)
		{
			std::wstring mouse;
			Vector4 LetterColor;

			if (MouseInTheBox(CombineRACE[0]))
			{
				mouse += L"야수족 : 전체 공격력 상승";
				if (CombineCheacker[0] >= 6)
				{
					LetterColor = Vector4::WHITE;
				}
				else
				{
					LetterColor = Vector4::GRAY;
				}
			}
			if (MouseInTheBox(CombineRACE[1]))
			{
				mouse += L"해양족 : 전체 마법 저항 상승";
				if (CombineCheacker[1] >= 4)
				{
					LetterColor = Vector4::WHITE;
				}
				else
				{
					LetterColor = Vector4::GRAY;
				}
			}
			if (MouseInTheBox(CombineRACE[2]))
			{
				mouse += L"인간족 : 전체 체력 상승";
				if (CombineCheacker[2] >= 6)
				{
					LetterColor = Vector4::WHITE;
				}
				else
				{
					LetterColor = Vector4::GRAY;
				}
			}
			if (MouseInTheBox(CombineRACE[3]))
			{
				mouse += L"악마족 : 악마족 공격력 상승";
				if (CombineCheacker[3] >= 3)
				{
					LetterColor = Vector4::WHITE;
				}
				else
				{
					LetterColor = Vector4::GRAY;
				}
			}
			if (MouseInTheBox(CombineRACE[4]))
			{
				mouse += L"신 : 스킬 쿨타임 단축";
				if (CombineCheacker[4] >= 2)
				{
					LetterColor = Vector4::WHITE;
				}
				else
				{
					LetterColor = Vector4::GRAY;
				}
			}
			if (MouseInTheBox(CombineCLASS[0]))
			{
				mouse += L"전사 : 전사 방어력 상승";
				if (CombineCheacker[5] >= 6)
				{
					LetterColor = Vector4::WHITE;
				}
				else
				{
					LetterColor = Vector4::GRAY;
				}
			}
			if (MouseInTheBox(CombineCLASS[1]))
			{
				mouse += L"사냥꾼 : 사냥꾼 명중률 상승";
				if (CombineCheacker[6] >= 4)
				{
					LetterColor = Vector4::WHITE;
				}
				else
				{
					LetterColor = Vector4::GRAY;
				}
			}
			if (MouseInTheBox(CombineCLASS[2]))
			{
				mouse += L"흑마법사 : 적 전체 방어력 저하";
				if (CombineCheacker[7] >= 3)
				{
					LetterColor = Vector4::WHITE;
				}
				else
				{
					LetterColor = Vector4::GRAY;
				}
			}
			if (MouseInTheBox(CombineCLASS[3]))
			{
				mouse += L"마법사 : 적 전체 마법 저항 저하";
				if (CombineCheacker[8] >= 6)
				{
					LetterColor = Vector4::WHITE;
				}
				else
				{
					LetterColor = Vector4::GRAY;
				}
			}
			if (MouseInTheBox(CombineCLASS[4]))
			{
				mouse += L"드루이드 : 조합 비용 감소";
				if (CombineCheacker[9] >= 2)
				{
					LetterColor = Vector4::WHITE;
				}
				else
				{
					LetterColor = Vector4::GRAY;
				}
			}

			Font->DrawUI(mouse.c_str(), 15.0f, { 50.f, 280.f, -5.0f }, LetterColor);

		}
	}
}
