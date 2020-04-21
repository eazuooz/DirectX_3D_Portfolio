#pragma once
#include "HSHADER.h"

class HVTXSHADER : public HSHADER
{


private:
	ID3D11VertexShader* m_pVtxShader;

public:
	bool Load(unsigned int _VH, unsigned int _VL, const char* _FuncName);
	void Update() override;


private:
	class INPUTLAYOUT : public Ref
	{
	public:
		UINT m_Offset;
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_InputData;
		

	public:
		INPUTLAYOUT() : m_Offset(0) {}
		~INPUTLAYOUT() 
		{

		}
	};

	ID3D11InputLayout* m_pLayout;
	std::vector<HPTR<INPUTLAYOUT>> m_LayOut;

public:
	void AddLayOut(
		const char* _SmName, 
		unsigned int _Index,
		DXGI_FORMAT _Format,
		unsigned int _inputSlot,
		unsigned int _IDSR, 
		D3D11_INPUT_CLASSIFICATION _Input = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA
		);

	void EndLayOut(
		const char* _SmName,
		unsigned int _Index,
		DXGI_FORMAT _Format,
		unsigned int _inputSlot,
		unsigned int _IDSR,
		D3D11_INPUT_CLASSIFICATION _Input = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA
	);

public:
	HVTXSHADER();
	~HVTXSHADER();
};

