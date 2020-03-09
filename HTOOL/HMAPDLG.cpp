// HMAPDLG.cpp: 구현 파일
//

#include "stdafx.h"
#include "HTOOL.h"
#include "HMAPDLG.h"
#include "HTOOLVALUE.h"
#include "afxdialogex.h"
#include <LOGICHEADER.h>
#include <GAMETHREAD.h>
#include <ACTOR.h>
#include <GRIDRENDER.h>
#include <FBX.h>
#include <3DANIRENDER.h>
#include "HMapUpdater.h"
#include <FREECAM.h>
#include <TestPlayer.h>
#include <BoneChaser.h>

// HMAPDLG 대화 상자

IMPLEMENT_DYNAMIC(HMAPDLG, CDialogEx)

HMAPDLG::HMAPDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAPDLG, pParent)
{

}

HMAPDLG::~HMAPDLG()
{
}

void HMAPDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// DDX_Control(pDX, IDC_FLOOR, m_FloorSl);
	// DDX_Control(pDX, IDC_LEFTFLOORINDEX, m_LeftSpriteIdx);
	// DDX_Control(pDX, IDC_RIGHTFLOORINDEX, m_RightSpriteIdx);
	DDX_Control(pDX, IDC_SLIDER1, Frame);
	DDX_Control(pDX, IDC_LIST1, BoneList);
}


