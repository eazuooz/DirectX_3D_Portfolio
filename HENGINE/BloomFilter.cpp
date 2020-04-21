#include "BloomFilter.h"

#include "HMULTIRENDERTARGET.h"
#include "HMESH.h"
#include "RENDERPATH.h"
#include "CAMERA.h"


BloomFilter::BloomFilter()
{
}


BloomFilter::~BloomFilter()
{
}

void BloomFilter::InitFilter()
{
	m_Mesh = Resources::Find<HMESH>(L"TargetMESH");

	FilterSize.x = 64.0f;
	FilterSize.y = 64.0f;

	SFilterSize = FilterSize * 0.5f;

	{
		// 빛을 뭉개기 위한 타겟
		// 작게해서 연산을 줄이기 위해서 사용한다.
		SmallTaregt = new HRENDERTARGET();
		SmallTaregt->Create(
			{ 0.0f, 0.0f, 0.0f, 0.0f } // 이 타겟이 클리어타겟 될때 색깔
			, (UINT)FilterSize.x// 너비
			, (UINT)FilterSize.y// 높이
			, DXGI_FORMAT_R32G32B32A32_FLOAT // 색깔 형태
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	}

	{
		// 빛을 뭉개기 위한 타겟
		// 작게해서 연산을 줄이기 위해서 사용한다.
		SSmallTaregt = new HRENDERTARGET();
		SSmallTaregt->Create(
			{ 0.0f, 0.0f, 0.0f, 0.0f } // 이 타겟이 클리어타겟 될때 색깔
			, (UINT)SFilterSize.x // 너비
			, (UINT)SFilterSize.y // 높이
			, DXGI_FORMAT_R32G32B32A32_FLOAT // 색깔 형태
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	}

	{
		SpecCopyTarget = new HRENDERTARGET();
		SpecCopyTarget->Create(
			{ 0.0f, 0.0f, 0.0f, 0.0f } // 이 타겟이 클리어타겟 될때 색깔
			, (UINT)m_Cam->CamTarget()->GetTex()->GetWidth()// 너비
			, (UINT)m_Cam->CamTarget()->GetTex()->GetHeight() // 높이
			, DXGI_FORMAT_R32G32B32A32_FLOAT // 색깔 형태
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	}


	{
		m_BloomSmallPath = Resources::Find<RenderPath>(L"BLOOMSMALLPATH");
		m_BloomSmallPath = m_BloomSmallPath->Clone();
		m_BloomSmallPath->LinkTEX(L"LightDiffTarget", m_Cam->GetLightTarget()->Target(0)->GetTex());
		m_BloomSmallPath->LinkTEX(L"LightSpecTarget", m_Cam->GetLightTarget()->Target(1)->GetTex());
		m_BloomSmallPath->LinkSMP(L"PS_LSMP", L"LSMP");
		m_BloomSmallPath->LinkCB(L"VS_FilterSize", &FilterSize);
		m_BloomSmallPath->LinkCB(L"PS_FilterSize", &FilterSize);
	}


	{
		m_BloomSmallEffectPath = Resources::Find<RenderPath>(L"BLOOMSMALLPATH");
		m_BloomSmallEffectPath = m_BloomSmallEffectPath->Clone();
		m_BloomSmallEffectPath->LinkTEX(L"LightDiffTarget", m_Cam->GetLightTarget()->Target(0)->GetTex());
		m_BloomSmallEffectPath->LinkTEX(L"LightSpecTarget", SmallTaregt->GetTex());
		m_BloomSmallEffectPath->LinkSMP(L"PS_LSMP", L"LSMP");
		m_BloomSmallEffectPath->LinkCB(L"VS_FilterSize", &SFilterSize);
		m_BloomSmallEffectPath->LinkCB(L"PS_FilterSize", &SFilterSize);
	}



	{
		m_BloomEffectPath = Resources::Find<RenderPath>(L"BLOOMEFFECTPATH");
		m_BloomEffectPath = m_BloomEffectPath->Clone();
		m_BloomEffectPath->LinkTEX(L"SmallTarget", SSmallTaregt->GetTex());
		m_BloomEffectPath->LinkTEX(L"OriTarget", SpecCopyTarget->GetTex());
		m_BloomEffectPath->LinkSMP(L"PS_LSMP", L"LSMP");
		m_BloomEffectPath->LinkCB(L"VS_FilterSize", &FilterSize);
		m_BloomEffectPath->LinkCB(L"PS_FilterSize", &FilterSize);
	}



	Template::MemZero(m_SmallViewPort);
	m_SmallViewPort.TopLeftX = 0;
	m_SmallViewPort.TopLeftY = 0;
	m_SmallViewPort.Width = FilterSize.x;
	m_SmallViewPort.Height = FilterSize.y;
	m_SmallViewPort.MinDepth = 0.0f;
	m_SmallViewPort.MaxDepth = 1.0f;

	//HTEMPLATE::MemZero(m_SSmallViewPort);
	//m_SSmallViewPort.TopLeftX = -SFilterSize.x * 0.3f;
	//m_SSmallViewPort.TopLeftY = -SFilterSize.y * 0.3f;
	//m_SSmallViewPort.Width = SFilterSize.x * 1.6f;
	//m_SSmallViewPort.Height = SFilterSize.y * 1.6f;
	//m_SSmallViewPort.MinDepth = 0.0f;
	//m_SSmallViewPort.MaxDepth = 1.0f;

	Template::MemZero(m_SSmallViewPort);
	m_SSmallViewPort.TopLeftX = 0;
	m_SSmallViewPort.TopLeftY = 0;
	m_SSmallViewPort.Width = SFilterSize.x;
	m_SSmallViewPort.Height = SFilterSize.y;
	m_SSmallViewPort.MinDepth = 0.0f;
	m_SSmallViewPort.MaxDepth = 1.0f;
}

void BloomFilter::MergeBeforeEffect()
{
	GraphicDevice::MainContext()->RSSetViewports(1, &m_SmallViewPort);

	SmallTaregt->Clear();
	SmallTaregt->OmSet(nullptr);
	m_Mesh->UpdateVB(0);
	m_Mesh->UpdateIB(0);
	m_BloomSmallPath->Update();
	m_Mesh->Render(0);

	GraphicDevice::MainContext()->RSSetViewports(1, &m_SSmallViewPort);

	SSmallTaregt->Clear();
	SSmallTaregt->OmSet(nullptr);
	m_Mesh->UpdateVB(0);
	m_Mesh->UpdateIB(0);
	m_BloomSmallEffectPath->Update();
	m_Mesh->Render(0);

	m_Cam->Update();

	// 스펙큘러 타겟 카피
	SpecCopyTarget->GetTex()->Copy(m_Cam->GetLightTarget()->Target(1)->GetTex());

	// m_Cam->LightTarget()->Target(1)->Clear();
	m_Cam->GetLightTarget()->Target(1)->OmSet(nullptr);
	m_Mesh->UpdateVB(0);
	m_Mesh->UpdateIB(0);
	m_BloomEffectPath->Update();
	m_Mesh->Render(0);
}
void BloomFilter::MergeAfterEffect()
{

}