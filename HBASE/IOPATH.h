#pragma once
#include "IO.h"
#include "FILESTREAM.h"
#include <vector>
#include <string>
#include <list>

class IOPath
{
protected:
	// C::\\Project
	std::wstring m_FullPath; // C::\\Project

public:
	static std::wstring PathToFileName(const wchar_t* _Path);
	void PathSetting(const wchar_t* _Path);
	void PathSetting(const std::wstring& _Path);

public:
	bool FileCheck();

public:

	const wchar_t* GetFullPath()
	{
		return m_FullPath.c_str();
	}


	std::wstring GetFullPathString()
	{
		return m_FullPath;
	}

	int GetFullPathSize()
	{
		return (int)m_FullPath.size();
	}

protected:
	std::wstring GetFileName();

public:
	IOPath();
	~IOPath();
};

