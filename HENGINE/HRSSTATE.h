#pragma once 
#include "RES.h"

class HRSSTATE : public Resources
{
private:
	// Unmap 내가 안좋다.
	// 몰아서 해주는 최적화 기능
	D3D11_RASTERIZER_DESC		m_Decs;
	ID3D11RasterizerState*		m_pState; // 컨텍스

public:
	bool Create();
	bool Create(const D3D11_RASTERIZER_DESC& _Desc);
	void Update();

public:
	HRSSTATE();
	~HRSSTATE();
};

