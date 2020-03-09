#include "CollectSceneUpdater.h"


#include <INPUT.h>
#include <GAMEWIN.h>
#include <GAMETHREAD.h>

#include <SPRITE.h>
#include <SPRITERENDER.h>
#include <2DANICOM.h>

#include <GRAPHICDEBUG.h>
#include <LOGICHEADER.h>
#include <SOUND.h>




CollectSceneUpdater::CollectSceneUpdater()
{
}


CollectSceneUpdater::~CollectSceneUpdater()
{
}



void CollectSceneUpdater::Loading()
{

	Pieces.resize(63);
	StandEx.resize(63);
	ClassEx.resize(13);
	RaceEx.resize(15);
	StarEx.resize(6);

	StandNamePlate.resize(63);
	StandNamePlateClass.resize(63);
	StandNamePlateRace.resize(63);
	StandNamePlateRace2.resize(63);

	StandName.resize(63);
	StandNameClass.resize(63);
	StandNameRace.resize(63);
	StandNameRace2.resize(63);

	MenuLetter.resize(3);
	FaceComp.resize(5);

	Font = Resources::Find<HEFONT>(L"1훈떡볶이");

	{
		HPTR<Actor> mainCam = GetScene()->CreateActor(L"LOADINGCAM");
		mainCam->GetTransform()->SetPosition({ 0.0f, 0.0f, -10.0f });
		HPTR<Camera> camera = mainCam->AddComponent<Camera>(RENDERGROUP::RG_PLAYER);
		camera->SetProjMode(PROJMODE::PM_ORTH);
		camera->SetArea(GameWindow::MainGameWin()->Size().x / 100.0f, GameWindow::MainGameWin()->Size().y / 100.0f);
	}

	{
		HPTR<Actor> BPlate = GetScene()->CreateActor();
		BPlate->GetTransform()->SetScale({ 16.0f, 9.0f });
		HPTR<SpriteRenderer> SR = BPlate->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		SR->SetSprite(L"collBG.png", 0);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"collBG.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
	}
	{
		Pieces[0].ID = 1;	Pieces[0].CLASS = 2;	Pieces[0].RACE1 = 1;	Pieces[0].RACE2 = 0;	Pieces[0].LEVEL = 1;  Pieces[0].NAME = L"붉은 도끼 수장";
		Pieces[1].ID = 2;	Pieces[1].CLASS = 9;	Pieces[1].RACE1 = 2;	Pieces[1].RACE2 = 0;	Pieces[1].LEVEL = 1;  Pieces[1].NAME = L"유니콘";
		Pieces[2].ID = 3;   Pieces[2].CLASS = 7;	Pieces[2].RACE1 = 12;	Pieces[2].RACE2 = 0;	Pieces[2].LEVEL = 1;  Pieces[2].NAME = L"오거 마법사";
		Pieces[3].ID = 4;   Pieces[3].CLASS = 2;	Pieces[3].RACE1 = 2;	Pieces[3].RACE2 = 0;	Pieces[3].LEVEL = 1;  Pieces[3].NAME = L"바다코끼리 챔피언";
		Pieces[4].ID = 5;   Pieces[4].CLASS = 3;	Pieces[4].RACE1 = 4;	Pieces[4].RACE2 = 0;	Pieces[4].LEVEL = 1;  Pieces[4].NAME = L"언데드 순찰자";
		Pieces[5].ID = 6;   Pieces[5].CLASS = 5;	Pieces[5].RACE1 = 9;	Pieces[5].RACE2 = 0;	Pieces[5].LEVEL = 1;  Pieces[5].NAME = L"영혼 파괴자";
		Pieces[6].ID = 7;   Pieces[6].CLASS = 11;	Pieces[6].RACE1 = 9;	Pieces[6].RACE2 = 0;	Pieces[6].LEVEL = 1;  Pieces[6].NAME = L"패천자";
		Pieces[7].ID = 8;   Pieces[7].CLASS = 8;	Pieces[7].RACE1 = 3;	Pieces[7].RACE2 = 0;	Pieces[7].LEVEL = 1;  Pieces[7].NAME = L"배신자";
		Pieces[8].ID = 9;   Pieces[8].CLASS = 4;	Pieces[8].RACE1 = 3;	Pieces[8].RACE2 = 0;	Pieces[8].LEVEL = 1;  Pieces[8].NAME = L"프로스트 나이트";
		Pieces[9].ID = 10;  Pieces[9].CLASS = 11;	Pieces[9].RACE1 = 9;	Pieces[9].RACE2 = 0;	Pieces[9].LEVEL = 1;  Pieces[9].NAME = L"땜장이";
		Pieces[10].ID = 11; Pieces[10].CLASS = 10;	Pieces[10].RACE1 = 11;	Pieces[10].RACE2 = 0;	Pieces[10].LEVEL = 1; Pieces[10].NAME = L"금기 주술사";
		Pieces[11].ID = 12; Pieces[11].CLASS = 2;	Pieces[11].RACE1 = 3;	Pieces[11].RACE2 = 0;	Pieces[11].LEVEL = 1; Pieces[11].NAME = L"땅의 정령";
		Pieces[12].ID = 13; Pieces[12].CLASS = 7;	Pieces[12].RACE1 = 6;	Pieces[12].RACE2 = 0;	Pieces[12].LEVEL = 2; Pieces[12].NAME = L"마나의 원천";
		Pieces[13].ID = 14; Pieces[13].CLASS = 3;	Pieces[13].RACE1 = 1;	Pieces[13].RACE2 = 0;	Pieces[13].LEVEL = 2; Pieces[13].NAME = L"해골 사냥꾼";
		Pieces[14].ID = 15; Pieces[14].CLASS = 2;	Pieces[14].RACE1 = 1;	Pieces[14].RACE2 = 0;	Pieces[14].LEVEL = 2; Pieces[14].NAME = L"검객";
		Pieces[15].ID = 16; Pieces[15].CLASS = 11;	Pieces[15].RACE1 = 9;	Pieces[15].RACE2 = 0;	Pieces[15].LEVEL = 2; Pieces[15].NAME = L"절단기";
		Pieces[16].ID = 17; Pieces[16].CLASS = 5;	Pieces[16].RACE1 = 10;	Pieces[16].RACE2 = 0;	Pieces[16].LEVEL = 2; Pieces[16].NAME = L"팬텀 퀸";
		Pieces[17].ID = 18; Pieces[17].CLASS = 7;	Pieces[17].RACE1 = 7;	Pieces[17].RACE2 = 11;	Pieces[17].LEVEL = 2; Pieces[17].NAME = L"빛나는 용";
		Pieces[18].ID = 19; Pieces[18].CLASS = 6;	Pieces[18].RACE1 = 3;	Pieces[18].RACE2 = 0;	Pieces[18].LEVEL = 2; Pieces[18].NAME = L"저주술사";
		Pieces[19].ID = 20; Pieces[19].CLASS = 2;	Pieces[19].RACE1 = 5;	Pieces[19].RACE2 = 0;	Pieces[19].LEVEL = 2; Pieces[19].NAME = L"어비스 가드";
		Pieces[20].ID = 21; Pieces[20].CLASS = 4;	Pieces[20].RACE1 = 10;	Pieces[20].RACE2 = 0;	Pieces[20].LEVEL = 2; Pieces[20].NAME = L"헬 나이트";
		Pieces[21].ID = 22; Pieces[21].CLASS = 9;	Pieces[21].RACE1 = 11;	Pieces[21].RACE2 = 0;	Pieces[21].LEVEL = 3; Pieces[21].NAME = L"자연의 예언자";
		Pieces[22].ID = 23; Pieces[22].CLASS = 4;	Pieces[22].RACE1 = 11;	Pieces[22].RACE2 = 0;	Pieces[22].LEVEL = 2; Pieces[22].NAME = L"블레이드 나이트";
		Pieces[23].ID = 24; Pieces[23].CLASS = 9;	Pieces[23].RACE1 = 11;	Pieces[23].RACE2 = 0;	Pieces[23].LEVEL = 2; Pieces[23].NAME = L"현명한 예언자";
		Pieces[24].ID = 25; Pieces[24].CLASS = 5;	Pieces[24].RACE1 = 13;	Pieces[24].RACE2 = 0;	Pieces[24].LEVEL = 2; Pieces[24].NAME = L"물의 정령";
		Pieces[25].ID = 26; Pieces[25].CLASS = 2;	Pieces[25].RACE1 = 2;	Pieces[25].RACE2 = 6;	Pieces[25].LEVEL = 3; Pieces[25].NAME = L"늑대인간";
		Pieces[26].ID = 27; Pieces[26].CLASS = 6;	Pieces[26].RACE1 = 2;	Pieces[26].RACE2 = 0;	Pieces[26].LEVEL = 3; Pieces[26].NAME = L"악마 독충";
		Pieces[27].ID = 28; Pieces[27].CLASS = 4;	Pieces[27].RACE1 = 6;	Pieces[27].RACE2 = 0;	Pieces[27].LEVEL = 3; Pieces[27].NAME = L"아르갈리 나이트";
		Pieces[28].ID = 29; Pieces[28].CLASS = 7;	Pieces[28].RACE1 = 13;	Pieces[28].RACE2 = 0;	Pieces[28].LEVEL = 3; Pieces[28].NAME = L"우뢰의 정령";
		Pieces[29].ID = 30; Pieces[29].CLASS = 3;	Pieces[29].RACE1 = 11;	Pieces[29].RACE2 = 0;	Pieces[29].LEVEL = 3; Pieces[29].NAME = L"바람 순찰자";
		Pieces[30].ID = 31; Pieces[30].CLASS = 5;	Pieces[30].RACE1 = 11;	Pieces[30].RACE2 = 0;	Pieces[30].LEVEL = 3; Pieces[30].NAME = L"유령의 그림자";
		Pieces[31].ID = 32; Pieces[31].CLASS = 4;	Pieces[31].RACE1 = 4;	Pieces[31].RACE2 = 0;	Pieces[31].LEVEL = 3; Pieces[31].NAME = L"악령 기사";
		Pieces[32].ID = 33; Pieces[32].CLASS = 5;	Pieces[32].RACE1 = 2;	Pieces[32].RACE2 = 0;	Pieces[32].LEVEL = 3; Pieces[32].NAME = L"사막의 주인";
		Pieces[33].ID = 34; Pieces[33].CLASS = 5;	Pieces[33].RACE1 = 5;	Pieces[33].RACE2 = 0;	Pieces[33].LEVEL = 3; Pieces[33].NAME = L"심연의 그림자";
		Pieces[34].ID = 35; Pieces[34].CLASS = 3;	Pieces[34].RACE1 = 8;	Pieces[34].RACE2 = 0;	Pieces[34].LEVEL = 3; Pieces[34].NAME = L"드워프 소총수";
		Pieces[35].ID = 36; Pieces[35].CLASS = 5;	Pieces[35].RACE1 = 7;	Pieces[35].RACE2 = 0;	Pieces[35].LEVEL = 3; Pieces[35].NAME = L"베놈";
		Pieces[36].ID = 37; Pieces[36].CLASS = 6;	Pieces[36].RACE1 = 10;	Pieces[36].RACE2 = 0;	Pieces[36].LEVEL = 3; Pieces[36].NAME = L"그림자 마귀";
		Pieces[37].ID = 38; Pieces[37].CLASS = 7;	Pieces[37].RACE1 = 6;	Pieces[37].RACE2 = 0;	Pieces[37].LEVEL = 3; Pieces[37].NAME = L"화염의 마법사";
		Pieces[38].ID = 39; Pieces[38].CLASS = 10;	Pieces[38].RACE1 = 10;	Pieces[38].RACE2 = 0;	Pieces[38].LEVEL = 3; Pieces[38].NAME = L"악마 주술사";
		Pieces[39].ID = 40; Pieces[39].CLASS = 2;	Pieces[39].RACE1 = 10;	Pieces[39].RACE2 = 0;	Pieces[39].LEVEL = 5; Pieces[39].NAME = L"둠 아비터";
		Pieces[40].ID = 41; Pieces[40].CLASS = 2;	Pieces[40].RACE1 = 6;	Pieces[40].RACE2 = 0;	Pieces[40].LEVEL = 4; Pieces[40].NAME = L"해적 선장";
		Pieces[41].ID = 42; Pieces[41].CLASS = 2;	Pieces[41].RACE1 = 3;	Pieces[41].RACE2 = 0;	Pieces[41].LEVEL = 4; Pieces[41].NAME = L"버서커";
		Pieces[42].ID = 43; Pieces[42].CLASS = 7;	Pieces[42].RACE1 = 6;	Pieces[42].RACE2 = 0;	Pieces[42].LEVEL = 4; Pieces[42].NAME = L"토르톨란족 장로";
		Pieces[43].ID = 44; Pieces[43].CLASS = 6;	Pieces[43].RACE1 = 4;	Pieces[43].RACE2 = 0;	Pieces[43].LEVEL = 4; Pieces[43].NAME = L"소울 리퍼";
		Pieces[44].ID = 45; Pieces[44].CLASS = 5;	Pieces[44].RACE1 = 11;	Pieces[44].RACE2 = 0;	Pieces[44].LEVEL = 4; Pieces[44].NAME = L"빛의 자객";
		Pieces[45].ID = 46; Pieces[45].CLASS = 6;	Pieces[45].RACE1 = 9;	Pieces[45].RACE2 = 0;	Pieces[45].LEVEL = 4; Pieces[45].NAME = L"베노맨서";
		Pieces[46].ID = 47; Pieces[46].CLASS = 8;	Pieces[46].RACE1 = 1;	Pieces[46].RACE2 = 0;	Pieces[46].LEVEL = 4; Pieces[46].NAME = L"폭풍 주술사";
		Pieces[47].ID = 48; Pieces[47].CLASS = 3;	Pieces[47].RACE1 = 5;	Pieces[47].RACE2 = 0;	Pieces[47].LEVEL = 4; Pieces[47].NAME = L"세이렌";
		Pieces[48].ID = 49; Pieces[48].CLASS = 4;	Pieces[48].RACE1 = 7;	Pieces[48].RACE2 = 6;	Pieces[48].LEVEL = 4; Pieces[48].NAME = L"용기사";
		Pieces[49].ID = 50; Pieces[49].CLASS = 9;	Pieces[49].RACE1 = 2;	Pieces[49].RACE2 = 0;	Pieces[49].LEVEL = 4; Pieces[49].NAME = L"날카로운 발톱";
		Pieces[50].ID = 51; Pieces[50].CLASS = 11;	Pieces[50].RACE1 = 8;	Pieces[50].RACE2 = 0;	Pieces[50].LEVEL = 5; Pieces[50].NAME = L"자이로콥터";
		Pieces[51].ID = 52; Pieces[51].CLASS = 7;	Pieces[51].RACE1 = 4;	Pieces[51].RACE2 = 0;	Pieces[51].LEVEL = 5; Pieces[51].NAME = L"황혼의 마법사";
		Pieces[52].ID = 53; Pieces[52].CLASS = 3;	Pieces[52].RACE1 = 5;	Pieces[52].RACE2 = 0;	Pieces[52].LEVEL = 5; Pieces[52].NAME = L"해일 사냥꾼";
		Pieces[53].ID = 54; Pieces[53].CLASS = 6;	Pieces[53].RACE1 = 13;	Pieces[53].RACE2 = 0;	Pieces[53].LEVEL = 5; Pieces[53].NAME = L"어둠의 정령";
		Pieces[54].ID = 55; Pieces[54].CLASS = 11;	Pieces[54].RACE1 = 9;	Pieces[54].RACE2 = 0;	Pieces[54].LEVEL = 5; Pieces[54].NAME = L"파멸자";
		Pieces[55].ID = 56; Pieces[55].CLASS = 1;	Pieces[55].RACE1 = 11;	Pieces[55].RACE2 = 0;	Pieces[55].LEVEL = 5; Pieces[55].NAME = L"기이한 알";
		Pieces[56].ID = 57; Pieces[56].CLASS = 2;	Pieces[56].RACE1 = 14;	Pieces[56].RACE2 = 0;	Pieces[56].LEVEL = 1; Pieces[56].NAME = L"전쟁의 신";
		Pieces[57].ID = 58; Pieces[57].CLASS = 3;	Pieces[57].RACE1 = 11;	Pieces[57].RACE2 = 0;	Pieces[57].LEVEL = 2; Pieces[57].NAME = L"날빛 궁수";
		Pieces[58].ID = 59; Pieces[58].CLASS = 6;	Pieces[58].RACE1 = 4;	Pieces[58].RACE2 = 0;	Pieces[58].LEVEL = 5; Pieces[58].NAME = L"고스트 예언가";
		Pieces[59].ID = 60; Pieces[59].CLASS = 7;	Pieces[59].RACE1 = 14;	Pieces[59].RACE2 = 0;	Pieces[59].LEVEL = 5; Pieces[59].NAME = L"천둥의 신";
		Pieces[60].ID = 61; Pieces[60].CLASS = 12;	Pieces[60].RACE1 = 3;	Pieces[60].RACE2 = 0;	Pieces[60].LEVEL = 3; Pieces[60].NAME = L"점술사";
		Pieces[61].ID = 62; Pieces[61].CLASS = 7;	Pieces[61].RACE1 = 7;	Pieces[61].RACE2 = 0;	Pieces[61].LEVEL = 1; Pieces[61].NAME = L"얼음 박쥐";
		Pieces[62].ID = 63; Pieces[62].CLASS = 2;	Pieces[62].RACE1 = 10;	Pieces[62].RACE2 = 0;	Pieces[62].LEVEL = 5; Pieces[62].NAME = L"추락 수호자";
	}
	{
		Pieces[0].Anim[0]  = L"Stand01_1.png"; Pieces[0].Anim[1]  = L"Stand01_2.png"; Pieces[0].Anim[2]  = L"Stand01_3.png";
		Pieces[1].Anim[0]  = L"Stand02_1.png"; Pieces[1].Anim[1]  = L"Stand02_2.png"; Pieces[1].Anim[2]  = L"Stand02_3.png";
		Pieces[2].Anim[0]  = L"Stand03_1.png"; Pieces[2].Anim[1]  = L"Stand03_2.png"; Pieces[2].Anim[2]  = L"Stand03_3.png";
		Pieces[3].Anim[0]  = L"Stand04_1.png"; Pieces[3].Anim[1]  = L"Stand04_2.png"; Pieces[3].Anim[2]  = L"Stand04_3.png";
		Pieces[4].Anim[0]  = L"Stand05_1.png"; Pieces[4].Anim[1]  = L"Stand05_2.png"; Pieces[4].Anim[2]  = L"Stand05_3.png";
		Pieces[5].Anim[0]  = L"Stand06_1.png"; Pieces[5].Anim[1]  = L"Stand06_2.png"; Pieces[5].Anim[2]  = L"Stand06_3.png";
		Pieces[6].Anim[0]  = L"Stand07_1.png"; Pieces[6].Anim[1]  = L"Stand07_2.png"; Pieces[6].Anim[2]  = L"Stand07_3.png";
		Pieces[7].Anim[0]  = L"Stand08_1.png"; Pieces[7].Anim[1]  = L"Stand08_2.png"; Pieces[7].Anim[2]  = L"Stand08_3.png";
		Pieces[8].Anim[0]  = L"Stand09_1.png"; Pieces[8].Anim[1]  = L"Stand09_2.png"; Pieces[8].Anim[2]  = L"Stand09_3.png";
		Pieces[9].Anim[0]  = L"Stand10_1.png"; Pieces[9].Anim[1]  = L"Stand10_2.png"; Pieces[9].Anim[2]  = L"Stand10_3.png";
		Pieces[10].Anim[0] = L"Stand11_1.png"; Pieces[10].Anim[1] = L"Stand11_2.png"; Pieces[10].Anim[2] = L"Stand11_3.png";
		Pieces[11].Anim[0] = L"Stand12_1.png"; Pieces[11].Anim[1] = L"Stand12_2.png"; Pieces[11].Anim[2] = L"Stand12_3.png";
		Pieces[12].Anim[0] = L"Stand13_1.png"; Pieces[12].Anim[1] = L"Stand13_2.png"; Pieces[12].Anim[2] = L"Stand13_3.png";
		Pieces[13].Anim[0] = L"Stand14_1.png"; Pieces[13].Anim[1] = L"Stand14_2.png"; Pieces[13].Anim[2] = L"Stand14_3.png";
		Pieces[14].Anim[0] = L"Stand15_1.png"; Pieces[14].Anim[1] = L"Stand15_2.png"; Pieces[14].Anim[2] = L"Stand15_3.png";
		Pieces[15].Anim[0] = L"Stand16_1.png"; Pieces[15].Anim[1] = L"Stand16_2.png"; Pieces[15].Anim[2] = L"Stand16_3.png";
		Pieces[16].Anim[0] = L"Stand17_1.png"; Pieces[16].Anim[1] = L"Stand17_2.png"; Pieces[16].Anim[2] = L"Stand17_3.png";
		Pieces[17].Anim[0] = L"Stand18_1.png"; Pieces[17].Anim[1] = L"Stand18_2.png"; Pieces[17].Anim[2] = L"Stand18_3.png";
		Pieces[18].Anim[0] = L"Stand19_1.png"; Pieces[18].Anim[1] = L"Stand19_2.png"; Pieces[18].Anim[2] = L"Stand19_3.png";
		Pieces[19].Anim[0] = L"Stand20_1.png"; Pieces[19].Anim[1] = L"Stand20_2.png"; Pieces[19].Anim[2] = L"Stand20_3.png";
		Pieces[20].Anim[0] = L"Stand21_1.png"; Pieces[20].Anim[1] = L"Stand21_2.png"; Pieces[20].Anim[2] = L"Stand21_3.png";
		Pieces[21].Anim[0] = L"Stand22_1.png"; Pieces[21].Anim[1] = L"Stand22_2.png"; Pieces[21].Anim[2] = L"Stand22_3.png";
		Pieces[22].Anim[0] = L"Stand23_1.png"; Pieces[22].Anim[1] = L"Stand23_2.png"; Pieces[22].Anim[2] = L"Stand23_3.png";
		Pieces[23].Anim[0] = L"Stand24_1.png"; Pieces[23].Anim[1] = L"Stand24_2.png"; Pieces[23].Anim[2] = L"Stand24_3.png";
		Pieces[24].Anim[0] = L"Stand25_1.png"; Pieces[24].Anim[1] = L"Stand25_2.png"; Pieces[24].Anim[2] = L"Stand25_3.png";
		Pieces[25].Anim[0] = L"Stand26_1.png"; Pieces[25].Anim[1] = L"Stand26_2.png"; Pieces[25].Anim[2] = L"Stand26_3.png";
		Pieces[26].Anim[0] = L"Stand27_1.png"; Pieces[26].Anim[1] = L"Stand27_2.png"; Pieces[26].Anim[2] = L"Stand27_3.png";
		Pieces[27].Anim[0] = L"Stand28_1.png"; Pieces[27].Anim[1] = L"Stand28_2.png"; Pieces[27].Anim[2] = L"Stand28_3.png";
		Pieces[28].Anim[0] = L"Stand29_1.png"; Pieces[28].Anim[1] = L"Stand29_2.png"; Pieces[28].Anim[2] = L"Stand29_3.png";
		Pieces[29].Anim[0] = L"Stand30_1.png"; Pieces[29].Anim[1] = L"Stand30_2.png"; Pieces[29].Anim[2] = L"Stand30_3.png";
		Pieces[30].Anim[0] = L"Stand31_1.png"; Pieces[30].Anim[1] = L"Stand31_2.png"; Pieces[30].Anim[2] = L"Stand31_3.png";
		Pieces[31].Anim[0] = L"Stand32_1.png"; Pieces[31].Anim[1] = L"Stand32_2.png"; Pieces[31].Anim[2] = L"Stand32_3.png";
		Pieces[32].Anim[0] = L"Stand33_1.png"; Pieces[32].Anim[1] = L"Stand33_2.png"; Pieces[32].Anim[2] = L"Stand33_3.png";
		Pieces[33].Anim[0] = L"Stand34_1.png"; Pieces[33].Anim[1] = L"Stand34_2.png"; Pieces[33].Anim[2] = L"Stand34_3.png";
		Pieces[34].Anim[0] = L"Stand35_1.png"; Pieces[34].Anim[1] = L"Stand35_2.png"; Pieces[34].Anim[2] = L"Stand35_3.png";
		Pieces[35].Anim[0] = L"Stand36_1.png"; Pieces[35].Anim[1] = L"Stand36_2.png"; Pieces[35].Anim[2] = L"Stand36_3.png";
		Pieces[36].Anim[0] = L"Stand37_1.png"; Pieces[36].Anim[1] = L"Stand37_2.png"; Pieces[36].Anim[2] = L"Stand37_3.png";
		Pieces[37].Anim[0] = L"Stand38_1.png"; Pieces[37].Anim[1] = L"Stand38_2.png"; Pieces[37].Anim[2] = L"Stand38_3.png";
		Pieces[38].Anim[0] = L"Stand39_1.png"; Pieces[38].Anim[1] = L"Stand39_2.png"; Pieces[38].Anim[2] = L"Stand39_3.png";
		Pieces[39].Anim[0] = L"Stand40_1.png"; Pieces[39].Anim[1] = L"Stand40_2.png"; Pieces[39].Anim[2] = L"Stand40_3.png";
		Pieces[40].Anim[0] = L"Stand41_1.png"; Pieces[40].Anim[1] = L"Stand41_2.png"; Pieces[40].Anim[2] = L"Stand41_3.png";
		Pieces[41].Anim[0] = L"Stand42_1.png"; Pieces[41].Anim[1] = L"Stand42_2.png"; Pieces[41].Anim[2] = L"Stand42_3.png";
		Pieces[42].Anim[0] = L"Stand43_1.png"; Pieces[42].Anim[1] = L"Stand43_2.png"; Pieces[42].Anim[2] = L"Stand43_3.png";
		Pieces[43].Anim[0] = L"Stand44_1.png"; Pieces[43].Anim[1] = L"Stand44_2.png"; Pieces[43].Anim[2] = L"Stand44_3.png";
		Pieces[44].Anim[0] = L"Stand45_1.png"; Pieces[44].Anim[1] = L"Stand45_2.png"; Pieces[44].Anim[2] = L"Stand45_3.png";
		Pieces[45].Anim[0] = L"Stand46_1.png"; Pieces[45].Anim[1] = L"Stand46_2.png"; Pieces[45].Anim[2] = L"Stand46_3.png";
		Pieces[46].Anim[0] = L"Stand47_1.png"; Pieces[46].Anim[1] = L"Stand47_2.png"; Pieces[46].Anim[2] = L"Stand47_3.png";
		Pieces[47].Anim[0] = L"Stand48_1.png"; Pieces[47].Anim[1] = L"Stand48_2.png"; Pieces[47].Anim[2] = L"Stand48_3.png";
		Pieces[48].Anim[0] = L"Stand49_1.png"; Pieces[48].Anim[1] = L"Stand49_2.png"; Pieces[48].Anim[2] = L"Stand49_3.png";
		Pieces[49].Anim[0] = L"Stand50_1.png"; Pieces[49].Anim[1] = L"Stand50_2.png"; Pieces[49].Anim[2] = L"Stand50_3.png";
		Pieces[50].Anim[0] = L"Stand51_1.png"; Pieces[50].Anim[1] = L"Stand51_2.png"; Pieces[50].Anim[2] = L"Stand51_3.png";
		Pieces[51].Anim[0] = L"Stand52_1.png"; Pieces[51].Anim[1] = L"Stand52_2.png"; Pieces[51].Anim[2] = L"Stand52_3.png";
		Pieces[52].Anim[0] = L"Stand53_1.png"; Pieces[52].Anim[1] = L"Stand53_2.png"; Pieces[52].Anim[2] = L"Stand53_3.png";
		Pieces[53].Anim[0] = L"Stand54_1.png"; Pieces[53].Anim[1] = L"Stand54_2.png"; Pieces[53].Anim[2] = L"Stand54_3.png";
		Pieces[54].Anim[0] = L"Stand55_1.png"; Pieces[54].Anim[1] = L"Stand55_2.png"; Pieces[54].Anim[2] = L"Stand55_3.png";
		Pieces[55].Anim[0] = L"Stand56_1.png"; Pieces[55].Anim[1] = L"Stand56_2.png"; Pieces[55].Anim[2] = L"Stand56_3.png";
		Pieces[56].Anim[0] = L"Stand57_1.png"; Pieces[56].Anim[1] = L"Stand57_2.png"; Pieces[56].Anim[2] = L"Stand57_3.png";
		Pieces[57].Anim[0] = L"Stand58_1.png"; Pieces[57].Anim[1] = L"Stand58_2.png"; Pieces[57].Anim[2] = L"Stand58_3.png";
		Pieces[58].Anim[0] = L"Stand59_1.png"; Pieces[58].Anim[1] = L"Stand59_2.png"; Pieces[58].Anim[2] = L"Stand59_3.png";
		Pieces[59].Anim[0] = L"Stand60_1.png"; Pieces[59].Anim[1] = L"Stand60_2.png"; Pieces[59].Anim[2] = L"Stand60_3.png";
		Pieces[60].Anim[0] = L"Stand61_1.png"; Pieces[60].Anim[1] = L"Stand61_2.png"; Pieces[60].Anim[2] = L"Stand61_3.png";
		Pieces[61].Anim[0] = L"Stand62_1.png"; Pieces[61].Anim[1] = L"Stand62_2.png"; Pieces[61].Anim[2] = L"Stand62_3.png";
		Pieces[62].Anim[0] = L"Stand63_1.png"; Pieces[62].Anim[1] = L"Stand63_2.png"; Pieces[62].Anim[2] = L"Stand63_3.png";
	}
	{
		Resources::Create<Sprite>(Pieces[0].Anim[0].c_str(), Pieces[0].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[0].Anim[1].c_str(), Pieces[0].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[0].Anim[2].c_str(), Pieces[0].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[1].Anim[0].c_str(), Pieces[1].Anim[0].c_str(), 8, 8, 0, 60);
		Resources::Create<Sprite>(Pieces[1].Anim[1].c_str(), Pieces[1].Anim[1].c_str(), 8, 8, 0, 60);
		Resources::Create<Sprite>(Pieces[1].Anim[2].c_str(), Pieces[1].Anim[2].c_str(), 8, 8, 0, 60);

		Resources::Create<Sprite>(Pieces[2].Anim[0].c_str(), Pieces[2].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[2].Anim[1].c_str(), Pieces[2].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[2].Anim[2].c_str(), Pieces[2].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[3].Anim[0].c_str(), Pieces[3].Anim[0].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[3].Anim[1].c_str(), Pieces[3].Anim[1].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[3].Anim[2].c_str(), Pieces[3].Anim[2].c_str(), 8, 7, 0, 50);

		Resources::Create<Sprite>(Pieces[4].Anim[0].c_str(), Pieces[4].Anim[0].c_str(), 8, 8, 0, 64);
		Resources::Create<Sprite>(Pieces[4].Anim[1].c_str(), Pieces[4].Anim[1].c_str(), 8, 8, 0, 64);
		Resources::Create<Sprite>(Pieces[4].Anim[2].c_str(), Pieces[4].Anim[2].c_str(), 8, 8, 0, 64);

		Resources::Create<Sprite>(Pieces[5].Anim[0].c_str(), Pieces[5].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[5].Anim[1].c_str(), Pieces[5].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[5].Anim[2].c_str(), Pieces[5].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[6].Anim[0].c_str(), Pieces[6].Anim[0].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[6].Anim[1].c_str(), Pieces[6].Anim[1].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[6].Anim[2].c_str(), Pieces[6].Anim[2].c_str(), 8, 7, 0, 50);

		Resources::Create<Sprite>(Pieces[7].Anim[0].c_str(), Pieces[7].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[7].Anim[1].c_str(), Pieces[7].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[7].Anim[2].c_str(), Pieces[7].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[8].Anim[0].c_str(), Pieces[8].Anim[0].c_str(), 8, 4, 0, 28);
		Resources::Create<Sprite>(Pieces[8].Anim[1].c_str(), Pieces[8].Anim[1].c_str(), 8, 4, 0, 28);
		Resources::Create<Sprite>(Pieces[8].Anim[2].c_str(), Pieces[8].Anim[2].c_str(), 8, 4, 0, 28);

		Resources::Create<Sprite>(Pieces[9].Anim[0].c_str(), Pieces[9].Anim[0].c_str(), 8, 4, 0, 32);
		Resources::Create<Sprite>(Pieces[9].Anim[1].c_str(), Pieces[9].Anim[1].c_str(), 8, 4, 0, 32);
		Resources::Create<Sprite>(Pieces[9].Anim[2].c_str(), Pieces[9].Anim[2].c_str(), 8, 4, 0, 32);

		Resources::Create<Sprite>(Pieces[10].Anim[0].c_str(), Pieces[10].Anim[0].c_str(), 8, 6, 0, 48);
		Resources::Create<Sprite>(Pieces[10].Anim[1].c_str(), Pieces[10].Anim[1].c_str(), 8, 6, 0, 48);
		Resources::Create<Sprite>(Pieces[10].Anim[2].c_str(), Pieces[10].Anim[2].c_str(), 8, 6, 0, 48);

		Resources::Create<Sprite>(Pieces[11].Anim[0].c_str(), Pieces[11].Anim[0].c_str(), 8, 6, 0, 48);
		Resources::Create<Sprite>(Pieces[11].Anim[1].c_str(), Pieces[11].Anim[1].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[11].Anim[2].c_str(), Pieces[11].Anim[2].c_str(), 8, 7, 0, 50);

		Resources::Create<Sprite>(Pieces[12].Anim[0].c_str(), Pieces[12].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[12].Anim[1].c_str(), Pieces[12].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[12].Anim[2].c_str(), Pieces[12].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[13].Anim[0].c_str(), Pieces[13].Anim[0].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[13].Anim[1].c_str(), Pieces[13].Anim[1].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[13].Anim[2].c_str(), Pieces[13].Anim[2].c_str(), 8, 7, 0, 50);

		Resources::Create<Sprite>(Pieces[14].Anim[0].c_str(), Pieces[14].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[14].Anim[1].c_str(), Pieces[14].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[14].Anim[2].c_str(), Pieces[14].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[15].Anim[0].c_str(), Pieces[15].Anim[0].c_str(), 8, 8, 0, 60);
		Resources::Create<Sprite>(Pieces[15].Anim[1].c_str(), Pieces[15].Anim[1].c_str(), 8, 8, 0, 60);
		Resources::Create<Sprite>(Pieces[15].Anim[2].c_str(), Pieces[15].Anim[2].c_str(), 8, 8, 0, 60);

		Resources::Create<Sprite>(Pieces[16].Anim[0].c_str(), Pieces[16].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[16].Anim[1].c_str(), Pieces[16].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[16].Anim[2].c_str(), Pieces[16].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[17].Anim[0].c_str(), Pieces[17].Anim[0].c_str(), 8, 3, 0, 20);
		Resources::Create<Sprite>(Pieces[17].Anim[1].c_str(), Pieces[17].Anim[1].c_str(), 8, 3, 0, 20);
		Resources::Create<Sprite>(Pieces[17].Anim[2].c_str(), Pieces[17].Anim[2].c_str(), 8, 3, 0, 20);

		Resources::Create<Sprite>(Pieces[18].Anim[0].c_str(), Pieces[18].Anim[0].c_str(), 8, 6, 0, 45);
		Resources::Create<Sprite>(Pieces[18].Anim[1].c_str(), Pieces[18].Anim[1].c_str(), 8, 6, 0, 45);
		Resources::Create<Sprite>(Pieces[18].Anim[2].c_str(), Pieces[18].Anim[2].c_str(), 8, 6, 0, 45);

		Resources::Create<Sprite>(Pieces[19].Anim[0].c_str(), Pieces[19].Anim[0].c_str(), 8, 8, 0, 60);
		Resources::Create<Sprite>(Pieces[19].Anim[1].c_str(), Pieces[19].Anim[1].c_str(), 8, 8, 0, 60);
		Resources::Create<Sprite>(Pieces[19].Anim[2].c_str(), Pieces[19].Anim[2].c_str(), 8, 8, 0, 60);

		Resources::Create<Sprite>(Pieces[20].Anim[0].c_str(), Pieces[20].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[20].Anim[1].c_str(), Pieces[20].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[20].Anim[2].c_str(), Pieces[20].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[21].Anim[0].c_str(), Pieces[21].Anim[0].c_str(), 8, 10, 0, 80);
		Resources::Create<Sprite>(Pieces[21].Anim[1].c_str(), Pieces[21].Anim[1].c_str(), 8, 10, 0, 80);
		Resources::Create<Sprite>(Pieces[21].Anim[2].c_str(), Pieces[21].Anim[2].c_str(), 8, 10, 0, 80);

		Resources::Create<Sprite>(Pieces[22].Anim[0].c_str(), Pieces[22].Anim[0].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[22].Anim[1].c_str(), Pieces[22].Anim[1].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[22].Anim[2].c_str(), Pieces[22].Anim[2].c_str(), 8, 7, 0, 50);

		Resources::Create<Sprite>(Pieces[23].Anim[0].c_str(), Pieces[23].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[23].Anim[1].c_str(), Pieces[23].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[23].Anim[2].c_str(), Pieces[23].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[24].Anim[0].c_str(), Pieces[24].Anim[0].c_str(), 8, 6, 0, 48);
		Resources::Create<Sprite>(Pieces[24].Anim[1].c_str(), Pieces[24].Anim[1].c_str(), 8, 6, 0, 48);
		Resources::Create<Sprite>(Pieces[24].Anim[2].c_str(), Pieces[24].Anim[2].c_str(), 8, 6, 0, 48);

		Resources::Create<Sprite>(Pieces[25].Anim[0].c_str(), Pieces[25].Anim[0].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[25].Anim[1].c_str(), Pieces[25].Anim[1].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[25].Anim[2].c_str(), Pieces[25].Anim[2].c_str(), 8, 7, 0, 50);

		Resources::Create<Sprite>(Pieces[26].Anim[0].c_str(), Pieces[26].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[26].Anim[1].c_str(), Pieces[26].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[26].Anim[2].c_str(), Pieces[26].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[27].Anim[0].c_str(), Pieces[27].Anim[0].c_str(), 8, 6, 0, 45);
		Resources::Create<Sprite>(Pieces[27].Anim[1].c_str(), Pieces[27].Anim[1].c_str(), 8, 6, 0, 45);
		Resources::Create<Sprite>(Pieces[27].Anim[2].c_str(), Pieces[27].Anim[2].c_str(), 8, 6, 0, 45);

		Resources::Create<Sprite>(Pieces[28].Anim[0].c_str(), Pieces[28].Anim[0].c_str(), 8, 7, 0, 52);
		Resources::Create<Sprite>(Pieces[28].Anim[1].c_str(), Pieces[28].Anim[1].c_str(), 8, 7, 0, 52);
		Resources::Create<Sprite>(Pieces[28].Anim[2].c_str(), Pieces[28].Anim[2].c_str(), 8, 7, 0, 52);

		Resources::Create<Sprite>(Pieces[29].Anim[0].c_str(), Pieces[29].Anim[0].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[29].Anim[1].c_str(), Pieces[29].Anim[1].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[29].Anim[2].c_str(), Pieces[29].Anim[2].c_str(), 8, 7, 0, 50);

		Resources::Create<Sprite>(Pieces[30].Anim[0].c_str(), Pieces[30].Anim[0].c_str(), 8, 8, 0, 64);
		Resources::Create<Sprite>(Pieces[30].Anim[1].c_str(), Pieces[30].Anim[1].c_str(), 8, 8, 0, 64);
		Resources::Create<Sprite>(Pieces[30].Anim[2].c_str(), Pieces[30].Anim[2].c_str(), 8, 8, 0, 64);

		Resources::Create<Sprite>(Pieces[31].Anim[0].c_str(), Pieces[31].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[31].Anim[1].c_str(), Pieces[31].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[31].Anim[2].c_str(), Pieces[31].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[32].Anim[0].c_str(), Pieces[32].Anim[0].c_str(), 8, 4, 0, 30);
		Resources::Create<Sprite>(Pieces[32].Anim[1].c_str(), Pieces[32].Anim[1].c_str(), 8, 4, 0, 30);
		Resources::Create<Sprite>(Pieces[32].Anim[2].c_str(), Pieces[32].Anim[2].c_str(), 8, 4, 0, 30);

		Resources::Create<Sprite>(Pieces[33].Anim[0].c_str(), Pieces[33].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[33].Anim[1].c_str(), Pieces[33].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[33].Anim[2].c_str(), Pieces[33].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[34].Anim[0].c_str(), Pieces[34].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[34].Anim[1].c_str(), Pieces[34].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[34].Anim[2].c_str(), Pieces[34].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[35].Anim[0].c_str(), Pieces[35].Anim[0].c_str(), 8, 4, 0, 26);
		Resources::Create<Sprite>(Pieces[35].Anim[1].c_str(), Pieces[35].Anim[1].c_str(), 8, 4, 0, 26);
		Resources::Create<Sprite>(Pieces[35].Anim[2].c_str(), Pieces[35].Anim[2].c_str(), 8, 4, 0, 26);

		Resources::Create<Sprite>(Pieces[36].Anim[0].c_str(), Pieces[36].Anim[0].c_str(), 8, 8, 0, 62);
		Resources::Create<Sprite>(Pieces[36].Anim[1].c_str(), Pieces[36].Anim[1].c_str(), 8, 8, 0, 62);
		Resources::Create<Sprite>(Pieces[36].Anim[2].c_str(), Pieces[36].Anim[2].c_str(), 8, 8, 0, 62);

		Resources::Create<Sprite>(Pieces[37].Anim[0].c_str(), Pieces[37].Anim[0].c_str(), 8, 8, 0, 60);
		Resources::Create<Sprite>(Pieces[37].Anim[1].c_str(), Pieces[37].Anim[1].c_str(), 8, 8, 0, 60);
		Resources::Create<Sprite>(Pieces[37].Anim[2].c_str(), Pieces[37].Anim[2].c_str(), 8, 8, 0, 60);

		Resources::Create<Sprite>(Pieces[38].Anim[0].c_str(), Pieces[38].Anim[0].c_str(), 8, 6, 0, 42);
		Resources::Create<Sprite>(Pieces[38].Anim[1].c_str(), Pieces[38].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[38].Anim[2].c_str(), Pieces[38].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[39].Anim[0].c_str(), Pieces[39].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[39].Anim[1].c_str(), Pieces[39].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[39].Anim[2].c_str(), Pieces[39].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[40].Anim[0].c_str(), Pieces[40].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[40].Anim[1].c_str(), Pieces[40].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[40].Anim[2].c_str(), Pieces[40].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[41].Anim[0].c_str(), Pieces[41].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[41].Anim[1].c_str(), Pieces[41].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[41].Anim[2].c_str(), Pieces[41].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[42].Anim[0].c_str(), Pieces[42].Anim[0].c_str(), 8, 6, 0, 45);
		Resources::Create<Sprite>(Pieces[42].Anim[1].c_str(), Pieces[42].Anim[1].c_str(), 8, 6, 0, 45);
		Resources::Create<Sprite>(Pieces[42].Anim[2].c_str(), Pieces[42].Anim[2].c_str(), 8, 6, 0, 45);

		Resources::Create<Sprite>(Pieces[43].Anim[0].c_str(), Pieces[43].Anim[0].c_str(), 8, 6, 0, 41);
		Resources::Create<Sprite>(Pieces[43].Anim[1].c_str(), Pieces[43].Anim[1].c_str(), 8, 6, 0, 41);
		Resources::Create<Sprite>(Pieces[43].Anim[2].c_str(), Pieces[43].Anim[2].c_str(), 8, 6, 0, 41);

		Resources::Create<Sprite>(Pieces[44].Anim[0].c_str(), Pieces[44].Anim[0].c_str(), 8, 7, 0, 56);
		Resources::Create<Sprite>(Pieces[44].Anim[1].c_str(), Pieces[44].Anim[1].c_str(), 8, 7, 0, 56);
		Resources::Create<Sprite>(Pieces[44].Anim[2].c_str(), Pieces[44].Anim[2].c_str(), 8, 7, 0, 56);

		Resources::Create<Sprite>(Pieces[45].Anim[0].c_str(), Pieces[45].Anim[0].c_str(), 8, 8, 0, 60);
		Resources::Create<Sprite>(Pieces[45].Anim[1].c_str(), Pieces[45].Anim[1].c_str(), 8, 8, 0, 60);
		Resources::Create<Sprite>(Pieces[45].Anim[2].c_str(), Pieces[45].Anim[2].c_str(), 8, 8, 0, 60);

		Resources::Create<Sprite>(Pieces[46].Anim[0].c_str(), Pieces[46].Anim[0].c_str(), 8, 6, 0, 44);
		Resources::Create<Sprite>(Pieces[46].Anim[1].c_str(), Pieces[46].Anim[1].c_str(), 8, 6, 0, 44);
		Resources::Create<Sprite>(Pieces[46].Anim[2].c_str(), Pieces[46].Anim[2].c_str(), 8, 6, 0, 44);

		Resources::Create<Sprite>(Pieces[47].Anim[0].c_str(), Pieces[47].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[47].Anim[1].c_str(), Pieces[47].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[47].Anim[2].c_str(), Pieces[47].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[48].Anim[0].c_str(), Pieces[48].Anim[0].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[48].Anim[1].c_str(), Pieces[48].Anim[1].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[48].Anim[2].c_str(), Pieces[48].Anim[2].c_str(), 8, 7, 0, 50);

		Resources::Create<Sprite>(Pieces[49].Anim[0].c_str(), Pieces[49].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[49].Anim[1].c_str(), Pieces[49].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[49].Anim[2].c_str(), Pieces[49].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[50].Anim[0].c_str(), Pieces[50].Anim[0].c_str(), 8, 5, 0, 33);
		Resources::Create<Sprite>(Pieces[50].Anim[1].c_str(), Pieces[50].Anim[1].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[50].Anim[2].c_str(), Pieces[50].Anim[2].c_str(), 8, 7, 0, 50);

		Resources::Create<Sprite>(Pieces[51].Anim[0].c_str(), Pieces[51].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[51].Anim[1].c_str(), Pieces[51].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[51].Anim[2].c_str(), Pieces[51].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[52].Anim[0].c_str(), Pieces[52].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[52].Anim[1].c_str(), Pieces[52].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[52].Anim[2].c_str(), Pieces[52].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[53].Anim[0].c_str(), Pieces[53].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[53].Anim[1].c_str(), Pieces[53].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[53].Anim[2].c_str(), Pieces[53].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[54].Anim[0].c_str(), Pieces[54].Anim[0].c_str(), 8, 9, 0, 70);
		Resources::Create<Sprite>(Pieces[54].Anim[1].c_str(), Pieces[54].Anim[1].c_str(), 8, 8, 0, 60);
		Resources::Create<Sprite>(Pieces[54].Anim[2].c_str(), Pieces[54].Anim[2].c_str(), 8, 12, 0, 90);

		Resources::Create<Sprite>(Pieces[55].Anim[0].c_str(), Pieces[55].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[55].Anim[1].c_str(), Pieces[55].Anim[1].c_str(), 8, 4, 0, 30);
		Resources::Create<Sprite>(Pieces[55].Anim[2].c_str(), Pieces[55].Anim[2].c_str(), 1, 1, 0, 1);

		Resources::Create<Sprite>(Pieces[56].Anim[0].c_str(), Pieces[56].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[56].Anim[1].c_str(), Pieces[56].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[56].Anim[2].c_str(), Pieces[56].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[57].Anim[0].c_str(), Pieces[57].Anim[0].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[57].Anim[1].c_str(), Pieces[57].Anim[1].c_str(), 8, 5, 0, 40);
		Resources::Create<Sprite>(Pieces[57].Anim[2].c_str(), Pieces[57].Anim[2].c_str(), 8, 5, 0, 40);

		Resources::Create<Sprite>(Pieces[58].Anim[0].c_str(), Pieces[58].Anim[0].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[58].Anim[1].c_str(), Pieces[58].Anim[1].c_str(), 8, 7, 0, 50);
		Resources::Create<Sprite>(Pieces[58].Anim[2].c_str(), Pieces[58].Anim[2].c_str(), 8, 7, 0, 50);

		Resources::Create<Sprite>(Pieces[59].Anim[0].c_str(), Pieces[59].Anim[0].c_str(), 8, 7, 0, 54);
		Resources::Create<Sprite>(Pieces[59].Anim[1].c_str(), Pieces[59].Anim[1].c_str(), 8, 7, 0, 54);
		Resources::Create<Sprite>(Pieces[59].Anim[2].c_str(), Pieces[59].Anim[2].c_str(), 8, 7, 0, 54);

		Resources::Create<Sprite>(Pieces[60].Anim[0].c_str(), Pieces[60].Anim[0].c_str(), 8, 4, 0, 30);
		Resources::Create<Sprite>(Pieces[60].Anim[1].c_str(), Pieces[60].Anim[1].c_str(), 8, 4, 0, 30);
		Resources::Create<Sprite>(Pieces[60].Anim[2].c_str(), Pieces[60].Anim[2].c_str(), 8, 4, 0, 30);

		Resources::Create<Sprite>(Pieces[61].Anim[0].c_str(), Pieces[61].Anim[0].c_str(), 8, 4, 0, 30);
		Resources::Create<Sprite>(Pieces[61].Anim[1].c_str(), Pieces[61].Anim[1].c_str(), 8, 4, 0, 30);
		Resources::Create<Sprite>(Pieces[61].Anim[2].c_str(), Pieces[61].Anim[2].c_str(), 8, 4, 0, 30);

		Resources::Create<Sprite>(Pieces[62].Anim[0].c_str(), Pieces[62].Anim[0].c_str(), 8, 7, 0, 56);
		Resources::Create<Sprite>(Pieces[62].Anim[1].c_str(), Pieces[62].Anim[1].c_str(), 8, 7, 0, 56);
		Resources::Create<Sprite>(Pieces[62].Anim[2].c_str(), Pieces[62].Anim[2].c_str(), 8, 7, 0, 56);
	}

	{
		Pieces[0 ].Health =	2800;	Pieces[0 ].Strength = 210;	Pieces[0 ].Range = 1; 
		Pieces[1 ].Health = 1500;	Pieces[1 ].Strength = 220;	Pieces[1 ].Range = 3; 
		Pieces[2 ].Health = 2800;	Pieces[2 ].Strength = 240;	Pieces[2 ].Range = 1; 
		Pieces[3 ].Health = 2600;	Pieces[3 ].Strength = 210;	Pieces[3 ].Range = 1; 
		Pieces[4 ].Health = 1800;	Pieces[4 ].Strength = 180;	Pieces[4 ].Range = 4; 
		Pieces[5 ].Health = 2200;	Pieces[5 ].Strength = 240;	Pieces[5 ].Range = 1; 
		Pieces[6 ].Health = 2800;	Pieces[6 ].Strength = 200;	Pieces[6 ].Range = 1; 
		Pieces[7 ].Health = 2200;	Pieces[7 ].Strength = 180;	Pieces[7 ].Range = 3; 
		Pieces[8 ].Health = 2000;	Pieces[8 ].Strength = 190;	Pieces[8 ].Range = 3; 
		Pieces[9 ].Health = 2000;	Pieces[9 ].Strength = 180;	Pieces[9 ].Range = 4; 
		Pieces[10].Health = 2200;	Pieces[10].Strength = 200;	Pieces[10].Range = 1; 
		Pieces[11].Health = 2600;	Pieces[11].Strength = 320;	Pieces[11].Range = 1; 
		Pieces[12].Health = 1800;	Pieces[12].Strength = 170;	Pieces[12].Range = 4; 
		Pieces[13].Health = 2600;	Pieces[13].Strength = 260;	Pieces[13].Range = 1; 
		Pieces[14].Health = 2400;	Pieces[14].Strength = 270;	Pieces[14].Range = 1; 
		Pieces[15].Health = 3200;	Pieces[15].Strength = 230;	Pieces[15].Range = 1; 
		Pieces[16].Health = 2200;	Pieces[16].Strength = 230;	Pieces[16].Range = 3; 
		Pieces[17].Health = 1800;	Pieces[17].Strength = 180;	Pieces[17].Range = 3; 
		Pieces[18].Health = 2200;	Pieces[18].Strength = 180;	Pieces[18].Range = 3; 
		Pieces[19].Health = 2600;	Pieces[19].Strength = 340;	Pieces[19].Range = 1; 
		Pieces[20].Health = 2800;	Pieces[20].Strength = 300;	Pieces[20].Range = 1; 
		Pieces[21].Health = 2600;	Pieces[21].Strength = 300;	Pieces[21].Range = 1; 
		Pieces[22].Health = 2000;	Pieces[22].Strength = 220;	Pieces[22].Range = 3; 
		Pieces[23].Health = 2000;	Pieces[23].Strength = 190;	Pieces[23].Range = 3; 
		Pieces[24].Health = 2200;	Pieces[24].Strength = 230;	Pieces[24].Range = 3; 
		Pieces[25].Health = 2800;	Pieces[25].Strength = 220;	Pieces[25].Range = 1; 
		Pieces[26].Health = 2400;	Pieces[26].Strength = 220;	Pieces[26].Range = 4; 
		Pieces[27].Health = 3000;	Pieces[27].Strength = 220;	Pieces[27].Range = 1; 
		Pieces[28].Health = 3000;	Pieces[28].Strength = 220;	Pieces[28].Range = 3; 
		Pieces[29].Health = 2200;	Pieces[29].Strength = 240;	Pieces[29].Range = 4; 
		Pieces[30].Health = 2200;	Pieces[30].Strength = 340;	Pieces[30].Range = 1; 
		Pieces[31].Health = 3000;	Pieces[31].Strength = 200;	Pieces[31].Range = 1; 
		Pieces[32].Health = 2400;	Pieces[32].Strength = 210;	Pieces[32].Range = 1; 
		Pieces[33].Health = 2000;	Pieces[33].Strength = 220;	Pieces[33].Range = 1; 
		Pieces[34].Health = 1800;	Pieces[34].Strength = 320;	Pieces[34].Range = 4; 
		Pieces[35].Health = 2200;	Pieces[35].Strength = 220;	Pieces[35].Range = 3; 
		Pieces[36].Health = 1800;	Pieces[36].Strength = 320;	Pieces[36].Range = 4; 
		Pieces[37].Health = 2400;	Pieces[37].Strength = 210;	Pieces[37].Range = 4; 
		Pieces[38].Health = 2800;	Pieces[38].Strength = 240;	Pieces[38].Range = 1; 
		Pieces[39].Health = 3800;	Pieces[39].Strength = 460;	Pieces[39].Range = 5; 
		Pieces[40].Health = 3800;	Pieces[40].Strength = 330;	Pieces[40].Range = 1; 
		Pieces[41].Health = 3600;	Pieces[41].Strength = 400;	Pieces[41].Range = 3; 
		Pieces[42].Health = 2600;	Pieces[42].Strength = 170;	Pieces[42].Range = 4; 
		Pieces[43].Health = 3400;	Pieces[43].Strength = 220;	Pieces[43].Range = 3; 
		Pieces[44].Health = 3000;	Pieces[44].Strength = 400;	Pieces[44].Range = 1; 
		Pieces[45].Health = 4000;	Pieces[45].Strength = 260;	Pieces[45].Range = 1; 
		Pieces[46].Health = 3200;	Pieces[46].Strength = 190;	Pieces[46].Range = 4; 
		Pieces[47].Health = 3000;	Pieces[47].Strength = 240;	Pieces[47].Range = 4; 
		Pieces[48].Health = 3200;	Pieces[48].Strength = 220;	Pieces[48].Range = 1; 
		Pieces[49].Health = 3000;	Pieces[49].Strength = 220;	Pieces[49].Range = 3; 
		Pieces[50].Health = 3600;	Pieces[50].Strength = 310;	Pieces[50].Range = 3; 
		Pieces[51].Health = 3400;	Pieces[51].Strength = 170;	Pieces[51].Range = 3; 
		Pieces[52].Health = 3800;	Pieces[52].Strength = 200;	Pieces[52].Range = 1; 
		Pieces[53].Health = 4000;	Pieces[53].Strength = 200;	Pieces[53].Range = 3; 
		Pieces[54].Health = 4000;	Pieces[54].Strength = 200;	Pieces[54].Range = 4; 
		Pieces[55].Health = 2800;	Pieces[55].Strength = 200;	Pieces[55].Range = 3; 
		Pieces[56].Health = 2600;	Pieces[56].Strength =   0;	Pieces[56].Range = 1; 
		Pieces[57].Health = 2400;	Pieces[57].Strength = 240;	Pieces[57].Range = 4; 
		Pieces[58].Health = 4000;	Pieces[58].Strength = 220;	Pieces[58].Range = 3; 
		Pieces[59].Health = 3800;	Pieces[59].Strength = 240;	Pieces[59].Range = 3; 
		Pieces[60].Health = 2200;	Pieces[60].Strength = 250;	Pieces[60].Range = 3; 
		Pieces[61].Health = 2200;	Pieces[61].Strength = 190;	Pieces[61].Range = 4; 
		Pieces[62].Health = 4800;	Pieces[62].Strength = 500;	Pieces[62].Range = 1; 
	}

	{
		StandFrameUpper = GetScene()->CreateActor();
		StandFrameUpper->GetTransform()->SetScale({ 3.5f*2.1f, 3.5f*2.1f });
		StandFrameUpper->GetTransform()->SetPosition({ -3.0f , 0.5f, -0.9f });
		HPTR<SpriteRenderer> SR = StandFrameUpper->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		SR->SetSprite(L"Level.png", 0);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"Level.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
	}

	{
		StandSprite = GetScene()->CreateActor();
		StandSprite->GetTransform()->SetScale({ 3.5f * 0.8f, 3.5f});
		StandSprite->GetTransform()->SetPosition({ -3.0f, 0.8f, -1.0f });
		HPTR<SpriteRenderer> SR = StandSprite->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		SR->SetSprite(Pieces[0].Anim[0].c_str(), 0);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", Pieces[0].Anim[0].c_str());
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
		SR->LinkSampler(L"PS_PSMP", L"PSMP");
		{
			SpriteAnimaion* SANI = StandSprite->AddComponent<SpriteAnimaion>();
			SANI->CreateAni(Pieces[0].Anim[0].c_str(), Pieces[0].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[0].Anim[1].c_str(), Pieces[0].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[0].Anim[2].c_str(), Pieces[0].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[1].Anim[0].c_str(), Pieces[1].Anim[0].c_str(), 0, 60, 0.03f);
			SANI->CreateAni(Pieces[1].Anim[1].c_str(), Pieces[1].Anim[1].c_str(), 0, 60, 0.03f);
			SANI->CreateAni(Pieces[1].Anim[2].c_str(), Pieces[1].Anim[2].c_str(), 0, 60, 0.03f);
			SANI->CreateAni(Pieces[2].Anim[0].c_str(), Pieces[2].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[2].Anim[1].c_str(), Pieces[2].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[2].Anim[2].c_str(), Pieces[2].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[3].Anim[0].c_str(), Pieces[3].Anim[0].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[3].Anim[1].c_str(), Pieces[3].Anim[1].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[3].Anim[2].c_str(), Pieces[3].Anim[2].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[4].Anim[0].c_str(), Pieces[4].Anim[0].c_str(), 0, 64, 0.03f);
			SANI->CreateAni(Pieces[4].Anim[1].c_str(), Pieces[4].Anim[1].c_str(), 0, 64, 0.03f);
			SANI->CreateAni(Pieces[4].Anim[2].c_str(), Pieces[4].Anim[2].c_str(), 0, 64, 0.03f);
			SANI->CreateAni(Pieces[5].Anim[0].c_str(), Pieces[5].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[5].Anim[1].c_str(), Pieces[5].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[5].Anim[2].c_str(), Pieces[5].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[6].Anim[0].c_str(), Pieces[6].Anim[0].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[6].Anim[1].c_str(), Pieces[6].Anim[1].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[6].Anim[2].c_str(), Pieces[6].Anim[2].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[7].Anim[0].c_str(), Pieces[7].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[7].Anim[1].c_str(), Pieces[7].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[7].Anim[2].c_str(), Pieces[7].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[8].Anim[0].c_str(), Pieces[8].Anim[0].c_str(), 0, 28, 0.03f);
			SANI->CreateAni(Pieces[8].Anim[1].c_str(), Pieces[8].Anim[1].c_str(), 0, 28, 0.03f);
			SANI->CreateAni(Pieces[8].Anim[2].c_str(), Pieces[8].Anim[2].c_str(), 0, 28, 0.03f);
			SANI->CreateAni(Pieces[9].Anim[0].c_str(), Pieces[9].Anim[0].c_str(), 0, 32, 0.03f);
			SANI->CreateAni(Pieces[9].Anim[1].c_str(), Pieces[9].Anim[1].c_str(), 0, 32, 0.03f);
			SANI->CreateAni(Pieces[9].Anim[2].c_str(), Pieces[9].Anim[2].c_str(), 0, 32, 0.03f);
			SANI->CreateAni(Pieces[10].Anim[0].c_str(), Pieces[10].Anim[0].c_str(), 0, 48, 0.03f);
			SANI->CreateAni(Pieces[10].Anim[1].c_str(), Pieces[10].Anim[1].c_str(), 0, 48, 0.03f);
			SANI->CreateAni(Pieces[10].Anim[2].c_str(), Pieces[10].Anim[2].c_str(), 0, 48, 0.03f);
			SANI->CreateAni(Pieces[11].Anim[0].c_str(), Pieces[11].Anim[0].c_str(), 0, 48, 0.03f);
			SANI->CreateAni(Pieces[11].Anim[1].c_str(), Pieces[11].Anim[1].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[11].Anim[2].c_str(), Pieces[11].Anim[2].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[12].Anim[0].c_str(), Pieces[12].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[12].Anim[1].c_str(), Pieces[12].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[12].Anim[2].c_str(), Pieces[12].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[13].Anim[0].c_str(), Pieces[13].Anim[0].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[13].Anim[1].c_str(), Pieces[13].Anim[1].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[13].Anim[2].c_str(), Pieces[13].Anim[2].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[14].Anim[0].c_str(), Pieces[14].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[14].Anim[1].c_str(), Pieces[14].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[14].Anim[2].c_str(), Pieces[14].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[15].Anim[0].c_str(), Pieces[15].Anim[0].c_str(), 0, 60, 0.03f);
			SANI->CreateAni(Pieces[15].Anim[1].c_str(), Pieces[15].Anim[1].c_str(), 0, 60, 0.03f);
			SANI->CreateAni(Pieces[15].Anim[2].c_str(), Pieces[15].Anim[2].c_str(), 0, 60, 0.03f);
			SANI->CreateAni(Pieces[16].Anim[0].c_str(), Pieces[16].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[16].Anim[1].c_str(), Pieces[16].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[16].Anim[2].c_str(), Pieces[16].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[17].Anim[0].c_str(), Pieces[17].Anim[0].c_str(), 0, 20, 0.03f);
			SANI->CreateAni(Pieces[17].Anim[1].c_str(), Pieces[17].Anim[1].c_str(), 0, 20, 0.03f);
			SANI->CreateAni(Pieces[17].Anim[2].c_str(), Pieces[17].Anim[2].c_str(), 0, 20, 0.03f);
			SANI->CreateAni(Pieces[18].Anim[0].c_str(), Pieces[18].Anim[0].c_str(), 0, 45, 0.03f);
			SANI->CreateAni(Pieces[18].Anim[1].c_str(), Pieces[18].Anim[1].c_str(), 0, 45, 0.03f);
			SANI->CreateAni(Pieces[18].Anim[2].c_str(), Pieces[18].Anim[2].c_str(), 0, 45, 0.03f);
			SANI->CreateAni(Pieces[19].Anim[0].c_str(), Pieces[19].Anim[0].c_str(), 0, 60, 0.03f);
			SANI->CreateAni(Pieces[19].Anim[1].c_str(), Pieces[19].Anim[1].c_str(), 0, 60, 0.03f);
			SANI->CreateAni(Pieces[19].Anim[2].c_str(), Pieces[19].Anim[2].c_str(), 0, 60, 0.03f);
			SANI->CreateAni(Pieces[20].Anim[0].c_str(), Pieces[20].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[20].Anim[1].c_str(), Pieces[20].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[20].Anim[2].c_str(), Pieces[20].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[21].Anim[0].c_str(), Pieces[21].Anim[0].c_str(), 0, 80, 0.03f);
			SANI->CreateAni(Pieces[21].Anim[1].c_str(), Pieces[21].Anim[1].c_str(), 0, 80, 0.03f);
			SANI->CreateAni(Pieces[21].Anim[2].c_str(), Pieces[21].Anim[2].c_str(), 0, 80, 0.03f);
			SANI->CreateAni(Pieces[22].Anim[0].c_str(), Pieces[22].Anim[0].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[22].Anim[1].c_str(), Pieces[22].Anim[1].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[22].Anim[2].c_str(), Pieces[22].Anim[2].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[23].Anim[0].c_str(), Pieces[23].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[23].Anim[1].c_str(), Pieces[23].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[23].Anim[2].c_str(), Pieces[23].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[24].Anim[0].c_str(), Pieces[24].Anim[0].c_str(), 0, 48, 0.03f);
			SANI->CreateAni(Pieces[24].Anim[1].c_str(), Pieces[24].Anim[1].c_str(), 0, 48, 0.03f);
			SANI->CreateAni(Pieces[24].Anim[2].c_str(), Pieces[24].Anim[2].c_str(), 0, 48, 0.03f);
			SANI->CreateAni(Pieces[25].Anim[0].c_str(), Pieces[25].Anim[0].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[25].Anim[1].c_str(), Pieces[25].Anim[1].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[25].Anim[2].c_str(), Pieces[25].Anim[2].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[26].Anim[0].c_str(), Pieces[26].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[26].Anim[1].c_str(), Pieces[26].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[26].Anim[2].c_str(), Pieces[26].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[27].Anim[0].c_str(), Pieces[27].Anim[0].c_str(), 0, 45, 0.03f);
			SANI->CreateAni(Pieces[27].Anim[1].c_str(), Pieces[27].Anim[1].c_str(), 0, 45, 0.03f);
			SANI->CreateAni(Pieces[27].Anim[2].c_str(), Pieces[27].Anim[2].c_str(), 0, 45, 0.03f);
			SANI->CreateAni(Pieces[28].Anim[0].c_str(), Pieces[28].Anim[0].c_str(), 0, 52, 0.03f);
			SANI->CreateAni(Pieces[28].Anim[1].c_str(), Pieces[28].Anim[1].c_str(), 0, 52, 0.03f);
			SANI->CreateAni(Pieces[28].Anim[2].c_str(), Pieces[28].Anim[2].c_str(), 0, 52, 0.03f);
			SANI->CreateAni(Pieces[29].Anim[0].c_str(), Pieces[29].Anim[0].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[29].Anim[1].c_str(), Pieces[29].Anim[1].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[29].Anim[2].c_str(), Pieces[29].Anim[2].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[30].Anim[0].c_str(), Pieces[30].Anim[0].c_str(), 0, 64, 0.03f);
			SANI->CreateAni(Pieces[30].Anim[1].c_str(), Pieces[30].Anim[1].c_str(), 0, 64, 0.03f);
			SANI->CreateAni(Pieces[30].Anim[2].c_str(), Pieces[30].Anim[2].c_str(), 0, 64, 0.03f);
			SANI->CreateAni(Pieces[31].Anim[0].c_str(), Pieces[31].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[31].Anim[1].c_str(), Pieces[31].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[31].Anim[2].c_str(), Pieces[31].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[32].Anim[0].c_str(), Pieces[32].Anim[0].c_str(), 0, 30, 0.03f);
			SANI->CreateAni(Pieces[32].Anim[1].c_str(), Pieces[32].Anim[1].c_str(), 0, 30, 0.03f);
			SANI->CreateAni(Pieces[32].Anim[2].c_str(), Pieces[32].Anim[2].c_str(), 0, 30, 0.03f);
			SANI->CreateAni(Pieces[33].Anim[0].c_str(), Pieces[33].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[33].Anim[1].c_str(), Pieces[33].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[33].Anim[2].c_str(), Pieces[33].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[34].Anim[0].c_str(), Pieces[34].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[34].Anim[1].c_str(), Pieces[34].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[34].Anim[2].c_str(), Pieces[34].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[35].Anim[0].c_str(), Pieces[35].Anim[0].c_str(), 0, 26, 0.03f);
			SANI->CreateAni(Pieces[35].Anim[1].c_str(), Pieces[35].Anim[1].c_str(), 0, 26, 0.03f);
			SANI->CreateAni(Pieces[35].Anim[2].c_str(), Pieces[35].Anim[2].c_str(), 0, 26, 0.03f);
			SANI->CreateAni(Pieces[36].Anim[0].c_str(), Pieces[36].Anim[0].c_str(), 0, 62, 0.03f);
			SANI->CreateAni(Pieces[36].Anim[1].c_str(), Pieces[36].Anim[1].c_str(), 0, 62, 0.03f);
			SANI->CreateAni(Pieces[36].Anim[2].c_str(), Pieces[36].Anim[2].c_str(), 0, 62, 0.03f);
			SANI->CreateAni(Pieces[37].Anim[0].c_str(), Pieces[37].Anim[0].c_str(), 0, 60, 0.03f);
			SANI->CreateAni(Pieces[37].Anim[1].c_str(), Pieces[37].Anim[1].c_str(), 0, 60, 0.03f);
			SANI->CreateAni(Pieces[37].Anim[2].c_str(), Pieces[37].Anim[2].c_str(), 0, 60, 0.03f);
			SANI->CreateAni(Pieces[38].Anim[0].c_str(), Pieces[38].Anim[0].c_str(), 0, 42, 0.03f);
			SANI->CreateAni(Pieces[38].Anim[1].c_str(), Pieces[38].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[38].Anim[2].c_str(), Pieces[38].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[39].Anim[0].c_str(), Pieces[39].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[39].Anim[1].c_str(), Pieces[39].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[39].Anim[2].c_str(), Pieces[39].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[40].Anim[0].c_str(), Pieces[40].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[40].Anim[1].c_str(), Pieces[40].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[40].Anim[2].c_str(), Pieces[40].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[41].Anim[0].c_str(), Pieces[41].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[41].Anim[1].c_str(), Pieces[41].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[41].Anim[2].c_str(), Pieces[41].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[42].Anim[0].c_str(), Pieces[42].Anim[0].c_str(), 0, 45, 0.03f);
			SANI->CreateAni(Pieces[42].Anim[1].c_str(), Pieces[42].Anim[1].c_str(), 0, 45, 0.03f);
			SANI->CreateAni(Pieces[42].Anim[2].c_str(), Pieces[42].Anim[2].c_str(), 0, 45, 0.03f);
			SANI->CreateAni(Pieces[43].Anim[0].c_str(), Pieces[43].Anim[0].c_str(), 0, 41, 0.03f);
			SANI->CreateAni(Pieces[43].Anim[1].c_str(), Pieces[43].Anim[1].c_str(), 0, 41, 0.03f);
			SANI->CreateAni(Pieces[43].Anim[2].c_str(), Pieces[43].Anim[2].c_str(), 0, 41, 0.03f);
			SANI->CreateAni(Pieces[44].Anim[0].c_str(), Pieces[44].Anim[0].c_str(), 0, 56, 0.03f);
			SANI->CreateAni(Pieces[44].Anim[1].c_str(), Pieces[44].Anim[1].c_str(), 0, 56, 0.03f);
			SANI->CreateAni(Pieces[44].Anim[2].c_str(), Pieces[44].Anim[2].c_str(), 0, 56, 0.03f);
			SANI->CreateAni(Pieces[45].Anim[0].c_str(), Pieces[45].Anim[0].c_str(), 0, 60, 0.03f);
			SANI->CreateAni(Pieces[45].Anim[1].c_str(), Pieces[45].Anim[1].c_str(), 0, 60, 0.03f);
			SANI->CreateAni(Pieces[45].Anim[2].c_str(), Pieces[45].Anim[2].c_str(), 0, 60, 0.03f);
			SANI->CreateAni(Pieces[46].Anim[0].c_str(), Pieces[46].Anim[0].c_str(), 0, 44, 0.03f);
			SANI->CreateAni(Pieces[46].Anim[1].c_str(), Pieces[46].Anim[1].c_str(), 0, 44, 0.03f);
			SANI->CreateAni(Pieces[46].Anim[2].c_str(), Pieces[46].Anim[2].c_str(), 0, 44, 0.03f);
			SANI->CreateAni(Pieces[47].Anim[0].c_str(), Pieces[47].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[47].Anim[1].c_str(), Pieces[47].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[47].Anim[2].c_str(), Pieces[47].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[48].Anim[0].c_str(), Pieces[48].Anim[0].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[48].Anim[1].c_str(), Pieces[48].Anim[1].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[48].Anim[2].c_str(), Pieces[48].Anim[2].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[49].Anim[0].c_str(), Pieces[49].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[49].Anim[1].c_str(), Pieces[49].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[49].Anim[2].c_str(), Pieces[49].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[50].Anim[0].c_str(), Pieces[50].Anim[0].c_str(), 0, 33, 0.03f);
			SANI->CreateAni(Pieces[50].Anim[1].c_str(), Pieces[50].Anim[1].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[50].Anim[2].c_str(), Pieces[50].Anim[2].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[51].Anim[0].c_str(), Pieces[51].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[51].Anim[1].c_str(), Pieces[51].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[51].Anim[2].c_str(), Pieces[51].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[52].Anim[0].c_str(), Pieces[52].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[52].Anim[1].c_str(), Pieces[52].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[52].Anim[2].c_str(), Pieces[52].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[53].Anim[0].c_str(), Pieces[53].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[53].Anim[1].c_str(), Pieces[53].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[53].Anim[2].c_str(), Pieces[53].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[54].Anim[0].c_str(), Pieces[54].Anim[0].c_str(), 0, 70, 0.03f);
			SANI->CreateAni(Pieces[54].Anim[1].c_str(), Pieces[54].Anim[1].c_str(), 0, 60, 0.03f);
			SANI->CreateAni(Pieces[54].Anim[2].c_str(), Pieces[54].Anim[2].c_str(), 0, 90, 0.03f);
			SANI->CreateAni(Pieces[55].Anim[0].c_str(), Pieces[55].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[55].Anim[1].c_str(), Pieces[55].Anim[1].c_str(), 0, 30, 0.03f);
			SANI->CreateAni(Pieces[55].Anim[2].c_str(), Pieces[55].Anim[2].c_str(), 0, 1, 0.03f);
			SANI->CreateAni(Pieces[56].Anim[0].c_str(), Pieces[56].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[56].Anim[1].c_str(), Pieces[56].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[56].Anim[2].c_str(), Pieces[56].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[57].Anim[0].c_str(), Pieces[57].Anim[0].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[57].Anim[1].c_str(), Pieces[57].Anim[1].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[57].Anim[2].c_str(), Pieces[57].Anim[2].c_str(), 0, 40, 0.03f);
			SANI->CreateAni(Pieces[58].Anim[0].c_str(), Pieces[58].Anim[0].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[58].Anim[1].c_str(), Pieces[58].Anim[1].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[58].Anim[2].c_str(), Pieces[58].Anim[2].c_str(), 0, 50, 0.03f);
			SANI->CreateAni(Pieces[59].Anim[0].c_str(), Pieces[59].Anim[0].c_str(), 0, 54, 0.03f);
			SANI->CreateAni(Pieces[59].Anim[1].c_str(), Pieces[59].Anim[1].c_str(), 0, 54, 0.03f);
			SANI->CreateAni(Pieces[59].Anim[2].c_str(), Pieces[59].Anim[2].c_str(), 0, 54, 0.03f);
			SANI->CreateAni(Pieces[60].Anim[0].c_str(), Pieces[60].Anim[0].c_str(), 0, 30, 0.03f);
			SANI->CreateAni(Pieces[60].Anim[1].c_str(), Pieces[60].Anim[1].c_str(), 0, 30, 0.03f);
			SANI->CreateAni(Pieces[60].Anim[2].c_str(), Pieces[60].Anim[2].c_str(), 0, 30, 0.03f);
			SANI->CreateAni(Pieces[61].Anim[0].c_str(), Pieces[61].Anim[0].c_str(), 0, 30, 0.03f);
			SANI->CreateAni(Pieces[61].Anim[1].c_str(), Pieces[61].Anim[1].c_str(), 0, 30, 0.03f);
			SANI->CreateAni(Pieces[61].Anim[2].c_str(), Pieces[61].Anim[2].c_str(), 0, 30, 0.03f);
			SANI->CreateAni(Pieces[62].Anim[0].c_str(), Pieces[62].Anim[0].c_str(), 0, 56, 0.03f);
			SANI->CreateAni(Pieces[62].Anim[1].c_str(), Pieces[62].Anim[1].c_str(), 0, 56, 0.03f);
			SANI->CreateAni(Pieces[62].Anim[2].c_str(), Pieces[62].Anim[2].c_str(), 0, 56, 0.03f);

			SANI->CreateAni(L"Stand26_1a.png", L"Stand26_1a.png", 0, 36, 0.03f);
			SANI->CreateAni(L"Stand26_2a.png", L"Stand26_2a.png", 0, 36, 0.03f);
			SANI->CreateAni(L"Stand26_3a.png", L"Stand26_3a.png", 0, 36, 0.03f);

			SANI->CreateAni(L"Stand39_1a.png", L"Stand39_1a.png", 0, 40, 0.03f);
			SANI->CreateAni(L"Stand39_2a.png", L"Stand39_2a.png", 0, 40, 0.03f);
			SANI->CreateAni(L"Stand39_3a.png", L"Stand39_3a.png", 0, 40, 0.03f);

			SANI->CreateAni(L"Stand49_1a.png", L"Stand49_1a.png", 0, 26, 0.03f);
			SANI->CreateAni(L"Stand49_2a.png", L"Stand49_2a.png", 0, 26, 0.03f);
			SANI->CreateAni(L"Stand49_3a.png", L"Stand49_3a.png", 0, 26, 0.03f);
		}
	}

	{
		StandFrameLower = GetScene()->CreateActor();
		StandFrameLower->GetTransform()->SetScale({ 3.5f*2.1f, 3.5f*2.1f });
		StandFrameLower->GetTransform()->SetPosition({ -3.0f , 0.5f, -1.1f });
		HPTR<SpriteRenderer> SR = StandFrameLower->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		SR->SetSprite(L"Level.png", 0);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"Level.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
	}

	{
		LButton = GetScene()->CreateActor();
		LButton->GetTransform()->SetScale({ 0.4f, 0.7f });
		LButton->GetTransform()->SetPosition({ -6.0f , 0.5f, -1.1f });
		HPTR<SpriteRenderer> SR = LButton->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		SR->SetSprite(L"Pointer.png", 0);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"Pointer.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
	}

	{
		RButton = GetScene()->CreateActor();
		RButton->GetTransform()->SetScale({ 0.4f, 0.7f });
		RButton->GetTransform()->SetPosition({ 0.0f , 0.5f, -1.1f });
		HPTR<SpriteRenderer> SR = RButton->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		SR->SetSprite(L"Pointer.png", 1);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"Pointer.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
	}
	
	{
		SCBar = GetScene()->CreateActor();
		SCBar->GetTransform()->SetScale({ 0.15f, 1.5f });
		SCBar->GetTransform()->SetPosition({ 7.5f , 3.0f, -1.0f });
		HPTR<SpriteRenderer> SR = SCBar->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		SR->SetSprite(L"ScrollBar.png", 0);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"ScrollBar.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
	}

	for (int i = 0; i < 63; ++i)
	{
		StandEx[i] = GetScene()->CreateActor();
		HPTR<SpriteRenderer> SR = StandEx[i]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
		SR->SetSprite(L"Stand.png", i);
		SR->LinkTexture(L"PS_MAPTEX", L"Stand.png");

		{
			StandNamePlateClass[i] = GetScene()->CreateActor();
			StandNamePlateClass[i]->GetTransform()->SetScale({ 0.18f, 0.18f });
			StandNamePlateClass[i]->GetTransform()->SetPosition({ 0.39f, -0.6f, -1.1f });
			HPTR<SpriteRenderer> SR = StandNamePlateClass[i]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

			SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
			SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
			SR->LinkSampler(L"PS_LSMP", L"LSMP");
			SR->SetSprite(L"Class.png", Pieces[i].CLASS);
			SR->LinkTexture(L"PS_MAPTEX", L"Class.png");

			StandNamePlateClass[i]->SetParent(StandEx[i]);

			{
				StandNameClass[i] = GetScene()->CreateActor();
				StandNameClass[i]->GetTransform()->SetScale({ 0.6f, 0.09f });
				StandNameClass[i]->GetTransform()->SetPosition({ 0.0f, -0.65f, -1.2f });
				HPTR<SpriteRenderer> SR = StandNameClass[i]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

				SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
				SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
				SR->LinkSampler(L"PS_LSMP", L"LSMP");
				SR->SetSprite(L"LetterData.png", Pieces[i].CLASS * 3 + 1);
				SR->LinkTexture(L"PS_MAPTEX", L"LetterData.png");

				StandNameClass[i]->SetParent(StandEx[i]);
			}
		}
		{
			StandNamePlateRace[i] = GetScene()->CreateActor();
			StandNamePlateRace[i]->GetTransform()->SetScale({ 0.18f, 0.18f });
			StandNamePlateRace[i]->GetTransform()->SetPosition({ -0.385f, -0.6f, -1.1f });
			HPTR<SpriteRenderer> SR = StandNamePlateRace[i]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

			SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
			SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
			SR->LinkSampler(L"PS_LSMP", L"LSMP");
			SR->SetSprite(L"Race.png", Pieces[i].RACE1);
			SR->LinkTexture(L"PS_MAPTEX", L"Race.png");

			StandNamePlateRace[i]->SetParent(StandEx[i]);

			{
				StandNameRace[i] = GetScene()->CreateActor();
				StandNameRace[i]->GetTransform()->SetScale({ 0.6f, 0.09f });
				HPTR<SpriteRenderer> SR = StandNameRace[i]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

				SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
				SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
				SR->LinkSampler(L"PS_LSMP", L"LSMP");
				SR->SetSprite(L"LetterData.png", Pieces[i].RACE1 * 3 + 2);
				SR->LinkTexture(L"PS_MAPTEX", L"LetterData.png");

				StandNameRace[i]->SetParent(StandEx[i]);
			}

			if (Pieces[i].RACE2 != 0)
			{
				StandNamePlateRace2[i] = GetScene()->CreateActor();
				StandNamePlateRace2[i]->GetTransform()->SetScale({ 0.18f, 0.18f });
				StandNamePlateRace2[i]->GetTransform()->SetPosition({ -0.285f, -0.6f, -1.05f });
				HPTR<SpriteRenderer> SR = StandNamePlateRace2[i]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

				SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
				SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
				SR->LinkSampler(L"PS_LSMP", L"LSMP");
				SR->SetSprite(L"Race.png", Pieces[i].RACE2);
				SR->LinkTexture(L"PS_MAPTEX", L"Race.png");

				StandNamePlateRace2[i]->SetParent(StandEx[i]);

				{
					StandNameRace2[i] = GetScene()->CreateActor();
					StandNameRace2[i]->GetTransform()->SetScale({ 0.6f, 0.09f });
					StandNameRace[i]->GetTransform()->SetPosition({ -0.09f, -0.55f, -1.2f });
					StandNameRace2[i]->GetTransform()->SetPosition({ 0.15f, -0.55f, -1.2f });
					HPTR<SpriteRenderer> SR = StandNameRace2[i]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

					SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
					SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
					SR->LinkSampler(L"PS_LSMP", L"LSMP");
					SR->SetSprite(L"LetterData.png", Pieces[i].RACE2 * 3 + 2);
					SR->LinkTexture(L"PS_MAPTEX", L"LetterData.png");

					StandNameRace2[i]->SetParent(StandEx[i]);
				}
			}
			else
			{
				StandNameRace[i]->GetTransform()->SetPosition({ 0.0f, -0.55f, -1.2f });
			}
		}
		{
			StandNamePlate[i] = GetScene()->CreateActor();
			StandNamePlate[i]->GetTransform()->SetScale({ 1.0f, 0.2f });
			StandNamePlate[i]->GetTransform()->SetPosition({ 0.0f, -0.6f, -1.0f });
			HPTR<SpriteRenderer> SR = StandNamePlate[i]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

			SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
			SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
			SR->LinkSampler(L"PS_LSMP", L"LSMP");

			if (Pieces[i].RACE2 != 0)
				SR->SetSprite(L"NamePlate.png", 0);
			else
				SR->SetSprite(L"NamePlate.png", 1);

			SR->LinkTexture(L"PS_MAPTEX", L"NamePlate.png");

			StandNamePlate[i]->SetParent(StandEx[i]);

			{
				StandName[i] = GetScene()->CreateActor();
				StandName[i]->GetTransform()->SetScale({ 0.6f, 0.09f });
				StandName[i]->GetTransform()->SetPosition({ 0.0f, -0.75f, -1.2f });
				HPTR<SpriteRenderer> SR = StandName[i]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

				SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
				SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
				SR->LinkSampler(L"PS_LSMP", L"LSMP");
				SR->SetSprite(L"LetterData.png", i * 3);
				SR->LinkTexture(L"PS_MAPTEX", L"LetterData.png");

				StandName[i]->SetParent(StandEx[i]);
			}
		}
	}

	for (int i = 0; i < 13; ++i)
	{
		ClassEx[i] = GetScene()->CreateActor();
		ClassEx[i]->GetTransform()->SetScale({ 0.4f, 0.4f });
		HPTR<SpriteRenderer> SR = ClassEx[i]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		SR->SetSprite(L"Class.png", i);
		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"Class.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");

		if (0 == i)
			ClassEx[i]->GetTransform()->SetPosition({ -7.0f , 2.5f, -1.0f });
		else
			ClassEx[i]->GetTransform()->SetPosition({ -7.0f + ((i - 1) % 7) * 0.5f , 2.0f - ((i - 1) / 7) * 0.5f, -1.0f });
	}

	for (int i = 0; i < 15; ++i)
	{
		RaceEx[i] = GetScene()->CreateActor();
		RaceEx[i]->GetTransform()->SetScale({ 0.4f, 0.4f });
		HPTR<SpriteRenderer> SR = RaceEx[i]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		SR->SetSprite(L"Race.png", i);
		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"Race.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");

		if (0 == i)
			RaceEx[i]->GetTransform()->SetPosition({ -7.0f , 0.0f, -1.0f });
		else
			RaceEx[i]->GetTransform()->SetPosition({ -7.0f + ((i - 1) % 7) * 0.5f , -0.5f - ((i - 1) / 7) * 0.5f, -1.0f });
	}

	for (int i = 0; i < 6; ++i)
	{
		StarEx[i] = GetScene()->CreateActor();
		StarEx[i]->GetTransform()->SetScale({ 0.4f, 0.4f });
		HPTR<SpriteRenderer> SR = StarEx[i]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		SR->SetSprite(L"Stars.png", i);
		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"Stars.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");

		if (0 == i)
			StarEx[i]->GetTransform()->SetPosition({ -7.0f , -2.5f, -1.0f });
		else
			StarEx[i]->GetTransform()->SetPosition({ -7.0f + (i - 1) * 0.5f , -3.0f, -1.0f });
	}


	{
		{
			MenuLetter[0] = GetScene()->CreateActor();
			MenuLetter[0]->GetTransform()->SetScale({ 1.8f, 0.3f });
			MenuLetter[0]->GetTransform()->SetPosition({ -6.25f , 2.5f, -1.0f });
			HPTR<SpriteRenderer> SR = MenuLetter[0]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
			SR->SetSprite(L"LetterData.png", 182);
			SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
			SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
			SR->LinkTexture(L"PS_MAPTEX", L"LetterData.png");
			SR->LinkSampler(L"PS_LSMP", L"LSMP");
		}

		{
			MenuLetter[1] = GetScene()->CreateActor();
			MenuLetter[1]->GetTransform()->SetScale({ 1.8f, 0.3f });
			MenuLetter[1]->GetTransform()->SetPosition({ -6.25f , 0.0f, -1.0f });
			HPTR<SpriteRenderer> SR = MenuLetter[1]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
			SR->SetSprite(L"LetterData.png", 185);
			SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
			SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
			SR->LinkTexture(L"PS_MAPTEX", L"LetterData.png");
			SR->LinkSampler(L"PS_LSMP", L"LSMP");
		}

		{
			MenuLetter[2] = GetScene()->CreateActor();
			MenuLetter[2]->GetTransform()->SetScale({ 1.8f, 0.3f });
			MenuLetter[2]->GetTransform()->SetPosition({ -6.25f , -2.52f, -1.0f });
			HPTR<SpriteRenderer> SR = MenuLetter[2]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
			SR->SetSprite(L"LetterData.png", 188);
			SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
			SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
			SR->LinkTexture(L"PS_MAPTEX", L"LetterData.png");
			SR->LinkSampler(L"PS_LSMP", L"LSMP");
		}
	}

	{
		Face = GetScene()->CreateActor();
		Face->GetTransform()->SetScale({ 1.0f, 1.0f });
		Face->GetTransform()->SetPosition({ 1.6f, 2.2f, -1.2f });
		HPTR<SpriteRenderer> SR = Face->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
		SR->SetSprite(L"Face.png", 0);
		SR->LinkTexture(L"PS_MAPTEX", L"Face.png");
	}
	{
		FaceComp[0] = GetScene()->CreateActor();
		FaceComp[0]->GetTransform()->SetScale({ 0.5f, 0.5f });
		FaceComp[0]->GetTransform()->SetPosition({ 2.6f, 0.5f, -1.2f });
		HPTR<SpriteRenderer> SR = FaceComp[0]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
		SR->SetSprite(L"Class.png", 0);
		SR->LinkTexture(L"PS_MAPTEX", L"Class.png");
	}

	{
		FaceComp[1] = GetScene()->CreateActor();
		FaceComp[1]->GetTransform()->SetScale({ 0.5f, 0.5f });
		FaceComp[1]->GetTransform()->SetPosition({ 1.3f, 0.5f, -1.2f });
		HPTR<SpriteRenderer> SR = FaceComp[1]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
		SR->SetSprite(L"Race.png", 0);
		SR->LinkTexture(L"PS_MAPTEX", L"Race.png");
	}

	{
		FaceComp[2] = GetScene()->CreateActor();
		FaceComp[2]->GetTransform()->SetScale({ 0.5f, 0.5f });
		FaceComp[2]->GetTransform()->SetPosition({ 1.7f, 0.5f, -1.1f });
		HPTR<SpriteRenderer> SR = FaceComp[2]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
		SR->SetSprite(L"Race.png", 0);
		SR->LinkTexture(L"PS_MAPTEX", L"Race.png");
	}
	{
		FaceComp[3] = GetScene()->CreateActor();
		FaceComp[3]->GetTransform()->SetScale({ 0.425f, 0.425f });
		FaceComp[3]->GetTransform()->SetPosition({ 2.5f, 1.325f, -1.1f });
		HPTR<SpriteRenderer> SR = FaceComp[3]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
		SR->SetSprite(L"Stars.png", 7);
		SR->LinkTexture(L"PS_MAPTEX", L"Stars.png");
	}
	{
		FaceComp[4] = GetScene()->CreateActor();
		FaceComp[4]->GetTransform()->SetScale({ 1.0f, 1.0f });
		FaceComp[4]->GetTransform()->SetPosition({ 3.65f, 1.325f, -1.1f });
		HPTR<SpriteRenderer> SR = FaceComp[4]->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
		SR->SetSprite(L"Stars.png", 8);
		SR->LinkTexture(L"PS_MAPTEX", L"Stars.png");
	}

	{
		ESCButton = GetScene()->CreateActor();
		ESCButton->GetTransform()->SetScale({ 0.7f, 0.7f });
		ESCButton->GetTransform()->SetPosition({ -7.0f, 3.4f, -1.1f });
		HPTR<SpriteRenderer> SR = ESCButton->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
		SR->SetSprite(L"ESC.png", 0);
		SR->LinkTexture(L"PS_MAPTEX", L"ESC.png");
	}
	
	{
		Metamorphosis = GetScene()->CreateActor();
		Metamorphosis->GetTransform()->SetScale({ 0.4f, 0.4f });
		Metamorphosis->GetTransform()->SetPosition({ -2.62f, 3.0f, -1.1f });
		HPTR<SpriteRenderer> SR = Metamorphosis->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
		SR->SetSprite(L"Metamorphosis.png", 0);
		SR->LinkTexture(L"PS_MAPTEX", L"Metamorphosis.png");
	}

	{
		GetOutButton = GetScene()->CreateActor();
		GetOutButton->GetTransform()->SetScale({ 0.6f, 0.6f });
		GetOutButton->GetTransform()->SetPosition({ 7.0f, 4.15f, -3.0f });
		HPTR<SpriteRenderer> SR = GetOutButton->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
		SR->SetSprite(L"Exit.png", 0);
		SR->LinkTexture(L"PS_MAPTEX", L"Exit.png");
	}

	{
		HPTR<Actor> BPlate2 = GetScene()->CreateActor();
		BPlate2->GetTransform()->SetScale({ 16.0f, 9.0f });
		BPlate2->GetTransform()->SetPosition({ 0.0f , 0.0f, -2.1f });
		HPTR<SpriteRenderer> SR = BPlate2->AddComponent<SpriteRenderer>(RENDERGROUP::RG_PLAYER);
		SR->SetSprite(L"collBGMask.png", 0);

		SR->RenderData(L"DTEXMESH", L"BASESPRITEPATH");
		SR->LinkCB(L"VS_CUTDATA", &(SR->GetCutData()));
		SR->LinkTexture(L"PS_MAPTEX", L"collBGMask.png");
		SR->LinkSampler(L"PS_LSMP", L"LSMP");
	}


}
void CollectSceneUpdater::SceneUpdate()
{

#pragma region OnOff

	if (false == SelectON)
	{
		{	SCBar->UpdateOn();	}
		for (int i = 0; i < 63; ++i) { StandEx[i]->UpdateOn(); }
		for (int i = 0; i < 13; ++i) { ClassEx[i]->UpdateOn(); }
		for (int i = 0; i < 15; ++i) { RaceEx[i]->UpdateOn(); }
		for (int i = 0; i < 6; ++i) { StarEx[i]->UpdateOn(); }
		StandSprite->UpdateOff();
		StandFrameUpper->UpdateOff(); StandFrameLower->UpdateOff();
		LButton->UpdateOff(); RButton->UpdateOff();
		Face->UpdateOff();

		ESCButton->UpdateOff();
		Metamorphosis->UpdateOff();
		GetOutButton->UpdateOn();

		for (int i = 0; i < 3; ++i)
		{
			MenuLetter[i]->UpdateOn();
			//
		}
		for (int i = 0; i < 5; ++i)
		{
			FaceComp[i]->UpdateOff();
			//
		}

		CurStand = -1;
		CurStar = 0;
		TransOn = false;
	}
	else
	{
		{	SCBar->UpdateOff();	}
		for (int i = 0; i < 63; ++i) { StandEx[i]->UpdateOff(); }
		for (int i = 0; i < 13; ++i) { ClassEx[i]->UpdateOff(); }
		for (int i = 0; i < 15; ++i) { RaceEx[i]->UpdateOff(); }
		for (int i = 0; i < 6; ++i) { StarEx[i]->UpdateOff(); }
		StandSprite->UpdateOn(); StandFrameUpper->UpdateOn(); StandFrameLower->UpdateOn();
		LButton->UpdateOn(); RButton->UpdateOn();
		Face->UpdateOn();

		ESCButton->UpdateOn();
		Metamorphosis->UpdateOn();
		GetOutButton->UpdateOff();

		for (int i = 0; i < 3; ++i)
		{
			MenuLetter[i]->UpdateOff();
		}

		for (int i = 0; i < 5; ++i)
		{
			FaceComp[i]->UpdateOn();
		}

//		CurClass = 0;
//		CurRace = 0;
//		CurLevel = 0;
	}
#pragma endregion






#pragma region Scroll

	

	if (4 < CurCount)
	{
		ScrbarVal += Window::_delta * -0.001f;
		Window::_delta = 0;

		if (ScrbarVal < 0.0f)
			ScrbarVal = 0.0f;
		if (ScrbarVal > 6.0f)
			ScrbarVal = 6.0f;
	}
	else
	{
		ScrbarVal = 0.0f;
	}
#pragma endregion

#pragma region Value
	ClassEx[0]->GetComponent<SpriteRenderer>()->SetSprite(L"Class.png", CurClass);
	RaceEx[0]->GetComponent<SpriteRenderer>()->SetSprite(L"Race.png", CurRace);
	StarEx[0]->GetComponent<SpriteRenderer>()->SetSprite(L"Stars.png", CurLevel);

	if(CurClass != 0)
		MenuLetter[0]->GetComponent<SpriteRenderer>()->SetSprite(L"LetterData.png", CurClass * 3 + 1);
	else
		MenuLetter[0]->GetComponent<SpriteRenderer>()->SetSprite(L"LetterData.png", 182);

	if (CurRace != 0)
		MenuLetter[1]->GetComponent<SpriteRenderer>()->SetSprite(L"LetterData.png", CurRace * 3 + 2);
	else
		MenuLetter[1]->GetComponent<SpriteRenderer>()->SetSprite(L"LetterData.png", 185);

	CurCount = 0;

	for (int i = 0; i < 63; ++i)
	{
		if ((CurClass == 0 || CurClass == Pieces[i].CLASS) && 
			(CurRace == 0 || CurRace == Pieces[i].RACE1 || CurRace == Pieces[i].RACE2) &&
			(CurLevel == 0 || CurLevel == Pieces[i].LEVEL))
		{
			StandEx[i]->GetTransform()->SetPosition({ -2.0f + 2.5f * (CurCount % 4) , 2.5f - (CurCount / 4) * 4.0f, -1.0f });
			++CurCount;
		}
		else
		{
			StandEx[i]->GetTransform()->SetPosition({ -100.0f , 0.0f, -1.0f });
			StandEx[i]->UpdateOff();
		}
	}

	if (SelectON == true && CurStand != -1)
	{
		if (TransOn == false)
		{
			StandSprite->GetComponent<SpriteAnimaion>()->ChanageAni(Pieces[CurStand].Anim[CurStar].c_str());
			StandSprite->GetComponent<SpriteRenderer>()->SetSprite(Pieces[CurStand].Anim[CurStar].c_str());
			StandSprite->GetComponent<SpriteRenderer>()->LinkTexture(L"PS_MAPTEX", Pieces[CurStand].Anim[CurStar].c_str());
			Metamorphosis->GetComponent<SpriteRenderer>()->SetSprite(L"Metamorphosis.png", 0);
		}
		else if (TransOn == true)
		{
			switch (CurStand) //변신 하는 애들 스프라이트세팅
			{
			case 25:
				if (CurStar == 0)
				{
					StandSprite->GetComponent<SpriteAnimaion>()->ChanageAni(L"Stand26_1a.png");
					StandSprite->GetComponent<SpriteRenderer>()->SetSprite(L"Stand26_1a.png");
					StandSprite->GetComponent<SpriteRenderer>()->LinkTexture(L"PS_MAPTEX", L"Stand26_1a.png");
				}
				else if (CurStar == 1)
				{
					StandSprite->GetComponent<SpriteAnimaion>()->ChanageAni(L"Stand26_2a.png");
					StandSprite->GetComponent<SpriteRenderer>()->SetSprite(L"Stand26_2a.png");
					StandSprite->GetComponent<SpriteRenderer>()->LinkTexture(L"PS_MAPTEX", L"Stand26_2a.png");
				}
				else
				{
					StandSprite->GetComponent<SpriteAnimaion>()->ChanageAni(L"Stand26_3a.png");
					StandSprite->GetComponent<SpriteRenderer>()->SetSprite(L"Stand26_3a.png");
					StandSprite->GetComponent<SpriteRenderer>()->LinkTexture(L"PS_MAPTEX", L"Stand26_3a.png");
				}
				break;
			case 38:
				if (CurStar == 0)
				{
					StandSprite->GetComponent<SpriteAnimaion>()->ChanageAni(L"Stand39_1a.png");
					StandSprite->GetComponent<SpriteRenderer>()->SetSprite(L"Stand39_1a.png");
					StandSprite->GetComponent<SpriteRenderer>()->LinkTexture(L"PS_MAPTEX", L"Stand39_1a.png");
				}
				else if (CurStar == 1)
				{
					StandSprite->GetComponent<SpriteAnimaion>()->ChanageAni(L"Stand39_2a.png");
					StandSprite->GetComponent<SpriteRenderer>()->SetSprite(L"Stand39_2a.png");
					StandSprite->GetComponent<SpriteRenderer>()->LinkTexture(L"PS_MAPTEX", L"Stand39_2a.png");
				}
				else
				{
					StandSprite->GetComponent<SpriteAnimaion>()->ChanageAni(L"Stand39_3a.png");
					StandSprite->GetComponent<SpriteRenderer>()->SetSprite(L"Stand39_3a.png");
					StandSprite->GetComponent<SpriteRenderer>()->LinkTexture(L"PS_MAPTEX", L"Stand39_3a.png");
				}
				break;
			case 48:
				if (CurStar == 0)
				{
					StandSprite->GetComponent<SpriteAnimaion>()->ChanageAni(L"Stand49_1a.png");
					StandSprite->GetComponent<SpriteRenderer>()->SetSprite(L"Stand49_1a.png");
					StandSprite->GetComponent<SpriteRenderer>()->LinkTexture(L"PS_MAPTEX", L"Stand49_1a.png");
				}
				else if (CurStar == 1)
				{
					StandSprite->GetComponent<SpriteAnimaion>()->ChanageAni(L"Stand49_2a.png");
					StandSprite->GetComponent<SpriteRenderer>()->SetSprite(L"Stand49_2a.png");
					StandSprite->GetComponent<SpriteRenderer>()->LinkTexture(L"PS_MAPTEX", L"Stand49_2a.png");
				}
				else
				{
					StandSprite->GetComponent<SpriteAnimaion>()->ChanageAni(L"Stand49_3a.png");
					StandSprite->GetComponent<SpriteRenderer>()->SetSprite(L"Stand49_3a.png");
					StandSprite->GetComponent<SpriteRenderer>()->LinkTexture(L"PS_MAPTEX", L"Stand49_3a.png");
				}
				break;
			default:
				break;
			}

			Metamorphosis->GetComponent<SpriteRenderer>()->SetSprite(L"Metamorphosis.png", 1);
		}
		StandFrameUpper->GetComponent<SpriteRenderer>()->SetSprite(L"Level.png", CurStar);
		StandFrameLower->GetComponent<SpriteRenderer>()->SetSprite(L"Level.png", CurStar + 4);

		if (CurStar == 2)
		{
			StandFrameUpper->GetComponent<SpriteRenderer>()->SetSprite(L"Level.png", 3);
			StandFrameLower->GetComponent<SpriteRenderer>()->SetSprite(L"Level.png", 7);
		}

		Face->GetComponent<SpriteRenderer>()->SetSprite(L"Face.png", CurStand);

		FaceComp[0]->GetComponent<SpriteRenderer>()->SetSprite(L"Class.png", Pieces[CurStand].CLASS);
		FaceComp[1]->GetComponent<SpriteRenderer>()->SetSprite(L"Race.png", Pieces[CurStand].RACE1);
		FaceComp[4]->GetComponent<SpriteRenderer>()->SetSprite(L"Stars.png", 8 + CurStar);

		if (Pieces[CurStand].RACE2 != 0)
		{
			FaceComp[2]->GetComponent<SpriteRenderer>()->SetSprite(L"Race.png", Pieces[CurStand].RACE2);
		}
		else
		{
			FaceComp[2]->UpdateOff();
		}

		if (CurStand != 25 && CurStand != 38 && CurStand != 48)
		{
			Metamorphosis->UpdateOff();
		}
	}

	{
		SCBar->GetTransform()->SetLoclaPosition({ 7.5f , 3.0f - ScrbarVal, -1.0f });

		for (int i = 0; i < 63; ++i)
		{
			StandEx[i]->GetTransform()->WMoveY(ScrbarVal * ((CurCount - 1) / 4) * 2.0f / 3.0f);
		}
	}
#pragma endregion

#pragma region MouseClick
	if (SelectON == false)
	{
		if (Input::Down(L"LCLICK") == true)
		{
			Sound::Play(L"backbtn.wav");
			for (int i = 0; i < 13; ++i)
			{
				if (MouseInTheBox(ClassEx[i]) == true)
				{
					CurClass = i;
					ScrbarVal = 0.0f;
					break;
				}
			}
			for (int i = 0; i < 15; ++i)
			{
				if (MouseInTheBox(RaceEx[i]) == true)
				{
					CurRace = i;
					ScrbarVal = 0.0f;
					break;
				}
			}
			for (int i = 0; i < 6; ++i)
			{
				if (MouseInTheBox(StarEx[i]) == true)
				{
					ScrbarVal = 0.0f;
					CurLevel = i;
					break;
				}
			}
		}
		for (int i = 0; i < 63; ++i)
		{
			if (MouseInTheBox(StandEx[i]) == true)
			{
				if (Input::Down(L"LCLICK") == true)
				{
					Sound::Play(L"backbtn.wav");
					CurStand = i;
					SelectON = true;
					break;
				}
				else
				{
					if (isScale[i] == false)
					{
						Sound::Play(L"battlebtn.wav");
						isScale[i] = true;
					}
					StandEx[i]->GetTransform()->SetScale({ 2.2f * 1.15f, 2.2f * 1.15f });
				}
			}
			else
			{
				isScale[i] = false;
				StandEx[i]->GetTransform()->SetScale({ 2.2f, 2.2f });
			}
		}
	}
	else
	{
		if (Input::Down(L"LCLICK") == true)
		{
			Sound::Play(L"backbtn.wav");
			if (MouseInTheBox(LButton) == true)
				CurStar--;
			else if (MouseInTheBox(RButton) == true)
				CurStar++;
			if (CurStand == 55)
			{
				if (1 < CurStar)
					CurStar = 0;
				else if (0 > CurStar)
					CurStar = 1;
			}
			else
			{
				if (2 < CurStar)
					CurStar = 0;
				else if (0 > CurStar)
					CurStar = 2;
			}
		}
	}
	if ((MouseInTheBox(ESCButton) == true))
	{
		ESCButton->GetComponent<SpriteRenderer>()->SetSprite(L"ESC.png", 1);

		if (Input::Down(L"LCLICK") == true)
		{
			Sound::Play(L"backbtn.wav");
			SelectON = false;
		}
	}
	else
	{
		ESCButton->GetComponent<SpriteRenderer>()->SetSprite(L"ESC.png", 0);
	}

	if ((MouseInTheBox(Metamorphosis) == true))
	{

		if (Input::Down(L"LCLICK") == true)
		{
			Sound::Play(L"backbtn.wav");
			if (TransOn == false)
				TransOn = true;
			else
				TransOn = false;
		}
	}

	if ((MouseInTheBox(GetOutButton) == true))
	{
		GetOutButton->GetComponent<SpriteRenderer>()->SetSprite(L"Exit.png", 1);

		if (Input::Down(L"LCLICK") == true)
		{
			Sound::Play(L"backbtn.wav");
			EscapeCollScene = true;
		}
	}
	else
	{
		GetOutButton->GetComponent<SpriteRenderer>()->SetSprite(L"Exit.png", 0);
	}

#pragma endregion

#pragma region after

	for (int i = 0; i < 63; ++i)
	{
		if (true == StandEx[i]->IsUpdate())
		{
			StandNamePlate[i]->UpdateOn();
			StandNamePlateClass[i]->UpdateOn();
			StandNamePlateRace[i]->UpdateOn();
			StandName[i]->UpdateOn();
			StandNameClass[i]->UpdateOn();
			StandNameRace[i]->UpdateOn();

			if (Pieces[i].RACE2 != 0)
			{
				StandNamePlateRace2[i]->UpdateOn();
				StandNameRace2[i]->UpdateOn();
			}
		}
		else
		{
			StandNamePlate[i]->UpdateOff();
			StandNamePlateClass[i]->UpdateOff();
			StandNamePlateRace[i]->UpdateOff();
			StandName[i]->UpdateOff();
			StandNameClass[i]->UpdateOff();
			StandNameRace[i]->UpdateOff();

			if (Pieces[i].RACE2 != 0)
			{
				StandNamePlateRace2[i]->UpdateOff();
				StandNameRace2[i]->UpdateOff();
			}
		}
	}

#pragma endregion

#pragma region portal

	if (EscapeCollScene == true)
	{
		EscapeCollScene = false;
		CurClass = 0;
		CurRace = 0;
		CurLevel = 0;
		GameWindow::MainGameWin()->ChangeScene(L"LobyScene");
	}

#pragma endregion

}
void CollectSceneUpdater::ChangeEnd()
{

}
void CollectSceneUpdater::ChangeStart()
{

}
void CollectSceneUpdater::DebugRender()
{
	/*swprintf_s(CollArr, L"MPos : X %f Y %f", GameWindow::MainGameWin()->MousePos().x, GameWindow::MainGameWin()->MousePos().y);
	GraphicDebug::DrawFontUI(CollArr, Vector4::BLUE);
	swprintf_s(CollArr, L"MPos : X %f Y %f", GameWindow::MainGameWin()->MousePosInt().x, GameWindow::MainGameWin()->MousePosInt().y);
	GraphicDebug::DrawFontUI(CollArr, Vector4::RED);*/

	if (true == SelectON)
	{
		ShowInformation(CurStand, CurStar);
	}
	else
	{
		Font->DrawUI(L"도감", 50.0f, { 170.0f, 50.0f, -5.0f }, Vector4::WHITE);
		Font->DrawUI(L"로비", 16.0f, { 1486.0f, 60.0f, -5.0f }, Vector4::WHITE);
	}

}

