#pragma once
#include "RES.h"

#include <fmod.hpp>
#include <map>
#include <set>
// FILO 처음 추가한녀석이 마지막에 나온다.
// 스택은 어찌보면 리스트에서 기능을 제한한 녀석.
#include <stack>



#ifdef X32
#ifdef DEBUGMODE
#pragma comment(lib, "fmodL_vc.lib")
#elif RELEASEMODE
#pragma comment(lib, "fmod_vc.lib")
#endif
#elif X64
#ifdef DEBUGMODE
#pragma comment(lib, "fmodL_vc.lib")
#elif RELEASEMODE
#pragma comment(lib, "fmod_vc.lib")
#endif
#endif

class Sound;
class SoundPlayer : public Ref
{
	friend Sound;

private:
	Sound* m_SoundClip;
	FMOD::Channel* m_pChannel;

public:
	void Stop();
};

class Sound : public Resources
{
public:
	static FMOD::System* m_pMainSoundSystem;

	static FMOD_RESULT __stdcall SoundPlayerCallBack(
		FMOD_CHANNELCONTROL *channelcontrol,
		FMOD_CHANNELCONTROL_TYPE controltype,
		FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype,
		void *commanddata1,
		void *commanddata2);


	static bool Init();
	static void Update();

	// Stop도 못시킨다.
	static void Play(const wchar_t* _Name);

	// 재생에 
	static void Play(const wchar_t* _PlayerName, const wchar_t* _Name, int _LoopCount = 1);
	static void Stop(const wchar_t* _PlayerName);

	static HPTR<SoundPlayer> FindPlayer(const wchar_t* _PlayerName);

	static std::map<std::wstring, HPTR<SoundPlayer>> m_PlayList;


	// FMOD::Channel* m_pChannel;

private:
	FMOD::Sound* m_pSound;

	// 밀리세컨드 단위를 리턴한다.
public:
	unsigned int SoundLen()
	{
		unsigned int Value = -1;
		m_pSound->getLength(&Value, FMOD_TIMEUNIT_MS);
		return Value;
	}

public:
	bool Load();


public:
	Sound();
	~Sound();
};

