#include "IOPATH.h"
#include "DEBUG.h"


IOPath::IOPath()
{
}


IOPath::~IOPath()
{
}


bool IOPath::FileCheck()
{
//C:\MyStudy\AR22LJJ\Dx\BIN\FBX\FbxTexture
	if (false == IO::ExistFile(m_FullPath.c_str()))
	{
		BOOM;
		return false;
	}
	return true;
}

void IOPath::PathSetting(const wchar_t* _Path)
{
	m_FullPath = _Path;
	FileCheck();
}

void IOPath::PathSetting(const std::wstring& _Path)
{
	m_FullPath = _Path;
	FileCheck();
}


std::wstring IOPath::GetFileName()
{
	std::wstring FolderName = m_FullPath;
	size_t CutCount = FolderName.find_last_of(L'\\', m_FullPath.size());
	FolderName.replace(0, CutCount + 1, L"");
	return FolderName;
}

std::wstring IOPath::PathToFileName(const wchar_t* _Path)
{
	std::wstring FolderName = _Path;
	size_t CutCount = FolderName.find_last_of(L'\\', FolderName.size());
	FolderName.replace(0, CutCount + 1, L"");
	return FolderName;
}