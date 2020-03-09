#include "LoadingSceneUpdater.h"

#include "LobySceneUpdater.h"

//BASE
#include <INPUT.h>
#include <GAMEWIN.h>
#include <RANDOM.h>
#include <JTIME.h>

//ENGINE
#include <GRAPHICDEBUG.h>
#include <GRIDRENDER.h>
#include <CAMERA.h>
#include <FREECAM.h>
#include <3DANIRENDER.h>
#include <StaticFbxRender.h>
#include <BOUNDINGBOX.h>


//LOGIC
#include <LOGICHEADER.h>
#include <ChessBoard.h>
#include <StatusLogic.h>

//CLIENT
#include "LOGICHEADER.h"

#include <GAMEWIN.h>
#include <FREECAM.h>
#include <SPRITERENDER.h>
#include <RECTRENDER.h>
#include <TEX.h>
#include <SPRITE.h>
#include <2DANICOM.h>
#include <INPUT.h>
#include <SOUND.h>
#include <RENDER.h>
#include <GRIDRENDER.h>
#include <TEX.h>

#include <BoneChaser.h>
#include <BOUNDINGRECT.h>
#include <BOUNDINGCIRCLE.h>
#include <BOUNDINGBOX.h>
#include <GRAPHICDEBUG.h>
#include <DEBUG.h>
#include <GAMETHREAD.h>
#include <HMULTIRENDERTarget.h>
#include <HRENDERTARGET.h>
#include <BloomFilter.h>


#include <GRAPHICDEBUG.h>
#include <3DANIRENDER.h>


#include <TERRAIN.h>
#include <FREECAM.h>

#include <GAMETHREAD.h>


//
#include <ChessBoard.h>
#include <MpMagicion.h>
#include <MonsterLogic.h>

#include <Video.h>
#include <SOUND.h>


CriticalSection CS2;

bool LoadingSceneUpdater::LobyReady = false;
bool LoadingSceneUpdater::CollectReady = false;
bool LoadingSceneUpdater::StartReady = false;
bool LoadingSceneUpdater::PlayReady = false;
bool LoadingSceneUpdater::EndingReady = false;

wchar_t LodArr[256];

