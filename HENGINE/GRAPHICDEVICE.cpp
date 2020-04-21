#include "GRAPHICDEVICE.h"
#include <JWINDOW.h>

// mgr static

GraphicDevice* GraphicDevice::m_MainDevice = nullptr;
ID3D11Device* GraphicDevice::m_pMainDevice = nullptr; // 그래픽카드의 모든 자원에 대한 권한을 담당. (o)
ID3D11DeviceContext* GraphicDevice::m_pMainContext = nullptr; // 그래픽카드의 모든 연산에 대한 권한을 담당한다.(랜더) (o)
bool GraphicDevice::m_PathInit = false;

///////////////////////////// member

void GraphicDevice::MainReset() 
{
	ID3D11RenderTargetView* ArrView[16] = { nullptr };

	m_pMainContext->VSSetShader(nullptr, nullptr, 0);
	m_pMainContext->HSSetShader(nullptr, nullptr, 0);
	m_pMainContext->GSSetShader(nullptr, nullptr, 0);
	m_pMainContext->CSSetShader(nullptr, nullptr, 0);
	m_pMainContext->DSSetShader(nullptr, nullptr, 0);
	m_pMainContext->PSSetShader(nullptr, nullptr, 0);
	m_pMainContext->OMSetDepthStencilState(nullptr, 0);

	ID3D11ShaderResourceView* RTV = nullptr;

	for (UINT i = 0; i < 16; i++)
	{
		GraphicDevice::MainContext()->VSSetShaderResources(i, 1, &RTV);
		GraphicDevice::MainContext()->HSSetShaderResources(i, 1, &RTV);
		GraphicDevice::MainContext()->DSSetShaderResources(i, 1, &RTV);
		GraphicDevice::MainContext()->GSSetShaderResources(i, 1, &RTV);
		GraphicDevice::MainContext()->PSSetShaderResources(i, 1, &RTV);
	}


	m_pMainContext->OMSetRenderTargets(8, &ArrView[0], nullptr);
}

GraphicDevice::GraphicDevice() : m_pDevice(nullptr), m_pContext(nullptr), m_pSwapChain(nullptr), m_pDepthStencilView(nullptr), m_pDepthStencilTex(nullptr), m_pBackBufferView(nullptr), m_BaseInit(false)
{
}


GraphicDevice::~GraphicDevice()
{
	m_TargetWindow = nullptr;
	
	
	if (nullptr != m_pDepthStencilTex) { m_pDepthStencilTex->Release(); m_pDepthStencilTex = nullptr;};
	if (nullptr != m_pDepthStencilView) { m_pDepthStencilView->Release(); m_pDepthStencilView = nullptr; };
	if (nullptr != m_pBackBufferView) { m_pBackBufferView->Release(); m_pBackBufferView = nullptr; };
	if (nullptr != m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = nullptr;};
	if (nullptr != m_pContext) { m_pContext->Release(); m_pContext = nullptr;};
	if (nullptr != m_pDevice) { m_pDevice->Release(); m_pDevice = nullptr;};
}


void GraphicDevice::Init(Window* _TargetWindow)
{
	// 디바이스를 만들려고 하는데.
	int iFlag = 0;
#ifdef _DEBUG
	// 디바이스의 모드가 디버그 모드인지 릴리즈 모드인지 세팅해줘야 한다.
	// 그리고 디버그모드는 느리다.
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif 
	m_TargetWindow = _TargetWindow;

	D3D_FEATURE_LEVEL eLv = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	m_BackColor = { 4.0f / 255.0f, 104.0f / 255.0f, 216.0f / 255.0f, 1.0f };

	//m_BackColor = { 129.0f / 255.0f,193.0f / 255.0f, 71.0f / 255.0f,1.0f};
	//m_BackColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	// 
	// 
	HRESULT Check;

	Check = D3D11CreateDevice
	(
		nullptr,  // 1 디바이스 만들때 부가정보가 있나요? nullptr 
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, // 2 그래픽카드가 어떤 기반에 의한 것인가요? -> 그냥 하드웨어 
		nullptr, // 몰라
		iFlag, // 디버그냐 릴리즈냐
		nullptr, // 몰라
		0, // 몰라
		D3D11_SDK_VERSION, // 현재 윈도우에 설치된 SDK버전이 몇이냐?
		&m_pDevice, // 디바이스 얻어오기
		&eLv, // 몰라
		&m_pContext // 컨텍스트 얻어온다.
	);

	if (S_OK != Check) { BOOM; return; }

	// 이걸 색깔을 기반으로 처리하는 것이기 때문에 가장 기준이 되는 색상을
	// DXGI_FORMAT_R8G8B8A8_UNORM

	// 4에
	m_MC = 4;
	m_MQ = 0;

	D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;
	Check = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, m_MC, &m_MQ);
	if (S_OK != Check) 
	{ 
		m_MC = 1;
		m_MQ = 0;
		Check = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, m_MC, &m_MQ);
		return; 
	}

	m_MC = 1;
	m_MQ = 0;

	// m_ML = 4;

	// 스왑체인을 만든다.
	// 내일 나머지 
	if (false == CreateSwapChain())
	{
		BOOM;
	}

	if (false == CreateDepthStencilView())
	{
		BOOM;
	}

	if (false == CreateViewPort())
	{
		BOOM;
	}

	if (nullptr == m_MainDevice)
	{
		m_MainDevice = this;
		m_pMainDevice = this->GetDevice();
		m_pMainContext = this->GetContext();
	}

	BaseResInit();
}

