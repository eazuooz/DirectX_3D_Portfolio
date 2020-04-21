#pragma once
#include "RES.h"

class H3DBUFFER : public Resources
{
protected:
	D3D11_BUFFER_DESC m_Desc;
	ID3D11Buffer* m_Buffer;

public:
	ID3D11Buffer* Buffer() 
	{
		return m_Buffer;
	}

public:
	bool CreateBuffer(unsigned int _Size, D3D11_USAGE _Usage, unsigned int _BindFlag);

public:
	H3DBUFFER();
	~H3DBUFFER();
};