unsigned __stdcall LoadingThread(Thread* _Thread, void* _Arg)
{


	if(false == LoadingSceneUpdater::LobyReady)
	{
		LoadingSceneUpdater::LobyReady = true;

		{
			Directory Dic;
			Dic.ReplaceParentPath();
			Dic.MoveDirectory(L"BIN");
			Dic.MoveDirectory(L"FBX");

			std::vector<File> AllFileList;
			Dic.GetFilesName(AllFileList, L"*.FBXDATA");

			std::wstring Path;
			for (size_t i = 0; i < AllFileList.size(); i++)
			{
				Path = AllFileList[i].GetFullPath();

				Resources::Load<Fbx>(Path.c_str(), Fbx::LOADMODE::CONVERT);

				/*Path += L" 로딩 완료";
				Debug::ConsolePrintLine(Path.c_str());*/
			}
		}

	}

	if (false == LoadingSceneUpdater::CollectReady)
	{
		LoadingSceneUpdater::CollectReady = true;

		{
			Directory Dic;
			Dic.ReplaceParentPath();
			Dic.MoveDirectory(L"BIN");
			Dic.MoveDirectory(L"TEX");

			{
				std::vector<File> AllFileList;
				Dic.GetFilesName(AllFileList, L"*.png");

				std::wstring Name;
				for (size_t i = 0; i < AllFileList.size(); i++)
				{
					Name = AllFileList[i].GetFileName();

					if (Name == L"LoadingBar.png")
					{
						int a = 0;
					}

					Resources::Load<Texture>(Path::PlusPath(L"TEX", Name.c_str()));
				}
			}

			Dic.MoveDirectory(L"stand");

			{
				std::vector<File> AllFileList;
				Dic.GetFilesName(AllFileList, L"*.png");

				std::wstring Name;
				for (size_t i = 0; i < AllFileList.size(); i++)
				{
					Name = AllFileList[i].GetFileName();

					if (Name == L"LoadingBar.png")
					{
						int a = 0;
					}

					Resources::Load<Texture>(Path::PlusPath(L"STAND", Name.c_str()));
				}
			}

			Resources::Create<Sprite>(L"collBG.png", L"collBG.png", 1, 1, 0, 1);
			Resources::Create<Sprite>(L"collBGMask.png", L"collBGMask.png", 1, 1, 0, 1);
			Resources::Create<Sprite>(L"Stand.png", L"Stand.png", 8, 8, 0, 64);
			Resources::Create<Sprite>(L"Race.png", L"Race.png", 4, 4, 0, 15);
			Resources::Create<Sprite>(L"Class.png", L"Class.png", 4, 4, 0, 13);
			Resources::Create<Sprite>(L"Race_HF.png", L"Race_HF.png", 4, 4, 0, 15);
			Resources::Create<Sprite>(L"Class_HF.png", L"Class_HF.png", 4, 4, 0, 13);
			Resources::Create<Sprite>(L"Stars.png", L"Stars.png", 4, 3, 0, 11);
			Resources::Create<Sprite>(L"ScrollBar.png", L"ScrollBar.png", 1, 1, 0, 1);
			Resources::Create<Sprite>(L"Level.png", L"Level.png", 4, 2, 0, 8);
			Resources::Create<Sprite>(L"Pointer.png", L"Pointer.png", 2, 1, 0, 2);
			Resources::Create<Sprite>(L"NamePlate.png", L"NamePlate.png", 1, 2, 0, 2);
			Resources::Create<Sprite>(L"LetterData.png", L"LetterData.png", 3, 63, 0, 189);
			Resources::Create<Sprite>(L"Face.png", L"Face.png", 8, 8, 0, 63);
			Resources::Create<Sprite>(L"ESC.png", L"ESC.png", 2, 1, 0, 2);
			Resources::Create<Sprite>(L"Exit.png", L"Exit.png", 2, 1, 0, 2);
			Resources::Create<Sprite>(L"Metamorphosis.png", L"Metamorphosis.png", 2, 1, 0, 2);

			Resources::Create<Sprite>(L"Stand26_1a.png", L"Stand26_1a.png", 8, 5, 0, 36);
			Resources::Create<Sprite>(L"Stand26_2a.png", L"Stand26_2a.png", 8, 5, 0, 36);
			Resources::Create<Sprite>(L"Stand26_3a.png", L"Stand26_3a.png", 8, 5, 0, 36);

			Resources::Create<Sprite>(L"Stand39_1a.png", L"Stand39_1a.png", 8, 5, 0, 40);
			Resources::Create<Sprite>(L"Stand39_2a.png", L"Stand39_2a.png", 8, 5, 0, 40);
			Resources::Create<Sprite>(L"Stand39_3a.png", L"Stand39_3a.png", 8, 5, 0, 40);

			Resources::Create<Sprite>(L"Stand49_1a.png", L"Stand49_1a.png", 8, 4, 0, 26);
			Resources::Create<Sprite>(L"Stand49_2a.png", L"Stand49_2a.png", 8, 4, 0, 26);
			Resources::Create<Sprite>(L"Stand49_3a.png", L"Stand49_3a.png", 8, 4, 0, 26);

			
			
		}

	}

	if (false == LoadingSceneUpdater::StartReady)
	{
		LoadingSceneUpdater::StartReady = true;

		{

		}
	}

	if (false == LoadingSceneUpdater::PlayReady)
	{
		LoadingSceneUpdater::PlayReady = true;

		{
			{
				Directory dic;
				dic.ReplaceParentPath();

				std::wstring pathD = dic.GetFullPath();
				pathD += L"\\BIN\\FBX\\FbxTexture\\Brick_D_02.tga";
				HPTR<Texture> texD = Resources::Load<Texture>(pathD.c_str());

				std::wstring pathN = dic.GetFullPath();
				pathN += L"\\BIN\\FBX\\FbxTexture\\Brick_N.tga";
				HPTR<Texture> texN = Resources::Load<Texture>(pathN.c_str());

				std::wstring pathH = dic.GetFullPath();
				pathH += L"\\BIN\\FBX\\FbxTexture\\Brick_H.tga";
				HPTR<Texture> texH = Resources::Load<Texture>(pathH.c_str());

				std::wstring wpathN = dic.GetFullPath();
				wpathN += L"\\BIN\\FBX\\FbxTexture\\Water_N2.png";
				HPTR<Texture> wtexN = Resources::Load<Texture>(wpathN.c_str());

				std::wstring wpathH = dic.GetFullPath();
				wpathH += L"\\BIN\\FBX\\FbxTexture\\Water_D2.png";
				HPTR<Texture> wtexH = Resources::Load<Texture>(wpathH.c_str());


				std::wstring spathH = dic.GetFullPath();
				spathH += L"\\BIN\\FBX\\FbxTexture\\SAND_D.png";
				HPTR<Texture> stexH = Resources::Load<Texture>(spathH.c_str());


				std::wstring pathD2 = dic.GetFullPath();
				pathD2 += L"\\BIN\\FBX\\FbxTexture\\EF_water_33_03_w.png";
				HPTR<Texture> texD2 = Resources::Load<Texture>(pathD2.c_str());

				std::wstring pathD4 = dic.GetFullPath();
				pathD4 += L"\\BIN\\FBX\\FbxTexture\\sky_PNG5464.png";
				HPTR<Texture> texD4 = Resources::Load<Texture>(pathD4.c_str());

				std::wstring pathD5 = dic.GetFullPath();
				pathD5 += L"\\BIN\\FBX\\FbxTexture\\T_GPP_Plasma_A.tga";
				HPTR<Texture> texD5 = Resources::Load<Texture>(pathD5.c_str());

				std::wstring pathD6 = dic.GetFullPath();
				pathD6 += L"\\BIN\\FBX\\FbxTexture\\shineArrow.tga";
				HPTR<Texture> texD6 = Resources::Load<Texture>(pathD6.c_str());


			}

			{
				Directory dic;
				dic.ReplaceParentPath();

				std::wstring pathD = dic.GetFullPath();
				pathD += L"\\BIN\\FBX\\FbxTexture\\ChessStay_D.tga";
				HPTR<Texture> texD = Resources::Load<Texture>(pathD.c_str());

				std::wstring pathN = dic.GetFullPath();
				pathN += L"\\BIN\\FBX\\FbxTexture\\ChessStay_N.tga";
				HPTR<Texture> texN = Resources::Load<Texture>(pathN.c_str());

				std::wstring pathH = dic.GetFullPath();
				pathH += L"\\BIN\\FBX\\FbxTexture\\ChessStay_H.tga";
				HPTR<Texture> texH = Resources::Load<Texture>(pathH.c_str());
			}

			{
				Directory dic;
				dic.ReplaceParentPath();

				std::wstring pathD = dic.GetFullPath();
				pathD += L"\\BIN\\FBX\\FbxTexture\\Part_01_D.tga";
				HPTR<Texture> texD = Resources::Load<Texture>(pathD.c_str());

				std::wstring pathN = dic.GetFullPath();
				pathN += L"\\BIN\\FBX\\FbxTexture\\FlameMagicionAtt.tga";
				HPTR<Texture> texN = Resources::Load<Texture>(pathN.c_str());
			}

			{
				Directory dic;
				dic.ReplaceParentPath();



				std::wstring pathN = dic.GetFullPath();
				pathN += L"\\BIN\\FBX\\FbxTexture\\Part_01_N.tga";
				HPTR<Texture> texN = Resources::Load<Texture>(pathN.c_str());

				std::wstring pathH = dic.GetFullPath();
				pathH += L"\\BIN\\FBX\\FbxTexture\\fireball.tga";
				HPTR<Texture> texH = Resources::Load<Texture>(pathH.c_str());
				Resources::Create<Sprite>(L"fireball.tga", L"fireball.tga", 4, 4, 0, 16);
			}




			Resources::Create<Sprite>(L"HP_Prograss.png", L"HP_Prograss.png", 1, 1, 0, 1);
			Resources::Create<Sprite>(L"MP_Prograss.png", L"HP_Prograss.png", 1, 1, 0, 1);
			Resources::Create<Sprite>(L"COOL_Prograss.png", L"HP_Prograss.png", 1, 1, 0, 1);

			Resources::Create<Sprite>(L"buyBg.png", L"buyBg.png", 1, 1, 0, 1);
			Resources::Create<Sprite>(L"NumberSprite.png", L"NumberSprite.png", 10, 4, 0, 41);

			//
			Resources::Create<Sprite>(L"BladeKnight.png", L"BladeKnight.png", 8, 7, 0, 50);
			Resources::Create<Sprite>(L"DoomArbiter.png", L"DoomArbiter.png", 8, 5, 0, 40);
			Resources::Create<Sprite>(L"Egg.png", L"Egg.png", 8, 5, 0, 40);
			Resources::Create<Sprite>(L"DemonLarva.png", L"DemonLarva.png", 8, 5, 0, 40);
			Resources::Create<Sprite>(L"FireMagician.png", L"FireMagician.png", 8, 8, 0, 60);
			Resources::Create<Sprite>(L"ThunderSpirit.png", L"ThunderSpirit.png", 8, 7, 0, 52);//
			Resources::Create<Sprite>(L"GodofThunder.png", L"GodofThunder.png", 8, 7, 0, 54);//
			Resources::Create<Sprite>(L"GodofWar.png", L"GodofWar.png", 8, 5, 0, 42);//
			Resources::Create<Sprite>(L"TideHunter.png", L"TideHunter.png", 8, 5, 0, 40);
			Resources::Create<Sprite>(L"LittleDragon.png", L"LittleDragon.png", 8, 3, 0, 20);
			Resources::Create<Sprite>(L"ShiningArcher.png", L"ShiningArcher.png", 8, 5, 0, 40);
			Resources::Create<Sprite>(L"ManaMagician.png", L"ManaMagician.png", 8, 5, 0, 40);
			Resources::Create<Sprite>(L"PirateCaptain.png", L"PirateCaptain.png", 8, 5, 0, 40);
			Resources::Create<Sprite>(L"Siren.png", L"Siren.png", 8, 5, 0, 40);
			Resources::Create<Sprite>(L"ShadowDemon.png", L"ShadowDemon.png", 8, 8, 0, 62);
			Resources::Create<Sprite>(L"ToenailDruid.png", L"ToenailDruid.png", 8, 5, 0, 40);
			Resources::Create<Sprite>(L"SwordMaster.png", L"SwordMaster.png", 8, 5, 0, 40);
			Resources::Create<Sprite>(L"TortolanElder.png", L"TortolanElder.png", 8, 6, 0, 45);
			Resources::Create<Sprite>(L"Unicorn.png", L"Unicorn.png", 8, 8, 0, 60);
			Resources::Create<Sprite>(L"WindRanger.png", L"WindRanger.png", 8, 7, 0, 50);
			Resources::Create<Sprite>(L"Warewolf.png", L"Warewolf.png", 8, 7, 0, 50);
			Resources::Create<Sprite>(L"WalrusChamp.png", L"WalrusChamp.png", 8, 7, 0, 50);


			Resources::Create<Sprite>(L"EatDragonSprite2.png", L"EatDragonSprite2.png", 8, 8, 0, 62);
			Resources::Create<Sprite>(L"MouseCursor.png", L"MouseCursor.png", 4, 1, 0, 1);
			Resources::Create<Sprite>(L"BattleTopFrame.png", L"BattleTopFrame.png", 1, 1, 0, 1);

			Resources::Create<Sprite>(L"Butterfly4X4.png", L"Butterfly4X4.png", 4, 4, 0, 16);

			Resources::Create<Sprite>(L"Unicorn_Circle.png", L"Unicorn_Circle.png", 4, 4, 0, 16);
			Resources::Create<Sprite>(L"unicornwave.png", L"unicornwave.png", 4, 4, 0, 16);

			Resources::Create<Sprite>(L"CircleEffectSprite.png", L"CircleEffectSprite.png", 5, 5, 0, 25);
			Resources::Create<Sprite>(L"CircleEffectSprite_Purple.png", L"CircleEffectSprite_Purple.png", 5, 5, 0, 25);

			Resources::Create<Sprite>(L"EF_water_03_01.png", L"EF_water_03_01.png", 4, 4, 0, 16);
			Resources::Create<Sprite>(L"expl.png", L"expl.png", 6, 6, 0, 36);
			Resources::Create<Sprite>(L"starsp.png", L"starsp.png", 4, 4, 0, 16);
			Resources::Create<Sprite>(L"starfl.png", L"starfl.png", 5, 7, 0, 35);
			Resources::Create<Sprite>(L"staref.png", L"staref.png", 4, 4, 0, 16);
			Resources::Create<Sprite>(L"explo.png", L"explo.png", 5, 3, 0, 15);
			Resources::Create<Sprite>(L"burst.png", L"burst.png", 8, 6, 0, 48);
			Resources::Create<Sprite>(L"MagicRuneRed.png", L"MagicRuneRed.png", 1, 1, 0, 1);
			Resources::Create<Sprite>(L"wthunder.png", L"wthunder.png", 10, 3, 0, 26);
			Resources::Create<Sprite>(L"Bang.png", L"Bang.png", 1, 1, 0, 1);
			Resources::Create<Sprite>(L"ending.png", L"ending.png", 1, 1, 0, 1);
			Resources::Create<Sprite>(L"FireRing.png", L"FireRing.png", 6, 2, 0, 12);

			{
				Directory dic;
				dic.ReplaceParentPath();
				std::wstring pathN = dic.GetFullPath();
				pathN += L"\\BIN\\FBX\\FbxTexture\\thunderCircleEffect.tga";
				HPTR<Texture> texN = Resources::Load<Texture>(pathN.c_str());
				Resources::Create<Sprite>(L"thunderCircleEffect.tga", L"thunderCircleEffect.tga", 5, 5, 0, 21);
			}

			Resources::Create<Sprite>(L"FlameMagicionAtt.tga", L"FlameMagicionAtt.tga", 4, 8, 0, 32);
			{
				Directory dic;
				dic.ReplaceParentPath();
				std::wstring pathN = dic.GetFullPath();
				pathN += L"\\BIN\\FBX\\FbxTexture\\UnicornHeal.tga";
				HPTR<Texture> texN = Resources::Load<Texture>(pathN.c_str());
				Resources::Create<Sprite>(L"UnicornHeal.tga", L"UnicornHeal.tga", 5, 16, 0, 80);
			}

			{
				Directory dic;
				dic.ReplaceParentPath();
				std::wstring pathN = dic.GetFullPath();
				pathN += L"\\BIN\\FBX\\FbxTexture\\flameMagicionLight.tga";
				HPTR<Texture> texN = Resources::Load<Texture>(pathN.c_str());
				Resources::Create<Sprite>(L"flameMagicionLight.tga", L"flameMagicionLight.tga", 1, 5, 0, 5);
			}

			{
				Directory dic;
				dic.ReplaceParentPath();
				std::wstring pathN = dic.GetFullPath();
				pathN += L"\\BIN\\FBX\\FbxTexture\\Lightning01.tga";
				HPTR<Texture> texN = Resources::Load<Texture>(pathN.c_str());
				Resources::Create<Sprite>(L"Lightning01.tga", L"Lightning01.tga", 1, 4, 0, 4);
			}

			{
				Directory dic;
				dic.ReplaceParentPath();
				std::wstring pathN = dic.GetFullPath();
				pathN += L"\\BIN\\FBX\\FbxTexture\\Lightning05.tga";
				HPTR<Texture> texN = Resources::Load<Texture>(pathN.c_str());
				Resources::Create<Sprite>(L"Lightning05.tga", L"Lightning05.tga", 4, 4, 0, 16);
			}


			{
				Directory dic;
				dic.ReplaceParentPath();
				std::wstring pathN = dic.GetFullPath();
				pathN += L"\\BIN\\FBX\\FbxTexture\\chargeEffect.tga";
				HPTR<Texture> texN = Resources::Load<Texture>(pathN.c_str());
				Resources::Create<Sprite>(L"chargeEffect.tga", L"chargeEffect.tga", 5, 12, 0, 60);
			}

			{
				Directory dic;
				dic.ReplaceParentPath();
				std::wstring pathN = dic.GetFullPath();
				pathN += L"\\BIN\\FBX\\FbxTexture\\torEffect.tga";
				HPTR<Texture> texN = Resources::Load<Texture>(pathN.c_str());
				Resources::Create<Sprite>(L"torEffect.tga", L"torEffect.tga", 10, 15, 0, 150);
			}


			{
				Directory dic;
				dic.ReplaceParentPath();
				std::wstring pathN = dic.GetFullPath();
				pathN += L"\\BIN\\FBX\\FbxTexture\\T_GPP_MagicBall.tga";
				HPTR<Texture> texN = Resources::Load<Texture>(pathN.c_str());
				Resources::Create<Sprite>(L"T_GPP_MagicBall.tga", L"T_GPP_MagicBall.tga", 4, 4, 0, 16);
			}
			//T_GPP_MagicBall
			
			//Resources::Create<Sprite>(L"EF_water_03_01.png", L"EF_water_03_01.png", 4, 4, 0, 16);

			//Resources::Create<Sprite>(L"ExWave.png", L"ExWave.png", 4, 4, 0, 16);

			{
				Directory dic;
				dic.ReplaceParentPath();
				std::wstring pathN = dic.GetFullPath();
				pathN += L"\\BIN\\FBX\\FbxTexture\\windRangerEffect.tga";
				HPTR<Texture> texN = Resources::Load<Texture>(pathN.c_str());
				Resources::Create<Sprite>(L"windRangerEffect.tga", L"windRangerEffect.tga", 5, 4, 0, 20);
			}

			{
				Directory dic;
				dic.ReplaceParentPath();
				std::wstring pathN = dic.GetFullPath();
				pathN += L"\\BIN\\FBX\\FbxTexture\\EF_shape_95_01_z.tga";
				HPTR<Texture> texN = Resources::Load<Texture>(pathN.c_str());
				Resources::Create<Sprite>(L"EF_shape_95_01_z.tga", L"EF_shape_95_01_z.tga", 4, 4, 0, 16);
				Resources::Create<Sprite>(L"MagicRuneBlue.png", L"MagicRuneBlue.png", 1, 1, 0, 1);
			}

			{
				Directory dic;
				dic.ReplaceParentPath();
				std::wstring pathN = dic.GetFullPath();
				pathN += L"\\BIN\\FBX\\FbxTexture\\T_ky_circle9_4x4.tga";
				HPTR<Texture> texN = Resources::Load<Texture>(pathN.c_str());
				Resources::Create<Sprite>(L"T_ky_circle9_4x4.tga", L"T_ky_circle9_4x4.tga", 4, 4, 0, 16);
			}

			{
				Directory Dic;
				Dic.ReplaceParentPath();
				Dic.MoveDirectory(L"BIN");
				Dic.MoveDirectory(L"Sound");

				std::vector<File> AllFileList;
				Dic.GetFilesName(AllFileList, L"*.wav");

				std::wstring Path;
				for (size_t i = 0; i < AllFileList.size(); i++)
				{
					Path = AllFileList[i].GetFullPath();

					Resources::Load<Sound>(Path.c_str());

					/*Path += L" 로딩 완료";
					Debug::ConsolePrintLine(Path.c_str());*/
				}
			}
		}
	}

	if (false == LoadingSceneUpdater::EndingReady)
	{
		LoadingSceneUpdater::EndingReady = true;

		{

		}
	}


	return 0;
}


