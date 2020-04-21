#include "GAMEWIN.h"
#include "SOUND.h"
#include <INPUT.h>
#include "RENMGR.h"

//////////////////////////////////// static

std::map<std::wstring, HPTR<GameWindow>>::iterator GameWindow::StartGameWin;
std::map<std::wstring, HPTR<GameWindow>>::iterator GameWindow::EndGameWin;
std::map<std::wstring, HPTR<GameWindow>> GameWindow::g_GameWinMap;
GameWindow* GameWindow::m_MainGameWin = nullptr;

void GameWindow::EngineLoop()
{
	// 로딩이 껴있으면
	// 처리.
	Sound::Init();
	Time::Reset();
	Window::BasicPeekLoop(WindowUpdate);
}

void __stdcall GameWindow::WindowUpdate()
{
	Sound::Update();
	Time::Update();
	Input::Update();
	Vector4::TimeUpdate();

	StartGameWin = g_GameWinMap.begin();
	EndGameWin = g_GameWinMap.end();

	for (; StartGameWin != EndGameWin; ++StartGameWin)
	{
		// 마우스 방향 잡고
		StartGameWin->second->Update();
		// 씬들을 업데이트 하는데.
		StartGameWin->second->SceneUpdate();
	}
}

GameWindow::GameWindow() : NextScene(nullptr), CurScene(nullptr)
{
}


GameWindow::~GameWindow()
{
}


void GameWindow::SceneUpdate() 
{
	// 맨 처음
	if (nullptr != NextScene)
	{
		if (NextScene == CurScene)
		{
			BOOM;
		}

		// 나는 이제 끝났다.
		if (nullptr != CurScene)
		{
			CurScene->ChangeEnd();
			NextScene->OverActorPush(CurScene->ExtractOverActor(NextScene->GetName()));
		}

		// 이미 액터는 옮겨와 있는게 좋을듯.

		NextScene->LoadingCheck();
		CurScene = NextScene;
		CurScene->ChangeStart();
		NextScene = nullptr;
	}

	CurScene->Progress();
}

HPTR<GameWindow> GameWindow::FindGameWin(const wchar_t* _WinName) 
{
	std::map<std::wstring, HPTR<GameWindow>>::iterator FindIter 
		= g_GameWinMap.find(_WinName);

	if (FindIter != g_GameWinMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}

HPTR<GameWindow> GameWindow::CreateGameWin(const wchar_t* _WinName)
{
	if (nullptr != FindGameWin(_WinName))
	{
		BOOM;
	}

	GameWindow* NewGameWin = new GameWindow();
	NewGameWin->SetName(_WinName);

	// 만들수 있으니까 진짜 만들어 보자.
	if (false == NewGameWin->Create(L"Default", _WinName))
	{
		// 실패했다면 터트려.
		BOOM;
		return nullptr;
	}

	if (0 == g_GameWinMap.size())
	{
		m_MainWindow = NewGameWin;
		m_MainGameWin = NewGameWin;
	}

	NewGameWin->Show();
	g_GameWinMap.insert(std::map<std::wstring, HPTR<GameWindow>>::value_type(_WinName, NewGameWin));
	InsertOtherWindow(NewGameWin);

	return NewGameWin;
}

void GameWindow::ChangeScene(const wchar_t* _SceneName)
{
	if (nullptr != CurScene && _SceneName == CurScene->CWStringName())
	{
		return;
	}

	std::map<std::wstring, HPTR<Scene>>::iterator FindIter 
		= m_SceneMap.find(_SceneName);

	if (FindIter == m_SceneMap.end())
	{
		BOOM;
	}

	NextScene = FindIter->second;
}

void GameWindow::CreateDevice() 
{
	m_HDevice = new GraphicDevice();
	m_HDevice->Init(this);
	// 랜더매니저의 이닛을 실행해줄것이다.
	// 디바이스가 가지는게 
}

HPTR<GameWindow> GameWindow::CreateGameWin(const wchar_t* _WinName, HWND _Hwnd) 
{
	if (nullptr == _Hwnd)
	{
		BOOM;
	}

	GameWindow* NewGameWin = new GameWindow();
	NewGameWin->SetName(_WinName);
	NewGameWin->m_HWnd = _Hwnd;
	NewGameWin->Init();
	if (0 == g_GameWinMap.size())
	{
		m_MainWindow = NewGameWin;
		m_MainGameWin = NewGameWin;
	}

	g_GameWinMap.insert(std::map<std::wstring, HPTR<GameWindow>>::value_type(_WinName, NewGameWin));
	InsertOtherWindow(NewGameWin);

	return NewGameWin;
}