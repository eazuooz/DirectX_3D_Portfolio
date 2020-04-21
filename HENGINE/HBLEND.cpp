#include "HBLEND.h"
#include "GRAPHICDEVICE.h"


HBLEND::HBLEND() : m_Color(1.0f)
{
}


HBLEND::~HBLEND()
{
	if (nullptr != m_pState)
	{
		m_pState->Release();
		m_pState = nullptr;
	}
}


bool HBLEND::Create() 
{
	// 각자 할거냐 
	// false면 한개만 사용하겠다라는 이야기
	// 그중 0번만 사용하겠다.
	// 랜더타겟이 여러개라도 0번의 블랜드 방식만을 이용하겠다는 거지
	// 세팅 안해준 녀석들을 블랜딩을 안하겠다는 것은 아니다.

	//BOOL BlendEnable;
	//D3D11_BLEND SrcBlend;
	//D3D11_BLEND DestBlend;
	//D3D11_BLEND_OP BlendOp;
	//D3D11_BLEND SrcBlendAlpha;
	//D3D11_BLEND DestBlendAlpha;
	//D3D11_BLEND_OP BlendOpAlpha;
	//UINT8 RenderTargetWriteMask;

	//AlphaToCoverageEnable	FALSE
	//IndependentBlendEnable	FALSE
	//RenderTarget[0].BlendEnable	FALSE
	//RenderTarget[0].SrcBlend	D3D11_BLEND_ONE
	//RenderTarget[0].DestBlend	D3D11_BLEND_ZERO
	//RenderTarget[0].BlendOp	D3D11_BLEND_OP_ADD
	//RenderTarget[0].SrcBlendAlpha	D3D11_BLEND_ONE
	//RenderTarget[0].DestBlendAlpha	D3D11_BLEND_ZERO
	//RenderTarget[0].BlendOpAlpha	D3D11_BLEND_OP_ADD
	//RenderTarget[0].RenderTargetWriteMask	D3D11_COLOR_WRITE_ENABLE_ALL

	// https://docs.microsoft.com/ko-kr/windows/desktop/api/d3d11/ne-d3d11-d3d11_blend
	//typedef enum D3D11_BLEND {
	//	D3D11_BLEND_ZERO,
	//	D3D11_BLEND_ONE,
	//	D3D11_BLEND_SRC_COLOR,
	//	D3D11_BLEND_INV_SRC_COLOR,
	//	D3D11_BLEND_SRC_ALPHA,
	//	D3D11_BLEND_INV_SRC_ALPHA,
	//	D3D11_BLEND_DEST_ALPHA,
	//	D3D11_BLEND_INV_DEST_ALPHA,
	//	D3D11_BLEND_DEST_COLOR,
	//	D3D11_BLEND_INV_DEST_COLOR,
	//	D3D11_BLEND_SRC_ALPHA_SAT,
	//	D3D11_BLEND_BLEND_FACTOR,
	//	D3D11_BLEND_INV_BLEND_FACTOR,
	//	D3D11_BLEND_SRC1_COLOR,
	//	D3D11_BLEND_INV_SRC1_COLOR,
	//	D3D11_BLEND_SRC1_ALPHA,
	//	D3D11_BLEND_INV_SRC1_ALPHA
	//};

	// m_Decs.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// rgba다 그냥 연산대로 씌워라.
		// 일반적인 기본 블랜드 방식으로 처리.
	// 카운트가 4이상인 상태에서 

	// 디바이스쪽
	// SWINFO.SampleDesc.Count = 4;
	// tDesc.SampleDesc.Count = 4;

	m_Decs.AlphaToCoverageEnable = true;
	m_Decs.IndependentBlendEnable = false;
	m_Decs.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_Decs.RenderTarget[0].BlendEnable = true;
	// 일반컬러 쪽
	m_Decs.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	m_Decs.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	m_Decs.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	m_Decs.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	m_Decs.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	m_Decs.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	//CAMBLEND.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//CAMBLEND.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	//CAMBLEND.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//CAMBLEND.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	//CAMBLEND.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	//CAMBLEND.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	return Create(m_Decs);
}
bool HBLEND::Create(const D3D11_BLEND_DESC& _Desc) 
{
	if (&m_Decs != &_Desc)
	{
		m_Decs = _Desc;
	}

	if (S_OK != GraphicDevice::MainDevice()->CreateBlendState(&m_Decs, &m_pState))
	{
		BOOM;
		return false;
	}

	return true;
}
void HBLEND::Update() 
{
	// 0xffffffff 4가지 색깔이란 and 둘다 0이면 0하는 연산.
	// 255
	GraphicDevice::MainContext()->OMSetBlendState(m_pState, m_Color.Arr, 0xffffffff);
}