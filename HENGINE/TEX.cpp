#include "TEX.h"
#include <FILE.h>

Texture::Texture() :m_pRTV(nullptr), m_pDSV(nullptr), m_pSRV(nullptr), m_pTex(nullptr)
{
	Template::MemZero(m_Image);
}

Texture::~Texture()
{
	m_Image.Release();

	// 텍스처가 만들어내고 나머지를 뽑아냈으므로 항상 안전하려면
	// 역순으로
	if (nullptr != m_pRTV) { m_pRTV->Release();		m_pRTV = nullptr; }
	if (nullptr != m_pDSV) { m_pDSV->Release();		m_pDSV = nullptr; }
	if (nullptr != m_pSRV) { m_pSRV->Release();		m_pSRV = nullptr; }
	if (nullptr != m_pTex) { m_pTex->Release();		m_pTex = nullptr; }

}

bool Texture::Load()
{

	// 파일에 제목달아놓는다. 저는 PNG24242424입니다.
	// 이미지파일마다 압축방식.
	// 그나머지에 색깔넣어놓는 형식

	if (nullptr == GraphicDevice::MainDevice())
	{
		BOOM;
		return false;
	}

	if (true == m_File.CheckExtension(L"DDs") || m_File.CheckExtension(L"dds"))
	{
		if (S_OK != DirectX::LoadFromDDSFile(m_File.GetFullPathString().c_str()
			, DirectX::DDS_FLAGS_NONE, nullptr, m_Image))
		{
			BOOM;
			return false;
		}
	}
	else if (true == m_File.CheckExtension(L"TGA") || m_File.CheckExtension(L"tga"))
	{
		if (S_OK != DirectX::LoadFromTGAFile(m_File.GetFullPathString().c_str(), nullptr, m_Image))
		{
			BOOM;
			return false;
		}
	}
	else // if (true == m_File.IsExt(L"Png") || m_File.IsExt(L"png"))
	{
		if (S_OK != DirectX::LoadFromWICFile(m_File.GetFullPathString().c_str()
			, DirectX::WIC_FLAGS_NONE, nullptr, m_Image))
		{
			BOOM;
			return false;
		}
	}

	if (0 == m_Image.GetImageCount())
	{
		BOOM;
		return false;
	}

	// uint8_t

	// m_Image.GetPixels()

	// 쉐이더에 세팅해줄수 있는 권한을 얻어온다.
	if (S_OK != DirectX::CreateShaderResourceView(GraphicDevice::MainDevice()
		, m_Image.GetImages()
		, m_Image.GetImageCount()
		, m_Image.GetMetadata()
		, &m_pSRV))
	{
		BOOM;
		return false;
	}

	m_Width = (UINT)m_Image.GetMetadata().width;
	m_Height = (UINT)m_Image.GetMetadata().height;

	return true;
}

void Texture::Update(HSHADER::SHADERTYPE _ShaderType, int _SetIndex)
{
	if (nullptr == m_pSRV)
	{
		BOOM;
	}

	switch (_ShaderType)
	{
	case HSHADER::SHT_VS:
		GraphicDevice::MainContext()->VSSetShaderResources(_SetIndex, 1, &m_pSRV);
		break;
	case HSHADER::SHT_HS:
		GraphicDevice::MainContext()->HSSetShaderResources(_SetIndex, 1, &m_pSRV);
		break;
	case HSHADER::SHT_DS:
		GraphicDevice::MainContext()->DSSetShaderResources(_SetIndex, 1, &m_pSRV);
		break;
	case HSHADER::SHT_GS:
		GraphicDevice::MainContext()->GSSetShaderResources(_SetIndex, 1, &m_pSRV);
		break;
	case HSHADER::SHT_PS:
		GraphicDevice::MainContext()->PSSetShaderResources(_SetIndex, 1, &m_pSRV);
		break;
	default:
		BOOM;
		break;
	}

}

// Color {
// }

Vector4 Texture::GetColorToFloat(int _X, int _Y)
{
	uint8_t* ColorPtr = m_Image.GetPixels();
	// 픽셀 하나의 크기
	// 4일 것이다.
	// 32
	// 이미지 포맷이 픽셀마다 다를수 있다.
	/*unsigned int Size = (unsigned int)m_Image.GetPixelsSize();
	ColorPtr += (Size * _X) + (Size * _X * _Y);*/

	unsigned int Size = (unsigned int)4;
	ColorPtr += (Size * _X) + (Size * GetWidth() * _Y);

	// 155 / 255
	// rgba값을 확인해서 넣어라.
	Vector4 Return;
	Return.x = ColorPtr[0] / 255.0f;
	Return.y = ColorPtr[1] / 255.0f;
	Return.z = ColorPtr[2] / 255.0f;
	Return.w = ColorPtr[3] / 255.0f;

	return Return;
}



