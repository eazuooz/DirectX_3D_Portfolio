#include "PATH.h"
#include "DEBUG.h"


std::map<std::wstring, Directory> Path::m_PathMap;
Directory Path::Root;


Path::Path()
{
}


Path::~Path()
{
}


void Path::CreatePath(const wchar_t* _Path) 
{
	CreatePath(_Path, _Path);
}
void Path::CreatePath(const wchar_t* _Name, const wchar_t* _Path)
{
	Directory NewPath = Root;

	if (false == NewPath.MoveDirectory(_Path))
	{
		return;
	}



	m_PathMap.insert(std::map<std::wstring, Directory>::value_type(_Name, NewPath));
	return;
}

Directory Path::FindPathToDir(const wchar_t* _Name) 
{
	std::map<std::wstring, Directory>::iterator Iter = m_PathMap.find(_Name);

	if (Iter == m_PathMap.end())
	{
		BOOM;
		return Directory();
	}

	return Iter->second;
}

std::wstring Path::FindPathToString(const wchar_t* _Name)
{
	return FindPathToDir(_Name).GetFullPath();
}

std::wstring Path::PlusPath(const wchar_t* _Name, const wchar_t* _PlusPath) 
{
	std::wstring Path = FindPathToDir(_Name).GetFullPath();
	Path += L"\\";
	Path += _PlusPath;
	return Path;
}

std::wstring Path::ExtractFileName(const wchar_t* _Path) 
{
	std::wstring FileName = _Path;
	size_t CutCount = FileName.find_last_of(L'\\', FileName.size());
	return FileName.replace(0, CutCount + 1, L"");

}