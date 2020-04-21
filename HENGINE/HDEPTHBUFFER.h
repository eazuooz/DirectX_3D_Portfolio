#pragma once
#include "TEX.h"

class HDEPTHBUFFER : Resources
{
private:
	HPTR<Texture> m_Tex;
	float m_ClearDepth;
	__int8 m_Stencil;
	ID3D11DepthStencilView* m_DSV;

public:
	ID3D11DepthStencilView* DSV() {
		return m_DSV;
	}

public:
	void Create(UINT _W, UINT _H, DXGI_FORMAT _eFormat, float _ClearDepth = 1.0f, __int8 _Stencil = 0);
	void Clear();

public:
	HDEPTHBUFFER();
	~HDEPTHBUFFER();
};

