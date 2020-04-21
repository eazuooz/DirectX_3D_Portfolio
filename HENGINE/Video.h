#pragma once
#include <dshow.h>
#include <iostream>
#include <locale>
#pragma comment(lib, "strmiids.lib")

class Video
{
private:
	IGraphBuilder	*pGraph;
	IMediaControl	*pControl;
	IMediaEvent		*pEvent;
	IVideoWindow	*pWindow;

	std::wstring m_Dir;

private:
	void Init();

public:
	void SetVideoFileDirectory(const wchar_t* _Dir);
	void PlayVideoFile();

public:
	Video();
	~Video();
};

//wchar_t str[100] = L"..\\..\\Resource\\LoadingMovie.avi";
//
//hr = pGraph->RenderFile(str, NULL);
//
//if (SUCCEEDED(hr))
//{
//	pWindow->put_Left(0);
//	pWindow->put_Top(0);
//
//	pWindow->put_FullScreenMode(OATRUE);
//
//	std::wcout << L"플레이 파일 : " << str << std::endl;
//	hr = pControl->Run();
//	if (SUCCEEDED(hr))
//	{
//		long ev;
//		pEvent->WaitForCompletion(INFINITE, &ev);
//	}
//
//}
//else
//{
//	std::wcout << L"파일 오픈 에러 " << std::endl;
//}
//
//IMediaSeeking *pSeeking = nullptr;
//
//
//pControl->Release();
//pEvent->Release();
//pGraph->Release();
//pWindow->Release();