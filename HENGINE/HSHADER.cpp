#include "HSHADER.h"
#include "H3DBUFFER.h"

unsigned int HSHADER::SizeOfFormat(DXGI_FORMAT _Fmt) 
{
	switch (_Fmt)
	{
	case DXGI_FORMAT_UNKNOWN:
		return 0;
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		return 16;
	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		return 12;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
		return 8;
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		return 4;
	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
	case DXGI_FORMAT_R1_UNORM:
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
	case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
	case DXGI_FORMAT_B8G8R8A8_TYPELESS:
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8X8_TYPELESS:
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
	case DXGI_FORMAT_BC6H_TYPELESS:
	case DXGI_FORMAT_BC6H_UF16:
	case DXGI_FORMAT_BC6H_SF16:
	case DXGI_FORMAT_BC7_TYPELESS:
	case DXGI_FORMAT_BC7_UNORM:
	case DXGI_FORMAT_BC7_UNORM_SRGB:
	case DXGI_FORMAT_AYUV:
	case DXGI_FORMAT_Y410:
	case DXGI_FORMAT_Y416:
	case DXGI_FORMAT_NV12:
	case DXGI_FORMAT_P010:
	case DXGI_FORMAT_P016:
	case DXGI_FORMAT_420_OPAQUE:
	case DXGI_FORMAT_YUY2:
	case DXGI_FORMAT_Y210:
	case DXGI_FORMAT_Y216:
	case DXGI_FORMAT_NV11:
	case DXGI_FORMAT_AI44:
	case DXGI_FORMAT_IA44:
	case DXGI_FORMAT_P8:
	case DXGI_FORMAT_A8P8:
	case DXGI_FORMAT_B4G4R4A4_UNORM:
	case DXGI_FORMAT_P208:
	case DXGI_FORMAT_V208:
	case DXGI_FORMAT_V408:
	case DXGI_FORMAT_FORCE_UINT:
		return 0;
	default:
		return 0;
	}

}

char HSHADER::VERBUFFERTEXT[256];
// std::map<unsigned int, HPTR<H3DBUFFER>> HSHADER::m_BufferPool[HSHADER::SHADERTYPE::SHT_END];

HSHADER::HSHADER(SHADERTYPE _Type) : m_pBlob(nullptr), m_pErrBlob(nullptr), m_Type(_Type)
{
}


HSHADER::~HSHADER()
{
	//for (size_t i = 0; i < m_AllCB.size(); i++)
	//{
	//	delete[] m_AllCB[i].m_pData;
	//	m_AllCB[i].m_pData = nullptr;
	//}

	// m_pErrBlob->AddRef();
	// 순서는 역순으로 처리해라.
	if (nullptr != m_pErrBlob) { m_pErrBlob->Release(); }
	if (nullptr != m_pBlob) { m_pBlob->Release(); }
}


//HPTR<H3DBUFFER> HSHADER::CBBufferCheck(HSHADER::SHADERTYPE _Type, unsigned int _ByteSize)
//{
//	// 없으면 만들기까지 할건데. 없다면 말도 안된다.
//
//	std::map<unsigned int, HPTR<H3DBUFFER>>::iterator FindIter = m_BufferPool[_Type].find((unsigned int)_ByteSize);
//
//	// 버텍스 쉐이더에....
//	if (FindIter == m_BufferPool[_Type].end())
//	{
//		H3DBUFFER* NewBuffer = new H3DBUFFER();
//		NewBuffer->CreateBuffer((unsigned int)_ByteSize, D3D11_USAGE::D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER);
//
//		m_BufferPool[_Type].insert(std::map<unsigned int, HPTR<H3DBUFFER>>::value_type(_ByteSize, NewBuffer));
//
//		FindIter = m_BufferPool[_Type].find((unsigned int)_ByteSize);
//	}
//
//	return FindIter->second;
//}

