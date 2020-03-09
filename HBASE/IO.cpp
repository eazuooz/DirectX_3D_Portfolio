#include "IO.h"

bool IO::ExistFile(const char* _FileName)
{
	return _access_s(_FileName, 0) == 0;
}

bool IO::ExistFile(const wchar_t* _FileName)
{
	return _waccess_s(_FileName, 0) == 0;
}