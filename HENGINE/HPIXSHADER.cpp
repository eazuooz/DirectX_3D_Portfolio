#include "HPIXSHADER.h"



HPIXSHADER::HPIXSHADER() : HSHADER(HSHADER::SHADERTYPE::SHT_PS), m_TargetCount(1)
{
}


HPIXSHADER::~HPIXSHADER()
{
	if (nullptr != m_pPixShader)
	{
		m_pPixShader->Release();
		m_pPixShader = nullptr;
	}
}


bool HPIXSHADER::Load(unsigned int _VH, unsigned int _VL, const char* _FuncName)
{
	m_VH = _VH;
	m_VL = _VL;

	unsigned int _Flag = D3D10_SHADER_DEBUG;

#ifdef _DEBUG 
	_Flag = D3D10_SHADER_DEBUG;
#endif

	sprintf_s(VERBUFFERTEXT, "ps_%d_%d", m_VH, m_VL);


	// 여기단계에서부터 좀 관리를 해줘야 할것으로 보인다.
	if (
		S_OK != D3DCompileFromFile
		(
			m_File.GetFullPath(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			_FuncName,
			VERBUFFERTEXT,
			_Flag, // 디버그모드일때 디버그 모드라는 것을 알려준다.
			0, // 뭔지도 모르겠음
			&m_pBlob,
			&m_pErrBlob
		)
		)
	{
		std::string ErrorText = (char*)m_pErrBlob->GetBufferPointer();

		Debug::ConsolePrintA(ErrorText.c_str());
		BOOM;
		return false;
	}

	if (
		GraphicDevice::MainDevice()->CreatePixelShader
		(
			m_pBlob->GetBufferPointer(),
			m_pBlob->GetBufferSize(),
			nullptr,
			&m_pPixShader
		)
		)
	{
		BOOM;
		return false;
	}

	return true;
}

void HPIXSHADER::Update()
{
	if (nullptr != m_pPixShader)
	{
		GraphicDevice::MainContext()->PSSetShader(m_pPixShader, nullptr, 0);
	}
}