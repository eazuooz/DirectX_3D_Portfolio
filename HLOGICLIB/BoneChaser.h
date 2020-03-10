#pragma once
#include <LOGIC.h>
#include <3DANIRENDER.h>
#include <FBX.h>

class BoneChaser :	public Logic
{
private:
	HPTR<FbxAnimation> AniRender;
	std::wstring BoneName;

public:
	void Target(HPTR<FbxAnimation> _Render);
	void Target(HPTR<FbxAnimation> _Render, const wchar_t* _Name);
	void Target(const wchar_t* _Name);

private:
	void Update() override;

public:
	BoneChaser();
	~BoneChaser();
};

