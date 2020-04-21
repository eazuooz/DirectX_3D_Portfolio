#pragma once
#include <JWINDOW.h>
#include "GRAPHICDEVICE.h"
#include "SCENE.h"

class GameWindow : public Window
{
private:
	static std::map<std::wstring, HPTR<GameWindow>>::iterator StartGameWin;
	static std::map<std::wstring, HPTR<GameWindow>>::iterator EndGameWin;
	static std::map<std::wstring, HPTR<GameWindow>> g_GameWinMap;
	static GameWindow* m_MainGameWin;

public:
	static HPTR<GameWindow> FindGameWin(const wchar_t* _WinName);
	static HPTR<GameWindow> CreateGameWin(const wchar_t* _WinName);
	// 이미 만들어진 윈도우를 받아서 그걸 내가 관리하는 형식으로 가는 
	static HPTR<GameWindow> CreateGameWin(const wchar_t* _WinName, HWND _Hwnd);

public:
	static GameWindow* MainGameWin() {
		return m_MainGameWin;
	}

public:
	static void EngineLoop();
	static void __stdcall WindowUpdate();

////////////////////// 내부에서 씬을 그냥 지가 관리한다.
private:
	std::map<std::wstring, HPTR<Scene>> m_SceneMap;
	HPTR<Scene> NextScene;
	HPTR<Scene> CurScene;
	Scene* MadeScene;

public:
	Scene* GetCurScene() {
		return CurScene;
	}

public:
	void ChangeScene(const wchar_t* _SceneName);

	template<typename ...Rest>
	void CreateScene(const wchar_t* _SceneName, Rest ..._Arg)
	{
		if (nullptr == m_HDevice)
		{
			BOOM;
		}

		MadeScene = new Scene();
		MadeScene->Window(this);
		MadeScene->SetName(_SceneName);
		PushScript(_Arg...);
	}

	template<typename ...Rest>
	void PushScript(Scene::Updater* _Com, Rest ..._Arg)
	{
		MadeScene->AddUpdater(_Com);
		PushScript(_Arg...);
	}

	void PushScript()
	{
		m_SceneMap.insert(std::map<std::wstring, HPTR<Scene>>::value_type(MadeScene->GetName(), MadeScene));
		MadeScene = nullptr;
	}



	void SceneUpdate();

private:
	HPTR<GraphicDevice> m_HDevice;

public:
	void CreateDevice();

private:
	GameWindow();
	~GameWindow();
};

