#include "stdafx.h"
#include "HMapUpdater.h"
#include "LOGICHEADER.h"

#include <GAMEWIN.h>
#include <FREECAM.h>
#include <SPRITERENDER.h>
#include <RECTRENDER.h>
#include <TEX.h>
#include <SPRITE.h>
#include <2DANICOM.h>
#include <INPUT.h>
#include "HTOOLVALUE.h"
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

HPTR<FbxAnimation> HMapUpdater::ANIREN;

HPTR<Camera> HMapUpdater::CAMCOM = nullptr;
HPTR<FreeCamera> HMapUpdater::FREECAMCOM = nullptr;
HPTR<Terrain> HMapUpdater::MAPCOM;
HPTR<Actor> HMapUpdater::PLAYER;




// #include "MOVESCRIPT.h"

HMapUpdater::HMapUpdater()
{
}


HMapUpdater::~HMapUpdater()
{
}


void HMapUpdater::SceneUpdate() 
{
	// H3DDEBUG::DrawFontUI(L"하하하", HVEC4::RED);
	// H3DDEBUG::DrawFontUI(L"호호호", HVEC4::WHITE);
	//Thread::CreateThread(L"FbxLoadingThread", TestThread, nullptr);

	{
		wchar_t Arr[256];
		swprintf_s(Arr, L"CamPos : X %f Y %f Z %f", MAINCAM->GetTransform()->GetPosition().x, MAINCAM->GetTransform()->GetPosition().y, MAINCAM->GetTransform()->GetPosition().z);
		GraphicDebug::DrawFontUI(Arr, Vector4::RED);
	}

	{
		if (nullptr != ANIREN)
		{
			{
				wchar_t Arr[256];
				swprintf_s(Arr, L"frame %d", ANIREN->GetFrameIndex());
				GraphicDebug::DrawFontUI(Arr, Vector4::RED);
			}

			{
				wchar_t Arr[256];
				swprintf_s(Arr, L"frame %f", ANIREN->GetUpdateTime());
				GraphicDebug::DrawFontUI(Arr, Vector4::RED);
			}
		}
	}
	

}

// 접속자들을 처리한다. 
// 쓰레드를 만들어서 감시한다. 

CriticalSection CS;

unsigned __stdcall TestThread(Thread* _Thread, void* _Arg)
{
	Directory Dic;
	Dic.ReplaceParentPath();
	Dic.MoveDirectory(L"BIN");
	Dic.MoveDirectory(L"TEX");
	Dic.MoveDirectory(L"Face");
	
	std::vector<File> AllFileList;
	Dic.GetFilesName(AllFileList, L"*.png");

	std::wstring Path;
	for (size_t i = 0; i < AllFileList.size(); i++)
	{
		Syncronize Sc(CS);

		if (_Thread->IsDeath())
		{
			Debug::ConsolePrintLine(L"강제종료 되었습니다");
			return 0;
		}

		Path = AllFileList[i].GetFullPath();

		Resources::Load<Texture>(Path.c_str());
		Path += L" 로딩 완료";
		Debug::ConsolePrintLine(Path.c_str());

		if (_Thread->IsDeath())
		{
			Debug::ConsolePrintLine(L"강제종료 되었습니다");
			return 0;
		}

	}

	Debug::ConsolePrintLine(L"Face 로딩완료");
	
	return 0;
}


unsigned __stdcall TestThread2(Thread* _Thread, void* _Arg)
{
	Directory Dic;
	Dic.ReplaceParentPath();
	Dic.MoveDirectory(L"BIN");
	Dic.MoveDirectory(L"TEX");
	Dic.MoveDirectory(L"Map");

	std::vector<File> AllFileList;
	Dic.GetFilesName(AllFileList, L"*.png");

	std::wstring Path;
	for (size_t i = 0; i < AllFileList.size(); i++)
	{
		Syncronize Sc = Syncronize(CS);

		if (_Thread->IsDeath())
		{
			Debug::ConsolePrintLine(L"강제종료 되었습니다");
			return 0;
		}

		Path = AllFileList[i].GetFullPath();

		Resources::Load<Texture>(Path.c_str());
		Path += L" 로딩 완료";
		Debug::ConsolePrintLine(Path.c_str());

		if (_Thread->IsDeath())
		{
			Debug::ConsolePrintLine(L"강제종료 되었습니다");
			return 0;
		}


	}

	Debug::ConsolePrintLine(L"Map 로딩완료");

	return 0;
}

