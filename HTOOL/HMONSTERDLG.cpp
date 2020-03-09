// HMONSTERDLG.cpp: 구현 파일
//

#include "stdafx.h"
#include "HTOOL.h"
#include "HMONSTERDLG.h"
#include "afxdialogex.h"


// HMONSTERDLG 대화 상자

IMPLEMENT_DYNAMIC(HMONSTERDLG, CDialogEx)

HMONSTERDLG::HMONSTERDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MONSTERDLG, pParent)
{

}

HMONSTERDLG::~HMONSTERDLG()
{
}

void HMONSTERDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(HMONSTERDLG, CDialogEx)
END_MESSAGE_MAP()


// HMONSTERDLG 메시지 처리기
