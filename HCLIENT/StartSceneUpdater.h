#pragma once
#include <SCENE.h>

class StartSceneUpdater : public Scene::Updater
{
private:
	void Loading()		override;
	void SceneUpdate()	override;
	void ChangeEnd()	override;
	void ChangeStart()	override;
	void DebugRender()	override;

public:
	StartSceneUpdater();
	~StartSceneUpdater();
};

