#include "MOVESCRIPT.h"
#include <GAMEWIN.h>
#include <RECTRENDER.h>
#include <list>

MOVESCRIPT::MOVESCRIPT()
{
}


MOVESCRIPT::~MOVESCRIPT()
{
}

void MOVESCRIPT::Update() 
{
	// 랜더 비포에서 해주던가 해야한다.

	//std::vector<HPTR<HRECTRENDER>> List = GetActor()->GetComToList<HRECTRENDER>();
	//List[1]->SetPosition({ 0.0f });

	//if (Input::Press(L"LEFT"))
	//{
	//	GetActor()->GetTransform()->LocalTranslate3D(Vector4::DLEFT);
	//} 

	//if (Input::Press(L"RIGHT"))
	//{
	//	GetActor()->GetTransform()->LocalTranslate3D(Vector4::DRIGHT);
	//}

	//if (Input::Press(L"UP"))
	//{
	//	GetActor()->GetTransform()->LocalTranslate3D(Vector4::DUP);
	//}

	//if (Input::Press(L"DOWN"))
	//{
	//	GetActor()->GetTransform()->LocalTranslate3D(Vector4::DDOWN);
	//}

	//if (Input::Press(L"ZPROT"))
	//{
	//	// HDEBUG::OutputDebugStringW(L"하하핳핳ㅎㅎ\n");

	//	// 1
	//	//   1
	//	//     1
	//	// HGAMEWIN::MainGameWin()->ChangeScene(L"PlayScene");
	//	// GetActor()->Trans()->WScalePlus(10.0f * HTIME::DeltaTime());
	//	GetActor()->GetTransform()->RotationZSum(360.0f * Time::DeltaTime());
	//}

	//if (Input::Press(L"ZMROT"))
	//{
	//	// GetActor()->Trans()->WScalePlus(-10.0f * HTIME::DeltaTime());
	//	GetActor()->GetTransform()->RotationZSum(-360.0f * Time::DeltaTime());
	//}
}