#include "Video.h"
#include "Debug.h"


Video::Video() :pGraph(nullptr), pControl(nullptr), pEvent(nullptr), pWindow(nullptr)
{
	Init();
}


Video::~Video()
{
	pControl->Release();
	pEvent->Release();
	pGraph->Release();
	pWindow->Release();
}

void Video::Init()
{
	_wsetlocale(LC_ALL, L"korean");

	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		BOOM;
	}

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_SERVER, IID_IGraphBuilder, (void**)&pGraph);
	if (FAILED(hr))
	{
		BOOM;
	}

	hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
	if (FAILED(hr))
	{
		BOOM;
	}

	hr = pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);
	if (FAILED(hr))
	{
		BOOM;
	}

	hr = pGraph->QueryInterface(IID_IVideoWindow, (void**)&pWindow);
	if (FAILED(hr))
	{
		BOOM;
	}
}

void Video::SetVideoFileDirectory(const wchar_t* _Dir)
{
	m_Dir = _Dir;
	
	//HRESULT hr = pGraph->RenderFile(m_Dir.c_str(), NULL);

	//if (hr != S_OK)
	//{
	//	BOOM;
	//}
}

void Video::PlayVideoFile()
{
	HRESULT hr = pGraph->RenderFile(m_Dir.c_str() , NULL);
	
	if (SUCCEEDED(hr))
	{
		pWindow->put_Left(0);
		pWindow->put_Top(0);
	
		pWindow->put_FullScreenMode(OATRUE);
	
		std::wcout << L"플레이 파일 : " << m_Dir.c_str() << std::endl;
		hr = pControl->Run();
		if (SUCCEEDED(hr))
		{
			long ev;
			pEvent->WaitForCompletion(INFINITE, &ev);
		}	
	}
	else
	{
		BOOM; // 파일 오픈 에러
	}
}