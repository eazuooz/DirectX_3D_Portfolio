#pragma once
#include "RES.h"

class HBLEND :
	public Resources
{
private:
	D3D11_BLEND_DESC	m_Decs;
	ID3D11BlendState*	m_pState; // 컨텍스
	Vector4				m_Color;

	// ID3D11DepthStencilState*	m_pState; // 컨텍스


public:
	bool Create();
	bool Create(const D3D11_BLEND_DESC& _Desc);
	void Update();

public:
	HBLEND();
	~HBLEND();
};

