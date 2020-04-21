#pragma once
#include "COMPONENT.h"


class SpriteRenderer;
class Sprite;
class SpriteAnimaion : public Component
{
private:
	class AnimationData 
	{
	public:
		unsigned int m_SpriteIndex;
		float m_Time;

	public:
		AnimationData(unsigned int _Sprite, float _Time) : m_SpriteIndex(_Sprite), m_Time(_Time)
		{

		}
	};

	class AnimationClip : public Ref, public Name
	{
	public:
		HPTR<Sprite> m_Sprite;
		// 애니메이션을 만들기위한 정보
		bool m_Loop;
		std::vector<AnimationData> FrameData;
		// 실제 애니메이션의 실행정보
		int CurFrame; // Start End
		float CurTime; 

	public:
		int FrameIndex() 
		{
			return FrameData[CurFrame].m_SpriteIndex;
		}

	public:
		void Init(HPTR<Sprite> _Sprite, unsigned int _Start, unsigned int _End,  float _BaseTime);
		void InitRoll(HPTR<Sprite> _Sprite, unsigned int _Start, unsigned int _End, float _BaseTime);
		void Update();
		void Reset();

	public:
		AnimationClip();
		~AnimationClip();
	};

private:
	// 캐칭이라고 하는데.
	SpriteRenderer* m_pRender;
	std::map<std::wstring, HPTR<AnimationClip>> m_AniMap;

	HPTR<AnimationClip> CurAni;

public:
	void Stop() 
	{
		CurAni = nullptr;
	}

	void ChanageAni(const wchar_t* _AniName);

	bool CreateAni(const wchar_t* _AniName
		, const wchar_t* _SpriteName
		, unsigned int _Start, unsigned int _End, float _Time = 0.1f, bool _Loop = true);

	bool CreateAniRoll(const wchar_t* _AniName
		, const wchar_t* _SpriteName
		, unsigned int _Start, unsigned int _End, float _Time = 0.1f, bool _Loop = true);

public:
	// bool IsAni();

private:
	HPTR<AnimationClip> FindAni(const wchar_t* _AniName);


public:
	void Init() override;
	void RenderBefore() override;

public:
	SpriteAnimaion();
	~SpriteAnimaion();
};