bool GraphicDevice::CreateSwapChain() 
{
	// 이녀석이 nullptr이 아니게 만드는 것.
	// m_pSwapChain

	// DESC단어 나오면 디스크립션 정보
	DXGI_SWAP_CHAIN_DESC SWINFO;

	SWINFO.BufferDesc.Width = m_TargetWindow->Size().UIX();
	SWINFO.BufferDesc.Height = m_TargetWindow->Size().UIY();

	// 모니터의 최대해상도도 얻어올수 있다.
	// 모니터의 헤르츠를 얻어올수 있을 것이다.

	// 분자 분모
	SWINFO.BufferDesc.RefreshRate.Numerator = 60;
	// 1 / 60의 헤르츠 표시.
	SWINFO.BufferDesc.RefreshRate.Denominator = 1;

	SWINFO.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SWINFO.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SWINFO.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 백버퍼의 용도
	// 화면에 출력하는 용도로 쓰겠다. 
	SWINFO.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 백버퍼 개수
	// 멀티셈플링 퀄리티 카운트
	// 0 알아서 
	// SWINFO.SampleDesc.Count = m_ML;
	// MSAA 
	
	SWINFO.SampleDesc.Count = m_MC;
	SWINFO.SampleDesc.Quality = m_MQ;

	// 더블버퍼링과 비슷한데.
	// 2개의 이미지로 
	SWINFO.OutputWindow = m_TargetWindow->WH();
	// 버퍼개수 1 ~ 
	SWINFO.BufferCount = 2;


	// 0 -> 화면에 나오고 있다.
	// 1 -> 그려지고 있다. -> 다 그려졌다.
	// 기존 정보를 다 지운다.
	// 2개의 그림을 그리기 시작할때 지우고 새로그린다. -> 다 그려지면 -> 기존걸 
	SWINFO.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	// DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD

	// 풀스크린 출력인가요?????
	SWINFO.Windowed = !m_TargetWindow->IsFull();
	SWINFO.Flags = 0;
	DXGI_SWAP_CHAIN_FLAG_HW_PROTECTED;
    // 정보만 세팅된거고
	// 이제 만드는 것을 해야한다.
	// 그런데 다이렉트는 스왑체인을 만들기 위해서 좀 복잡한 과정을 거ㅕ야 한다.

	// 다이렉트 11부터는 난이도가 높아졌다는 이유가
	// com객체방식이라는 것을 사용해서 처리하게 만들어졌다.
	// 이건 COM객체 방식이라는 것은
	// m_pDevice->createsw
	// 이녀석들은 단순히 힙에 할당하고 관리되는 녀석이 아니라는 이야기.
	// 마소에서 만든 메모리 관리 방식에 의해서 관리되고
	// 마소에서는 절대로 HWND
	// 그래픽카드에 뭔가를 요청하거나 메모리를 할당하는 방식에 대한 인터페이스는
	// new Texture();
	// 이녀석들을 통해서만 그래픽카드에 요청해라.
	// ID3D11Device* m_pDevice

	IDXGIDevice* pIdxDevice = nullptr;
	IDXGIAdapter* pIdxAdapter = nullptr;
	IDXGIFactory* pIdxFactory = nullptr;
	// 이걸 통하지 않고서는 
	// 못만들게 해놨다.
	// 그래픽 카드와의 통로
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pIdxDevice);
	if (nullptr == pIdxDevice)
	{
		BOOM;
		return false;
	}
	 
	// GUID를 통해서 얻어오게 만들어 놨다.
	pIdxDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pIdxAdapter);
	if (nullptr == pIdxAdapter)
	{
		BOOM;
		return false;
	}

	pIdxAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pIdxFactory);
	if (nullptr == pIdxFactory)
	{
		BOOM;
		return false;
	}

	if (S_OK != pIdxFactory->CreateSwapChain(m_pDevice, &SWINFO, &m_pSwapChain))
	{
		BOOM;
		return false;
	}
	

	pIdxFactory->Release();
	pIdxAdapter->Release();
	pIdxDevice->Release();

	return true;

}

