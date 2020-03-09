#pragma once
#include <SCENE.h>


class HEFONT;
class CollectSceneUpdater : public Scene::Updater
{
private:
	void Loading()      override;
	void SceneUpdate()   override;
	void ChangeEnd()   override;
	void ChangeStart()   override;
	void DebugRender()   override;


	void ShowInformation(int _StandIndex, int _StarIndex);

private:
	struct Piece
	{
		int ID;
		int RACE1;
		int RACE2;
		int CLASS;
		int LEVEL;
		std::wstring NAME;
		std::wstring Anim[3];
		int Health;
		int Strength;
		int Range;
	};
	std::vector<Piece> Pieces;

	std::vector<HPTR<Actor>> StandEx;
	std::vector<HPTR<Actor>> ClassEx;
	std::vector<HPTR<Actor>> RaceEx;
	std::vector<HPTR<Actor>> StarEx;

	std::vector<HPTR<Actor>> StandNamePlate;
	std::vector<HPTR<Actor>> StandNamePlateClass;
	std::vector<HPTR<Actor>> StandNamePlateRace;
	std::vector<HPTR<Actor>> StandNamePlateRace2;

	std::vector<HPTR<Actor>> StandName;
	std::vector<HPTR<Actor>> StandNameClass;
	std::vector<HPTR<Actor>> StandNameRace;
	std::vector<HPTR<Actor>> StandNameRace2;

	std::vector<HPTR<Actor>> MenuLetter;

	HPTR<Actor> SCBar;

	HPTR<Actor> StandSprite;
	HPTR<Actor> StandFrameUpper;
	HPTR<Actor> StandFrameLower;
	HPTR<Actor> Stars;
	HPTR<Actor> LButton;
	HPTR<Actor> RButton;

	HPTR<Actor> ESCButton;
	HPTR<Actor> GetOutButton;
	HPTR<Actor> Metamorphosis;

	HPTR<Actor> Face;
	std::vector<HPTR<Actor>> FaceComp;

	HPTR<HEFONT> Font;

	wchar_t CollArr[256];
	float ScrbarVal = 0.0f;
	int CurCount = 0;
	int CurStand = -1;
	int CurClass = 0;
	int CurRace = 0;
	int CurLevel = 0;
	int CurStar = 0;
	bool SelectON = false;
	bool TransOn = false;
	bool EscapeCollScene = false;

	bool isScale[63];

public:
	CollectSceneUpdater();
	~CollectSceneUpdater();
};