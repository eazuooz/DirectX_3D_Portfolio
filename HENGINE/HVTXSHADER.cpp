#include "HVTXSHADER.h"



HVTXSHADER::HVTXSHADER() : HSHADER(HSHADER::SHADERTYPE::SHT_VS), m_pVtxShader(nullptr)
{
	// m_LayOut = new INPUTLAYOUT();
}


HVTXSHADER::~HVTXSHADER()
{
	if (nullptr != m_pLayout)
	{
		m_pLayout->Release();
		m_pLayout = nullptr;
	}

	if (nullptr != m_pVtxShader)
	{
		m_pVtxShader->Release();
		m_pVtxShader = nullptr;
	}
}

bool HVTXSHADER::Load(unsigned int _VH, unsigned int _VL, const char* _FuncName)
{
	m_VH = _VH;
	m_VL = _VL;

	unsigned int _Flag = D3D10_SHADER_DEBUG;

#ifdef _DEBUG 
	_Flag = D3D10_SHADER_DEBUG;
#endif

	sprintf_s(VERBUFFERTEXT, "vs_%d_%d", m_VH, m_VL);


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
		GraphicDevice::MainDevice()->CreateVertexShader
		(
			m_pBlob->GetBufferPointer(),
			m_pBlob->GetBufferSize(),
			nullptr,
			&m_pVtxShader
		)
		)
	{
		BOOM;
		return false;
	}

	return true;
}

void HVTXSHADER::Update()
{
	if (nullptr != m_pLayout)
	{
		GraphicDevice::MainContext()->IASetInputLayout(m_pLayout);
	}

	if (nullptr != m_pVtxShader)
	{
		GraphicDevice::MainContext()->VSSetShader(m_pVtxShader, nullptr, 0);
	}
}

void HVTXSHADER::AddLayOut(
	const char* _SmName,
	unsigned int _Index,
	DXGI_FORMAT _Format,
	unsigned int _inputSlot,
	unsigned int _IDSR,
	D3D11_INPUT_CLASSIFICATION _Input
)
{
	if (m_LayOut.size() <= _inputSlot)
	{
		m_LayOut.resize(_inputSlot + 1);
		m_LayOut[_inputSlot] = new INPUTLAYOUT();
	}

	D3D11_INPUT_ELEMENT_DESC InputData;

	InputData.SemanticName = _SmName;
	InputData.SemanticIndex = _Index;
	InputData.Format = _Format;
	InputData.InputSlot = _inputSlot;
	InputData.InputSlotClass = _Input;
	InputData.InstanceDataStepRate = _IDSR;
	InputData.AlignedByteOffset = m_LayOut[_inputSlot]->m_Offset;

	unsigned int Size = SizeOfFormat(InputData.Format);

	if (0 == Size)
	{
		BOOM;
		return;
	}

	m_LayOut[_inputSlot]->m_Offset += Size;
	m_LayOut[_inputSlot]->m_InputData.push_back(InputData);
}


void HVTXSHADER::EndLayOut(
	const char* _SmName,
	unsigned int _Index,
	DXGI_FORMAT _Format,
	unsigned int _inputSlot,
	unsigned int _IDSR,
	D3D11_INPUT_CLASSIFICATION _Input
)
{
	AddLayOut(_SmName, _Index, _Format, _inputSlot, _IDSR, _Input);

	std::vector<D3D11_INPUT_ELEMENT_DESC> AllLayOut;

	for (size_t i = 0; i < m_LayOut.size(); i++)
	{
		for (size_t j = 0; j < m_LayOut[i]->m_InputData.size(); j++)
		{
			AllLayOut.push_back(m_LayOut[i]->m_InputData[j]);
		}
	}


	if (S_OK != GraphicDevice::MainDevice()->CreateInputLayout(&AllLayOut[0], (int)AllLayOut.size(), m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), &m_pLayout))
	{
		BOOM;
		return;
	}

	// m_LayOut.clear();
}