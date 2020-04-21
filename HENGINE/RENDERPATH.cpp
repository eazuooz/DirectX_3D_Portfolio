#include "RENDERPATH.h"
#include "RENDER.h"
#include "TEX.h"
#include "HSHADER.h"
#include "H3DBUFFER.h"

void RenderPath::SamplerInfo::Init() 
{
	switch (m_Data.m_Shader)
	{
	case HSHADER::SHT_VS:
		ShUpdateFunc = &SamplerInfo::VSUpdate;
		break;
	case HSHADER::SHT_HS:
		ShUpdateFunc = &SamplerInfo::HSUpdate;
		break;
	case HSHADER::SHT_DS:
		ShUpdateFunc = &SamplerInfo::DSUpdate;
		break;
	case HSHADER::SHT_GS:
		ShUpdateFunc = &SamplerInfo::GSUpdate;
		break;
	case HSHADER::SHT_PS:
		ShUpdateFunc = &SamplerInfo::PSUpdate;
		break;
	case HSHADER::SHT_END:
		break;
	default:
		break;
	}
}

void RenderPath::SamplerInfo::Update() 
{
	(this->*ShUpdateFunc)();
}

void RenderPath::SamplerInfo::VSUpdate() { GraphicDevice::MainContext()->VSSetSamplers(m_Data.m_uiReg, 1, &m_pState);  }
void RenderPath::SamplerInfo::HSUpdate() { GraphicDevice::MainContext()->HSSetSamplers(m_Data.m_uiReg, 1, &m_pState); }
void RenderPath::SamplerInfo::DSUpdate() { GraphicDevice::MainContext()->DSSetSamplers(m_Data.m_uiReg, 1, &m_pState); }
void RenderPath::SamplerInfo::GSUpdate() { GraphicDevice::MainContext()->GSSetSamplers(m_Data.m_uiReg, 1, &m_pState); }
void RenderPath::SamplerInfo::PSUpdate() { 
	GraphicDevice::MainContext()->PSSetSamplers(m_Data.m_uiReg, 1, &m_pState); 
}

void RenderPath::TextureInfo::Init() 
{
	switch (m_Data.m_Shader)
	{
	case HSHADER::SHT_VS:
		ShUpdateFunc = &TextureInfo::VSUpdate;
		break;
	case HSHADER::SHT_HS:
		ShUpdateFunc = &TextureInfo::HSUpdate;
		break;
	case HSHADER::SHT_DS:
		ShUpdateFunc = &TextureInfo::DSUpdate;
		break;
	case HSHADER::SHT_GS:
		ShUpdateFunc = &TextureInfo::GSUpdate;
		break;
	case HSHADER::SHT_PS:
		ShUpdateFunc = &TextureInfo::PSUpdate;
		break;
	case HSHADER::SHT_END:
		break;
	default:
		break;
	}

	// m_pSRV = m_Tex->SRV();
}

void RenderPath::TextureInfo::Update() 
{
	(this->*ShUpdateFunc)();
}
void RenderPath::TextureInfo::VSUpdate() { GraphicDevice::MainContext()->VSSetShaderResources(m_Data.m_uiReg, 1, &m_pSRV);}
void RenderPath::TextureInfo::HSUpdate() { GraphicDevice::MainContext()->HSSetShaderResources(m_Data.m_uiReg, 1, &m_pSRV);}
void RenderPath::TextureInfo::DSUpdate() { GraphicDevice::MainContext()->DSSetShaderResources(m_Data.m_uiReg, 1, &m_pSRV);}
void RenderPath::TextureInfo::GSUpdate() { GraphicDevice::MainContext()->GSSetShaderResources(m_Data.m_uiReg, 1, &m_pSRV);}
void RenderPath::TextureInfo::PSUpdate() { GraphicDevice::MainContext()->PSSetShaderResources(m_Data.m_uiReg, 1, &m_pSRV);}

