#include "HIBBUFFER.h"



HIBBUFFER::HIBBUFFER()
{
}


HIBBUFFER::~HIBBUFFER()
{
}


bool HIBBUFFER::Create(DXGI_FORMAT _IdxFormat, UINT _iIdxCount, UINT _iIdxSize, D3D11_USAGE _IdxUsage, void* _Data)
{
	m_IDXCount = _iIdxCount; // 4개다
	m_IDXSize = _iIdxSize; // 16바이트
	m_IBType = _IdxFormat; // 16바이트
	m_Desc.ByteWidth = _iIdxCount * _iIdxSize;
	m_Desc.Usage = _IdxUsage;

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == m_Desc.Usage)
	{
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	m_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA DataStruct = D3D11_SUBRESOURCE_DATA();
	DataStruct.pSysMem = _Data;

	if (S_OK != GraphicDevice::MainDevice()->CreateBuffer(&m_Desc, &DataStruct, &m_Buffer))
	{
		BOOM;
		return false;
	}

	return true;
}