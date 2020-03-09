#pragma once
// io input output의 약자
// 파일입출력 함수나 아니면 헤더라면
// 대부분 io 라는 
// img Texture
// #include <Windows.h>
// #include <Windows.h>
// 두가지를 동시에 


// #include <Windows.h>
#include <Windows.h>
// 기본적인 디파인은 내가 건들지 않아도 잡아준다.

#include <iostream>
#include <io.h>

class IO
{
public:
	static bool ExistFile(const char* _FileName);
	static bool ExistFile(const wchar_t* _FileName);

private:
	IO() {}
	~IO() {}
};

