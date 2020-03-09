#pragma once
#include <iostream>
#include "IO.h"
// 어떤 경로 넣어주면 파일이 존재하는지
// 존재하지 않는지 알려주는 함수가 있을까?
// 없을까? 

class FileStream
{
protected:
	FILE* pFile;

public:
	bool IsOpen() {
		return nullptr != pFile;
	}

public:
	FileStream(const char* _FileName, const char* _Mode);
	FileStream(const wchar_t* _FileName, const wchar_t* _Mode);
	virtual ~FileStream();
};


class FileStream_WB : public FileStream
{
public:
	void Write(void* _Data, UINT _Size)
	{
		fwrite(_Data, _Size, 1, pFile);
	}

	void Write(void* _Data, size_t _Size)
	{
		fwrite(_Data, _Size, 1, pFile);
	}

	template<typename T>
	void operator <<(const T& _Data) 
	{
		fwrite(&_Data, sizeof(T), 1, pFile);
	}

	template<>
	void operator <<(const std::wstring& _Data)
	{
		int Size = (int)_Data.size();
		fwrite(&Size, sizeof(int), 1, pFile);
		fwrite(_Data.c_str(), sizeof(wchar_t) * Size, 1, pFile);
	}



public:
	FileStream_WB(const char* _FileName);
	FileStream_WB(const wchar_t* _FileName);
	~FileStream_WB() {}
};

class FileStream_WT : public FileStream
{
public:
	void operator <<(const std::wstring& _Data)
	{
		fwrite(_Data.c_str(), _Data.size(), 1, pFile);
	}

	void operator <<(const std::string& _Data)
	{
		fwrite(_Data.c_str(), _Data.size(), 1, pFile);
	}

public:
	FileStream_WT(const char* _FileName);
	FileStream_WT(const wchar_t* _FileName);
	~FileStream_WT() {}
};


class FileStream_RB : public FileStream
{
public:
	void Read(void* _Data, int _Size)
	{
		fread_s(&_Data, _Size, _Size, 1, pFile);
	}

	void Read(void* _Data, size_t _Size)
	{
		fread_s(&_Data, _Size, _Size, 1, pFile);
	}


	template<typename T>
	void operator >>(T& _Data)
	{
		fread_s(&_Data, sizeof(T), sizeof(T), 1, pFile);
	}


	template<>
	void operator >>(std::wstring& _Data)
	{

		int Size;
		fread_s(&Size, sizeof(int), sizeof(int), 1, pFile);

		// 받은 버퍼를 동적으로 만든다.
		wchar_t* Arr = new wchar_t[Size + 1]{ 0, };
		fread_s(Arr, sizeof(wchar_t) * Size, sizeof(wchar_t) * Size, 1, pFile);
		_Data = Arr;

		if (nullptr != Arr)
		{
			delete[] Arr;
			Arr = nullptr;
		}
	}

public:
	FileStream_RB(const char* _FileName);
	FileStream_RB(const wchar_t* _FileName);
	~FileStream_RB() {}
};