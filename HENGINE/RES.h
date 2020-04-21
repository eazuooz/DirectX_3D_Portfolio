#pragma once
#include <REF.h>
#include <Name.h>
#include <string>
#include <PATH.h>
#include <FILE.h>
#include <map>
#include <unordered_map>

#include "GRAPHICDEVICE.h"


// 다중상속.
// 이름이 안겹치면 된다.
// Name 함수와 맴버변수만을 가졌고 다형성을 사용하지 않았기 때문에 안전하다.
// 만약 여러분들이 다중상속을 사용하려는데
// 다형성을 사용하고 싶다면 순수가상함수를 통해서 사용하라.
// 그 순수가상함수를 구현하지 않는 편
// c#에서는 이런 문법을 인터페이스라고 하여 아예 문법으로 지원한다.


// 변환이 안된다는 것은
// 선생님 피셜.
// 뷔페식 상속 방식.
// HRES가 필요한 이유는 단순히 구분을 해주기위한 클래스가 된다.
class Resources : public Ref, public Name
{
private:
	// 디폴트 타입
	// 디폴트 인자와 마찬가지로 함수의 인자중 가장 오른쪽에
	// 있는 녀석만 디폴트를 넣어줄 수 있다.
	template<typename RESTYPE>
	class HRESMGR
	{
	public:
		// 속력위주로 하려고.
		// MAP이면 정렬위주로
		// unordered_map 속력이 빠르다.
		static std::unordered_map<std::wstring, HPTR<RESTYPE>> m_ResMap;

		// 템플릿 할때 이너클래스에 대한 타입이 불명확해지는 문제가 있는데.
		// static std::unordered_map<std::wstring, HPTR<RESTYPE>>::iterator FindIter;

	public:
		static HPTR<RESTYPE> Find(const std::wstring& _Key)
		{
			typename std::unordered_map<std::wstring, HPTR<RESTYPE>>::iterator FindIter = m_ResMap.find(_Key);

			if (FindIter == m_ResMap.end())
			{
				return nullptr;
			}

			return FindIter->second;
		}

		template<class... _Types>
		static HPTR<RESTYPE> Load(const std::wstring& _Path, _Types&&... _Args)
		{
			File NewFile = _Path;
			std::wstring FName = NewFile.GetFileName();
			return Load(FName, _Path, _Args...);
		}


		template<class... _Types>
		static HPTR<RESTYPE> Load(const std::wstring& _Name, const std::wstring& _Path, _Types&&... _Args)
		{
			// 쓰레드는 공유하는 메모리는???
			// 코드 데이터 힙 
			// 쓰레드에 문제가 없을때
			// 메모리 공유하지 않을때
			// 수정을 안할때. 


			if (nullptr != Find(_Name))
			{
				BOOM;
				return nullptr;
			}

			RESTYPE* NewRes = new RESTYPE();
			// 키의 타입을 봐서 키가 wstring이 아니라면
			// char*
			// wchar_t* 가 아니라면 문자열이 아니라면
			NewRes->SetName(_Name);
			NewRes->FileSetting(_Path);
			if (false == NewRes->Load(_Args...))
			{
				BOOM;
				return nullptr;
			}


			// 이녀석
			m_ResMap.insert(std::unordered_map<std::wstring, HPTR<RESTYPE>>::value_type(_Name, NewRes));
			return NewRes;
		}


		template<class... _Types>
		static HPTR<RESTYPE> LoadToKey(const std::wstring& _Key, const std::wstring& _Path, _Types&&... _Args)
		{
			if (nullptr != Find(_Key))
			{
				BOOM;
				return nullptr;
			}

			RESTYPE* NewRes = new RESTYPE();
			// 키의 타입을 봐서 키가 wstring이 아니라면
			// char*
			// wchar_t* 가 아니라면 문자열이 아니라면
			NewRes->SetName(_Key);
			NewRes->FileSetting(_Path);
			if (false == NewRes->Load(_Args...))
			{
				BOOM;
				return nullptr;
			}


			m_ResMap.insert(std::unordered_map<std::wstring, HPTR<RESTYPE>>::value_type(_Key, NewRes));

			return NewRes;
		}

		// Create그래서 Res는 Create라는 이름을 가진 함수 자체는 만들어 줘야 한다.
		template<class... _Types>
		static HPTR<RESTYPE> Create(const std::wstring& _Key, _Types&&... _Args)
		{
			if (nullptr != Find(_Key))
			{
				BOOM;
				return nullptr;
			}

			RESTYPE* NewRes = new RESTYPE();
			// 키의 타입을 봐서 키가 wstring이 아니라면
			// char*
			// wchar_t* 가 아니라면 문자열이 아니라면
			NewRes->SetName(_Key);
			if (false == NewRes->Create(_Args...))
			{
				BOOM;
				return nullptr;
			}
			m_ResMap.insert(std::unordered_map<std::wstring, HPTR<RESTYPE>>::value_type(_Key, NewRes));

			return NewRes;
		}
	};

public:
	template<typename RESTYPE>
	static HPTR<RESTYPE> Find(const std::wstring& _Key)
	{
		return HRESMGR<RESTYPE>::Find(_Key);
	}

	template<typename RESTYPE, class... _Types >
	static HPTR<RESTYPE> Create(const std::wstring& _Key, _Types&&... _Args)
	{
		return HRESMGR<RESTYPE>::Create(_Key, _Args...);
	}

	// 임시객체를 보관 리턴받는 레퍼런스
	// RValue 레퍼런스
	template<typename RESTYPE, class... _Types>
	static HPTR<RESTYPE> Load(const std::wstring& _Path, _Types&&... _Args)
	{
		return HRESMGR<RESTYPE>::Load(_Path, _Args...);
	}

	template<typename RESTYPE, class... _Types>
	static HPTR<RESTYPE> LoadToName(const std::wstring& _Name, const std::wstring& _Path, _Types&&... _Args)
	{
		return HRESMGR<RESTYPE>::Load(_Name, _Path, _Args...);
	}

	template<typename RESTYPE, class... _Types>
	static HPTR<RESTYPE> LoadToKey(const std::wstring& _Key, const std::wstring& _Path, _Types&&... _Args)
	{
		return HRESMGR<RESTYPE>::LoadToKey(_Key, _Path, _Args...);
	}

protected:
	// 파일에서 
	File m_File;

public:
	virtual bool Create() { return true; }
	virtual bool Load() { return true; }

public:
	void FileSetting(const wchar_t* _Path) {
		m_File.PathSetting(_Path);
	}

	void FileSetting(const std::wstring& _Path) {
		m_File.PathSetting(_Path.c_str());
	}

public:
	Resources();
	~Resources();
};

template<typename RESTYPE>
std::unordered_map<std::wstring, HPTR<RESTYPE>> Resources::HRESMGR<RESTYPE>::m_ResMap;
