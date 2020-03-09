
//#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
//// Windows 헤더 파일
//#include <windows.h>

#include <JWINDOW.h>
#include <GRAPHICDEVICE.h>
#include <INPUT.h>
#include <MATHS.h>
#include <SCENE.h>

#include <DEBUG.h>
#include <GAMEWIN.h>

#include "InitFunc.h"
#include "LobySceneUpdater.h"




// C 런타임 헤더 파일입니다.w
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow){
	//new int;

//	Debug::OpenConsole();
	Debug::LeakCheck();
		
	// 리팩토링
	GameWindow::Init(hInstance);
	GameWindow::CreateGameWin(L"MainWindow");
	GameWindow::MainGameWin()->Pos(0, 0);
	GameWindow::MainGameWin()->Size(WIN_SIZE_X, WIN_SIZE_Y);
	GameWindow::MainGameWin()->CreateDevice();
	// 개념은 
	ClientInit();
	GameWindow::EngineLoop();


	// HSCENE::EngineLoop();

	// 에러가 나는걸 겪어봐야 한다.
	 

	//Init();
	//HWINDOW::BasicPeekLoop(TestLoop);

	// HPTR<HSCENE> p = HSCENE::MainScene();
}