void RenderPath::ConstBufferInfo::Init()
{
	switch (m_Data.m_Shader)
	{
	case HSHADER::SHT_VS:
		ShUpdateFunc = &ConstBufferInfo::VSUpdate;
		break;
	case HSHADER::SHT_HS:
		ShUpdateFunc = &ConstBufferInfo::HSUpdate;
		break;
	case HSHADER::SHT_DS:
		ShUpdateFunc = &ConstBufferInfo::DSUpdate;
		break;
	case HSHADER::SHT_GS:
		ShUpdateFunc = &ConstBufferInfo::GSUpdate;
		break;
	case HSHADER::SHT_PS:
		ShUpdateFunc = &ConstBufferInfo::PSUpdate;
		break;
	case HSHADER::SHT_END:
		break;
	default:
		break;
	}
}

void RenderPath::ConstBufferInfo::Update() 
{
	// 잠깐 풀어달라.
	
	if (nullptr != ShUpdateFunc)
	{
		// 메모리적으로 봤을때 이게 부담이 되나?
		// 4기가
		// 이 버퍼에 세팅해서 랜더링할게 있다 잠깐 열어 달라.
		// 자기 버퍼를 무조건 가지게 할지 생각해 보자.
		// 이걸 덜 하려고 만든게 인스턴싱이다.
		// 카메라가 계속 움직이기 때문에. 
		//if (true == IsChange)
		//{
		//if (m_uiReg == 0)
		//{
		//	int a = 0;
		//}

		if (m_SetData == nullptr)
		{
			int a = 0;
		}

			GraphicDevice::MainContext()->Map(m_Buffer,
				0, D3D11_MAP_WRITE_DISCARD, 0, &m_tMapSub);
			memcpy_s(m_tMapSub.pData, m_Data.m_Size, m_SetData, m_Data.m_Size);
			GraphicDevice::MainContext()->Unmap(m_Buffer, 0);
			IsChange = false;
		//}

		(this->*ShUpdateFunc)();
	}
}

void RenderPath::ConstBufferInfo::VSUpdate() { GraphicDevice::MainContext()->VSSetConstantBuffers(m_Data.m_uiReg, 1, &m_Buffer); }
void RenderPath::ConstBufferInfo::HSUpdate() { GraphicDevice::MainContext()->HSSetConstantBuffers(m_Data.m_uiReg, 1, &m_Buffer); }
void RenderPath::ConstBufferInfo::DSUpdate() { GraphicDevice::MainContext()->DSSetConstantBuffers(m_Data.m_uiReg, 1, &m_Buffer); }
void RenderPath::ConstBufferInfo::GSUpdate() { GraphicDevice::MainContext()->GSSetConstantBuffers(m_Data.m_uiReg, 1, &m_Buffer); }
void RenderPath::ConstBufferInfo::PSUpdate() { GraphicDevice::MainContext()->PSSetConstantBuffers(m_Data.m_uiReg, 1, &m_Buffer); }


RenderPath::RenderPath() : m_Type(RT_FORWARD), m_LIGHT(PIX), m_BUMP(BUMPNONE)
{
	m_Original = true;
	m_AllShader.resize(HSHADER::SHADERTYPE::SHT_END);
}


RenderPath::~RenderPath()
{

}

void RenderPath::Init(Renderer* _Parent) 
{
	m_Parent = _Parent;

	for (size_t i = 0; i < m_AllShader.size(); i++)
	{
		if (nullptr == m_AllShader[i])
		{
			continue;
		}

		if (0 != m_CBInfoSheet.size())
		{
			CBInfoStart = m_CBInfoSheet.begin();
			CBInfoEnd = m_CBInfoSheet.end();

			for (; CBInfoStart != CBInfoEnd; ++CBInfoStart)
			{
				if (CBInfoStart->second->m_Data.m_CBType == HSHADER::CBTYPE::TRANS)
				{
					CBInfoStart->second->m_SetData = (void*)&m_Parent->GetMatrixData();
				}
				//else if (CBInfoStart->second->m_Data.m_CBType == HSHADER::CBTYPE::NONE)
				//{

				//	CBInfoStart->second->m_SetData = (void*)&m_Parent->GetTransform();
				//}
			}
		}
	}


}

