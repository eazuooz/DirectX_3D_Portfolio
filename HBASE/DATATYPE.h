#pragma once
#include <typeinfo>
// #include <type_traits>

// #include <typeinfo>

// C++ 
// RTTI 리얼 타임 타입 인포
// RTTI 리얼 타임 타입 아이덴티테이션
// 연산자가 있다.
// type_info Data = typeid(자료형 혹은 식별자)

class DataType
{
private:
	const type_info* m_Info;

public:
	void TypeSetting();

public:
	bool IsType(const type_info* _Info)
	{
		return m_Info == _Info;
	}

	template<typename T>
	bool IsType() 
	{
		return m_Info == &typeid(T);
	}

	bool IsType(const DataType& _Name)
	{
		return m_Info == _Name.m_Info;
	}



public:
	DataType();
	virtual ~DataType();
};

