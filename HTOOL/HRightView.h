#pragma once

// HRightView 보기

class HRIGHTMENUDLG;
class HRightView : public CFormView
{
	DECLARE_DYNCREATE(HRightView)

protected:
	HRightView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~HRightView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	HRIGHTMENUDLG* m_MenuDlg;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