BEGIN_MESSAGE_MAP(HMAPDLG, CDialogEx)
	// ON_NOTIFY(NM_CUSTOMDRAW, IDC_FLOOR, &HMAPDLG::OnNMCustomdrawFloor)
	// ON_NOTIFY(NM_CUSTOMDRAW, IDC_LEFTFLOORINDEX, &HMAPDLG::OnNMCustomdrawLeftfloorindex)
	// ON_NOTIFY(NM_CUSTOMDRAW, IDC_RIGHTFLOORINDEX, &HMAPDLG::OnNMCustomdrawRightfloorindex)
	ON_BN_CLICKED(IDC_BUTTON1, &HMAPDLG::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &HMAPDLG::OnBnClickedButton2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &HMAPDLG::OnNMCustomdrawSlider1)
	ON_LBN_SELCHANGE(IDC_LIST1, &HMAPDLG::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// HMAPDLG 메시지 처리기


BOOL HMAPDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 화면 다시 그릴거냐.

	//m_LeftSpriteIdx.SetRangeMin(0, TRUE);
	//m_LeftSpriteIdx.SetRangeMax(64, TRUE);

	//m_RightSpriteIdx.SetRangeMin(0, TRUE);
	//m_RightSpriteIdx.SetRangeMax(64, TRUE);
	

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//
//void HMAPDLG::OnNMCustomdrawFloor(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	*pResult = 0;
//
//	UpdateData(TRUE);
//
//	HTOOLVALUE::MAPFLOOR = m_FloorSl.GetPos();
//}
//
//
//void HMAPDLG::OnNMCustomdrawLeftfloorindex(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	*pResult = 0;
//
//	HTOOLVALUE::LEFTSPRITEFLOOR = m_LeftSpriteIdx.GetPos();
//}
//
//
//void HMAPDLG::OnNMCustomdrawRightfloorindex(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	*pResult = 0;
//
//	HTOOLVALUE::RIGHTSPRITEFLOOR = m_RightSpriteIdx.GetPos();
//}

#define MAX_CFileDialog_FILE_COUNT 99
#define FILE_LIST_BUFFER_SIZE ((MAX_CFileDialog_FILE_COUNT * (MAX_PATH + 1)) + 1)



CriticalSection CSFBX;


//class FbxPath
//{
//public:
//	wchar_t LoadPath[256];
//	wchar_t SavePath[256];
//
//	FbxPath() : LoadPath{ 0, }, SavePath{ 0, }
//	{
//
//	}
//};

unsigned __stdcall FbxLoadThread(Thread* _Thread, void* _Arg)
{
	Fbx New;

	FbxPath Path;

	memcpy_s(&Path, sizeof(FbxPath), _Arg, sizeof(FbxPath));

	New.FileSetting(Path.LoadPath);

	New.Load(Fbx::LOADMODE::FBX);

	std::wstring Log = Path.LoadPath;

	New.ConvertToSave(Path.SavePath);


	Debug::ConsolePrintLine((Log + L"LOADING END").c_str());

	return 0;
}

unsigned __stdcall ConvertLoadThread(Thread* _Thread, void* _Arg)
{
	Fbx New;

	FbxPath Path;
	memcpy_s(&Path, sizeof(FbxPath), _Arg, sizeof(FbxPath));
	New.FileSetting(Path.LoadPath);
	New.FbxToEngineRes();
	std::wstring Log = Path.LoadPath;

	Debug::ConsolePrintLine((Log + L"LOADING END").c_str());

	return 0;
}



void HMAPDLG::OnBnClickedButton1()
{
	CString InitPath;
	CFileDialog dlgFileOpen(TRUE);
	OPENFILENAME& ofnOpen = dlgFileOpen.GetOFN();
	InitPath = Path::FindPathToDir(L"Mesh").GetFullPathString().c_str();
	ofnOpen.lpstrInitialDir = InitPath.GetBuffer();

	// ofn.Flags |= OFN_ALLOWMULTISELECT;
	// ofn.nMaxFile = FILE_LIST_BUFFER_SIZE;

	FbxPath Path;

	std::wstring DicName;
	std::wstring FileName;
	std::wstring AllFileName;
	if (IDOK == dlgFileOpen.DoModal())
	{
		DicName = dlgFileOpen.GetFolderPath().GetString();
		FileName = dlgFileOpen.GetFileName().GetString();
		AllFileName = DicName + L"\\" + FileName;

		memcpy_s(Path.LoadPath, sizeof(wchar_t) * 256,  AllFileName.c_str(), AllFileName.size() * 2 + 2);
	}
	else {
		return;
	}

	InitPath.ReleaseBuffer();


	CString SaveFileName = FileName.c_str();
	CFileDialog dlgFileSave(FALSE);
	OPENFILENAME& ofnSave = dlgFileSave.GetOFN();
	InitPath = Path::FindPathToDir(L"Mesh").GetFullPathString().c_str();
	ofnSave.lpstrDefExt = L"FBXDATA";
	ofnSave.lpstrInitialDir = InitPath.GetBuffer();


	if (IDOK == dlgFileSave.DoModal())
	{
		DicName = dlgFileSave.GetFolderPath().GetString();
		FileName = dlgFileSave.GetFileName().GetString();


		CString Str = FileName.c_str();

		if (-1 == Str.Find(L".FBXDATA"))
		{
			int Count = Str.ReverseFind(L'.');
			Str.ReleaseBufferSetLength(Count);
			Str += L".FBXDATA";
		}

		AllFileName = DicName + L"\\" + Str.GetString();

		memcpy_s(Path.SavePath, sizeof(wchar_t) * 256, AllFileName.c_str(), AllFileName.size() * 2 + 2);
	}
	else {
		return;
	}


	InitPath.ReleaseBuffer();


	Thread::CreateThread(L"ConsoleThread", FbxLoadThread, Path);




	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void HMAPDLG::OnBnClickedButton2()
{
	CString InitPath;
	CFileDialog dlgFileOpen(TRUE);
	OPENFILENAME& ofnOpen = dlgFileOpen.GetOFN();
	InitPath = Path::FindPathToDir(L"Mesh").GetFullPathString().c_str();
	ofnOpen.lpstrInitialDir = InitPath.GetBuffer();

	// ofn.Flags |= OFN_ALLOWMULTISELECT;
	// ofn.nMaxFile = FILE_LIST_BUFFER_SIZE;

	FbxPath Path;

	std::wstring DicName;
	std::wstring FileName;
	std::wstring AllFileName;
	if (IDOK == dlgFileOpen.DoModal())
	{
		DicName = dlgFileOpen.GetFolderPath().GetString();
		FileName = dlgFileOpen.GetFileName().GetString();
		AllFileName = DicName + L"\\" + FileName;

		memcpy_s(Path.LoadPath, sizeof(wchar_t) * 256, AllFileName.c_str(), AllFileName.size() * 2 + 2);
	}
	else 
	{
		return;
	}

	// 이함수 내부에서 동기화를 걸어야 하는데.
	// 귀찮다.
	HPTR<Fbx> Mesh = Resources::Load<Fbx>(Path.LoadPath, Fbx::LOADMODE::CONVERT);

	{
		BoneList.ResetContent();
		for (size_t i = 0; i < Mesh->BoneData().size(); i++)
		{
			BoneList.AddString(Mesh->BoneData()[i]->Name.c_str());
		}



		HPTR<Actor> PLAYER = GameWindow::MainGameWin()->GetCurScene()->CreateActor();
		PLAYER->GetTransform()->SetLoclaPosition({ 0.0f, 0.0f, 0.0F });
		PLAYER->GetTransform()->SetLocalScale({ 1.0F, 1.0F, 1.0F });


		HPTR<TestPlayer> PlayerCom = PLAYER->AddComponent<TestPlayer>();

		PlayerCom->m_Map = HMapUpdater::MAPCOM;
		//HPTR<H3DANIRENDER> SR = PLAYER->AddCom<H3DANIRENDER>(RG_PLAYER);
		//SR->FbxSetting(Mesh->Name());
		
		
	/*	HPTR<Actor> MESH = GameWindow::MainGameWin()->GetCurScene()->CreateActor();
		MESH->SetParent(PLAYER);*/
		//MESH->GetTransform()->SetLocalRot(Vector4(0.0f, -90.0f, 0.0f, 0.0f));



		HPTR<FbxAnimation> SR = PLAYER->AddComponent<FbxAnimation>(RG_PLAYER);

		
		SR->FbxSetting(Mesh->GetName());

		SR->CreateAnimation(L"ATT", 0, 920, true);
		//SR->CreateAnimation(L"ATT", 886, 909, true);
		SR->ChangeAnimation(L"ATT");

		HMapUpdater::ANIREN = SR;
		if (0 != Mesh->AniData().size())
		{
			Frame.SetRangeMin(0, TRUE);
			Frame.SetRangeMax((int)Mesh->AniData()[0]->TimeEndCount - 1, TRUE);
		}
		HMapUpdater::FREECAMCOM->Trace(PLAYER->GetTransform(), Vector4(0.0f, 200.0f, -200.0f, 1.0f), Vector4(25.0f, 0.0f, 0.0f, 0.0f));


		//HPTR<BoneChaser> BC = HMapUpdater::PLAYER->GetComponent<BoneChaser>();
		//BC->Target(SR);
		// SR->RenderData(L"SPHEREMESH", L"LIGHTMESHDPATH");
		// SR->RenderData(L"BOXMESH", L"AURAPATH");
		// 상수 버퍼에는 2가지 종류가 필요하다.
		// 주소연결해주는 경우.
		// 0번 랜더 패스에
		// 버텍스 쉐이더 데이터에
		// SR->CB(L"VS_CUTDATA", HVEC4(0.0f, 0.0f, 1.0f, 1.0f));
		// 직접 내가 상수값을 들고 있는 경우.
		// SR->TEX(L"PS_MAPTEX", L"BUMPTEST.png");
		// SR->SMP(L"PS_DEFSMP", L"DSMP");

		// SR->RenderData(L"SPHEREMESH", L"LIGHTMESHDPATH");
		// SR->RenderData(L"BOXMESH", L"AURAPATH");
		// 상수 버퍼에는 2가지 종류가 필요하다.
		// 주소연결해주는 경우.
		// 0번 랜더 패스에
		// 버텍스 쉐이더 데이터에
		// 직접 내가 상수값을 들고 있는 경우.
		// SR->TEX(L"PS_MAPTEX", L"BUMPTEST.png");
		// SR->SMP(L"PS_DEFSMP", L"DSMP");
	}


	// HGAMETHREAD::CreateThread(L"ConsoleThread", ConvertLoadThread, Path);


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void HMAPDLG::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if (nullptr != HMapUpdater::ANIREN)
	{
		int UserFrame = Frame.GetPos();
		HMapUpdater::ANIREN->SetFrameIndex(UserFrame);
	}
}


void HMAPDLG::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(FALSE);

	CString Name;

	BoneList.GetText(BoneList.GetCurSel(), Name);

	std::wstring FindName = Name.GetString();

	HPTR<BoneChaser> BC = HMapUpdater::PLAYER->GetComponent<BoneChaser>();
	BC->Target(FindName.c_str());

	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
