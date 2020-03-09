#pragma once
// 빌드가 느려진다.

#include <Windows.h>
#include <map>
#include <string>
// #include <Windows.h>

#include "NAME.h"
#include "MATHS.h"
#include "REF.h"

#define WIN_SIZE_Y 900
#define WIN_SIZE_X 1600

class Window : public Ref, public Name
{
///////////////////////// static MgrFunc
private:
	static int g_WindowCount;
	static void(__stdcall *m_LoopFunc)();
	static HINSTANCE m_MainHInst;
	static std::map<std::wstring, WNDCLASSEXW> g_WinClassMap; // Windowclass를 관리하는 자료구조
	static std::map<std::wstring, HPTR<Window>> g_WinMap; // Windowclass를 통해서 만들어진 Window를 관리하는 자료구조

protected:
	static Window* m_MainWindow;

public:
	static Window* MainWindow();

public:
	static int WindowCount() 
	{
		return g_WindowCount;
	}

public:
	static void Init(const HINSTANCE& _MainHInst);
	// static HWINDOW* FindHWindow(const wchar_t* _TitleName);
	static LRESULT CALLBACK DEFMSG(HWND _HWnd, UINT _Msg, WPARAM _W, LPARAM _L);
	// static void BasicPeekLoop(MsgLoopFunc* _Loop);
	static void BasicPeekLoop(void(__stdcall *_LoopFunc)() = nullptr);

public:
	static bool CreateHWindowClass(const WNDCLASSEXW& wcex);
	static WNDCLASSEXW FindHWindowClass(const wchar_t* _Name);
	static HPTR<Window> FindHWindow(const wchar_t* _Name);

	static HPTR<Window> CreateHWindow(const wchar_t* _TitleName, bool _IsShow = true);
	static HPTR<Window> CreateHWindow(const wchar_t* _WindowName, const wchar_t* _TitleName, bool _IsShow = true);
	static HPTR<Window> CreateHWindow(const wchar_t* _ClassName, const wchar_t* _WindowName, const wchar_t* _TitleName, bool _IsShow = true);
	static bool DestroyHWindow(HWND _HWnd);
	static void AllWindowUpdate();

protected:
	static void InsertOtherWindow(Window* _Win);

/////////////////////////// Member
protected:
	// 헝가리안 표기법
	// m_ 맴버
	// m_이름
	bool m_IsFull;
	HWND m_HWnd;
	HDC m_HDC;
	int m_iStyle;
	int m_iMenu;
	Vector4 m_WinSize;

	Vector4 m_MosuePos;
	Vector4 m_PrevMosuePos;


public:
	bool IsFull() 
	{
		return m_IsFull;
	}

	HWND WH() const
	{
		return m_HWnd;
	}

	HDC DC() const
	{
		return m_HDC;
	}

	Vector4 MousePosInt();

	static int _delta;

protected:
	bool Create(const wchar_t* _Class, const wchar_t* _Title);
	void Init();

public:
	Vector4 Size();
	void Size(int _X, int _Y);
	void Pos(int _X, int _Y);
	void SizeAndPos(int _PosX, int _PosY, int _SizeX, int _SizeY);

	void Show();
	void Hide();

	void Update();

	Vector4 MousePos();
	Vector4 MouseDir();

protected:
	Window() : m_HWnd(nullptr), m_HDC(nullptr), m_IsFull(false) 	{	}
	virtual ~Window() 	{	}
};


