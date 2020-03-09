// HLeftView.cpp: 구현 파일
//

#include "stdafx.h"
#include "HTOOL.h"
#include "HLeftView.h"
#include <GAMEWIN.h>
#include "HMapUpdater.h"


// HLeftView

IMPLEMENT_DYNCREATE(HLeftView, CFormView)

HLeftView::HLeftView()
	: CFormView(IDD_LEFTVIEWDLG)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

HLeftView::~HLeftView()
{
}

BEGIN_MESSAGE_MAP(HLeftView, CFormView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// HLeftView 그리기

void HLeftView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// HLeftView 진단

#ifdef _DEBUG
void HLeftView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void HLeftView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// HLeftView 메시지 처리기


int HLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (nullptr == m_hWnd)
	{
		BOOM;
	}

	HPTR<GameWindow> WIN = GameWindow::CreateGameWin(L"MainView", m_hWnd);
	WIN->Size(TOOLWINX, TOOLWINY);
	WIN->CreateDevice();
	// WIN->Size(TOOLWINX, TOOLWINY);

	// WIN->CreateDevice();

	//HGAMEWIN::MainGameWin()->CreateScene(L"ToolScene", new HToolUpdater());
	//HGAMEWIN::MainGameWin()->ChangeScene(L"ToolScene");


	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}
