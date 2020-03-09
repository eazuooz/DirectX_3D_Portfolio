#pragma once


// HMAPDLG 대화 상자

class HMAPDLG : public CDialogEx
{
	DECLARE_DYNAMIC(HMAPDLG)

public:
	HMAPDLG(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~HMAPDLG();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	//CSliderCtrl m_FloorSl;
	//CSliderCtrl m_LeftSpriteIdx;
	//CSliderCtrl m_RightSpriteIdx;

public:

	virtual BOOL OnInitDialog();
	//afx_msg void OnNMCustomdrawFloor(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMCustomdrawLeftfloorindex(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMCustomdrawRightfloorindex(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl Frame;
	CListBox BoneList;
	afx_msg void OnLbnSelchangeList1();
};
