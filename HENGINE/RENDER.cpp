#include "RENDER.h"
#include "TRANS.h"
#include "CAMERA.h"
#include "SCENE.h"
#include "HMESH.h"
#include "RENDERPATH.h"
#include "LIGHT.h"


Renderer::Renderer() 
{
}


Renderer::~Renderer()
{
}

void Renderer::Init() 
{
	
	if (nullptr == GetActor()->GetTransform())
	{
		BOOM;
	}

	SetParentTrans(GetActor()->GetTransform());
}

// 자기를 랜더해주는 캠.
void Renderer::Render(const HPTR<Camera>& _CAM) 
{
	UpdateTransform();
	UpdateSubTrans(_CAM);

	for (size_t i = 0; i < m_AllRenderData.size(); i++)
	{
		m_AllRenderData[i]->m_Mesh->UpdateVB(m_AllRenderData[i]->VBIndex);
		m_AllRenderData[i]->m_Mesh->UpdateIB(m_AllRenderData[i]->IBIndex);
		m_AllRenderData[i]->m_Path->Update();
		m_AllRenderData[i]->m_Mesh->Render(m_AllRenderData[i]->IBIndex);
	}
}

void Renderer::DataCreate(int _Order) 
{
	m_Order = _Order;
	GetScene()->RMGR.PushRender(this);
}

void Renderer::SceneChangeStart() 
{
	GetScene()->RMGR.PushRender(this);
}

Renderer::HRENDERDATA* Renderer::RenderData(const wchar_t* _Mesh, const wchar_t* _Path, unsigned int _VBIndex /*= 0*/, unsigned int _IBIndex /*= 0*/)
{
	// 쉐이더중에는 직접 버텍스를 만들어낼수 있는 녀석도 있다.
	HPTR<HMESH> MESH = Resources::Find<HMESH>(_Mesh);
	HPTR<RenderPath> PATH = Resources::Find<RenderPath>(_Path);

	if (nullptr == MESH || nullptr == PATH)
	{
		BOOM;
	}

	HPTR<RenderPath> ClonePath = PATH->Clone();
	ClonePath->Init(this);
	HRENDERDATA* NewData = new HRENDERDATA( this, _VBIndex , _IBIndex,  MESH , ClonePath );

	m_AllRenderData.push_back(NewData);

	
	//포워드
	//디퍼드
	//UI

	if (NewData->m_Path->m_Type == RT_FORWARD)
	{
		GetScene()->RMGR.PushForwardender(NewData);
	}
	else
	{
		GetScene()->RMGR.PushDeferredRender(NewData);
	}
	// ClonePath->m_Type
	// 

	return NewData;
}

void Renderer::UpdateSubTrans(const HPTR<Camera>& _Cam) 
{
	m_MatData.ARRMAT[HMAT_SCALE] = m_ScaleMat;
	m_MatData.ARRMAT[HMAT_WORLD] = GetWorldMatrix();
	m_MatData.ARRMAT[HMAT_VIEW] = _Cam->GetViewMatrix();
	m_MatData.ARRMAT[HMAT_PROJ] = _Cam->PMAT();
	m_MatData.CAMPOS = m_MatData.ARRMAT[HMAT_VIEW].MulOne(_Cam->GetTransform()->GetPosition());
	m_MatData.CAMPOS.w = 1.0f;
	//m_MatData.LIGHTDATA.x = (float)m_LightMode;
	m_MatData.CalSubTrans();
	m_MatData.AllTP();
	// m_MatData.ARRMAT[HMAT_WVP] = W * V * P;
}

void Renderer::UpdateShadowTrans(const HPTR<Light>& _Light)
{
	m_MatData.ARRMAT[HMAT_WORLD] = GetWorldMatrix();
	m_MatData.ARRMAT[HMAT_VIEW] = _Light->VMAT();
	m_MatData.ARRMAT[HMAT_PROJ] = _Light->PMAT();
	m_MatData.CAMPOS = m_MatData.ARRMAT[HMAT_VIEW].MulOne(_Light->GetTransform()->GetPosition());
	m_MatData.CAMPOS.w = 1.0f;
	// m_MatData.LIGHTDATA.x = (float)m_LightMode;
	m_MatData.CalSubTrans();
	m_MatData.AllTP();
}

void Renderer::LinkCB(int _PathIndex, const wchar_t* _Name, void* _LinkData)
{
	// 오리지널

	// Res에 있는 오리지널이랑 어쨌든 달라지는것. 
	// 프로토타입 패턴
	// 새로운 랜더패스를 복제해서 가지고 있어야 한다.
	// 모든 랜더패스가 다 

	if(false == m_AllRenderData[_PathIndex]->m_Path->IsCB(_Name))
	{
		BOOM;
	}

	m_AllRenderData[_PathIndex]->m_Path->CBInfoStart = m_AllRenderData[_PathIndex]->m_Path->m_CBInfoSheet.find(_Name);
	m_AllRenderData[_PathIndex]->m_Path->CBInfoStart->second->m_Data.m_CBType = HSHADER::CBTYPE::LINK;
	m_AllRenderData[_PathIndex]->m_Path->CBInfoStart->second->m_SetData = _LinkData;
}
void Renderer::LinkTexture(int _PathIndex, const wchar_t* _Name, const wchar_t* _TexName)
{
	if (false == m_AllRenderData[_PathIndex]->m_Path->IsTEX(_Name))
	{
		BOOM;
	}

	m_AllRenderData[_PathIndex]->m_Path->TexInfoStart = m_AllRenderData[_PathIndex]->m_Path->m_TexInfoSheet.find(_Name);
	m_AllRenderData[_PathIndex]->m_Path->TexInfoStart->second->m_Tex = Resources::Find<Texture>(_TexName);
	m_AllRenderData[_PathIndex]->m_Path->TexInfoStart->second->m_pSRV = m_AllRenderData[_PathIndex]->m_Path->TexInfoStart->second->m_Tex->GetSRV();
}
void Renderer::LinkSampler(int _PathIndex, const wchar_t* _Name, const wchar_t* _SmpName)
{
	if (false == m_AllRenderData[_PathIndex]->m_Path->IsSMP(_Name))
	{
		BOOM;
	}

	m_AllRenderData[_PathIndex]->m_Path->SmpInfoStart = m_AllRenderData[_PathIndex]->m_Path->m_SmpInfoSheet.find(_Name);
	m_AllRenderData[_PathIndex]->m_Path->SmpInfoStart->second->m_Smp = Resources::Find<HSMP>(_SmpName);
	m_AllRenderData[_PathIndex]->m_Path->SmpInfoStart->second->m_pState = m_AllRenderData[_PathIndex]->m_Path->SmpInfoStart->second->m_Smp->State();
}