LoadingSceneUpdater::LoadingSceneUpdater()
{
}


LoadingSceneUpdater::~LoadingSceneUpdater()
{
}

void LoadingSceneUpdater::Loading()
{
	FbxPath fbxPath;
	Thread::CreateThread(L"ConsoleThread", LoadingThread, fbxPath);






	Sceneidx = -1;
	curidx = 0;

	Font = Resources::Find<HEFONT>(L"1훈떡볶이");

	Directory Dic;
	Dic.ReplaceParentPath();
	Dic.MoveDirectory(L"BIN");
	Dic.MoveDirectory(L"TEX");
	Dic.MoveDirectory(L"LOAD"); 

	{
		std::vector<File> AllFileList;
		Dic.GetFilesName(AllFileList, L"*.png");

		std::wstring Name;
		for (size_t i = 0; i < AllFileList.size(); i++)
		{
			Name = AllFileList[i].GetFileName();

			if (Name == L"LoadingBar.png")
			{
				
				loadingSpriteCheck = true;
			}

			Resources::Load<Texture>(Path::PlusPath(L"LOAD", Name.c_str()));
		}
	}

	Resources::Create<Sprite>(L"LoadingSceneSprite.png", L"LoadingSceneSprite.png", 15, 4, 0, 61);
	Resources::Create<Sprite>(L"LoadingBar.png", L"LoadingBar.png", 1, 5, 0, 5);
	Resources::Create<Sprite>(L"LoadingCircle.png", L"LoadingCircle.png", 4, 3, 0, 9);


	{
		HPTR<Actor> mainCam = GetScene()->CreateActor(L"LOADINGCAM");
		mainCam->GetTransform()->SetPosition({ 0.0f, 0.0f, -100.0f });
		HPTR<Camera> camera = mainCam->AddComponent<Camera>(RENDERGROUP::RG_GRID);
		camera->SetProjMode(PROJMODE::PM_ORTH);
		camera->SetArea(GameWindow::MainGameWin()->Size().x / 100.0f, GameWindow::MainGameWin()->Size().y / 100.0f);
	}

	{
		HPTR<Actor> BGIMG = GetScene()->CreateActor();
		BGIMG->GetTransform()->SetScale({ 16.0f, 9.0f });
		BGIMG->GetTransform()->SetPosition({ 0.0f,0.0f,0.0f });
		HPTR<SpriteRenderer> SR = BGIMG->AddComponent<SpriteRenderer>(RENDERGROUP::RG_GRID);
		SR->SetSprite(L"LoadingSceneSprite.png", 0);
		SpriteAnimaion* TESTANI = BGIMG->AddComponent<SpriteAnimaion>();
		TESTANI->CreateAni(L"LOOP", L"LoadingSceneSprite.png", 0, 61, 0.07f);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"LoadingSceneSprite.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
		SR->LinkSampler(L"PS_PSMP", L"PSMP");
	}

	{
		LoadingBar = GetScene()->CreateActor();
		LoadingBar->GetTransform()->SetScale({ 14.0f, 0.5f });
		LoadingBar->GetTransform()->SetPosition({ 0.0f,-4.0f,-1.0f });
		LoadingBarSR = LoadingBar->AddComponent<SpriteRenderer>(RENDERGROUP::RG_GRID);
		LoadingBarSR->SetSprite(L"LoadingBar.png", 0);

		LoadingBarSR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		LoadingBarSR->LinkCB(L"VS_CUTDATA", &(LoadingBarSR->GetCutData()));
		LoadingBarSR->LinkTexture(L"PS_MAPTEX", L"LoadingBar.png");
		LoadingBarSR->LinkSampler(L"PS_LSMP", L"LSMP");
	}

	{
		LoadingCircle = GetScene()->CreateActor();
		LoadingCircle->GetTransform()->SetScale({ 1.0f, 1.0f });
		LoadingCircle->GetTransform()->SetPosition({ 7.0f,-2.8f,-1.0f });
		HPTR<SpriteRenderer> SR = LoadingCircle->AddComponent<SpriteRenderer>(RENDERGROUP::RG_GRID);
		SR->SetSprite(L"LoadingCircle.png", 8);

		SpriteAnimaion* TESTANI = LoadingCircle->AddComponent<SpriteAnimaion>();
		TESTANI->CreateAni(L"LOOP", L"LoadingCircle.png", 0, 8, 0.1f);
		TESTANI->CreateAni(L"END", L"LoadingCircle.png", 8, 9, 0.1f, false);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"LoadingCircle.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
	}

	/*Video loadingVideo;
	loadingVideo.SetVideoFileDirectory(L"..\\BIN\\VIDEO\\LoadingMovie.avi");
	loadingVideo.PlayVideoFile();*/
}


