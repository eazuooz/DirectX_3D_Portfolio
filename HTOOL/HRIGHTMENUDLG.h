#pragma once
#include <SCENE.h>

// HRIGHTMENUDLG 대화 상자

class HRIGHTMENUDLG : public CDialogEx
{
	DECLARE_DYNAMIC(HRIGHTMENUDLG)

public:
	HRIGHTMENUDLG(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~HRIGHTMENUDLG();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RIGHTMENUDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	std::vector<CDialogEx*> m_DlgVec;
	std::vector<std::wstring> m_DlgName;

public:
	template<typename HDLG, typename ... Rest>
	void CreateDlg(const wchar_t* _DlgAndSceneName, int _DlgID, Rest ..._Arg)
	{
		m_Tab.InsertItem((int)m_DlgVec.size(), _DlgAndSceneName);
		HDLG* m_MenuDlg = new HDLG();
		m_MenuDlg->Create(_DlgID, &m_Tab);
		m_MenuDlg->ShowWindow(SW_HIDE);
		m_MenuDlg->SetWindowPos(nullptr, 4, 20, 450, 500, 0);
		m_DlgVec.push_back(m_MenuDlg);
		m_DlgName.push_back(_DlgAndSceneName);
		GameWindow::MainGameWin()->CreateScene(_DlgAndSceneName, _Arg...);
	}


public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult);
};
