// HRIGHTMENUDLG.cpp: 구현 파일
//

#include "stdafx.h"
#include "HTOOL.h"
#include "HRIGHTMENUDLG.h"
#include "afxdialogex.h"
#include "HMAPDLG.h"
#include "HMONSTERDLG.h"
#include "HMapUpdater.h"
#include "ToolHeader.h"


// HRIGHTMENUDLG 대화 상자

IMPLEMENT_DYNAMIC(HRIGHTMENUDLG, CDialogEx)

HRIGHTMENUDLG::HRIGHTMENUDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RIGHTMENUDLG, pParent)
{

}

HRIGHTMENUDLG::~HRIGHTMENUDLG()
{
	for (int i = 0; i < m_DlgVec.size(); i++)
	{
		delete m_DlgVec[i];
	}
}

void HRIGHTMENUDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MENUTAB, m_Tab);
}


BEGIN_MESSAGE_MAP(HRIGHTMENUDLG, CDialogEx)
	ON_WM_CREATE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MENUTAB, &HRIGHTMENUDLG::OnTcnSelchangeMenutab)
END_MESSAGE_MAP()




int HRIGHTMENUDLG::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;


	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}
//
//void HRIGHTMENUDLG::CreateDlg(const wchar_t* _DlgAndSceneName, int _DlgID, HSCENE::HSCENECOM* _Updater)
//{
//	m_Tab.InsertItem(m_DlgVec.size(), _DlgAndSceneName);
//	HMAPDLG* m_MenuDlg = new HMAPDLG();
//	m_MenuDlg->Create(_DlgID, &m_Tab);
//	m_MenuDlg->ShowWindow(SW_HIDE);
//	m_MenuDlg->SetWindowPos(nullptr, 4, 20, 450, 500, 0);
//	m_DlgVec.push_back(m_MenuDlg);
//	HGAMEWIN::MainGameWin()->CreateScene(_DlgAndSceneName, _Updater);
//}

BOOL HRIGHTMENUDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CreateDlg<HMAPDLG>(L"맵", IDD_MAPDLG, new HMapUpdater());
	CreateDlg<HMONSTERDLG>(L"몬스터", IDD_MONSTERDLG);

	GameWindow::MainGameWin()->ChangeScene(L"맵");
	m_DlgVec[0]->ShowWindow(SW_SHOW);




	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void HRIGHTMENUDLG::OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(TRUE);

	// 현선택된 탭를 말하는 것이고
	int SelectIndex = m_Tab.GetCurSel();

	if (-1 == SelectIndex )
	{
		return;
	}

	GameWindow::MainGameWin()->ChangeScene(m_DlgName[SelectIndex].c_str());
	m_DlgVec[SelectIndex]->ShowWindow(SW_SHOW);
}