void HSHADER::CreateCB(const wchar_t* _Name, unsigned int _Byte, UINT _Reg, CBTYPE _Type /*= CBTYPE::NONE*/)
{
	// 예 내가 HMAT하나 세팅해주고 싶다.
	// 만약 여태까지 그 크기의 버퍼가 만들어진적이 없다면
	// 만들면 된다.
	// HPTR<H3DBUFFER> Buffer = CBBufferCheck(m_Type, _Byte);

	if (m_AllCB.find(_Name) != m_AllCB.end())
	{
		BOOM;
	}

	if (m_AllCBIndex.find(_Reg) != m_AllCBIndex.end())
	{
		BOOM;
	}

	CBDATA CB;
	CB.m_Name = _Name;
	//CB.m_H3DBUFFER = new H3DBUFFER();
	//if (false == CB.m_H3DBUFFER->CreateBuffer((unsigned int)_Byte, D3D11_USAGE::D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER))
	//{
	//	BOOM;
	//}
	//CB.m_Buffer = CB.m_H3DBUFFER->Buffer();
	CB.m_CBType = _Type;
	CB.m_Shader = m_Type;
	CB.m_uiReg = _Reg;
	CB.m_Size = _Byte;

	m_AllCBIndex.insert(_Reg);
	m_AllCB.insert(std::unordered_map<std::wstring, CBDATA>::value_type(_Name, CB));
}

void HSHADER::CreateSMPDATA(const wchar_t* _Name, UINT _Reg)
{
	if (m_AllSMP.find(_Name) != m_AllSMP.end())
	{
		BOOM;
	}

	if (m_AllSMPIndex.find(_Reg) != m_AllSMPIndex.end())
	{
		BOOM;
	}

	m_AllSMPIndex.insert(_Reg);
	m_AllSMP.insert(std::unordered_map<std::wstring, SMPDATA>::value_type(_Name, { _Name, m_Type, _Reg }));
}

void HSHADER::CreateTEXDATA(const wchar_t* _Name, UINT _Reg)
{
	if (m_AllTEX.find(_Name) != m_AllTEX.end())
	{
		BOOM;
	}

	if (m_AllTEXIndex.find(_Reg) != m_AllTEXIndex.end())
	{
		BOOM;
	}

	m_AllTEXIndex.insert(_Reg);
	m_AllTEX.insert(std::unordered_map<std::wstring, TEXDATA>::value_type(_Name, { _Name,  m_Type, _Reg }));
}

bool HSHADER::IsOver(HSHADER* _pShader) 
{
	if (_pShader == this)
	{
		return false;
	}

	std::unordered_map<std::wstring, CBDATA>::iterator CBSTART = m_AllCB.begin();
	std::unordered_map<std::wstring, CBDATA>::iterator CBEND = m_AllCB.end();

	std::unordered_map<std::wstring, SMPDATA>::iterator SMPSTART = m_AllSMP.begin();
	std::unordered_map<std::wstring, SMPDATA>::iterator SMPEND = m_AllSMP.end();

	std::unordered_map<std::wstring, TEXDATA>::iterator TEXSTART = m_AllTEX.begin();
	std::unordered_map<std::wstring, TEXDATA>::iterator TEXEND = m_AllTEX.end();

	for (; CBSTART != CBEND; ++CBSTART)
	{
		if (_pShader->m_AllCB.find(CBSTART->first) != _pShader->m_AllCB.end())
		{
			BOOM;
			return true;
		}
	}

	for (; TEXSTART != TEXEND; ++TEXSTART)
	{
		if (_pShader->m_AllTEX.find(TEXSTART->first) != _pShader->m_AllTEX.end())
		{
			BOOM;
			return true;
		}
	}

	for (; SMPSTART != SMPEND; ++SMPSTART)
	{
		if (_pShader->m_AllSMP.find(SMPSTART->first) != _pShader->m_AllSMP.end())
		{
			BOOM;
			return true;
		}
	}

	return false;

}