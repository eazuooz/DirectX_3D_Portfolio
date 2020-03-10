#include "BoneChaser.h"


BoneChaser::BoneChaser() : AniRender(nullptr), BoneName(L"")
{
}


BoneChaser::~BoneChaser()
{
}

void BoneChaser::Target(HPTR<FbxAnimation> _Render)
{
	if (nullptr == _Render)
	{
		BOOM;
	}

	AniRender = _Render;
}

void BoneChaser::Target(HPTR<FbxAnimation> _Render, const wchar_t* _Name)
{
	if (nullptr == _Render)
	{
		BOOM;
	}

	AniRender = _Render;
	BoneName = _Name;
}

void BoneChaser::Target(const wchar_t * _Name)
{
	if (nullptr == AniRender)
	{
		BOOM;
	}

	BoneName = _Name;
}

void BoneChaser::Update()
{
	if (nullptr == AniRender)
	{
		return;
	}

	if (BoneName == L"")
	{
		return;
	}

	FbxAnimation::BONEDATA Data = AniRender->FindBoneData(BoneName.c_str());
	GetTransform()->SetPosition(Data.WT);
	GetTransform()->QtoWRot(Data.WQ);
	
	// Trans()->LScale(Data.LS);
}


