#pragma once
#include "GRAPHICDEVICE.h"
#include "H3DBUFFER.h"
#include "HSHADER.h"

class HCB : public H3DBUFFER
{
private:
	bool m_bLink;

	HSHADER::SHADERTYPE m_Shader;
	unsigned int m_uiReg;
	UINT BufferSize;
	D3D11_SUBRESOURCE_DATA m_tSub;
	D3D11_MAPPED_SUBRESOURCE m_tMapSub;
	// 상수버퍼에서 사용될 실제데이터는 이녀석이다.
	void* m_Data;
	void(HCB::*ShUpdateFunc)();

public:
	void Reg(unsigned int _uiReg) 
	{
		m_uiReg = _uiReg;
	}

	int Reg()
	{
		return m_uiReg;
	}

	//HSHADER::SHADERTYPE SHADERTYPE(HSHADER::SHADERTYPE _uiReg)
	//{
	//	m_uiReg = _uiReg;
	//}

	int SHADERTYPE()
	{
		return m_uiReg;
	}

public:
	bool Create(HSHADER::SHADERTYPE _Shader, size_t _Byte, UINT _Reg);

public:
	void Link(void* _Data, unsigned int _Size);

private:
	void VSUpdate();
	void HSUpdate();
	void DSUpdate();
	void GSUpdate();
	void PSUpdate();

public:
	void DataSetting(void* _pData, UINT _Size);
	void DataSetting();
	void Update();


public:
	HCB();
	~HCB();
};
