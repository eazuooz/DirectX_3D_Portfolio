#pragma once
#include "RES.h"

#ifdef X32
#ifdef DEBUGMODE

#pragma comment(lib, "FW1FontWrapperD32.lib")
#elif RELEASEMODE
#pragma comment(lib, "FW1FontWrapperR32.lib")
#endif
#elif X64
#ifdef DEBUGMODE
#pragma comment(lib, "FW1FontWrapperD64.lib")
#elif RELEASEMODE
#pragma comment(lib, "FW1FontWrapperR64.lib")
#endif
#endif

#include <FW1CompileSettings.h>
#include <FW1FontWrapper.h>

class HEFONT : public Resources
{
private:
	IFW1Factory* m_pFontFactory;
	IFW1FontWrapper* m_pFontWrapper;

public:
	bool Create(const wchar_t* _FontName);
	void DrawWorld(const wchar_t* _Text, float _fSize, Vector4 Pos, Vector4 COLOR, FW1_TEXT_FLAG _Flag);

	// 스크린좌표를 기준으로 하므로 캐릭터 머리위에 캐릭터 명을 띄우고 싶다던가 하면
	// 월드 좌표계에서 스크린 좌표계로 변환해서 수치를 넣어주면 될것이다.
	void DrawUI(const wchar_t* _Text, float _fSize, Vector4 Pos, Vector4 COLOR, FW1_TEXT_FLAG _Flag = FW1_TOP);

public:
	HEFONT();
	~HEFONT();
};

