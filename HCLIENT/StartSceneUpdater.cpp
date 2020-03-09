#include "StartSceneUpdater.h"

#include <INPUT.h>
#include <GAMEWIN.h>

#include <GRAPHICDEBUG.h>

#include <LOGICHEADER.h>
#include <GAMETHREAD.h>
#include <Video.h>
#include <SOUND.h>

CriticalSection CS;
unsigned __stdcall StartThread(Thread* _Thread, void* _Arg)
{
	
		std::wcout << L"스타트 쓰레드 동작중" << std::endl;
	
	
	return 0;
}


StartSceneUpdater::StartSceneUpdater()
{
}


StartSceneUpdater::~StartSceneUpdater()
{
}

void StartSceneUpdater::Loading()
{
	//Video loadingVideo;
	//loadingVideo.SetVideoFileDirectory(L"..\\BIN\\VIDEO\\StartMovie.avi");
	//loadingVideo.PlayVideoFile();
	//int a = 0;

	
}
void StartSceneUpdater::SceneUpdate()
{/*
	if (Input::Down(L"PLAY") == true)
	{
		GameWindow::MainGameWin()->ChangeScene(L"PlayScene");
	}*/
	int a = 0;

	GameWindow::MainGameWin()->ChangeScene(L"PlayScene");
}
void StartSceneUpdater::ChangeEnd()
{

}
void StartSceneUpdater::ChangeStart()
{

}
void StartSceneUpdater::DebugRender()
{
	GraphicDebug::DrawFontUI(L"START SCENE", Vector4(0.0f, 0.0f, 0.0f, 1.0f));
}