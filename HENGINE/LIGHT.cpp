#include "LIGHT.h"
#include "SCENE.h"
#include "HRENDERTARGET.h"
#include "HDEPTHBUFFER.h"


Light::Light()
{
	m_LightData.Color = Vector4::WHITE;
	m_LightData.DiffPower = 1.0f;
	m_LightData.SpecPower = 1.0F;
	SetLightType(LT_DIR);
	m_LightData.Range = 20.0F;
	m_LightData.SpecPow = 1;

	memset(&m_ShadowData, 0, sizeof(m_ShadowData));


	m_ShadowTarget = new HRENDERTARGET();
	m_ShadowTarget->Create(
		{ 0.0f, 0.0f, 0.0f, 0.0f } // 이 타겟이 클리어타겟 될때 색깔
		, (UINT)4096 // 너비
		, (UINT)4096 // 높이
		, DXGI_FORMAT_R32_FLOAT // 색깔 형태
		, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);


	/*m_BoneTex = new Texture();
	m_BoneTex->Create((int)m_FBXDATA->BoneData().size() * 4, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DYNAMIC);*/



	m_DepthBuffer = new HDEPTHBUFFER();
	m_DepthBuffer->Create( (UINT)4096 , (UINT)4096 , DXGI_FORMAT_D24_UNORM_S8_UINT);


	Template::MemZero(m_LightViewPort);
	m_LightViewPort.TopLeftX = 0;
	m_LightViewPort.TopLeftY = 0;
	m_LightViewPort.Width = 4096;
	m_LightViewPort.Height = 4096;
	m_LightViewPort.MinDepth = 0.0f;
	m_LightViewPort.MaxDepth = 1.0f;
}


Light::~Light()
{
}

void Light::RenderBefore()
{
	Vector4 Pos = GetActor()->GetTransform()->GetPosition();
	Vector4 For = GetActor()->GetTransform()->LookForward();
	Vector4 Up = GetActor()->GetTransform()->LookUp();

	// 트랜스 폼은 무조건 들어가 있네요????


	m_VIEW.SetViewMatrix(GetActor()->GetTransform()->GetPosition(), GetActor()->GetTransform()->LookForward(), GetActor()->GetTransform()->LookUp());
	// m_VIEW.VIEWTOLH(Actor()->Trans()->WPos(), Actor()->Trans()->WForward(), HVEC4());
	// 윈도우의 비율을 가져오던가 해야한다.

	m_Near = 0.1f;
	m_Far = 10000.f;
	m_Width = 4096.0f;
	m_Height = 4096.0f;

	float fWidth = m_Width * 0.5f * 1.0f;
	float fHeight = m_Height * 0.5f * 1.0f;

	m_PROJ = DirectX::XMMatrixOrthographicOffCenterLH(-fWidth, fWidth, -fHeight, fHeight
		, m_Near, m_Far);

	// m_PROJ.ORTHTOLH(m_Width, m_Height, m_Near, m_Far);
	// ViewCamPos = m_VIEW.MulOne(Pos); // 이것도 실수함
	m_VP = m_VIEW * m_PROJ;
	m_ShadowData.LightVP = m_VP;
	m_ShadowData.LightVP.TransPose();
}

void Light::Update() 
{
	// X크기가 빛의 레인지.
	m_LightData.LightPos = GetTransform()->GetPosition();
	m_LightData.LightDir = GetTransform()->LookForward();
	m_LightData.LightDirInv = GetTransform()->LookBack();
}

void Light::Init()
{
	
}

void Light::PushRenderGroup() 
{
	GetScene()->RMGR.PushLight(this);
}

void Light::ShadowUpdate()
{
	if (nullptr != GraphicDevice::MainContext())
	{
		// m_CamTarget->Clear();
		GraphicDevice::MainContext()->RSSetViewports(1, &m_LightViewPort);
		// 이걸 계속 바꿔줘야 한다.
	}

	m_ShadowTarget->Clear();
	m_DepthBuffer->Clear();

	m_ShadowTarget->OmSet(m_DepthBuffer->DSV());
}