void HMapUpdater::Loading()
{
	Debug::OpenConsole();








	// 중요!!! 이제부터 캠은 자신이 어떤 그룹을 봐야할지 여기에 넣어줘야 한다.
	// 잘 인지하고 있어야 한다.
	{
		MAINCAM = GetScene()->CreateActor(L"CAM");
		MAINCAM->GetTransform()->SetLoclaPosition({ 0.0f, 50.0f, -50.0f });
		MAINCAM->GetTransform()->SetLocalRotX(45.0f);
		CAMCOM = MAINCAM->AddComponent<Camera>(RG_GRID, RG_PLAYER);
		
		
		FREECAMCOM = MAINCAM->AddComponent<FreeCamera>();
		//BFilter = CAMCOM->CreateFilter<BloomFilter>();

		CAMCOM->Order(2);
		CAMCOM->SetFarValue(100000.0f);
		CAMCOM->SetProjMode(PROJMODE::PM_PERS);
		CAMCOM->SetArea(TOOLVIEWX, TOOLVIEWY);
	}

	{
		// 합쳐졌다.
		HPTR<Actor> ObjActor = GetScene()->CreateActor(L"CAM");
		ObjActor->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, -50.0f });
		HPTR<Camera> UICAM = ObjActor->AddComponent<Camera>(RG_UI);
		UICAM->Order(-10);
		UICAM->SetFarValue(100000.0f);
		UICAM->SetProjMode(PROJMODE::PM_ORTH);
		UICAM->SetArea(800, 600);
	}


	//{
	//	HPTR<HACTOR> OBJ = Scene()->CreateActor(L"CAM");
	//	OBJ->Trans()->LPos({ -10.0f, 0.0f, -20.0f });
	//	CAMCOM = OBJ->AddCom<HCAM>(RG_GRID, RG_PLAYER);
	//	CAMCOM->Order(12);
	//	CAMCOM->Far(100000.0f);
	//	CAMCOM->ProjMode(PROJMODE::PM_PERS);
	//	CAMCOM->ViewPortSizeSetting(400, 300, 400, 300);
	//}


	//GRID
	{
		HPTR<Actor> OBJ = GetScene()->CreateActor();
		OBJ->GetTransform()->SetLocalScale({ 10000.0F, 10000.0F, 10000.0F });

		HPTR<GridRenderer> SR = OBJ->AddComponent<GridRenderer>(RG_GRID);

	}

	// LIGHT
	//{
	//	HPTR<HACTOR> OBJ = Scene()->CreateActor();
	//	HPTR<HLIGHT> COM1 = OBJ->AddCom<HLIGHT>(RG_PLAYER);
	//	COM1->LightType(HLIGHT::LT_POINT);
	//	COM1->SpecPow(1);
	//}

	//{
	//	HPTR<HACTOR> OBJ = Scene()->CreateActor();
	//	OBJ->Trans()->WPos({ -15.0f, 0.0f, 0.0f });
	//	HPTR<HLIGHT> COM1 = OBJ->AddCom<HLIGHT>(RG_PLAYER);
	//	COM1->LightType(HLIGHT::LT_POINT);
	//	COM1->SpecPow(100);
	//}

	{
		HPTR<Actor> OBJ = GetScene()->CreateActor();
		OBJ->GetTransform()->SetLocalRotX(45.0f);
		OBJ->GetTransform()->SetLoclaPosition({ 0.0f, 1000.0f, -1000.0f });
		LIGHTCHECK = OBJ->AddComponent<Light>(RG_PLAYER);
		LIGHTCHECK->SetLightType(Light::LT_DIR);
		LIGHTCHECK->SetSpecPow(1);
		LIGHTCHECK->SetDiffPow(1.0f);
		LIGHTCHECK->SpecPower(2.0f);
	}

	//{
	//	HPTR<Actor> OBJ = GetScene()->CreateActor();
	//	OBJ->GetTransform()->SetLocalRotX(90.0f);
	//	HPTR<Light> COM1 = OBJ->AddComponent<Light>(RG_PLAYER);
	//	COM1->SetLightType(Light::LT_DIR);
	//	COM1->SetSpecPow(1);
	//	COM1->SetDiffPow(0.8f);
	//}


	//{
	//	PLAYER = GetScene()->CreateActor();
	//	PLAYER->GetTransform()->SetLoclaPosition({ -10.0f, 10.0f, 0.0F });
	//	PLAYER->GetTransform()->SetLocalScale({ 30.0F, 30.0F, 30.0F});
	//	PLAYER->GetTransform()->LocalScaleMul(1.0f);
	//	
	//	PLAYER->AddComponent<BoneChaser>();

	//	HPTR<BoundingBox> Col = PLAYER->AddComponent<BoundingBox>();
	//	Col->DebugOn();
	//	Col->SetScale(Vector4{ 10.f, 10.f , 10.f });

	//	HPTR<Renderer> SR = PLAYER->AddComponent<Renderer>(RG_PLAYER);
	//	Renderer::HRENDERDATA* Data = SR->RenderData(L"SPHEREMESH", L"LIGHTMESHDPATH");
	//	Data->m_Path->m_BUMP = BUMPOK;
	//	Data->m_bShadow = true;
	//	// SR->RenderData(L"BOXMESH", L"AURAPATH");
	//	// 상수 버퍼에는 2가지 종류가 필요하다.
	//	// 주소연결해주는 경우.
	//	// 0번 랜더 패스에
	//	// 버텍스 쉐이더 데이터에
	//	SR->CreateCB(L"VS_CUTDATA", Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	//	// 직접 내가 상수값을 들고 있는 경우.
	//	SR->LinkTexture(L"PS_MAPTEX", L"BUMPTEST.png");
	//	SR->LinkTexture(L"PS_BUMPTEX", L"BUMPTEST_N.png");
	//	SR->LinkSampler(L"PS_LSMP", L"LSMP");
	//	SR->LinkSampler(L"PS_PSMP", L"PSMP");
	//}

	//{
	//	PLAYER = GetScene()->CreateActor();
	//	PLAYER->GetTransform()->SetLoclaPosition({ -100.0f, 10.0f, 0.0F });
	//	PLAYER->GetTransform()->SetLocalRotZ(30.0f);
	//	PLAYER->GetTransform()->SetLocalScale({ 60.0F, 200.0F, 60.0F });
	//	PLAYER->GetTransform()->LocalScaleMul(1.0f);
	//		

	//	HPTR<BoundingBox> Col = PLAYER->AddComponent<BoundingBox>();
	//	Col->DebugOn();
	//	Col->SetScale(Vector4{ 10.f, 10.f , 10.f });

	//	HPTR<Renderer> SR = PLAYER->AddComponent<Renderer>(RG_PLAYER);
	//	Renderer::HRENDERDATA* Data = SR->RenderData(L"SPHEREMESH", L"LIGHTMESHDPATH");
	//	Data->m_Path->m_BUMP = BUMPOK;
	//	Data->m_bShadow = true;
	//	// Data->m_bShadow = true;
	//	// SR->RenderData(L"BOXMESH", L"AURAPATH");
	//	// 상수 버퍼에는 2가지 종류가 필요하다.
	//	// 주소연결해주는 경우.
	//	// 0번 랜더 패스에
	//	// 버텍스 쉐이더 데이터에
	//	SR->CreateCB(L"VS_CUTDATA", Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	//	// 직접 내가 상수값을 들고 있는 경우.
	//	SR->LinkTexture(L"PS_MAPTEX", L"BUMPTEST.png");
	//	SR->LinkTexture(L"PS_BUMPTEX", L"BUMPTEST_N.png");
	//	SR->LinkSampler(L"PS_LSMP", L"LSMP");
	//	SR->LinkSampler(L"PS_PSMP", L"PSMP");
	//}


	//{
	//	HPTR<Actor> PACTOR = GetScene()->CreateActor();
	//	PACTOR->GetTransform()->SetLoclaPosition({ 10.0f, 50.0f, -30.0F });
	//	PACTOR->GetTransform()->SetLocalScale({ 50.0F, 50.0F, 50.0F });
	//	PACTOR->GetTransform()->LocalScaleMul(1.0f);
	//	HPTR<BoundingBox> Col = PACTOR->AddComponent<BoundingBox>();
	//	Col->DebugOn();
	//	Col->SetScale(Vector4{ 10.f, 10.f , 10.f });

	//	HPTR<Renderer> SR = PACTOR->AddComponent<Renderer>(RG_PLAYER);
	//	Renderer::HRENDERDATA* Data = SR->RenderData(L"PLANEMESH", L"LIGHTMESHDPATH");
	//	Data->m_Path->m_BUMP = BUMPOK;
	//	Data->m_bShadow = true;
	//	// SR->RenderData(L"BOXMESH", L"AURAPATH");
	//	// 상수 버퍼에는 2가지 종류가 필요하다.
	//	// 주소연결해주는 경우.
	//	// 0번 랜더 패스에
	//	// 버텍스 쉐이더 데이터에

	//	SR->CreateCB(L"VS_CUTDATA", Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	//	// 직접 내가 상수값을 들고 있는 경우.
	//	SR->LinkTexture(L"PS_MAPTEX", L"BUMPTEST.png");
	//	SR->LinkTexture(L"PS_BUMPTEX", L"BUMPTEST_N.png");
	//	SR->LinkSampler(L"PS_LSMP", L"LSMP");
	//	SR->LinkSampler(L"PS_PSMP", L"PSMP");
	//}

	//{
	//	HPTR<Actor> PACTOR = GetScene()->CreateActor();
	//	PACTOR->GetTransform()->SetLoclaPosition({ 0.0f, 0.5f, 0.0F });
	//	// 맵의 크기
	//	PACTOR->GetTransform()->SetLocalScale({ 10000.0F, 1.0F, 10000.0F });
	//	PACTOR->GetTransform()->LocalScaleMul(1.0f);

	//	MAPCOM = PACTOR->AddComponent<Terrain>(RG_PLAYER);
	//	MAPCOM->CreateTerrain(100, 100, L"TILE_01.png", L"TILE02_S.png", 1000.0f, 1.0f);
	//	//MAPCOM->FloorTextureSetting(1, L"BUMPTEST.png", L"TILE02_S.png", 0.1F);

	//	MAPCOM->FloorBumpSetting(0, L"TILE_01_N.png");
	//	//MAPCOM->FloorBumpSetting(1, L"BUMPTEST_N.png");
	//	
	//	//// 101 * 101
	//	//SR->CreateTerrain(100, 100, nullptr);
	//	//HRENDER::HRENDERDATA* Data; // SR->RenderData(L"BOXMESH", L"LIGHTMESHPATH");
	//	//Data->m_Path->m_BUMP = BUMPOK;

	//	// SR->RenderData(L"BOXMESH", L"AURAPATH");
	//	// 상수 버퍼에는 2가지 종류가 필요하다.
	//	// 주소연결해주는 경우.
	//	// 0번 랜더 패스에
	//	// 버텍스 쉐이더 데이터에
	//}
	//Resources::Create<Sprite>(L"LoadingSceneSprite.png", L"LoadingSceneSprite.png", 5, 5);

	//{
	//	HPTR<Actor> PACTOR = GetScene()->CreateActor();
	//	PACTOR->GetTransform()->SetLoclaPosition({ 0.0f, 0.5f, 0.0F });
	//	// 맵의 크기
	//	PACTOR->GetTransform()->SetLocalScale({ 100.0F, 100.0F, 100.0F });
	//	HPTR<SpriteRenderer> RENDER = PACTOR->AddComponent<SpriteRenderer>(RG_UI);
	//	RENDER->SetSprite(L"LoadingSceneSprite.png", 0);

	//	RENDER->RenderData(L"DTEXMESH", L"UISPRITEPATH");
	//	RENDER->LinkCB(L"VS_UICUTDATA", &(RENDER->GetCutData()));
	//	RENDER->LinkCB(L"UIDATA", &(RENDER->GetUIData()));
	//	RENDER->LinkTexture(L"PS_MAPTEX", L"LoadingSceneSprite.png");
	//	RENDER->LinkSampler(L"PS_LSMP", L"LSMP");
	//	RENDER->LinkSampler(L"PS_PSMP", L"PSMP");
	//}



	//{

	//	HPTR<Actor> PACTOR = GetScene()->CreateActor();
	//	PACTOR->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
	//	// 맵의 크기
	//	PACTOR->GetTransform()->SetLocalScale({ 100.0F, 100.0F, 100.0F });
	//	HPTR<SpriteRenderer> RENDER = PACTOR->AddComponent<SpriteRenderer>(RG_PLAYER);
	//	RENDER->BillBoardOn();
	//	RENDER->SetSprite(L"LoadingSceneSprite.png", 0);

	//	RENDER->RenderData(L"DTEXMESH", L"UISPRITEPATH");
	//	RENDER->LinkCB(L"VS_UICUTDATA", &(RENDER->GetCutData()));
	//	RENDER->LinkCB(L"UIDATA", &(RENDER->GetUIData()));
	//	RENDER->LinkTexture(L"PS_MAPTEX", L"LoadingSceneSprite.png");
	//	RENDER->LinkSampler(L"PS_LSMP", L"LSMP");
	//	RENDER->LinkSampler(L"PS_PSMP", L"PSMP");
	//}

}

