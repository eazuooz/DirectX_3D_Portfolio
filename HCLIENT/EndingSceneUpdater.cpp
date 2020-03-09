#include "EndingSceneUpdater.h"

#include <GAMEWIN.h>
#include <Video.h>


#include <INPUT.h>
#include <GAMETHREAD.h>

#include <SPRITE.h>
#include <SPRITERENDER.h>
#include <2DANICOM.h>

#include <GRAPHICDEBUG.h>
#include <LOGICHEADER.h>
#include <SOUND.h>

#include <GRAPHICDEBUG.h>

EndingSceneUpdater::EndingSceneUpdater()
{
}


EndingSceneUpdater::~EndingSceneUpdater()
{
}

void EndingSceneUpdater::Loading()
{

	{
		HPTR<Actor> mainCam = GetScene()->CreateActor(L"LOADINGCAM");
		mainCam->GetTransform()->SetPosition({ 0.0f, 0.0f, -10.0f });
		HPTR<Camera> camera = mainCam->AddComponent<Camera>(RENDERGROUP::RG_PLAYER);
		camera->SetProjMode(PROJMODE::PM_ORTH);
		camera->SetArea(GameWindow::MainGameWin()->Size().x / 100.0f, GameWindow::MainGameWin()->Size().y / 100.0f);
	}

	{
		HPTR<Actor> end = GetScene()->CreateActor();
		end->GetTransform()->SetScale({ 16.0f, 9.0f });
		end->GetTransform()->SetPosition({ 0.0f, 0.0f, -1.1f });
		HPTR<SpriteRenderer> SR = end->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
		SR->SetSprite(L"ending.png", 0);
		SR->LinkTexture(L"PS_MAPTEX", L"ending.png");
	}

	//Video loadingVideo;
	//loadingVideo.SetVideoFileDirectory(L"..\\BIN\\VIDEO\\Ending.avi");
	//loadingVideo.PlayVideoFile();
}
void EndingSceneUpdater::SceneUpdate()
{


	if (Input::Down(L"LOADING") == true)
	{
		GameWindow::MainGameWin()->ChangeScene(L"LoadingScene");
	}
	else if (Input::Down(L"LOBY") == true)
	{
		GameWindow::MainGameWin()->ChangeScene(L"LobyScene");
	}
	else if (Input::Down(L"COLLECT") == true)
	{
		GameWindow::MainGameWin()->ChangeScene(L"CollectScene");
	}
	else if (Input::Down(L"START") == true)
	{
		GameWindow::MainGameWin()->ChangeScene(L"StartScene");
	}
	else if (Input::Down(L"PLAY") == true)
	{
		GameWindow::MainGameWin()->ChangeScene(L"PlayScene");
	}
	else if (Input::Down(L"ENDING") == true)
	{
		//HGAMEWIN::MainGameWin()->ChangeScene(L"EndingScene");
	}
}
void EndingSceneUpdater::ChangeEnd()
{

}
void EndingSceneUpdater::ChangeStart()
{

}
void EndingSceneUpdater::DebugRender()
{
	GraphicDebug::DrawFontUI(L"ENDING SCENE", Vector4(0.0f, 0.0f, 0.0f, 1.0f));
}