void CollectSceneUpdater::ShowInformation(int _StandIndex, int _StarIndex)
{
	Font->DrawUI(Pieces[_StandIndex].NAME.c_str(), 65.0f, { 1050.0f, 200.0f, -5.0f }, Vector4::WHITE);

	swprintf_s(CollArr, L"등급    %d", Pieces[_StandIndex].LEVEL);
	Font->DrawUI(CollArr, 40.0f, { 900.0f, 300.0f, -5.0f }, Vector4::WHITE);

	int tmp = 1;
	switch (_StarIndex)
	{
	case 0:
		tmp = 4;
		break;
	case 1:
		tmp = 2;
		break;
	default:
		break;
	}

	swprintf_s(CollArr, L"생명력		%d", Pieces[_StandIndex].Health / tmp);
	Font->DrawUI(CollArr, 25.0f, { 900.0f, 500.0f, -5.0f }, Vector4::WHITE);

	swprintf_s(CollArr, L"공격력		%d", Pieces[_StandIndex].Strength / tmp);
	Font->DrawUI(CollArr, 25.0f, { 900.0f, 540.0f, -5.0f }, Vector4::WHITE);

	swprintf_s(CollArr, L"사정거리		%d", Pieces[_StandIndex].Range);
	Font->DrawUI(CollArr, 25.0f, { 900.0f, 580.0f, -5.0f }, Vector4::WHITE);

	if (_StandIndex == 25 || _StandIndex == 38 || _StandIndex == 48)
	{
		Font->DrawUI(L"변신", 30.0f, { 465.0f, 136.0f, -5.0f }, Vector4::RED);
	}

	if (TransOn == true)
	{
		switch (_StandIndex)
		{
		case 25:
			swprintf_s(CollArr, L"+   %d", ((Pieces[_StandIndex].Health / tmp) / 10) * (_StarIndex + 2));
			Font->DrawUI(CollArr, 25.0f, { 1220.0f, 500.0f, -5.0f }, Vector4::GREEN);
			break;
		case 38:
			swprintf_s(CollArr, L"+   %d", 100 + (_StarIndex * 50));
			Font->DrawUI(CollArr, 25.0f, { 1220.0f, 540.0f, -5.0f }, Vector4::GREEN);

			swprintf_s(CollArr, L"+   %d", 3);
			Font->DrawUI(CollArr, 25.0f, { 1220.0f, 580.0f, -5.0f }, Vector4::GREEN);
			break;
		case 48:
			swprintf_s(CollArr, L"+   %d", 100 + (_StarIndex * 50));
			Font->DrawUI(CollArr, 25.0f, { 1220.0f, 540.0f, -5.0f }, Vector4::GREEN);

			swprintf_s(CollArr, L"+   %d", 2);
			Font->DrawUI(CollArr, 25.0f, { 1220.0f, 580.0f, -5.0f }, Vector4::GREEN);
			break;
		default:
			break;
		}
	}
}