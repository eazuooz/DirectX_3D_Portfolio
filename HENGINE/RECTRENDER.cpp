#include "RECTRENDER.h"
#include "HMESH.h"
#include "HCB.h"
#include "HVTXSHADER.h"
#include "HPIXSHADER.h"
#include "HSMP.h"
#include "SPRITE.h"
#include "HBLEND.h"
#include "HRSSTATE.h"
#include "HDSSTATE.h"

HRECTRENDER::HRECTRENDER()
{
}


HRECTRENDER::~HRECTRENDER()
{
}


void HRECTRENDER::Init()
{
	Renderer::Init();

	m_COLOR = 1.0f;

	m_SMP = Resources::Find<HSMP>(L"LSMP");
	m_HMESH = Resources::Find<HMESH>(L"DTEXMESH");
	m_HCB = Resources::Find<HCB>(L"MATBUF");
	m_HCBCOLOR = Resources::Find<HCB>(L"RECTCOLOR");
	m_HVTXSHADER = Resources::Find<HVTXSHADER>(L"COLORRECT.fx");
	m_HPIXSHADER = Resources::Find<HPIXSHADER>(L"COLORRECT.fx");
	m_BLEND = Resources::Find<HBLEND>(L"DBLEND");
	m_DSSTATE = Resources::Find<HDSSTATE>(L"DDSSTATE");
	m_RSSTATE = Resources::Find<HRSSTATE>(L"DRSSTATE");
}

void HRECTRENDER::Render(const HPTR<Camera>& _CAM)
{
	DebugCheck();
	// MATW를 모른다.
	UpdateTransform();

	Matrix W = GetWorldMatrix();
	Matrix V = _CAM->GetViewMatrix();
	Matrix P = _CAM->PMAT();

	Matrix WVP = W * V * P;

	Matrix WVPT = WVP;

	WVPT.TransPose();

	// 최대한 덜 호출하는 것이 빠를것이다.
	m_HCB->DataSetting(&WVPT, sizeof(Matrix));
	m_HCB->Update();
	m_BLEND->Update();
	// 이녀석이 업데이트가 제대로 안됐다.
	// 값이 들어갔는데 변형됐다.

	// 5 ~ 15 번째까지

	// 0, 5,
	// 1, 10
	// 1 / 12.0F, 1 / 5.0F

	GraphicDevice::MainContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// TEXUV = HVEC4(0.0f, 0.0f, 1 / 12.0F, 1 / 5.0F);
	m_HCBCOLOR->DataSetting(&m_COLOR, sizeof(m_COLOR));
	m_HCBCOLOR->Update();

	m_HVTXSHADER->Update();
	m_HPIXSHADER->Update();

	m_SMP->Update(HSHADER::SHADERTYPE::SHT_PS, 0);

	m_DSSTATE->Update();
	m_RSSTATE->Update();
	m_HMESH->Update(0, 0);
	// 세팅이 다른부분만 세팅해준다.
	m_HMESH->Render(0);

}