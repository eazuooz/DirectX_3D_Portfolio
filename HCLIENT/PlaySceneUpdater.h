#pragma once
#include <SCENE.h>
#include <CAMERA.h>
#include <PieceLogic.h>


class BloomFilter;
class Terrain;
class FreeCamera;
class FbxAnimation;
class StaticFbxRender;
class SpriteRenderer;
class HEFONT;
class PlaySceneUpdater : public Scene::Updater
{
private:
	HPTR<Actor> m_MainCam;
	HPTR<Camera> camCom;

	HPTR<BloomFilter> BFilter;
	HPTR<Light> LightCheck01;

	//HPTR<StaticFbxRender> WATERRENDER;
	HPTR<Renderer> WATERRENDER;
	//HPTR<StaticFbxRender> WATERRENDER;
	Vector4 waterTime;

	float rotSkyBox;

	Vector4 mousePos;
	HPTR<Actor> Mouse;
	HPTR<Actor> UIMouse;

	HPTR<Actor> chessBoardActor;
	HPTR<Terrain> chessBoardTerrain;
	
	HPTR<Actor> chessStayActor;
	HPTR<Terrain> chessStayTerrain;

	HPTR<Actor> unitSeller;

	std::vector<HPTR<Actor>> CombineRACE;
	std::vector<HPTR<Actor>> CombineCLASS;
	std::vector<HPTR<Actor>> CombineFIRE;
	HPTR<Actor> CombineBP;

	std::vector<bool> PieceCheacker;
	std::vector<int> CombineCheacker;

	bool isPick;
	BoardIndex destIdx;
	BoardIndex prevSoundIdx;
	Actor* pickActor;



	HPTR<Actor> skyShphere;

	//
	
private:
	bool isGraphicDebug;

	//std::list<Actor*> m_PieceList;
	//std::list<Actor*>::iterator m_PieceBegin;
	//std::list<Actor*>::iterator m_PieceEnd;

	//std::list<Actor*> m_MonsterList;
	//std::list<Actor*>::iterator m_MonsterBegin;
	//std::list<Actor*>::iterator m_MonsterEnd;

private:
	void Loading()		override;
	void SceneUpdate()	override;
	void ChangeEnd()	override;
	void ChangeStart()	override;
	void DebugRender()	override;

public:
	void ScenryInit();
	void UIInit();
	void MoveChessPiece();
	void CheatUpgradePiece();
	void PurchaseChessPiece();

	void CombinationPiece();
	void CombinationLetter();

	HPTR<HEFONT> Font;

public:
	PlaySceneUpdater();
	~PlaySceneUpdater();
};

