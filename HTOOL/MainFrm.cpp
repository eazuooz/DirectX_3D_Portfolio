
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "HTOOL.h"

#include "MainFrm.h"
#include "HLeftView.h"
#include "HRightView.h"
#include <LOGICHEADER.h>
#include <GAMETHREAD.h>
//#include <HRANDOM.h>
#include "HMapUpdater.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetMenu(nullptr);

	// 크기만큼
	RECT RC = { 0, 0, TOOLWINX + 500, TOOLWINY - 16 };
	AdjustWindowRect(&RC, WS_OVERLAPPEDWINDOW, true);
	SetWindowPos(nullptr, 0, 0, RC.right - RC.left, RC.bottom - RC.top, 0);

	// 몇개 나눌지 설정할수 있다.
	//                     열 행
	// this를 넣어주면 this내부에 있는 hwnd를 통해서 관계가 형성된다.
	m_Sp.CreateStatic(this, 1, 2);
	// 원하는 대로 

	// 여기서 디바이스가 이닛된다.
	m_Sp.CreateView(0, 0, RUNTIME_CLASS(HLeftView), SIZE{ TOOLWINX, TOOLWINY - 16 }, nullptr);

	// 여기서 씬이 만들어 진다.
	m_Sp.CreateView(0, 1, RUNTIME_CLASS(HRightView), SIZE{ 500, TOOLWINY - 16 }, nullptr);

 	// HPTR<HGAMEWIN> WIN = HGAMEWIN::FindGameWin(L"MainView");



	LOGICVALUE::Init();

	//HGAMEWIN::MainGameWin()->CreateScene(L"ToolScene", new HToolUpdater());
	//HGAMEWIN::MainGameWin()->ChangeScene(L"ToolScene");

	// m_Sp.ShowWindow(SW_SHOW);

	// frameWnd는 내부에 여러개의 뷰를 관리할수 있게 만들어진
	// 클래스이다.
	// 
	// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	//if (!m_wndView.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW,
	//	CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
	//{
	//	TRACE0("뷰 창을 만들지 못했습니다.\n");
	//	return -1;
	//}
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 뷰 창으로 포커스를 이동합니다.
	// m_Sp.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 뷰에서 첫째 크랙이 해당 명령에 나타나도록 합니다.
	//if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;

	// WinProc

	// 그렇지 않으면 기본 처리합니다.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



BOOL CMainFrame::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	Thread::AllThreadReset();
	return CFrameWnd::DestroyWindow();
}
