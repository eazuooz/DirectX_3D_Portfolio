#include "HMULTIRENDERTarget.h"


HMULTIRENDERTarget::HMULTIRENDERTarget() : SizeX(-1), SizeY(-1)
{
}


HMULTIRENDERTarget::~HMULTIRENDERTarget()
{
}


void HMULTIRENDERTarget::OmSet() 
{
	GraphicDevice::MainContext()->OMSetRenderTargets((UINT)ArrRTV.size(), &ArrRTV[0], m_DSV);
}

void HMULTIRENDERTarget::Depth(HPTR<HDEPTHBUFFER> _DSBUFFER) 
{
	
	if (nullptr != _DSBUFFER)
	{
		m_DSBUFFER = _DSBUFFER;
		m_DSV = _DSBUFFER->DSV();
	}
}

void HMULTIRENDERTarget::CreateTarget(int _W, int _H)
{
	HPTR<HRENDERTARGET> NewTarget = new HRENDERTARGET();

	NewTarget->Create({ 0.0f, 0.0f, 0.0f, 0.0f } // ÀÌ Å¸°ÙÀÌ Å¬¸®¾îÅ¸°Ù µÉ¶§ »ö±ò
		, (UINT)_W // ³Êºñ
		, (UINT)_H // ³ôÀÌ
		, DXGI_FORMAT_R32G32B32A32_FLOAT // »ö±ò ÇüÅÂ
		, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

	ArrRenderTarget.push_back(NewTarget);
	ArrRTV.push_back(NewTarget->m_pRTV);
}

void HMULTIRENDERTarget::Clear() 
{
	for (size_t i = 0; i < ArrRenderTarget.size(); i++)
	{
		ArrRenderTarget[i]->Clear();
	}

}