#include "HCB.h"


HCB::HCB() : m_uiReg(0), m_bLink(false)
{
}


HCB::~HCB()
{

	if (m_bLink == false && nullptr != m_Data)
	{
		delete[](char*)m_Data;
		m_Data = nullptr;
	}

}

// 동시에 두곳에 세팅해줘야 한다면????
bool HCB::Create(HSHADER::SHADERTYPE _Shader, size_t _Byte, UINT _Reg)
{
	m_uiReg = _Reg;

	switch (_Shader)
	{
	case HSHADER::SHT_VS:
		ShUpdateFunc = &HCB::VSUpdate;
		break;
	case HSHADER::SHT_HS:
		ShUpdateFunc = &HCB::HSUpdate;
		break;
	case HSHADER::SHT_DS:
		ShUpdateFunc = &HCB::DSUpdate;
		break;
	case HSHADER::SHT_GS:
		ShUpdateFunc = &HCB::GSUpdate;
		break;
	case HSHADER::SHT_PS:
		ShUpdateFunc = &HCB::PSUpdate;
		break;
	default:
		break;
	}

	// 데이터를 동적할당으로 가지고 있는다.
	m_Desc.ByteWidth = (UINT)_Byte;
	m_Data = new char[_Byte];
	// 이거 그래픽카드가 관리할까 아니면 CPU에 할당할까 뭐 이런것들
	// 그리고 관리할때 수정할수 있게 할거야????
	m_Desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

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

// 행렬이 여기에 들어온다.
void HCB::DataSetting(void* _pData, UINT _Size) {

	// 정보 바꿀꺼다.
	GraphicDevice::MainContext()->Map(m_Buffer,
		0, D3D11_MAP_WRITE_DISCARD, 0, &m_tMapSub);
	// m_tMapSub.pData 그래픽카드와 통하는 통로를 얻어온다.

	// 정보 바꾸는 작업
	memcpy_s(m_tMapSub.pData, m_Desc.ByteWidth, _pData, _Size);

	// 끝났다 닫는다.
	GraphicDevice::MainContext()->Unmap(m_Buffer, 0);
}

void HCB::DataSetting() 
{
	if (false == m_bLink)
	{
		BOOM;
	}

	GraphicDevice::MainContext()->Map(m_Buffer,
		0, D3D11_MAP_WRITE_DISCARD, 0, &m_tMapSub);
	// m_tMapSub.pData 그래픽카드와 통하는 통로를 얻어온다.

	// 정보 바꾸는 작업
	memcpy_s(m_tMapSub.pData, m_Desc.ByteWidth, m_Data, BufferSize);

	// 끝났다 닫는다.
	GraphicDevice::MainContext()->Unmap(m_Buffer, 0);
}

void HCB::Update()
{
	(this->*ShUpdateFunc)();
}

void HCB::VSUpdate() { GraphicDevice::MainContext()->VSSetConstantBuffers(m_uiReg, 1, &m_Buffer); }
void HCB::HSUpdate() { GraphicDevice::MainContext()->HSSetConstantBuffers(m_uiReg, 1, &m_Buffer); }
void HCB::DSUpdate() { GraphicDevice::MainContext()->DSSetConstantBuffers(m_uiReg, 1, &m_Buffer); }
void HCB::GSUpdate() { GraphicDevice::MainContext()->GSSetConstantBuffers(m_uiReg, 1, &m_Buffer); }
void HCB::PSUpdate() { GraphicDevice::MainContext()->PSSetConstantBuffers(m_uiReg, 1, &m_Buffer); }


void HCB::Link(void* _Data, unsigned int _Size)
{
	if (nullptr != m_Data)
	{
		delete[] m_Data;
	}

	if (0 >= _Size)
	{
		BOOM;
	}

	if (m_Desc.ByteWidth != _Size)
	{
		// 새로 만들어줘야 한다.
		BOOM;
	}

	m_Data = _Data;
	m_Desc.ByteWidth = _Size;
	BufferSize = _Size;
	m_bLink = true;
}