void RenderPath::SetBlend(const wchar_t* _Name) 
{
	m_BLEND = Resources::Find<HBLEND>(_Name);

	if (nullptr == m_BLEND)
	{
		BOOM;
	}

}
void RenderPath::SetDSState(const wchar_t* _Name) 
{
	m_DSSTATE = Resources::Find<HDSSTATE>(_Name);

	if (nullptr == m_DSSTATE)
	{
		BOOM;
	}
}

void RenderPath::SetRSState(const wchar_t* _Name) 
{
	m_RSSTATE = Resources::Find<HRSSTATE>(_Name);

	if (nullptr == m_RSSTATE)
	{
		BOOM;
	}
}

void RenderPath::ShaderSetting(HSHADER* _Shader)
{
	{
		auto Start = _Shader->AllCB().cbegin();
		auto End = _Shader->AllCB().cend();

		for (; Start != End; ++Start)
		{
			ConstBufferInfo* NewCBSET = new ConstBufferInfo();
			NewCBSET->m_Data = Start->second;
			NewCBSET->m_H3DBUFFER = new H3DBUFFER();
			if (false == NewCBSET->m_H3DBUFFER->CreateBuffer(NewCBSET->m_Data.m_Size, D3D11_USAGE::D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER))
			{
				BOOM;
			}
			NewCBSET->m_Buffer = NewCBSET->m_H3DBUFFER->Buffer();
			NewCBSET->m_SetData = nullptr;
			NewCBSET->Init();

			if (m_CBInfoSheet.end() != m_CBInfoSheet.find(NewCBSET->m_Data.m_Name))
			{
				BOOM;
				continue;
			}

			m_CBInfoSheet.insert(std::unordered_map<std::wstring, HPTR<ConstBufferInfo>>::value_type(NewCBSET->m_Data.m_Name, NewCBSET));
		}
	}

	{
		auto Start = _Shader->AllTEX().cbegin();
		auto End = _Shader->AllTEX().cend();

		for (; Start != End; ++Start)
		{
			TextureInfo* NewSET = new TextureInfo(Start->second);
			NewSET->m_Tex = nullptr;
			NewSET->Init();

			if (m_TexInfoSheet.end() != m_TexInfoSheet.find(NewSET->m_Data.m_Name))
			{
				BOOM;
				continue;
			}

			m_TexInfoSheet.insert(std::unordered_map<std::wstring, HPTR<TextureInfo>>::value_type(NewSET->m_Data.m_Name, NewSET));
		}
	}

	{
		auto Start = _Shader->AllSMP().cbegin();
		auto End = _Shader->AllSMP().cend();

		for (; Start != End; ++Start)
		{
			SamplerInfo* NewSET = new SamplerInfo();
			NewSET->m_Data = Start->second;
			NewSET->m_Smp = nullptr;
			NewSET->Init();

			if (m_SmpInfoSheet.end() != m_SmpInfoSheet.find(NewSET->m_Data.m_Name))
			{
				BOOM;
				continue;
			}

			m_SmpInfoSheet.insert(std::unordered_map<std::wstring, HPTR<SamplerInfo>>::value_type(NewSET->m_Data.m_Name, NewSET));
		}
	}

}

