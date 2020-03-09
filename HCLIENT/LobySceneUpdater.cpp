#include "LobySceneUpdater.h"

//BASE
#include <INPUT.h>
#include <GAMEWIN.h>
#include <RANDOM.h>
#include <JTIME.h>

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

//CLIENT
#include "LOGICHEADER.h"

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
#include <SOUND.h>


wchar_t LobArr[256];
bool GotoCollScene = false;
float buttonMouseChecker = -1.0f;

int LBchecker = 1;

HPTR<Light> LightCheck01;

void LobySceneUpdater::Loading()
{
	//Sound bgm;
	//bgm.Play(L"bgm_1.wav");
	Sound::Play(L"bgm_1.wav", L"bgm_1.wav", 999);

	
	Sound::Play(L"p005_showup_02_eng.wav");
	
	Font = Resources::Find<HEFONT>(L"1»∆∂±∫∫¿Ã");


	{
		HPTR<Actor> mainCam = GetScene()->CreateActor(L"LOADINGCAM");
		mainCam->GetTransform()->SetPosition({ 0.0f, 0.0f, -10.0f });
		HPTR<Camera> camera = mainCam->AddComponent<Camera>(RENDERGROUP::RG_UI);
		camera->SetProjMode(PROJMODE::PM_ORTH);
		camera->SetArea(GameWindow::MainGameWin()->Size().x / 100.0f, GameWindow::MainGameWin()->Size().y / 100.0f);
	}


	HPTR<Actor> Light01 = GetScene()->CreateActor();
	Light01->GetTransform()->SetLocalRotX(45.0f);
	Light01->GetTransform()->SetLocalRotY(45.0f);
	Light01->GetTransform()->SetLoclaPosition({ 0.0f, 300.0f, -15000.0f });
	LightCheck01 = Light01->AddComponent<Light>(RG_PLAYER);
	LightCheck01->SetLightType(Light::LT_DIR);


	{
		HPTR<Actor> subCam = GetScene()->CreateActor(L"MAINCAM");
		subCam->GetTransform()->SetLoclaPosition({ 0.0f, 300.0f, -750.0f });
		subCam->GetTransform()->SetLocalRot({ 0.0f, 0.0f, 0.0f });

		HPTR<Camera> camCom = subCam->AddComponent<Camera>(RG_PLAYER);
		camCom->Order(0);
		camCom->SetFarValue(100000.0f);
		camCom->SetProjMode(PROJMODE::PM_PERS);
		camCom->SetArea((WIN_SIZE_X / 100), (WIN_SIZE_Y / 100));
	}


	{
		chessPlayer = GetScene()->CreateActor();
		chessPlayer->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, -5.0F });
		chessPlayer->GetTransform()->SetScale({ 2.5f, 2.5f, 2.5f });
		//	chessPlayer->GetTransform()->SetLocalRot({ 90.0f, 0.0f, 0.0f });

		HPTR<FbxAnimation> chessPlayerAni = chessPlayer->AddComponent<FbxAnimation>(RG_PLAYER);
		chessPlayerAni->FbxSetting(L"EatingDragon.FBXDATA");
		chessPlayerAni->CreateAnimation(L"ROLE", 136, 332, false);
		chessPlayerAni->CreateAnimation(L"IDLE", 333, 363, true);
		chessPlayerAni->ChangeAnimation(L"IDLE");
	}


	Resources::Create<Sprite>(L"LBScene.png", L"LBScene.png", 1, 1, 0, 1);
	Resources::Create<Sprite>(L"LBCloud.png", L"LBCloud.png", 1, 1, 0, 1);
	Resources::Create<Sprite>(L"CCbtn.png", L"CCbtn.png", 2, 2, 0, 4);
	Resources::Create<Sprite>(L"STbtn.png", L"STbtn.png", 5, 2, 0, 10);
	Resources::Create<Sprite>(L"Shival.png", L"Shival.png", 1, 1, 0, 1);