bool GraphicDevice::CreateDepthStencilView() 
{
	ID3D11Texture2D* pBackBufferTex = nullptr;

	// 자동으로 텍스처 하나가 만들어져있기 때문에 이걸 얻어올수 있다.
	if (S_OK != m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTex))
	{
		BOOM;
		return false;
	}

	// 다이렉트의 View라고 표현된 객체들은
	// 텍스처에 대한 정보와 처리권한을 사용할수 있는 인터페이스이다.
	// 벡버퍼의 깊이에 관여하거나 명령내릴수 있는 권한 객체를 만들어줘
	// 나중에 백버퍼에 어떠한 명령을 내릴때 이녀석을 통해서 내려줄께.
	if (S_OK != m_pDevice->CreateRenderTargetView(pBackBufferTex, 0, &m_pBackBufferView))
	{
		BOOM;
		return false;
	}

	if (nullptr != pBackBufferTex)
	{
		pBackBufferTex->Release();
		pBackBufferTex = nullptr;
	}

	D3D11_TEXTURE2D_DESC tDesc = {};

	tDesc.ArraySize = 1;
	tDesc.Width = m_TargetWindow->Size().UIX();
	tDesc.Height = m_TargetWindow->Size().UIY();

	// 거울 만들거나 
	// 눈만 불타게 하고 싶다거나
	// 화면의 모든 픽셀중 스텐실 값이 4인 녀석만 뭘할께요.
	// 3바이트만 깊이값으로 사용하고 뒤에 8비튼는 0~255는 스텐실 값으로 쓴다.
	//             R8G8B8A8
	tDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tDesc.SampleDesc.Count = m_MC;
	tDesc.SampleDesc.Quality = m_MQ;
	tDesc.MipLevels = 1; // 꼭 설명해야 한다.
	// 메모리 관리 옵션
	// D3D11_USAGE_DEFAULT면 그래픽카드에서 관리해라.
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	// 텍스처의 용도가 뭐냐?
	tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_pDevice->CreateTexture2D(&tDesc, 0, &m_pDepthStencilTex);

	if (nullptr == m_pDepthStencilTex)
	{
		BOOM;
		return false;
	}

	m_pDevice->CreateDepthStencilView(m_pDepthStencilTex, 0, &m_pDepthStencilView);

	if (nullptr == m_pDepthStencilView)
	{
		BOOM;
		return false;
	}

	// D3D11_DEPTH_STENCIL_DESC TD;
	// m_pContext->OMSetDepthStencilState(TD, 1);

	return true;
}

bool GraphicDevice::CreateViewPort() 
{
	Template::MemZero(m_ViewPortInfo);

	m_ViewPortInfo.TopLeftX = 0;
	m_ViewPortInfo.TopLeftY = 0;

	m_ViewPortInfo.Width = m_TargetWindow->Size().x;
	m_ViewPortInfo.Height = m_TargetWindow->Size().y;
	m_ViewPortInfo.MinDepth = 0.0f;
	m_ViewPortInfo.MaxDepth = 1.0f;


	return true;
}

// 랜더시작하기 직전에 놓는다.
void GraphicDevice::ClearTarget() 
{
	if (nullptr != m_pContext)
	{
		m_pContext->ClearRenderTargetView(m_pBackBufferView, m_BackColor.Arr);
		m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

// 여태까지 다 그린걸 화면에 출력해라.
void GraphicDevice::Present() 
{
	if (nullptr != m_pSwapChain)
	{
		m_pSwapChain->Present(0, 0);
	}
	
}

void GraphicDevice::DefRenderTargetSetting()
{
	if (nullptr != m_pContext && nullptr != m_pDepthStencilView)
	{
		// 깊이 버퍼는 그린다 자체만 놓고보면 없어도 상관이 없다.
		// 내가 이 랜더타겟과 이 깊이버퍼를 사용해서
		// 그려지는 놈들의 색깔과 정보를 세팅하겠습니다.
		m_pContext->OMSetRenderTargets(1, &m_pBackBufferView, m_pDepthStencilView);
	}
}

void GraphicDevice::DefViewPortSetting() 
{
	if (nullptr != m_pContext)
	{
		m_pContext->RSSetViewports(1, &m_ViewPortInfo);
	}
}