#include "HVBBUFFER.h"



HVBBUFFER::HVBBUFFER()
{
}


HVBBUFFER::~HVBBUFFER()
{
}

bool HVBBUFFER::Create(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _VtxUsage, void* _Data)
{
	m_VTXCount = _iVtxCount; // 4개다
	m_VTXSize = _iVtxSize; // 16바이트
	m_Desc.ByteWidth = _iVtxCount * _iVtxSize;
	m_Desc.Usage = _VtxUsage;

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == m_Desc.Usage)
	{
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	m_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA DataStruct = D3D11_SUBRESOURCE_DATA();
	DataStruct.pSysMem = _Data;

	if (S_OK != GraphicDevice::MainDevice()->CreateBuffer(&m_Desc, &DataStruct, &m_Buffer))
	{
		BOOM;
		return false;
	}

	return true;
}