#include "HDEPTHBUFFER.h"



HDEPTHBUFFER::HDEPTHBUFFER()
{
}


HDEPTHBUFFER::~HDEPTHBUFFER()
{
}

void HDEPTHBUFFER::Create(UINT _W, UINT _H, DXGI_FORMAT _eFormat, float _ClearDepth, __int8 _Stencil)
{
	m_ClearDepth = _ClearDepth;
	m_Stencil = _Stencil;
	// 일반적인 세팅 
	m_Tex = new Texture();

	//D3D11_USAGE::D3D11_USAGE_DEFAULT 그래픽 카드에만 저장하는 세팅
	if (false == m_Tex->Create(_W, _H, _eFormat, D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE::D3D11_USAGE_DEFAULT))
	{
		BOOM;
	}

	m_DSV = m_Tex->GetDSV();
}

void HDEPTHBUFFER::Clear() 
{
	if (nullptr != GraphicDevice::MainContext())
	{
		// 행렬 때분이라는 것이 
		GraphicDevice::MainContext()->ClearDepthStencilView(m_Tex->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, m_ClearDepth, m_Stencil);
	}
}