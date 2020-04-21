#include "CAMERA.h"
#include "TRANS.h"
#include "SCENE.h"
#include "SPRITE.h"
#include "GAMEWIN.h"
#include "HRENDERTARGET.h"
#include "HDEPTHBUFFER.h"
#include "HMULTIRENDERTarget.h"
#include "LIGHT.h"

Light::ShadowData SData;
Camera* Camera::m_MainCam = nullptr;
Camera* Camera::m_UICam = nullptr;

Camera::Camera() : m_IsAllView(false), m_Order(0), m_Fov(60.0f), m_Near(1.0f), m_Far(1000.0f)
{
	
}


Camera::~Camera()
{
}

// 캠을 가진 엑터는 내부에 transform과 cam

void Camera::Init() 
{
	// 트랜스 폼이 없으면 나는 
	if (nullptr == GetActor()->GetTransform())
	{
		BOOM;
	}

	if (nullptr == m_MainCam)
	{
		m_MainCam = this;
	}

	m_Height = GetActor()->GetScene()->Window()->Size().y;
	m_Width = GetActor()->GetScene()->Window()->Size().x;

	Template::MemZero(m_CamViewPort);
	m_CamViewPort.TopLeftX = 0;
	m_CamViewPort.TopLeftY = 0;
	m_CamViewPort.Width = m_Width;
	m_CamViewPort.Height = m_Height;
	m_CamViewPort.MinDepth = 0.0f;
	m_CamViewPort.MaxDepth = 1.0f;

	// 랜더타겟 뷰를 몇개 가지고 있나요????
	// 1개 가지고 있다.
	{
		m_CamTarget = new HRENDERTARGET();
		m_CamTarget->Create(
			{ 0.0f, 0.0f, 0.0f, 0.0f } // 이 타겟이 클리어타겟 될때 색깔
			, (UINT)m_Width // 너비
			, (UINT)m_Height // 높이
			, DXGI_FORMAT_R32G32B32A32_FLOAT // 색깔 형태
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	}

	{
		m_DepthBuffer = new HDEPTHBUFFER();
		m_DepthBuffer->Create(
			(UINT)m_Width // 너비
			, (UINT)m_Height
			, DXGI_FORMAT_D24_UNORM_S8_UINT// 색깔 형태
		);
	}

	{
		m_DEFERREDTarget = new HMULTIRENDERTarget();
		m_DEFERREDTarget->CreateTarget((UINT)m_Width, (UINT)m_Height); // 디퓨즈.
		m_DEFERREDTarget->CreateTarget((UINT)m_Width, (UINT)m_Height); // 포지션.
		m_DEFERREDTarget->CreateTarget((UINT)m_Width, (UINT)m_Height); // 노말.
		m_DEFERREDTarget->CreateTarget((UINT)m_Width, (UINT)m_Height); // 깊이.
	
		m_DEFERREDTarget->Depth(m_DepthBuffer);

		m_DEFERREDTarget->CreateTarget((UINT)m_Width, (UINT)m_Height); // TEST
		m_DEFERREDTarget->CreateTarget((UINT)m_Width, (UINT)m_Height); // TEST2
	}

	{
		// 뎁스스테이트
		m_LightTarget = new HMULTIRENDERTarget();
		m_LightTarget->CreateTarget((UINT)m_Width, (UINT)m_Height); // 디퓨즈
		m_LightTarget->CreateTarget((UINT)m_Width, (UINT)m_Height); // 스펙큘러
		m_LightTarget->Depth(m_DepthBuffer);
	}

	// m_MultiRenderTarget->Create()

	// 내가 직접 크기 조정해서 백버퍼에 찍어버리면 된다.
	m_CamRenderMesh = Resources::Find<HMESH>(L"TargetMESH");
	m_CamRenderPath = Resources::Find<RenderPath>(L"CAMPATH");
	m_CamRenderPath = m_CamRenderPath->Clone();
	m_CamRenderPath->LinkSMP(L"PS_LSMP", L"LSMP");


	m_DEFERREDLightPath = Resources::Find<RenderPath>(L"DFFERDLIGHTINGPATH");
	m_DEFERREDLightPath = m_DEFERREDLightPath->Clone();
	m_DEFERREDLightPath->LinkCB(L"DEFERREDCAMPOS", &m_ViewCamPos);
	// m_DEFERREDLightPath->TEX(L"Diff", m_DEFERREDTarget->Target(0)->Tex());
	m_DEFERREDLightPath->LinkTEX(L"Pos", m_DEFERREDTarget->Target(1)->GetTex());
	m_DEFERREDLightPath->LinkTEX(L"Nomal", m_DEFERREDTarget->Target(2)->GetTex());
	m_DEFERREDLightPath->LinkTEX(L"Depth", m_DEFERREDTarget->Target(3)->GetTex());
	m_DEFERREDLightPath->LinkTEX(L"Test", m_DEFERREDTarget->Target(4)->GetTex());
	m_DEFERREDLightPath->LinkTEX(L"Test2", m_DEFERREDTarget->Target(5)->GetTex());
	m_DEFERREDLightPath->LinkSMP(L"PS_LSMP", L"LSMP");

	m_LightMergePath = Resources::Find<RenderPath>(L"DFFERDLIGHTMERGEPATH");
	m_LightMergePath = m_LightMergePath->Clone();
	m_LightMergePath->LinkTEX(L"DiffTex", m_DEFERREDTarget->Target(0)->GetTex());
	m_LightMergePath->LinkTEX(L"LightDiffTex", m_LightTarget->Target(0)->GetTex());
	m_LightMergePath->LinkTEX(L"LightSpecTex", m_LightTarget->Target(1)->GetTex());
	//m_LightMergePath->LinkTEX(L"TestTex", m_LightTarget->Target(4)->GetTex());
	m_LightMergePath->LinkSMP(L"PS_LSMP", L"LSMP");
	// m_CamRenderPath->linkcb

	// DATA = m_CamRenderPath;
}

void Camera::Order(int _Order)
{
	// 랜더 도중에 벌어지면 난리난다.
	// 했단봐.
	m_Order = _Order;
}

void Camera::RenderBefore() 
{
	Vector4 Pos = GetActor()->GetTransform()->GetPosition();
	Vector4 For = GetActor()->GetTransform()->LookForward();
	Vector4 Up = GetActor()->GetTransform()->LookUp();

	// 트랜스 폼은 무조건 들어가 있네요????
	m_VIEW.SetViewMatrix(GetActor()->GetTransform()->GetPosition(), GetActor()->GetTransform()->LookForward(), GetActor()->GetTransform()->LookUp());
	// 윈도우의 비율을 가져오던가 해야한다.

	switch (m_Mode)
	{
	case PROJMODE::PM_PERS:
		m_PROJ.SetPerspectiveMatrix(m_Fov, m_Width, m_Height, m_Near, m_Far);
		break;
	case PROJMODE::PM_ORTH:
		m_PROJ.SetOrthgonalMatrix(m_Width, m_Height, m_Near, m_Far);
		break;
	default:
		break;
	}

	// 
	// m_MatData.ARRMAT[HMAT_VIEW].MulOne(_Cam->Trans()->WPos())
	m_ViewCamPos = m_VIEW.MulOne( Pos); // 이것도 실수함
	m_VP = m_VIEW * m_PROJ;

}


Vector4 Camera::OrthMousePos()
{
	Vector4 MP = GameWindow::MainGameWin()->MousePos();
	MP.y *= -1.0f;
	MP.x  -= GameWindow::MainGameWin()->Size().HX();
	MP.y += GameWindow::MainGameWin()->Size().HY();

	float XRatio = m_Width/ GameWindow::MainGameWin()->Size().x;
	float YRatio = m_Height/ GameWindow::MainGameWin()->Size().y;

	// 10 / 600;

	MP.x *= XRatio;
	MP.y *= YRatio;

	MP += GetTransform()->GetPosition();

	return MP;
}

void Camera::PushRenderGroup() 
{
	GetScene()->RMGR.PushCamera(this);
}

void Camera::Update() 
{
	// 랜더타겟도 이때 세팅된다.
	if (nullptr != GraphicDevice::MainContext())
	{
		// m_CamTarget->Clear();
		GraphicDevice::MainContext()->RSSetViewports(1, &m_CamViewPort);
		// 이걸 계속 바꿔줘야 한다.
	}
}

void Camera::DEFERREDMerge() 
{
	if (nullptr != GraphicDevice::MainContext())
	{
		GraphicDevice::MainContext()->RSSetViewports(1, &m_CamViewPort);
	}

	// 항상 
	MergeBefore();

	m_CamTarget->OmSet(nullptr);
	m_CamRenderMesh->UpdateVB(0);
	m_CamRenderMesh->UpdateIB(0);
	m_LightMergePath->Update();
	m_CamRenderMesh->Render(0);

	MergeAfter();
}

void Camera::CamRender() 
{
	if (nullptr != GraphicDevice::MainContext())
	{
		GraphicDevice::MainContext()->RSSetViewports(1, &m_CamViewPort);
	}


	// 캠을 랜더하기 전에 모든 타겟을 합쳐서
	// 하나의 화면으로 만들어내고 해야해.
	// 머지작업하고


	// 카메라 만든 순서에 의해서
	// 깊이 버퍼를 끄자.
	m_CamTarget->ShaderUpdate(HSHADER::SHADERTYPE::SHT_PS, 0);

	m_CamRenderMesh->UpdateVB(0);
	m_CamRenderMesh->UpdateIB(0);
	m_CamRenderPath->Update();
	m_CamRenderMesh->Render(0);
}

void Camera::CamClear() 
{
	m_CamTarget->Clear();
	m_DEFERREDTarget->Clear();
	m_DepthBuffer->Clear();
}


void Camera::FORWARDTargetSet()
{
	m_CamTarget->OmSet(m_DepthBuffer->DSV());
}
void Camera::DEFERREDTargetSet()
{
	m_DEFERREDTarget->OmSet();
}

void Camera::DEFERREDLightRender(HPTR<Light> _Light)
{
	if (nullptr != GraphicDevice::MainContext())
	{
		GraphicDevice::MainContext()->RSSetViewports(1, &m_CamViewPort);
	}

	if (nullptr != _Light)
	{
		Matrix Inv = m_VIEW;
		Inv.Inverse();
		_Light->m_ShadowData.CamInvView = Inv;
		_Light->m_ShadowData.CamInvView.TransPose();
		_Light->m_ShadowData.m_ShadowData.x = 1.0F;
		m_DEFERREDLightPath->LinkCB(L"SHADOWDATA", &(_Light->m_ShadowData));
		m_DEFERREDLightPath->LinkTEX(L"Shadow", _Light->m_ShadowTarget->GetTex());
	}
	else 
	{
		SData.m_ShadowData.x = 0.0f;
		m_DEFERREDLightPath->LinkCB(L"SHADOWDATA", &SData);
	}

	// 하하 이녀석은 클리어 언제하나요? 안했네요.
	m_LightTarget->Clear();
	m_LightTarget->OmSet();

	m_CamRenderMesh->UpdateVB(0);
	m_CamRenderMesh->UpdateIB(0);
	m_DEFERREDLightPath->Update();
	m_CamRenderMesh->Render(0);
}

void Camera::MergeBefore()
{
	for (auto& IterFilter : m_Filters)
	{
		IterFilter->MergeBeforeEffect();
	}
}

void Camera::MergeAfter()
{
	for (auto& IterFilter : m_Filters)
	{
		IterFilter->MergeAfterEffect();
	}
}