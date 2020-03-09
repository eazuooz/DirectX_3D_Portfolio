#pragma once
#include <string>

// virtual을 하면 가상함수 테이블이 생기므로
// virtual이 없다면 일단 안전하다.
class Name
{
private:
	std::wstring m_Name;

public:
	const wchar_t* GetName() {
		return m_Name.c_str();
	}

	const std::wstring& CWStringName() {
		return m_Name;
	}

	void SetName(const wchar_t* _Name) 
	{
		m_Name = _Name;
	}

	void SetName(const std::wstring& _Name)
	{
		m_Name = _Name;
	}

protected:
	Name();
	~Name();
};