void RenderPath::SetVtxShader(const wchar_t* _Name) 
{
	HPTR<HVTXSHADER> SHADER = Resources::Find<HVTXSHADER>(_Name);

	if (nullptr == SHADER)
	{
		BOOM;
	}

	if (nullptr != m_AllShader[HSHADER::SHADERTYPE::SHT_VS])
	{
		BOOM;
	}

	for (size_t i = 0; i < HSHADER::SHADERTYPE::SHT_END; i++)
	{
		if (nullptr == m_AllShader[i])
		{
			continue;
		}

		if (HSHADER::SHADERTYPE::SHT_VS == i)
		{
			continue;
		}

		if (true == m_AllShader[i]->IsOver(SHADER))
		{
			BOOM;
		}
	}

	ShaderSetting(SHADER.Convert<HSHADER>());
	m_AllShader[HSHADER::SHADERTYPE::SHT_VS] = SHADER;
}

void RenderPath::SetPxlShader(const wchar_t* _Name) 
{
	HPTR<HPIXSHADER> SHADER = Resources::Find<HPIXSHADER>(_Name);

	if (nullptr == SHADER)
	{
		BOOM;
	}

	if (nullptr != m_AllShader[HSHADER::SHADERTYPE::SHT_PS])
	{
		BOOM;
	}

	for (size_t i = 0; i < HSHADER::SHADERTYPE::SHT_END; i++)
	{
		if (nullptr == m_AllShader[i])
		{
			continue;
		}

		if (HSHADER::SHADERTYPE::SHT_PS == i)
		{
			continue;
		}

		if (true == m_AllShader[i]->IsOver(SHADER))
		{
			BOOM;
		}
	}

	ShaderSetting(SHADER.Convert<HSHADER>());
	m_AllShader[HSHADER::SHADERTYPE::SHT_PS] = SHADER;
}


void RenderPath::Update() 
{
	if (nullptr != m_BLEND)
	{
		m_BLEND->Update();
	}

	if (nullptr != m_DSSTATE)
	{
		m_DSSTATE->Update();
	}

	if (nullptr != m_RSSTATE)
	{
		m_RSSTATE->Update();
	}

	for (size_t i = 0; i < m_AllShader.size(); i++)
	{
		if (nullptr == m_AllShader[i])
		{
			continue;
		}

		m_AllShader[i]->Update();
	}

	if (0 != m_CBInfoSheet.size())
	{
		CBInfoStart = m_CBInfoSheet.begin();
		CBInfoEnd = m_CBInfoSheet.end();

		for (; CBInfoStart != CBInfoEnd; ++CBInfoStart)
		{
			CBInfoStart->second->Update();
		}
	}

	if (0 != m_TexInfoSheet.size())
	{
		TexInfoStart = m_TexInfoSheet.begin();
		TexInfoEnd = m_TexInfoSheet.end();

		for (; TexInfoStart != TexInfoEnd; ++TexInfoStart)
		{
			TexInfoStart->second->Update();
		}
	}

	if (0 != m_SmpInfoSheet.size())
	{
		SmpInfoStart = m_SmpInfoSheet.begin();
		SmpInfoEnd = m_SmpInfoSheet.end();

		for (; SmpInfoStart != SmpInfoEnd; ++SmpInfoStart)
		{
			SmpInfoStart->second->Update();
		}
	}

	GraphicDevice::MainContext()->IASetPrimitiveTopology(m_DrawMode);

}