bool Texture::Create(UINT _W, UINT _H, DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _eUsage)
{
	//typedef struct D3D11_TEXTURE2D_DESC1
	//{
	//	UINT Width;
	//	UINT Height;
	//	UINT MipLevels;
	//	UINT ArraySize;
	//	DXGI_FORMAT Format;
	//	DXGI_SAMPLE_DESC SampleDesc;
	//	D3D11_USAGE Usage;
	//	UINT BindFlags;
	//	UINT CPUAccessFlags;
	//	UINT MiscFlags;
	//	D3D11_TEXTURE_LAYOUT TextureLayout;
	//} 	D3D11_TEXTURE2D_DESC1;

	D3D11_TEXTURE2D_DESC Desc = { 0 };
	Desc.Width = _W;
	Desc.Height = _H;
	Desc.ArraySize = 1;
	Desc.Usage = _eUsage;

	// Desc.Usage
	// cpu에서 수정가능
	// gpu에서만 수정가능
	if (Desc.Usage == D3D11_USAGE::D3D11_USAGE_DYNAMIC)
	{
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else {
		Desc.CPUAccessFlags = 0;
	}

	Desc.Format = _Format;
	// 멀티 샘플링 용 숫자와
	// 요녀석은 멀티샘플링 레벨
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.MipLevels = 1;
	Desc.BindFlags = _BindFlag;

	//D3D11 ERROR : ID3D11Device::CreateTexture2D : A D3D11_USAGE_DYNAMIC Resource cannot be bound to certain 
	// parts of the graphics pipeline, but must have at least one BindFlags bit set.The following BindFlags bits(0x28) cannot be set in this case: D3D11_BIND_STREAM_OUTPUT(0), D3D11_BIND_RENDER_Target(1), D3D11_BIND_DEPTH_STENCIL(0), D3D11_BIND_UNORDERED_ACCESS(0).[STATE_CREATION ERROR #99: CREATETEXTURE2D_INVALIDBINDFLAGS]
	//	예외 발생(0x00007FFFB87C9129, HTOOLU.exe) : Microsoft C++ 예외 : _com_error, 메모리 위치 0x0000005CE8AFD878.
	//	D3D11 ERROR : ID3D11Device::CreateTexture2D : Returning E_INVALIDARG, meaning invalid parameters were passed.[STATE_CREATION ERROR #104: CREATETEXTURE2D_INVALIDARG_RETURN]
	//	Assertion failed!

	if (S_OK != GraphicDevice::MainDevice()->CreateTexture2D(&Desc, nullptr, (ID3D11Texture2D**)&m_pTex))
	{
		BOOM;
		return false;
	}

	SettingData(_BindFlag);
	m_Width		= _W;
	m_Height	= _H;
	return true;
}

void Texture::Copy(HPTR<Texture> _Other)
{
	if (nullptr == m_pTex && nullptr == _Other->m_pTex)
	{
		BOOM;
		return;
	}

	GraphicDevice::MainContext()->CopyResource(m_pTex, _Other->m_pTex);
}

void Texture::SettingData(UINT _BindFlag) 
{
	if (nullptr == m_pTex)
	{
		BOOM;
		return;
	}

//	DXGI_FORMAT Format;
//	D3D11_DSV_DIMENSION ViewDimension;
//	UINT Flags;
//	union
//	{
//		D3D11_TEX1D_DSV Texture1D;
//		D3D11_TEX1D_ARRAY_DSV Texture1DArray;
//		D3D11_TEX2D_DSV Texture2D;
//		D3D11_TEX2D_ARRAY_DSV Texture2DArray;
//		D3D11_TEX2DMS_DSV Texture2DMS;
//		D3D11_TEX2DMS_ARRAY_DSV Texture2DMSArray;
//	};
//} 	D3D11_DEPTH_STENCIL_VIEW_DESC;
	// D3D11_TEX2D_ARRAY_DSV

	if (D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL & _BindFlag)
	{
		if (S_OK != GraphicDevice::MainDevice()->CreateDepthStencilView(m_pTex, nullptr, &m_pDSV))
		{
			BOOM;
			return;
		}
	}
	else 
	{
		// D3D11_BIND_FLAG::D3D11_BIND_RENDER_Target D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE
		// 스왑체인을 만들면서 만들어진 랜터타겟은 특별하다.
		if (D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET & _BindFlag)
		{
			if (S_OK != GraphicDevice::MainDevice()->CreateRenderTargetView(m_pTex, nullptr, &m_pRTV))
			{
				BOOM;
				return;
			}
		}

		// 쉐이더에 세팅도 해주겠다.
		if (D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE & _BindFlag)
		{
			if (S_OK != GraphicDevice::MainDevice()->CreateShaderResourceView(m_pTex, nullptr, &m_pSRV))
			{
				BOOM;
				return;
			}
		}
	}

}

void Texture::SetPixel(void* _Data, UINT _ByteSize)
{
	// 예외처리 생각.
	// UINT TexSize = 

	D3D11_MAPPED_SUBRESOURCE MSub;

	GraphicDevice::MainContext()->Map(m_pTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &MSub);
	memcpy_s(MSub.pData, _ByteSize, _Data, _ByteSize);
	GraphicDevice::MainContext()->Unmap(m_pTex, 0);
}

