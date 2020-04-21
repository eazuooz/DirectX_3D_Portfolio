#pragma once
#include "HVBBUFFER.h"

// 기본적으로는 만들때 버텍스 버퍼이다.
class HINSTBUFFER : public HVBBUFFER
{
private:
	const type_info*	DataType;
	unsigned int		m_CurDataCount;
	std::vector<char>	Data; // 인스턴싱용 데이터를 모아놓는 용도.

public:
	void ResetDataCount()
	{
		m_CurDataCount = 0;
	}

	unsigned int CurDataCount() 
	{
		return m_CurDataCount;
	}

	template<typename T>
	void PushData(const T& _Data)
	{
		if (DataType != &typeid(T))
		{
			BOOM;
		}

		if (((m_CurDataCount + 1) * m_VTXSize) > Data.size())
		{
			Data.resize(m_CurDataCount * 2 * m_VTXSize);
		}

		char* p = &Data[0];
		T* DataPointer = (T*)p;
		DataPointer[m_CurDataCount] = _Data;

		++m_CurDataCount;
	}


public:
	bool Create(unsigned int _MaxCount, const type_info* _Type, size_t _TypeSize);
	void UpdateInstData();

public:
	HINSTBUFFER();
	~HINSTBUFFER();
};

