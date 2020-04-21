#pragma once
#include "H3DBUFFER.h"

class HVBBUFFER : public H3DBUFFER
{
protected:
	UINT m_VTXSize; // 점하나가 x바이트의 크기이다.;
	UINT m_VTXCount; // 점이 x개다;

public:
	void VTXCount(unsigned int _VTXCount)
	{
		m_VTXCount = _VTXCount;
	}

	UINT VTXSize() {
		return m_VTXSize;
	}
	UINT VTXCount() {
		return m_VTXCount;
	}

public:
	template<typename VBTYPE>
	bool Create(std::vector<VBTYPE>& _VtxData, D3D11_USAGE _VtxUsage)
	{
		bool Check;
		Check = Create((int)_VtxData.size(), VBTYPE::MEMSIZE(), _VtxUsage, &_VtxData[0]);

		if (false == Check)
		{
			BOOM;
			return false;
		}

		return true;
	}


	bool Create(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _VtxUsage, void* _Data);

public:
	HVBBUFFER();
	~HVBBUFFER();
};

