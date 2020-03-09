#pragma once
#include <SCENE.h>

// ĳĪ
//class LogicVar 
//{
//public:
//	static Actor* MainPlayer;
//};

class LobySceneUpdater : public Scene::Updater
{
private:
	void Loading()		override;
	void SceneUpdate()	override;
	void ChangeEnd()	override;
	void ChangeStart()	override;
	void DebugRender()	override;


	HPTR<Actor> chessPlayer;
	HPTR<Actor> BPlate;
	HPTR<Actor> BCloud1;
	HPTR<Actor> BCloud2;
	HPTR<Actor> BCloud3;
	HPTR<Actor> CLSceneButton;
	HPTR<Actor> OptionButton;
	HPTR<Actor> STSceneButton;

	HPTR<Actor> BBPlate;

	HPTR<HEFONT> Font;

	wchar_t LBArr[256];

	float PlayerMouseChecker;
	float PlayerMouseCheckerDest;
	float PlayerMouseCheckerTmp;

	float ChessPlayerAniChecker;

	bool GotoCollScene;

public:
	LobySceneUpdater() : PlayerMouseCheckerTmp(0.0f), ChessPlayerAniChecker(-1.0f){};
	~LobySceneUpdater() {};

};

