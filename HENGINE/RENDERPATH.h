#pragma once
#include <vector>
#include "HMESH.h"
#include "HVTXSHADER.h"
#include "HPIXSHADER.h"
#include "TEX.h"
#include "HSMP.h"
#include "HBLEND.h"
#include "HDSSTATE.h"
#include "HRSSTATE.h"
#include "HCB.h"
#include <unordered_map>

// 시점 -> 카메라 

// 매쉬만 있어도 랜더링은 된다.

// 빛이란 결국 색깔에 관련된 연산.
// 색깔 -> (텍스처)

// 매쉬와 

// 랜더패스는 메테리얼 같은 역활로
// 어떤 랜더 세팅을 할것이냐를 처리하게 되는데
// 상수버퍼와 텍스처 이런 세팅들 때문에
// 모든 설정은 같지만 텍스만 다르거나
// 랜더패스가 처리할수 있게 만들고
// 여러분들이 만약 상속을 받거나 그런다면
// 그 기능을 이용해서 만들라는 것.

// 재질에 해당한다. 머티리얼 메티리얼 마테리얼
// 마테리얼 해주는 랜더패스는 복제될 필요는 없다.
enum RENDERTYPE
{
	RT_FORWARD,
	RT_DEFERRED,
};

enum LIGHTMODE
{
	VTX,
	PIX,
};

enum BUMPMODE
{
	BUMPNONE,
	BUMPOK,
};

class Renderer;
class HSHADER;
class Texture;
class RenderPath : public Resources
{
private:
	friend Renderer;

public:
	Renderer* m_Parent;
	RENDERTYPE m_Type;
	LIGHTMODE m_LIGHT;
	BUMPMODE m_BUMP;

private:
	class TextureInfo : public Ref
	{
	public:
		HSHADER::TEXDATA m_Data; 
		HPTR<Texture> m_Tex;
		ID3D11ShaderResourceView* m_pSRV;
		void(TextureInfo::*ShUpdateFunc)();

	public:
		void Init();
		void Update();
		void VSUpdate();
		void HSUpdate();
		void DSUpdate();
		void GSUpdate();
		void PSUpdate();

	public:
		TextureInfo(const HSHADER::TEXDATA& _Data) : m_Data(_Data), m_Tex(nullptr)
		{
		}

		TextureInfo(const HSHADER::TEXDATA& _Data, HPTR<Texture> _Tex) : m_Data(_Data), m_Tex(_Tex)
		{
			Init();
		}
	};

	class ConstBufferInfo : public Ref
	{
	public:
		HSHADER::CBDATA m_Data;
		D3D11_MAPPED_SUBRESOURCE m_tMapSub;
		void(ConstBufferInfo::*ShUpdateFunc)();
		HPTR<H3DBUFFER> m_H3DBUFFER;
		ID3D11Buffer* m_Buffer;

		bool IsChange;
		void* m_SetData;

		void Init();
		void Update();
		void VSUpdate(); 
		void HSUpdate(); 
		void DSUpdate(); 
		void GSUpdate(); 
		void PSUpdate(); 

	public:
		ConstBufferInfo() : IsChange(true) 
		{

		};

		~ConstBufferInfo() 
		{
			if (m_Data.m_CBType == HSHADER::CBTYPE::NEW)
			{
				delete[] (char*)m_SetData;
			}
		};
	};

	class SamplerInfo : public Ref
	{
	public:
		HSHADER::SMPDATA m_Data;
		HPTR<HSMP> m_Smp;
		void(SamplerInfo::*ShUpdateFunc)();
		ID3D11SamplerState* m_pState;

		void Init();
		void Update();
		void VSUpdate();
		void HSUpdate();
		void DSUpdate();
		void GSUpdate();
		void PSUpdate();
	};

private:
	bool m_Original;

	HPTR<HBLEND>				m_BLEND;
	HPTR<HDSSTATE>				m_DSSTATE;
	HPTR<HRSSTATE>				m_RSSTATE;
	D3D11_PRIMITIVE_TOPOLOGY	m_DrawMode;

	std::vector<HSHADER*> m_AllShader;

	// HSHADER::SHADERTYPE;
	// 버텍스 쉐이더의
	// 0번 슬롯에

	// 어떤 상수버퍼를 세팅해
	// 랜더러에 세팅되는 순간에 만들어 져야 한다.
	std::unordered_map<std::wstring, HPTR<ConstBufferInfo>>::iterator CBInfoStart;
	std::unordered_map<std::wstring, HPTR<ConstBufferInfo>>::iterator CBInfoEnd;
	std::unordered_map<std::wstring, HPTR<ConstBufferInfo>> m_CBInfoSheet;

	std::unordered_map<std::wstring, HPTR<TextureInfo>>::iterator TexInfoStart;
	std::unordered_map<std::wstring, HPTR<TextureInfo>>::iterator TexInfoEnd;
	std::unordered_map<std::wstring, HPTR<TextureInfo>> m_TexInfoSheet;

	std::unordered_map<std::wstring, HPTR<SamplerInfo>>::iterator SmpInfoStart;
	std::unordered_map<std::wstring, HPTR<SamplerInfo>>::iterator SmpInfoEnd;
	std::unordered_map<std::wstring, HPTR<SamplerInfo>> m_SmpInfoSheet;

	// 세팅과 연결을 해줘야 합니다.
	// 세팅안해놓거나 연결 안된 녀석이 있으면 터트릴 것이다.

public:
	void SetRenderType(RENDERTYPE _Type) 
	{
		m_Type = _Type;
	}

	bool IsCB(const wchar_t* _Name) 
	{
		return m_CBInfoSheet.end() != m_CBInfoSheet.find(_Name);
	}
	bool IsTEX(const wchar_t* _Name) 
	{
		return m_TexInfoSheet.end() != m_TexInfoSheet.find(_Name);
	}
	bool IsSMP(const wchar_t* _Name) {
		return m_SmpInfoSheet.end() != m_SmpInfoSheet.find(_Name);
	}

	bool IsOriginal() {
		return m_Original;
	}

	void SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY _DrawMode) 
	{
		m_DrawMode = _DrawMode;
	}

	void SetBlend(const wchar_t* _Name);
	void SetDSState(const wchar_t* _Name);
	void SetRSState(const wchar_t* _Name);
	void SetVtxShader(const wchar_t* _Name);
	void SetPxlShader(const wchar_t* _Name);

private:
	void ShaderSetting(HSHADER* _Shader);

public:
	void Init(Renderer* _Parent);

public:
	void Update();
	void LinkCB(const wchar_t* _Name, void* _LinkData);
	void LinkTEX(const wchar_t* _Name, const wchar_t* _TexName);
	void LinkSMP(const wchar_t* _Name, const wchar_t* _SmpName);
	void LinkTEX(const wchar_t* _Name, HPTR<Texture> _Tex);

public:
	HPTR<RenderPath> Clone();

	RenderPath();
	~RenderPath();
};