void HMapUpdater::DebugRender()
{
	//Vector4 Pos;
	//Pos = { -400 + 80, 250 };
	//Vector4 Size;
	//Size = { 160, 120 };

	//GraphicDebug::DrawTexture(CAMCOM->GetDeferredTarget()->Target(0)->GetTex(), Pos, Size);
	//Pos.x += Size.x;
	//GraphicDebug::DrawTexture(CAMCOM->GetDeferredTarget()->Target(1)->GetTex(), Pos, Size);
	//Pos.x += Size.x;
	//GraphicDebug::DrawTexture(CAMCOM->GetDeferredTarget()->Target(2)->GetTex(), Pos, Size);
	//Pos.x += Size.x;
	//GraphicDebug::DrawTexture(CAMCOM->GetDeferredTarget()->Target(3)->GetTex(), Pos, Size);

	//Pos = { -400 + 80, 250 - Size.y };
	//GraphicDebug::DrawTexture(CAMCOM->GetLightTarget()->Target(0)->GetTex(), Pos, Size);
	//Pos.x += Size.x;
	//GraphicDebug::DrawTexture(CAMCOM->GetLightTarget()->Target(1)->GetTex(), Pos, Size);

	//if (nullptr != BFilter)
	//{
	//	Pos.x += Size.x;
	//	GraphicDebug::DrawTexture(BFilter->SmallTaregt->GetTex(), Pos, Size/*, HVEC4{128, 128}*/);
	//	Pos.x += Size.x;
	//	GraphicDebug::DrawTexture(BFilter->SSmallTaregt->GetTex(), Pos, Size/*, HVEC4{128, 128}*/);
	//}

	//if (nullptr != LIGHTCHECK)
	//{
	//	Pos.x += Size.x;
	//	GraphicDebug::DrawTexture(LIGHTCHECK->m_ShadowTarget->GetTex(), Pos, Size/*, HVEC4{128, 128}*/);
	//}

}