//	Resources::Create<Sprite>(L"AlphaPlate.png", L"AlphaPlate.png", 8, 4, 0, 32);

	{
		BCloud1 = GetScene()->CreateActor();
		BCloud1->GetTransform()->SetScale({ 12.0f, 4.8f });
		BCloud1->GetTransform()->SetLoclaPosition({ -2.0f, 4.5f - 2.4f, 1.0f });
		HPTR<SpriteRenderer> SR = BCloud1->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
		SR->SetSprite(L"LBCloud.png", 0);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"LBCloud.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
	}

	{
		BCloud2 = GetScene()->CreateActor();
		BCloud2->GetTransform()->SetScale({ 12.0f, 4.8f });
		BCloud2->GetTransform()->SetLoclaPosition({ 0.0f, 4.5f - 2.4f, 1.0f });
		HPTR<SpriteRenderer> SR = BCloud2->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
		SR->SetSprite(L"LBCloud.png", 0);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"LBCloud.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
	}

	{
		BCloud3 = GetScene()->CreateActor();
		BCloud3->GetTransform()->SetScale({ 12.0f, 4.8f });
		BCloud3->GetTransform()->SetLoclaPosition({ 0.0f, 4.5f - 2.4f, 1.0f });
		HPTR<SpriteRenderer> SR = BCloud3->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
		SR->SetSprite(L"LBCloud.png", 0);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"LBCloud.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
	}


	{
		BPlate = GetScene()->CreateActor();
		BPlate->GetTransform()->SetScale({ 16.0f, 9.0f });
		HPTR<SpriteRenderer> SR = BPlate->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
		SR->SetSprite(L"LBScene.png", 0);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"LBScene.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
	}

	{
		HPTR<Actor> UpperBar = GetScene()->CreateActor();
		UpperBar->GetTransform()->SetScale({ 16.0f, 0.8f });
		UpperBar->GetTransform()->SetLoclaPosition({ 0.0f, 4.1f, -2.0f });
		HPTR<SpriteRenderer> SR = UpperBar->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);
		SR->SetSprite(L"Shival.png");

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"Shival.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
	}

	{
		CLSceneButton = GetScene()->CreateActor();
		CLSceneButton->GetTransform()->SetScale({ 0.4f, 0.4f });
		CLSceneButton->GetTransform()->SetPosition({ 6.0f, 4.2f, -3.0f });
		HPTR<SpriteRenderer> SR = CLSceneButton->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
		SR->SetSprite(L"CCbtn.png", 2);
		SR->LinkTexture(L"PS_MAPTEX", L"CCbtn.png");
	}

	{
		OptionButton = GetScene()->CreateActor();
		OptionButton->GetTransform()->SetScale({ 0.4f, 0.4f });
		OptionButton->GetTransform()->SetPosition({ 6.8f, 4.2f, -3.0f });
		HPTR<SpriteRenderer> SR = OptionButton->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
		SR->SetSprite(L"CCbtn.png", 0);
		SR->LinkTexture(L"PS_MAPTEX", L"CCbtn.png");
	}


	{
		STSceneButton = GetScene()->CreateActor();
		STSceneButton->GetTransform()->SetScale({ 3.0f, 1.25f });
		STSceneButton->GetTransform()->SetPosition({ 6.0f, -3.5f, -3.0f });
		HPTR<SpriteRenderer> SR = STSceneButton->AddComponent<SpriteRenderer>(RENDERGROUP::RG_UI);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
		SR->SetSprite(L"STbtn.png", 10);
		SR->LinkTexture(L"PS_MAPTEX", L"STbtn.png");

		SpriteAnimaion* SANI = STSceneButton->AddComponent<SpriteAnimaion>();
		SANI->CreateAni(L"None", L"STbtn.png", 9, 10);
		SANI->CreateAni(L"MouseOn", L"STbtn.png", 0, 5, 0.125f);
		SANI->CreateAni(L"MouseClick", L"STbtn.png", 5, 9, 0.125f);
	}
	

}
void LobySceneUpdater::SceneUpdate()
{
	if (-1.0f == ChessPlayerAniChecker)
	{
		if ((Input::Down(L"LCLICK") == true) &&
			((GameWindow::MainGameWin()->MousePosInt().x) < 2.0f) && ((GameWindow::MainGameWin()->MousePosInt().x) > -2.0f) &&
			((GameWindow::MainGameWin()->MousePosInt().y) < 2.0f) && ((GameWindow::MainGameWin()->MousePosInt().y) > -2.0f))
		{
			chessPlayer->GetComponent<FbxAnimation>()->ChangeAnimation(L"ROLE");
			ChessPlayerAniChecker = 0.0f;

		

			Sound::Play(L"p005_showup_01_eng.wav");
		}
	}
	else
	{
		if (ChessPlayerAniChecker > 6.5f)
		{
			ChessPlayerAniChecker = -1.0f;
			chessPlayer->GetComponent<FbxAnimation>()->ChangeAnimation(L"IDLE");
		}
		else
		{
			ChessPlayerAniChecker += Time::DeltaTime();
		}
	}


	BCloud1->GetTransform()->SetLoclaPosition(BCloud1->GetTransform()->GetLocalPosition() + Vector4{ Time::DeltaTime() * -0.25f, 0.0f, 0.0f });

	if (BCloud1->GetTransform()->GetPosition().x < -14.0f)
	{
		BCloud1->GetTransform()->SetLoclaPosition(BCloud1->GetTransform()->GetLocalPosition() + Vector4{ 36.0f, 0.0f, 0.0f });
	}
	else if (BCloud1->GetTransform()->GetPosition().x < -2.0f && BCloud1->GetTransform()->GetPosition().x > -14.0f)
	{
		BCloud2->GetTransform()->SetLoclaPosition(BCloud1->GetTransform()->GetLocalPosition() + Vector4{ 12.0f, 0.0f, 0.0f });
		BCloud3->GetTransform()->SetLoclaPosition(BCloud1->GetTransform()->GetLocalPosition() + Vector4{ 24.0f, 0.0f, 0.0f });
	}
	else if (BCloud1->GetTransform()->GetPosition().x < 22.0f && BCloud1->GetTransform()->GetPosition().x > 10.0f)
	{
		BCloud2->GetTransform()->SetLoclaPosition(BCloud1->GetTransform()->GetLocalPosition() + Vector4{ -24.0f, 0.0f, 0.0f });
		BCloud3->GetTransform()->SetLoclaPosition(BCloud1->GetTransform()->GetLocalPosition() + Vector4{ -12.0f, 0.0f, 0.0f });
	}
	else
	{
		BCloud2->GetTransform()->SetLoclaPosition(BCloud1->GetTransform()->GetLocalPosition() + Vector4{ 12.0f, 0.0f, 0.0f });
		BCloud3->GetTransform()->SetLoclaPosition(BCloud1->GetTransform()->GetLocalPosition() + Vector4{ -12.0f, 0.0f, 0.0f });
	}




	if ((MouseInTheBox(OptionButton) == true))
	{
		OptionButton->GetComponent<SpriteRenderer>()->SetSprite(L"CCbtn.png", 1);
	}
	else
	{
		OptionButton->GetComponent<SpriteRenderer>()->SetSprite(L"CCbtn.png", 0);
	}


	if ((MouseInTheBox(CLSceneButton) == true))
	{
		CLSceneButton->GetComponent<SpriteRenderer>()->SetSprite(L"CCbtn.png", 3);

		if (Input::Down(L"LCLICK") == true)
		{
			GotoCollScene = true;
		}
	}
	else
	{
		CLSceneButton->GetComponent<SpriteRenderer>()->SetSprite(L"CCbtn.png", 2);
	}


	if (buttonMouseChecker == -1.0f && (MouseInTheBox(STSceneButton) == true))
	{
		STSceneButton->GetComponent<SpriteAnimaion>()->ChanageAni(L"MouseOn");

		if (Input::Down(L"LCLICK") == true)
		{
			buttonMouseChecker = 0.0f;
			Sound::Play(L"backbtn.wav");
			
		}
	}
	else if (buttonMouseChecker > -1.0f)
	{
		STSceneButton->GetComponent<SpriteAnimaion>()->ChanageAni(L"MouseClick");

		if (Input::Down(L"LCLICK") == true && (MouseInTheBox(STSceneButton) == true))
		{
			buttonMouseChecker = -1.0f;
			Sound::Play(L"backbtn.wav");
		}
	}
	else
	{
		STSceneButton->GetComponent<SpriteAnimaion>()->ChanageAni(L"None");
		//Sound::Play(L"already.wav");
	}

	if (buttonMouseChecker != -1.0f)
	{
		buttonMouseChecker += Time::DeltaTime() * 1.0f;
	}












	if (GotoCollScene == true)
	{
		GotoCollScene = false;
		GameWindow::MainGameWin()->ChangeScene(L"CollectScene");
	}
	else if (buttonMouseChecker >= 5.0f)
	{
		buttonMouseChecker = -1.0f;
		GameWindow::MainGameWin()->ChangeScene(L"StartScene");
		Sound::Stop(L"bgm_1.wav");
	//	GameWindow::MainGameWin()->ChangeScene(L"PlayScene");
	}
}
void LobySceneUpdater::ChangeEnd()
{
	
}
void LobySceneUpdater::ChangeStart()
{
	
}
void LobySceneUpdater::DebugRender()
{
	/*GraphicDebug::DrawFontUI(L"LOBY SCENE", Vector4::WHITE); 
	swprintf_s(LobArr, L"MPos : X %f Y %f", GameWindow::MainGameWin()->MousePos().x, GameWindow::MainGameWin()->MousePos().y);
	GraphicDebug::DrawFontUI(LobArr, Vector4::BLUE);
	swprintf_s(LobArr, L"MPos : X %f Y %f", GameWindow::MainGameWin()->MousePosInt().x, GameWindow::MainGameWin()->MousePosInt().y);
	GraphicDebug::DrawFontUI(LobArr, Vector4::RED);
	swprintf_s(LobArr, L"Pos : X %f Y %f Z %f", chessPlayer->GetTransform()->GetLocalPosition().x, chessPlayer->GetTransform()->GetLocalPosition().y, chessPlayer->GetTransform()->GetLocalPosition().z);
	GraphicDebug::DrawFontUI(LobArr, Vector4::WHITE);*/

	Font->DrawUI(L"µµ∞®", 16.0f, { 1387.0f, 55.0f, -5.0f }, Vector4::WHITE);
	Font->DrawUI(L"º≥¡§", 16.0f, { 1467.0f, 55.0f, -5.0f }, Vector4::WHITE);

}