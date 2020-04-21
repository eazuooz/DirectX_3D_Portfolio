#pragma once
#include "H3DBUFFER.h"

class HIBBUFFER
	: public H3DBUFFER
{
private:
	DXGI_FORMAT m_IBType; // 인덱스 버퍼 타입 자료형이 뭐냐?
	UINT m_IDXSize; // 인덱스 버퍼 원소 1개의 사이즈;
	UINT m_IDXCount; // 점이 x개다 전체 그냥 정수가 몇개냐????;

public:
	DXGI_FORMAT IBType()
	{
		return m_IBType;
	}

	unsigned int IDXCount() 
	{
		return m_IDXCount;
	}


public:
	template<typename T>
	bool Create(std::vector<T>& _Data, D3D11_USAGE _VtxUsage)
	{
		if (0 >= _Data.size())
		{
			BOOM;
		}
		return Create(T::FORMAT(), (int)_Data.size() * 3, T::MEMSIZE(), _VtxUsage, &_Data[0]);
	}

	bool Create(std::vector<DWORD>& _Data, D3D11_USAGE _VtxUsage)
	{
		if (0 >= _Data.size())
		{
			BOOM;
		}
		return Create(DXGI_FORMAT_R32_UINT, (int)_Data.size(), sizeof(DWORD), _VtxUsage, &_Data[0]);
	}

	bool Create(std::vector<UINT>& _Data, D3D11_USAGE _VtxUsage)
	{
		if (0 >= _Data.size())
		{
			BOOM;
		}
		return Create(DXGI_FORMAT_R32_UINT, (int)_Data.size(), sizeof(DWORD), _VtxUsage, &_Data[0]);
	}


	bool Create(DXGI_FORMAT _IdxFormat, UINT _iIdxCount, UINT _iIdxSize, D3D11_USAGE _IdxUsage, void* _Data);


public:
	HIBBUFFER();
	~HIBBUFFER();
};

