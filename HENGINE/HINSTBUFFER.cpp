#include "HINSTBUFFER.h"



HINSTBUFFER::HINSTBUFFER()
{
}


HINSTBUFFER::~HINSTBUFFER()
{
}


bool HINSTBUFFER::Create(unsigned int _MaxCount, const type_info* _Type, size_t _TypeSize)
{
	if (nullptr != m_Buffer)
	{
		m_Buffer->Release();
	}

	DataType = _Type;
	m_VTXCount = _MaxCount;
	m_VTXSize = (unsigned int)_TypeSize;
	m_Desc.ByteWidth = m_VTXCount * m_VTXSize;
	m_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_Desc.Usage = D3D11_USAGE_DYNAMIC;
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Data.resize(m_Desc.ByteWidth);

	if (S_OK != GraphicDevice::MainDevice()->CreateBuffer(&m_Desc, nullptr, &m_Buffer))
	{
		BOOM;
		return false;
	}

	return true;

}

void HINSTBUFFER::UpdateInstData()
{
	if (m_CurDataCount > m_VTXCount)
	{
		Create(m_CurDataCount, DataType, m_VTXSize);
	}

	D3D11_MAPPED_SUBRESOURCE tMap = {};

	GraphicDevice::MainContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);
	memcpy_s(tMap.pData, m_Desc.ByteWidth, &Data[0], Data.size());
	GraphicDevice::MainContext()->Unmap(m_Buffer, 0);
}