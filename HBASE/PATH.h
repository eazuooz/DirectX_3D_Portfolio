#pragma once
#include "DIRECTORY.h"
#include <map>


// Sprite, D:\Project\AR22\AR22API\BIN\Sprite
class Path
{
private:
	static std::map<std::wstring, Directory> m_PathMap;
	// 생성자를 아무런 것도 안넣어주면
public:
	static Directory Root;

public:
	static inline void SetRootDir(const Directory& _Dir)
	{
		Root = _Dir;
	}

	// 있다.
	static inline Directory GetRootDir()
	{
		return Root;
	}
	// 루트를 기준으로 어떤 경로를 만들어주는 함수.
	static void CreatePath(const wchar_t* _Path);
	static void CreatePath(const wchar_t* _Name, const wchar_t* _Path);

	static Directory FindPathToDir(const wchar_t* _Name);

	static std::wstring PlusPath(const wchar_t* _Name, const wchar_t* _PlusPath);

	static std::wstring FindPathToString(const wchar_t* _Name);
	static std::wstring ExtractFileName(const wchar_t* _Path);

private:
	std::wstring m_Path;

public:
	const wchar_t* GetPath() {
		return m_Path.c_str();
	}

	void SetPath(const wchar_t* _Name)
	{
		m_Path = _Name;
	}

protected:
	Path();
	~Path();
};

