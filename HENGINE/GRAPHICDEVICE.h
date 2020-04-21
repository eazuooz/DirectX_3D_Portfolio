#pragma once
#include <MATHS.h>
// 윈도우 10부터는 윈도우 그자체의 랜더링에도 다이렉트가 쓰이고 있기 때문에
// 윈도우 기반 c++에서는 stdlib수준으로 이미 다 include 포함까지 다 되어있다.

#define D3D2

#include <d3d11_4.h> 

#include <d3dcompiler.h> // 쉐이더라는걸 컴파일하는데 사용되는 헤더
#include <JWINDOW.h>
#include <map>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

#ifdef X32
#ifdef DEBUGMODE
#pragma comment(lib, "\\X32\\BASED32.lib")
#elif RELEASEMODE
#pragma comment(lib, "\\X32\\BASER32.lib")
#endif
#elif X64
#ifdef DEBUGMODE
#pragma comment(lib, "\\X64\\BASED64.lib")
#elif RELEASEMODE
#pragma comment(lib, "\\X64\\BASER64.lib")
#endif
#endif


// 그래픽카드의 기능을 사용하기 위한 라이브러리인
// D3DX의 기능을 모아놓을 클래스	
class Window;
class GraphicDevice : public Ref
{
/////////////// static
	// HWINDOW의 구조를 따라서 만들어볼 생각. 
private:
	static GraphicDevice* m_MainDevice;
	static ID3D11Device* m_pMainDevice; // 그래픽카드의 모든 자원에 대한 권한을 담당. (o)
	static ID3D11DeviceContext* m_pMainContext; // 그래픽카드의 모든 연산에 대한 권한을 담당한다.(랜더) (o)
	static bool m_PathInit;


public:
	static HPTR<GraphicDevice> Inst() 
	{
		return m_MainDevice;
	}

	static ID3D11Device* MainDevice()
	{
		return m_pMainDevice;
	}

	static void MainReset();

	static ID3D11DeviceContext* MainContext()
	{
		return m_pMainContext;
	}

//////////////// MEMBER
private:
	Window* m_TargetWindow;

	UINT m_MQ; // 멀티샘플링 레벨 얻어오는거
	UINT m_MC; // 멀티샘플링 레벨 얻어오는거
	// 그래픽카드를 다이렉트로 프로그래밍하는 것에 대한 
	// 모든것은 아래의 두가지 포인터를 통해서 한다.
	ID3D11Device* m_pDevice; // 그래픽카드의 모든 자원에 대한 권한을 담당. (o)
	ID3D11DeviceContext* m_pContext; // 그래픽카드의 모든 연산에 대한 권한을 담당한다.(랜더) (o)
	// 아직 못어더옴
	// 그 기술을 그냥 객체적으로 지원한다.
	IDXGISwapChain* m_pSwapChain; // 깊이버퍼 랜더링파이프라인의 단계에 관여한다.

	// ID3D11RenderTargetView* m_pBackBufferView[3]; // 깊이버퍼 랜더링파이프라인의 단계에 관여한다.

	ID3D11RenderTargetView* m_pBackBufferView; // 깊이버퍼 랜더링파이프라인의 단계에 관여한다.
	ID3D11DepthStencilView* m_pDepthStencilView; // 깊이버퍼 랜더링파이프라인의 단계에 관여한다.
	ID3D11Texture2D* m_pDepthStencilTex; // 깊이버퍼 랜더링파이프라인의 단계에 관여한다.
	
	

	Vector4 m_BackColor;
	D3D11_VIEWPORT m_ViewPortInfo; // 뷰포트행렬용 정보.

	
	bool m_BaseInit;
	void BaseResInit();

public:
	ID3D11Device* GetDevice()
	{
		return m_pDevice;
	}

	ID3D11DeviceContext* GetContext()
	{
		return m_pContext;
	}

	ID3D11DepthStencilView* GetDSV()
	{
		return m_pDepthStencilView;
	}

public:
	void Init(Window* _TargetWindow);

	void ClearTarget();
	void Present();

private:
	// 다이렉트 x에서도 우리가 직접 만들필요가 없는 화면 갱신 시스템을 지원해준다.
	bool CreateSwapChain();
	bool CreateDepthStencilView();
	bool CreateViewPort();

public:
	void DefRenderTargetSetting();
	void DefViewPortSetting();

public:
	GraphicDevice();
	~GraphicDevice();
};

