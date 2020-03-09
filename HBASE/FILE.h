#pragma once
#include "DIRECTORY.h"

// 이거 C#따라하고 있는 겁니다.
// C#은 전부다 클래스.

class File : public IOPath
{

public:
	// 확장자.
	// 확장자 넣어주면 
	// true
	bool CheckExtension(const wchar_t* _Ext);
	std::wstring GetFileName();

	Directory GetDirectory();


public:
	File();
	File(const wchar_t* _Path);
	File(const std::wstring& _Path);
	~File();
};

