#include "HSMP.h"



HSMP::HSMP() : m_pState(nullptr)
{
}


HSMP::~HSMP()
{
	if (nullptr != m_pState)
	{
		m_pState->Release();
		m_pState = nullptr;
	}
}

bool HSMP::Create()
{
	// Min Mag Mip을 다 고려해서 색깔가져와라
	// Min Mag Mip안사용했으면 알아서 안사용하네 알아서 내가 가져올께.
	m_Decs.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
	// D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR
	// m_Decs.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
	// 개념
	// uv 한번더 
	m_Decs.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	m_Decs.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	m_Decs.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

	// D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER
	memcpy_s(m_Decs.BorderColor, sizeof(Vector4), &Vector4::RED, sizeof(Vector4));

	// 압축
	// 압축을 안한다.
	// 필요없다. 왜??? 압축안할꺼니까.
	m_Decs.ComparisonFunc = D3D11_COMPARISON_NEVER;
	m_Decs.MaxAnisotropy = 0;
	m_Decs.MinLOD = -FLT_MAX;
	m_Decs.MaxLOD = FLT_MAX;

	m_Decs.MipLODBias = 1.0F;

	return Create(m_Decs);
}

bool HSMP::Create(const D3D11_SAMPLER_DESC& _Decs)
{
	if (&m_Decs != &_Decs)
	{
		m_Decs = _Decs;
	}

	if (S_OK != GraphicDevice::MainDevice()->CreateSamplerState(&m_Decs, &m_pState))
	{
		BOOM;
		return false;
	}

	return true;
}

void HSMP::Update(HSHADER::SHADERTYPE _ShaderType, int _SetIndex)
{
	if (nullptr == m_pState)
	{
		BOOM;
	}

	// _SetIndex 세팅되는 위치
	// 그 위치부터 연속으로 몇개 세팅할래????

	switch (_ShaderType)
	{
	case HSHADER::SHT_VS:
		GraphicDevice::MainContext()->VSSetSamplers(_SetIndex, 1, &m_pState);
		break;
	case HSHADER::SHT_HS:
		GraphicDevice::MainContext()->HSSetSamplers(_SetIndex, 1, &m_pState);
		break;
	case HSHADER::SHT_DS:
		GraphicDevice::MainContext()->DSSetSamplers(_SetIndex, 1, &m_pState);
		break;
	case HSHADER::SHT_GS:
		GraphicDevice::MainContext()->GSSetSamplers(_SetIndex, 1, &m_pState);
		break;
	case HSHADER::SHT_PS:
		GraphicDevice::MainContext()->PSSetSamplers(_SetIndex, 1, &m_pState);
		break;
	default:
		BOOM;
		break;
	}

}