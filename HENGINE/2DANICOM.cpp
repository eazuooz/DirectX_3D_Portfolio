#include "2DANICOM.h"
#include <JTIME.h>

#include "SPRITERENDER.h"
#include "HSMP.h"
#include "HMESH.h"
#include "HCB.h"
#include "HVTXSHADER.h"
#include "HPIXSHADER.h"
#include "SPRITE.h"

/////////////////////////////////////////// H2DANI

SpriteAnimaion::AnimationClip::AnimationClip() {

}
SpriteAnimaion::AnimationClip::~AnimationClip() {

}

void SpriteAnimaion::AnimationClip::Reset() 
{
	CurFrame = 0;
	CurTime = FrameData[0].m_Time;
}

void SpriteAnimaion::AnimationClip::Init(HPTR<Sprite> _Sprite, unsigned int _Start, unsigned int _End, float _BaseTime)
{
	m_Sprite = _Sprite;

	for (unsigned int i = _Start; i < _End; i++)
	{
		FrameData.push_back({ i, _BaseTime });
	}
}

void SpriteAnimaion::AnimationClip::InitRoll(HPTR<Sprite> _Sprite, unsigned int _Start, unsigned int _End, float _BaseTime)
{
	m_Sprite = _Sprite;
	unsigned int i = 0;
	for (i = _Start; i < _End; i++)
	{
		FrameData.push_back({ i, _BaseTime });
	}
	
	for (i = (_End-2); i > _Start; i--)
	{
		FrameData.push_back({ i, _BaseTime });
	}

}


void SpriteAnimaion::AnimationClip::Update() 
{
	CurTime -= Time::DeltaTime();

	if (0.0f >= CurTime)
	{
		++CurFrame;

		if (true == m_Loop)
		{
			if ((int)FrameData.size() <= CurFrame)
			{
				Reset();
			}
		}
		else if (false == m_Loop 
			&& (int)FrameData.size() <= CurFrame)
		{
			CurFrame = (int)FrameData.size() - 1;
		}

		CurTime = FrameData[CurFrame].m_Time;

	}

}

/////////////////////////////////////////// H2DANICOM

SpriteAnimaion::SpriteAnimaion() : m_pRender(nullptr)
{
}

SpriteAnimaion::~SpriteAnimaion()
{
}

void SpriteAnimaion::Init() 
{
	// 전혀 서로 모르는 사람들 아무런 연관
	// GetCom만든적이 있나요?
	// 주소비교만 할경우는 헤더 몰라도 되는데

	// HPTR<HRENDER> PTR = GetActor()->GetCom<HRENDER>();
	m_pRender = GetActor()->GetComponent<SpriteRenderer>();
	if (nullptr == m_pRender)
	{
		BOOM;
	}
}



void SpriteAnimaion::RenderBefore()
{
	// 애니메이션은 
	if (nullptr == CurAni)
	{
		return;
	}

	CurAni->Update();

	m_pRender->SetSpriteIdx(CurAni->FrameIndex());

}

bool SpriteAnimaion::CreateAni(const wchar_t* _AniName
	, const wchar_t* _SpriteName
	, unsigned int _Start, unsigned int _End, float _Time, bool _Loop)
{
	if (nullptr != FindAni(_AniName))
	{
		BOOM;
		return false;
	}

	if (_Start > _End)
	{
		BOOM;
		return false;
	}

	HPTR<Sprite> FindSprite = Resources::Find<Sprite>(_SpriteName);

	if (nullptr ==  FindSprite)
	{
		BOOM;
		return false;
	}


	if (_End > FindSprite->MultiCount())
	{
		BOOM;
		return false;
	}

	AnimationClip* NewAni = new AnimationClip();

	NewAni->SetName(_AniName);
	NewAni->m_Loop = _Loop;
	NewAni->Init(FindSprite, _Start, _End, _Time);

	m_AniMap.insert(std::map<std::wstring, HPTR<AnimationClip>>::value_type(_AniName, NewAni));

	if (1 == m_AniMap.size())
	{
		ChanageAni(_AniName);
	}

	return true;
}

bool SpriteAnimaion::CreateAniRoll(const wchar_t* _AniName
	, const wchar_t* _SpriteName
	, unsigned int _Start, unsigned int _End, float _Time, bool _Loop)
{
	if (nullptr != FindAni(_AniName))
	{
		BOOM;
		return false;
	}

	if (_Start > _End)
	{
		BOOM;
		return false;
	}

	HPTR<Sprite> FindSprite = Resources::Find<Sprite>(_SpriteName);

	if (nullptr == FindSprite)
	{
		BOOM;
		return false;
	}


	if (_End > FindSprite->MultiCount())
	{
		BOOM;
		return false;
	}

	AnimationClip* NewAni = new AnimationClip();

	NewAni->SetName(_AniName);
	NewAni->m_Loop = _Loop;
	NewAni->InitRoll(FindSprite, _Start, _End, _Time);

	m_AniMap.insert(std::map<std::wstring, HPTR<AnimationClip>>::value_type(_AniName, NewAni));

	if (1 == m_AniMap.size())
	{
		ChanageAni(_AniName);
	}

	return true;
}

void SpriteAnimaion::ChanageAni(const wchar_t* _AniName) 
{
	if (FindAni(_AniName) == CurAni)
	{
		return;
	}
	CurAni = FindAni(_AniName);
	if (nullptr == CurAni)
	{
		BOOM;
	}

	CurAni->Reset();
}

HPTR<SpriteAnimaion::AnimationClip> SpriteAnimaion::FindAni(const wchar_t* _AniName) 
{
	std::map<std::wstring, HPTR<AnimationClip>>::iterator Iter 
		= m_AniMap.find(_AniName);

	if (Iter == m_AniMap.end())
	{
		return nullptr;
	}

	return  Iter->second;
}