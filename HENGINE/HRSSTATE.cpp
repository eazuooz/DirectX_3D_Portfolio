#include "HRSSTATE.h"

HRSSTATE::HRSSTATE()
{
}


HRSSTATE::~HRSSTATE()
{
	if (nullptr != m_pState)
	{
		m_pState->Release();
		m_pState = nullptr;
	}
}

bool HRSSTATE::Create()
{
	//typedef struct D3D11_RASTERIZER_DESC
	//{
	//	D3D11_FILL_MODE FillMode;
	//	D3D11_CULL_MODE CullMode;
	//	BOOL FrontCounterClockwise;
	//	INT DepthBias;
	//	FLOAT DepthBiasClamp;
	//	FLOAT SlopeScaledDepthBias;
	//	BOOL DepthClipEnable;
	//	BOOL ScissorEnable;
	//	BOOL MultisampleEnable;
	//	BOOL AntialiasedLineEnable;
	//} 	D3D11_RASTERIZER_DESC;

	// m_Decs.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	// 박스 만들어 봐라.
	// 스피어 만들어 봐라.
	m_Decs.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	m_Decs.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK; // 이녀석에 의해서 결정이 나게 된다.
	// 쓰면 느릴거다.
	// m_Decs.DepthBias \ 

	m_Decs.MultisampleEnable = FALSE;
	m_Decs.AntialiasedLineEnable = FALSE;

	return Create(m_Decs);
}

bool HRSSTATE::Create(const D3D11_RASTERIZER_DESC& _Desc)
{
	if (&m_Decs != &_Desc)
	{
		m_Decs = _Desc;
	}

	if (S_OK != GraphicDevice::MainDevice()->CreateRasterizerState(&m_Decs, &m_pState))
	{
		BOOM;
		return false;
	}

	return true;

}

void HRSSTATE::Update()
{
	GraphicDevice::MainContext()->RSSetState(m_pState);
}