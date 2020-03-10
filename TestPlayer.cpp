#include "TestPlayer.h"
#include <GRAPHICDEBUG.h>


TestPlayer::TestPlayer()
{
}


TestPlayer::~TestPlayer()
{
}

void TestPlayer::Init()
{
	if (false == Input::IsKey(L"MOVELEFT")) { Input::CreateKey(L"MOVELEFT", 'A'); }
	if (false == Input::IsKey(L"MOVERIGHT")) { Input::CreateKey(L"MOVERIGHT", 'D'); }
	if (false == Input::IsKey(L"MOVEFORWARD")) { Input::CreateKey(L"MOVEFORWARD", 'W'); }
	if (false == Input::IsKey(L"MOVEBACK")) { Input::CreateKey(L"MOVEBACK", 'S'); }
	if (false == Input::IsKey(L"MOVEUP")) { Input::CreateKey(L"MOVEUP", 'E'); }
	if (false == Input::IsKey(L"MOVEDOWN")) { Input::CreateKey(L"MOVEDOWN", 'Q'); }
}

void TestPlayer::Update()
{
	// 마우스가 움직였다면

	if (true == Input::Press(L"MOVERIGHT"))
	{
		GetActor()->GetTransform()->LocalRotYSum(100.0F * Time::DeltaTime());

	}

	if (true == Input::Press(L"MOVELEFT"))
	{
		GetActor()->GetTransform()->LocalRotYSum(-100.0F * Time::DeltaTime());
	}

	if (true == Input::Press(L"MOVEFORWARD"))
	{
		GetActor()->GetTransform()->WMove(GetTransform()->LookForward() * Time::SDeltaTime(100.0F));

	}

	if (true == Input::Press(L"MOVEBACK"))
	{
		GetActor()->GetTransform()->WMove(GetTransform()->LookBack() * Time::SDeltaTime(100.0F));
	}

	GetActor()->GetTransform()->WMove(Vector4::DOWN * Time::SDeltaTime(10.0F));
	float ColY = m_Map->WPosToY(GetTransform()->GetPosition());
	if (ColY != FLT_MAX)
	{
		GetTransform()->SetPostionY(ColY);
	}
	else {
		int a = 0;
	}


	{
		wchar_t Arr[256];

		swprintf_s(Arr, L"Player %f %f %f", GetTransform()->GetPosition().x, GetTransform()->GetPosition().y, GetTransform()->GetPosition().z);
		GraphicDebug::DrawFontUI(Arr, Vector4::RED);

		swprintf_s(Arr, L"Index %f %f", m_Map->WPosToIndex(GetTransform()->GetPosition()).x, m_Map->WPosToIndex(GetTransform()->GetPosition()).z);
		GraphicDebug::DrawFontUI(Arr, Vector4::RED);
	}




	if (nullptr != m_Map)
	{
		//Vector4 Pos = GetTransform()->WPos();
		//float YPos = m_Map->WPosToY(Pos);
		//Pos.y = YPos;
		//GetTransform()->WPos(Pos);
	}

	//if (true == Input::Press(L"CAMRIGHT"))
	//{
	//	Actor()->GetTransform()->LMove(Actor()->GetTransform()->LRight() * HTIME::SDeltaTime(m_Speed));
	//}
	//if (true == Input::Press(L"CAMFORWARD"))
	//{
	//	Actor()->GetTransform()->LMove(Actor()->GetTransform()->LForward() * HTIME::SDeltaTime(m_Speed));
	//}
	//if (true == Input::Press(L"CAMBACK")) {
	//	Actor()->GetTransform()->LMove(Actor()->GetTransform()->LBack() * HTIME::SDeltaTime(m_Speed));
	//}
	//if (true == Input::Press(L"CAMUP")) {
	//	Actor()->GetTransform()->LMove(Actor()->GetTransform()->LUp() * HTIME::SDeltaTime(m_Speed));
	//}
	//if (true == Input::Press(L"CAMDOWN")) {
	//	Actor()->GetTransform()->LMove(Actor()->GetTransform()->LDown() * HTIME::SDeltaTime(m_Speed));
	//}

}
