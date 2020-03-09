#pragma once


// HMONSTERDLG 대화 상자

class HMONSTERDLG : public CDialogEx
{
	DECLARE_DYNAMIC(HMONSTERDLG)

public:
	HMONSTERDLG(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~HMONSTERDLG();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MONSTERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
