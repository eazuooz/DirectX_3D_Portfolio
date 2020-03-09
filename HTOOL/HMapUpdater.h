#pragma once
#include <SCENE.h>
#include <CAMERA.h>
#include <JITILEMAP.h>


class BloomFilter;
class Terrain;
class FreeCamera;
class FbxAnimation;
class HMapUpdater : public Scene::Updater
{
private:
	float MoveTime;
	std::list<JITILEMAP::HGAMETILE*> MoveTile;

	HPTR<Actor> MAINCAM;
	static HPTR<Camera> CAMCOM;
	HPTR<BloomFilter> BFilter;

public:
	static HPTR<Terrain> MAPCOM;
	static HPTR<Actor> PLAYER;
public:
	static HPTR<FreeCamera> FREECAMCOM;

public:
	static  HPTR<FbxAnimation> ANIREN;
	HPTR<Light> LIGHTCHECK;

private:
	void SceneUpdate() override;
	void Loading() override;
	void DebugRender() override;

public:
	HMapUpdater();
	~HMapUpdater();
};

