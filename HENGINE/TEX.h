#pragma once
#include "RES.h"
#include "HSHADER.h"
#include <Windows.h>
#include <DirectXTex.h>


#ifdef X32
#ifdef DEBUGMODE
#pragma comment(lib, "DirectXTexD32.lib")
#elif RELEASEMODE
#pragma comment(lib, "DirectXTexR32.lib")
#endif
#elif X64
#ifdef DEBUGMODE
#pragma comment(lib, "DirectXTexD64.lib")
#elif RELEASEMODE
#pragma comment(lib, "DirectXTexR64.lib")
#endif
#endif

class Texture : public Resources
{

private:
	// 이게 일단 지원
	DirectX::ScratchImage m_Image;
	UINT m_Width;
	UINT m_Height;

	ID3D11Resource*		  m_pTex;

	// 랜더타겟 뷰는?????
	ID3D11RenderTargetView* m_pRTV;   // 도화지 
	ID3D11DepthStencilView* m_pDSV;   // 여기에 깊이버퍼를 만들수도 있다.
	ID3D11ShaderResourceView* m_pSRV; // 쉐이더에 세팅해줄때 필요한 인터페이스

public:
	ID3D11RenderTargetView* GetRTV() {	return m_pRTV;	}
	ID3D11DepthStencilView* GetDSV() { return m_pDSV; }
	ID3D11ShaderResourceView* GetSRV() { return m_pSRV; }

public:
	UINT GetWidth() {
		return m_Width;
	}

	UINT GetHeight() {
		return m_Height;
	}

public:
	void Update(HSHADER::SHADERTYPE _ShaderType, int _SetIndex);

	// class 
	// {
	// char a;
	// char r;
	// char g;
	// char b;
	// };
	// HVEC4 GetColorToByte(int _X, int _Y);
	Vector4 GetColorToFloat(int _X, int _Y);


	// 이 랜더타겟을 OMSETRENDERTargetVIEW를 찍었는데.
	// 아직 빼지 않았는데. 쉐이더 집어넣겠다는 것은 불가능하다는 이야기다.
	//ID3D11RenderTargetView* m_pRTV;   // 도화지 
	//ID3D11DepthStencilView* m_pDSV;   // 여기에 깊이버퍼를 만들수도 있다.
	//ID3D11ShaderResourceView* m_pSRV; // 쉐이더에 세팅해줄때 필요한 인터페이스

public:
	bool Load();
	bool Create(UINT _W, UINT _H, DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _eUsage);
	void SetPixel(void* _Data, UINT _ByteSize);
	void Copy(HPTR<Texture> _Other);

private:

	void SettingData(UINT _BindFlag);

public:
	Texture();
	~Texture();
};

