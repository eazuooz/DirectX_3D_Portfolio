#include "HEFONT.h"



HEFONT::HEFONT() : m_pFontFactory(nullptr), m_pFontWrapper(nullptr)
{
}


HEFONT::~HEFONT()
{
	if (nullptr != m_pFontFactory)
	{
		m_pFontFactory->Release();
	}

	if (nullptr != m_pFontWrapper)
	{
		m_pFontWrapper->Release();
	}
}

// 다이렉트에서 폰트를 만드는건 폰트 캐싱 
// 글자 조합에 따른 쉐이더 처리를 해줘야 한다.

bool HEFONT::Create(const wchar_t* _FontName) 
{
	//  C:: 무슨무슨.ttf

	if (S_OK != FW1CreateFactory(FW1_VERSION, &m_pFontFactory))
	{
		BOOM;
		return false;
	}

	// m_pFontFactory->CreateFontWrapper()

	if (S_OK != m_pFontFactory->CreateFontWrapper(GraphicDevice::MainDevice(), _FontName, &m_pFontWrapper))
	{
		BOOM;
		return false;
	}

	return true;
}

void HEFONT::DrawWorld(const wchar_t* _Text, float _fSize, Vector4 Pos, Vector4 COLOR, FW1_TEXT_FLAG _Flag)
{
	if (nullptr == m_pFontWrapper)
	{
		BOOM;
		return;
	}

	// 
	// m_pFontWrapper->DrawGeometry
}

void HEFONT::DrawUI(const wchar_t* _Text, float _fSize, Vector4 Pos, Vector4 COLOR, FW1_TEXT_FLAG _Flag /*= FW1_TOP*/)
{
	if (nullptr == m_pFontWrapper)
	{
		BOOM;
		return;
	}

	// 옵션 섞을수 있는거 같은데. 보자.
	// FW1_TEXT_FLAG 이거 비트 단위 연산인듯....
	m_pFontWrapper->DrawString(GraphicDevice::MainContext(), _Text, _fSize, Pos.x, Pos.y, COLOR.ConToUIntColorRGBA(), _Flag);
}