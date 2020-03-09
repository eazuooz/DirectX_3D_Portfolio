#include "InitFunc.h"
#include <PATH.h>
#include <RES.h>
#include <INPUT.h>
#include <TEX.h>
#include <SPRITE.h>
#include <SCENE.h>
#include <GAMEWIN.h>

#include "LobySceneUpdater.h"
#include "PlaySceneUpdater.h"
#include "LoadingSceneUpdater.h"
#include "StartSceneUpdater.h"
#include "CollectSceneUpdater.h"
#include "EndingSceneUpdater.h"


#include <LOGICHEADER.h>

#include <DEBUG.h>

void ClientInit() 
{
	LOGICVALUE::Init();

	GameWindow::MainGameWin()->CreateScene(L"LoadingScene",	new LoadingSceneUpdater());
	GameWindow::MainGameWin()->CreateScene(L"LobyScene",	new LobySceneUpdater());
	GameWindow::MainGameWin()->CreateScene(L"CollectScene",	new CollectSceneUpdater());
	GameWindow::MainGameWin()->CreateScene(L"StartScene",	new StartSceneUpdater());
	GameWindow::MainGameWin()->CreateScene(L"PlayScene",	new PlaySceneUpdater());
	GameWindow::MainGameWin()->CreateScene(L"EndingScene",	new EndingSceneUpdater());

	GameWindow::MainGameWin()->ChangeScene(L"LoadingScene");
}