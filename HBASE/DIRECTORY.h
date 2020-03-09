#pragma once
#include "IOPATH.h"
#include <vector>
#include <string>
#include <list>

class File;
class Directory : public IOPath
{
public:
	// 정말 중요하다.
	// 파일입출력
	// 문자열
	// 디버깅
	// 프로그래머의 3대 기본기

	// c::/
	void ReplaceParentPath();
	bool MoveDirectory(const wchar_t* _FolderName);
	bool ExistFile(const wchar_t* _FileName);

	std::wstring GetFoldersName();

	void GetFilesName(std::vector<File>& _List, const wchar_t* _FileEx);

public:
	// virtual std::list<HFILE> AllFiles();

public:
	// 이 프로그램이 실행된 위치를
	// 자신의 경로로 잡을것이다.
	Directory(const Directory& _Other);
	Directory(const wchar_t* _Name);

	Directory();
	// 경로가 지정된것.
	// HDIRECTORY(const wchar_t* _Name);
	~Directory();
};