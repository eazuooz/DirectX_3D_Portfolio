#include "H3DBUFFER.h"



H3DBUFFER::H3DBUFFER()
{
}


H3DBUFFER::~H3DBUFFER()
{
	if (nullptr != m_Buffer)
	{
		m_Buffer->Release();
		m_Buffer = nullptr;
	}
}


bool H3DBUFFER::CreateBuffer(unsigned int _Size, D3D11_USAGE _Usage, unsigned int _BindFlag)
{
	// 데이터를 동적할당으로 가지고 있는다.
	m_Desc.ByteWidth = (UINT)_Size;
	// 이거 그래픽카드가 관리할까 아니면 CPU에 할당할까 뭐 이런것들
	// 그리고 관리할때 수정할수 있게 할거야????
	m_Desc.Usage = _Usage;

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == m_Desc.Usage)
	{
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	m_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (S_OK != GraphicDevice::MainDevice()->CreateBuffer(&m_Desc, nullptr, &m_Buffer))
	{
		BOOM;
		return false;
	}

	return true;
}