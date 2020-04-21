#pragma once
#include "RES.h"

// 메테리얼내부의 함수를 랜더링 파이프라인이라고 만들고.

// 완벽해.
// 동균스꺼 보세요.
// 지금 진행상황으로는 굉장히 정석적으로
// 잘나가고 있다.
// 내부 코드까지 완벽했으면 좋겠다.

class HDSSTATE : public Resources
{
private:
	D3D11_DEPTH_STENCIL_DESC	m_Decs;
	ID3D11DepthStencilState*	m_pState; // 컨텍스

public:
	bool Create();
	bool Create(const D3D11_DEPTH_STENCIL_DESC& _Desc);
	void Update();


public:
	HDSSTATE();
	~HDSSTATE();
};