HPTR<RenderPath> RenderPath::Clone()
{
	HPTR<RenderPath> NewPath = new RenderPath();

	NewPath->m_Original = false;
	NewPath->m_Type = m_Type;
	NewPath->m_BLEND = m_BLEND;
	NewPath->m_DSSTATE = m_DSSTATE;
	NewPath->m_RSSTATE = m_RSSTATE;
	NewPath->m_AllShader = m_AllShader;
	NewPath->m_DrawMode = m_DrawMode;

	{
		NewPath->m_TexInfoSheet = m_TexInfoSheet;
		if (0 != m_TexInfoSheet.size())
		{
			NewPath->TexInfoStart = NewPath->m_TexInfoSheet.begin();
			NewPath->TexInfoEnd = NewPath->m_TexInfoSheet.end();

			for (; NewPath->TexInfoStart != NewPath->TexInfoEnd; ++NewPath->TexInfoStart)
			{
				TextureInfo* NewSET = new TextureInfo(NewPath->TexInfoStart->second->m_Data);
				NewSET->m_Tex = nullptr;
				NewSET->Init();

				NewPath->TexInfoStart->second = NewSET;
			}
		}
	}


	{
		NewPath->m_CBInfoSheet = m_CBInfoSheet;
		if (0 != m_CBInfoSheet.size())
		{
			NewPath->CBInfoStart = NewPath->m_CBInfoSheet.begin();
			NewPath->CBInfoEnd = NewPath->m_CBInfoSheet.end();

			for (; NewPath->CBInfoStart != NewPath->CBInfoEnd; ++NewPath->CBInfoStart)
			{
				ConstBufferInfo* NewCBSET = new ConstBufferInfo();
				NewCBSET->m_Data = NewPath->CBInfoStart->second->m_Data;
				NewCBSET->m_H3DBUFFER = new H3DBUFFER();
				if (false == NewCBSET->m_H3DBUFFER->CreateBuffer(NewCBSET->m_Data.m_Size, D3D11_USAGE::D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER))
				{
					BOOM;
				}
				NewCBSET->m_Buffer = NewCBSET->m_H3DBUFFER->Buffer();
				NewCBSET->m_SetData = nullptr;
				NewCBSET->Init();

				NewPath->CBInfoStart->second = NewCBSET;
			}
		}
	}


	{
		NewPath->m_SmpInfoSheet = m_SmpInfoSheet;
		if (0 != m_SmpInfoSheet.size())
		{
			NewPath->SmpInfoStart = NewPath->m_SmpInfoSheet.begin();
			NewPath->SmpInfoEnd = NewPath->m_SmpInfoSheet.end();

			for (; NewPath->SmpInfoStart != NewPath->SmpInfoEnd; ++NewPath->SmpInfoStart)
			{
				SamplerInfo* NewSET = new SamplerInfo();
				NewSET->m_Data = NewPath->SmpInfoStart->second->m_Data;
				NewSET->m_Smp = nullptr;
				NewSET->Init();

				NewPath->SmpInfoStart->second = NewSET;
			}
		}
	}


	return NewPath;
}



void RenderPath::LinkCB(const wchar_t* _Name, void* _LinkData) {
	CBInfoStart = m_CBInfoSheet.find(_Name);
	CBInfoStart->second->m_Data.m_CBType = HSHADER::CBTYPE::LINK;
	CBInfoStart->second->m_SetData = _LinkData;
}

void RenderPath::LinkTEX(const wchar_t* _Name, const wchar_t* _TexName)
{
	if (false == IsTEX(_Name))
	{
		BOOM;
	}

	TexInfoStart = m_TexInfoSheet.find(_Name);
	TexInfoStart->second->m_Tex = Resources::Find<Texture>(_TexName);
	TexInfoStart->second->m_pSRV = TexInfoStart->second->m_Tex->GetSRV();
}

void RenderPath::LinkTEX(const wchar_t* _Name, HPTR<Texture> _Tex) 
{
	if (false == IsTEX(_Name))
	{
		BOOM;
	}

	if (nullptr == _Tex)
	{
		BOOM;
	}

	TexInfoStart = m_TexInfoSheet.find(_Name);
	TexInfoStart->second->m_Tex = _Tex;
	TexInfoStart->second->m_pSRV = TexInfoStart->second->m_Tex->GetSRV();
}

void RenderPath::LinkSMP(const wchar_t* _Name, const wchar_t* _SmpName)
{
	if (false == IsSMP(_Name))
	{
		BOOM;
	}

	SmpInfoStart = m_SmpInfoSheet.find(_Name);
	SmpInfoStart->second->m_Smp = Resources::Find<HSMP>(_SmpName);
	SmpInfoStart->second->m_pState = SmpInfoStart->second->m_Smp->State();
}