#include "FILE.h"
#include "DEBUG.h"
// 디버깅
// 문자열
// 파일입출력

File::File() 
{

}

File::File(const std::wstring& _Path) 
{
	PathSetting(_Path);
}

File::File(const wchar_t* _Path)
{
	PathSetting(_Path);
}

File::~File()
{
}

bool File::CheckExtension(const wchar_t* _Ext)
{
	std::wstring Ext = _Ext;

	size_t CheckComma = Ext.find(L".");

	if (CheckComma >= Ext.size())
	{
		Ext = L"." + Ext;
	}

	size_t CheckExt = m_FullPath.find(Ext.c_str());
	if (CheckExt >= m_FullPath.size())
	{
		return false;
	}
	return true;
}


// }

std::wstring File::GetFileName() 
{
	return IOPath::GetFileName();
}

Directory File::GetDirectory()
{
	return Directory();
}