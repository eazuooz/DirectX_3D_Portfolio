// HRightView.cpp: 구현 파일
//

#include "stdafx.h"
#include "HTOOL.h"
#include "HRightView.h"
#include "HRIGHTMENUDLG.h"


// HRightView

IMPLEMENT_DYNCREATE(HRightView, CFormView)

HRightView::HRightView()
	: CFormView(IDD_RIGHTVIEWDLG)
{
	m_hWnd;
	// 여기 hwnd가 제대로 만들어지지 않은 순간.
}

HRightView::~HRightView()
{
	if (nullptr != m_MenuDlg)
	{
		delete m_MenuDlg;
		m_MenuDlg = nullptr;
	}
}

BEGIN_MESSAGE_MAP(HRightView, CFormView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// HRightView 그리기

void HRightView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// HRightView 진단

#ifdef _DEBUG
void HRightView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void HRightView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// HRightView 메시지 처리기

int HRightView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_MenuDlg = new HRIGHTMENUDLG();
	m_MenuDlg->Create(IDD_RIGHTMENUDLG, this);
	m_MenuDlg->ShowWindow(SW_SHOW);

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}