void LoadingSceneUpdater::SceneUpdate()
{
	if (true == EndingReady)
	{
		Sceneidx = 4;
		LoadingCircle->GetComponent<SpriteAnimaion>()->ChanageAni(L"END");
	}
	else if (true == PlayReady)
	{
		Sceneidx = 3;
	}
	else if (true == StartReady)
	{
		Sceneidx = 2;
	}
	else if (true == CollectReady)
	{
		Sceneidx = 1;
	}
	else if (true == LobyReady)
	{
		Sceneidx = 0;
	}


	if (Sceneidx > 0 && loadingSpriteCheck == true)
	{
		if (curidx != Sceneidx)
		{	
			curidx = Sceneidx;
			LoadingBarSR->SetSprite(L"LoadingBar.png", curidx);
		}
	}


	if (Input::Down(L"LCLICK") == true && 4 == Sceneidx)
	{
		GameWindow::MainGameWin()->ChangeScene(L"LobyScene");
	}
}
void LoadingSceneUpdater::ChangeEnd()
{

}
void LoadingSceneUpdater::ChangeStart()
{

}
void LoadingSceneUpdater::DebugRender()
{

	switch (Sceneidx)
	{
	case -1:
	case 0:
		Font->DrawUI(L"FBX 데이터 로딩 중...", 20.0f, { 1425.0f, 650.0f, -5.0f }, Vector4::BLACK);
		break;
	case 1:
		Font->DrawUI(L"스프라이트 로딩 중...", 20.0f, { 1420.0f, 650.0f, -5.0f }, Vector4::BLACK);
		break;
	case 2:
		Font->DrawUI(L"영상 로딩 중...", 20.0f, { 1450.0f, 650.0f, -5.0f }, Vector4::BLACK);
		break;
	case 3:
		Font->DrawUI(L"사운드 및 추가 파일 로딩 중...", 20.0f, { 1375.0f, 650.0f, -5.0f }, Vector4::BLACK);
		break;
	case 4:
		Font->DrawUI(L"로딩 완료!", 20.0f, { 1460.0f, 650.0f, -5.0f }, Vector4::BLACK);
		Font->DrawUI(L"아무 버튼이나 누르세요", 40.0f, { 650.0f, 830.0f, -5.0f }, Vector4::WHITE);
		break;
	default:
		break;
	}
}
