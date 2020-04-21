#include "GRAPHICDEVICE.h"
#include <PATH.h>
#include "HSMP.h"
#include "HDSSTATE.h"
#include "HRSSTATE.h"
#include "HCB.h"
#include "HMESH.h"
#include "HBLEND.h"
#include "HVTXSHADER.h"
#include "HPIXSHADER.h"
#include "GRAPHICDEBUG.h"
#include "HVBBUFFER.h"
#include "HIBBUFFER.h"
#include "HEFONT.h"
#include "RENDERPATH.h"
#include "TRANS.h"
#include "GRIDRENDER.h"
#include "LIGHT.h"
#include "HRENDERTARGET.h"

void DefStateInit();
void MeshInit();
void DebugMeshInit();
void ShaderInit();
void RenderTargetInit();

void RenderPathInit();

void GraphicDevice::BaseResInit()
{
	if (false == m_PathInit)
	{
		Path::Root.ReplaceParentPath();
		Path::Root.MoveDirectory(L"BIN");
		Path::CreatePath(L"SHADER");
		m_PathInit = true;
	}

	DefStateInit();
	DebugMeshInit();
	MeshInit();
	ShaderInit();
	// 가장 마지막에 만들어 질 것이다.
	RenderPathInit();
	RenderTargetInit();

	GraphicDebug::Init();

	m_BaseInit = true;
}


void DefStateInit()
{
	Resources::Create<HEFONT>(L"굴림체", L"궁서체");
	Resources::Create<HEFONT>(L"1훈떡볶이", L"1훈떡볶이");

	// 샘플러
	// 상수버퍼
	Resources::Create<HSMP>(L"LSMP");
	Resources::Create<HDSSTATE>(L"DDSSTATE");
	Resources::Create<HRSSTATE>(L"DRSSTATE");
	Resources::Create<HBLEND>(L"DBLEND");


	{
		D3D11_SAMPLER_DESC m_Decs;
		m_Decs.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
		m_Decs.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		m_Decs.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		m_Decs.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		memcpy_s(m_Decs.BorderColor, sizeof(Vector4), &Vector4::RED, sizeof(Vector4));
		m_Decs.ComparisonFunc = D3D11_COMPARISON_NEVER;
		m_Decs.MaxAnisotropy = 0;
		m_Decs.MinLOD = -FLT_MAX;
		m_Decs.MaxLOD = FLT_MAX;

		m_Decs.MipLODBias = 1.0F;


		Resources::Create<HSMP>(L"PSMP");
	}


	///////////////////////////////// CAMSTATE
	{
		D3D11_DEPTH_STENCIL_DESC DSDESC;
		DSDESC.DepthEnable = TRUE;
		DSDESC.DepthFunc = D3D11_COMPARISON_ALWAYS;
		DSDESC.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DSDESC.StencilEnable = FALSE;

		Resources::Create<HDSSTATE>(L"UIFRONTDEPTH", DSDESC);
	}
	
	{
		D3D11_DEPTH_STENCIL_DESC DSDESC;
		DSDESC.DepthEnable = FALSE;
		DSDESC.DepthFunc = D3D11_COMPARISON_NEVER;
		DSDESC.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		DSDESC.StencilEnable = FALSE;

		Resources::Create<HDSSTATE>(L"DSNONE", DSDESC);
	}

	{
		D3D11_DEPTH_STENCIL_DESC CAMDSDESC;
		CAMDSDESC.DepthEnable = TRUE;
		CAMDSDESC.DepthFunc = D3D11_COMPARISON_ALWAYS;
		CAMDSDESC.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		CAMDSDESC.StencilEnable = FALSE;

		Resources::Create<HDSSTATE>(L"CAMDSSTATE", CAMDSDESC);

		D3D11_DEPTH_STENCIL_DESC BLOOMDSSTATE;
		BLOOMDSSTATE.DepthEnable = FALSE;
		BLOOMDSSTATE.DepthFunc = D3D11_COMPARISON_ALWAYS;
		BLOOMDSSTATE.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		BLOOMDSSTATE.StencilEnable = FALSE;

		Resources::Create<HDSSTATE>(L"BLOOMDSSTATE", CAMDSDESC);




		D3D11_BLEND_DESC CAMBLEND;
		CAMBLEND.AlphaToCoverageEnable = true;
		CAMBLEND.IndependentBlendEnable = false;
		CAMBLEND.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		CAMBLEND.RenderTarget[0].BlendEnable = true;
		CAMBLEND.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		CAMBLEND.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		CAMBLEND.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		CAMBLEND.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		CAMBLEND.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		CAMBLEND.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		Resources::Create<HBLEND>(L"CAMBLEND", CAMBLEND);
	}

	///////////////////////////////// DEBUGSTATE
	{
		D3D11_DEPTH_STENCIL_DESC DEBUGDSDESC;
		DEBUGDSDESC.DepthEnable = TRUE;
		DEBUGDSDESC.DepthFunc = D3D11_COMPARISON_ALWAYS;
		DEBUGDSDESC.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DEBUGDSDESC.StencilEnable = FALSE;

		Resources::Create<HDSSTATE>(L"DEBUGDSSTATE", DEBUGDSDESC);

		D3D11_RASTERIZER_DESC DEBUGRSDESC;
		memset(&DEBUGRSDESC, 0, sizeof(DEBUGRSDESC));
		DEBUGRSDESC.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		DEBUGRSDESC.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		DEBUGRSDESC.MultisampleEnable = FALSE;
		DEBUGRSDESC.AntialiasedLineEnable = FALSE;
		Resources::Create<HRSSTATE>(L"DEBUGRSSTATE", DEBUGRSDESC);

		D3D11_BLEND_DESC DEBUGBLEND;
		DEBUGBLEND.AlphaToCoverageEnable = false;
		DEBUGBLEND.IndependentBlendEnable = false;
		DEBUGBLEND.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		DEBUGBLEND.RenderTarget[0].BlendEnable = true;
		DEBUGBLEND.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		DEBUGBLEND.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		DEBUGBLEND.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		DEBUGBLEND.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		DEBUGBLEND.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		DEBUGBLEND.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		Resources::Create<HBLEND>(L"DEBUGBLEND", DEBUGBLEND);
	}

	{
		D3D11_DEPTH_STENCIL_DESC DEBUGDSDESC;
		DEBUGDSDESC.DepthEnable = TRUE;
		DEBUGDSDESC.DepthFunc = D3D11_COMPARISON_LESS;
		DEBUGDSDESC.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DEBUGDSDESC.StencilEnable = FALSE;

		Resources::Create<HDSSTATE>(L"DEBUGCAMDSSTATE", DEBUGDSDESC);
	}

	{
		D3D11_RASTERIZER_DESC RSDESC;
		memset(&RSDESC, 0, sizeof(RSDESC));
		RSDESC.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		RSDESC.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		RSDESC.MultisampleEnable = FALSE;
		RSDESC.AntialiasedLineEnable = FALSE;
		Resources::Create<HRSSTATE>(L"GRIDRSSTATE", RSDESC);
	}

	{
		D3D11_RASTERIZER_DESC RSDESC;
		memset(&RSDESC, 0, sizeof(RSDESC));
		RSDESC.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		RSDESC.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		RSDESC.MultisampleEnable = FALSE;
		RSDESC.AntialiasedLineEnable = FALSE;
		Resources::Create<HRSSTATE>(L"SKYSTATE", RSDESC);
	}

	{
		D3D11_RASTERIZER_DESC RSDESC;
		memset(&RSDESC, 0, sizeof(RSDESC));
		RSDESC.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		RSDESC.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		RSDESC.MultisampleEnable = FALSE;
		RSDESC.AntialiasedLineEnable = FALSE;
		Resources::Create<HRSSTATE>(L"STATICRSSTATE", RSDESC);
	}

	{
		D3D11_BLEND_DESC CAMBLEND;
		CAMBLEND.AlphaToCoverageEnable = false;
		CAMBLEND.IndependentBlendEnable = false;
		CAMBLEND.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		CAMBLEND.RenderTarget[0].BlendEnable = false;
		CAMBLEND.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		CAMBLEND.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		CAMBLEND.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		CAMBLEND.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		CAMBLEND.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		CAMBLEND.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		Resources::Create<HBLEND>(L"SHADOWBLEND", CAMBLEND);
	}

	{
		D3D11_BLEND_DESC CAMBLEND;
		CAMBLEND.AlphaToCoverageEnable = false;
		CAMBLEND.IndependentBlendEnable = false;
		CAMBLEND.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		CAMBLEND.RenderTarget[0].BlendEnable = true;
		CAMBLEND.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		CAMBLEND.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		CAMBLEND.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		CAMBLEND.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		CAMBLEND.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		CAMBLEND.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		Resources::Create<HBLEND>(L"WATERBLEND", CAMBLEND);
	}

	{
		D3D11_BLEND_DESC CAMBLEND;
		CAMBLEND.AlphaToCoverageEnable = true;
		CAMBLEND.IndependentBlendEnable = false;
		CAMBLEND.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
		CAMBLEND.RenderTarget[0].BlendEnable = true;
		CAMBLEND.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
		CAMBLEND.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
		CAMBLEND.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
		CAMBLEND.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_MAX;
		CAMBLEND.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
		CAMBLEND.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;

		Resources::Create<HBLEND>(L"UIBLEND", CAMBLEND);
	}

	{
		D3D11_BLEND_DESC CAMBLEND;
		CAMBLEND.AlphaToCoverageEnable = true;
		CAMBLEND.IndependentBlendEnable = false;
		CAMBLEND.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		CAMBLEND.RenderTarget[0].BlendEnable = true;
		CAMBLEND.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		CAMBLEND.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		CAMBLEND.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		CAMBLEND.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		CAMBLEND.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		CAMBLEND.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		Resources::Create<HBLEND>(L"EFFECTBLEND", CAMBLEND);
		//CAMBLEND.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		//CAMBLEND.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		//CAMBLEND.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		//CAMBLEND.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		//CAMBLEND.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		//CAMBLEND.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	}

	{
		D3D11_BLEND_DESC CAMBLEND;
		//CAMBLEND.AlphaToCoverageEnable = true;
		//CAMBLEND.IndependentBlendEnable = false;
		//CAMBLEND.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		//CAMBLEND.RenderTarget[0].BlendEnable = true;
		//CAMBLEND.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		//CAMBLEND.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		//CAMBLEND.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		//CAMBLEND.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		//CAMBLEND.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		//CAMBLEND.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		CAMBLEND.AlphaToCoverageEnable = true;
		CAMBLEND.IndependentBlendEnable = false;
		CAMBLEND.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		CAMBLEND.RenderTarget[0].BlendEnable = true;
		CAMBLEND.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		CAMBLEND.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		CAMBLEND.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		CAMBLEND.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		CAMBLEND.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		CAMBLEND.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		Resources::Create<HBLEND>(L"3DANIBLEND", CAMBLEND);
	}
}

void DebugMeshInit()
{
#pragma region DEBUGBOX
	{
		std::vector<EngineVertex::VertexColor> ArrVtx;
		ArrVtx.resize(8);

		// 
		ArrVtx[0].Pos = Vector4(-0.5f, 0.5f, 0.5f, 1.0f);
		ArrVtx[1].Pos = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		ArrVtx[2].Pos = Vector4(0.5f, -0.5f, 0.5f, 1.0f);
		ArrVtx[3].Pos = Vector4(-0.5f, -0.5f, 0.5f, 1.0f);

		ArrVtx[4].Pos = Vector4(-0.5f, 0.5f, -0.5f, 1.0f);
		ArrVtx[5].Pos = Vector4(0.5f, 0.5f, -0.5f, 1.0f);
		ArrVtx[6].Pos = Vector4(0.5f, -0.5f, -0.5f, 1.0f);
		ArrVtx[7].Pos = Vector4(-0.5f, -0.5f, -0.5f, 1.0f);

		Resources::Create<HVBBUFFER>(L"DEBUGBOXVBBUFFER", ArrVtx, D3D11_USAGE_DYNAMIC);

		std::vector<DWORD> ArrIdx;
		ArrIdx.push_back(0);
		ArrIdx.push_back(1);
		ArrIdx.push_back(1);
		ArrIdx.push_back(2);
		ArrIdx.push_back(2);
		ArrIdx.push_back(3);
		ArrIdx.push_back(3);
		ArrIdx.push_back(0);

		ArrIdx.push_back(0 + 4);
		ArrIdx.push_back(1 + 4);
		ArrIdx.push_back(1 + 4);
		ArrIdx.push_back(2 + 4);
		ArrIdx.push_back(2 + 4);
		ArrIdx.push_back(3 + 4);
		ArrIdx.push_back(3 + 4);
		ArrIdx.push_back(0 + 4);

		ArrIdx.push_back(0);
		ArrIdx.push_back(4);
		ArrIdx.push_back(0 + 1);
		ArrIdx.push_back(4 + 1);
		ArrIdx.push_back(0 + 2);
		ArrIdx.push_back(4 + 2);
		ArrIdx.push_back(0 + 3);
		ArrIdx.push_back(4 + 3);

		Resources::Create<HIBBUFFER>(L"DEBUGBOXIBBUFFER", ArrIdx, D3D11_USAGE_DYNAMIC);
	}

	{
		HPTR<HMESH> NEWMESH = Resources::Create<HMESH>(L"DEBUGBOX");
		NEWMESH->AddVB(L"DEBUGBOXVBBUFFER");
		NEWMESH->AddIB(L"DEBUGBOXIBBUFFER");
	}
#pragma endregion
#pragma region DEBUGRECT
	{
		std::vector<DWORD> ArrIdx;
		ArrIdx.push_back(0);
		ArrIdx.push_back(1);
		ArrIdx.push_back(1);
		ArrIdx.push_back(2);
		ArrIdx.push_back(2);
		ArrIdx.push_back(3);
		ArrIdx.push_back(3);
		ArrIdx.push_back(0);

		Resources::Create<HIBBUFFER>(L"DEBUGRECTIBBUFFER", ArrIdx, D3D11_USAGE_DYNAMIC);

		std::vector<EngineVertex::VertexColor> ArrVtx;
		ArrVtx.resize(4);

		ArrVtx[0].Pos = Vector4(-0.5f, 0.5f, 0.0f, 1.0f);
		ArrVtx[1].Pos = Vector4(0.5f, 0.5f, 0.0f, 1.0f);
		ArrVtx[2].Pos = Vector4(0.5f, -0.5f, 0.0f, 1.0f);
		ArrVtx[3].Pos = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);

		Resources::Create<HVBBUFFER>(L"DEBUGRECTVBBUFFER", ArrVtx, D3D11_USAGE_DYNAMIC);

	}


	{
		HPTR<HMESH> NEWMESH = Resources::Create<HMESH>(L"DEBUGRECT");
		NEWMESH->AddVB(L"DEBUGRECTVBBUFFER");
		NEWMESH->AddIB(L"DEBUGRECTIBBUFFER");
	}
#pragma endregion
#pragma region DEBUGOCTAHEDRON
	{
		std::vector<EngineVertex::VertexColor> ArrVtx;
		ArrVtx.resize(6);

		ArrVtx[0].Pos = Vector4(0.0f, 0.5f, 0.0f, 1.0f);
		ArrVtx[1].Pos = Vector4(0.0f, 0.0f, 0.5f, 1.0f);
		ArrVtx[2].Pos = Vector4(0.5f, 0.0f, 0.0f, 1.0f);
		ArrVtx[3].Pos = Vector4(0.0f, 0.0f, -0.5f, 1.0f);
		ArrVtx[4].Pos = Vector4(-0.5f, 0.0f, 0.0f, 1.0f);
		ArrVtx[5].Pos = Vector4(0.0f, -0.5f, 0.0f, 1.0f);

		Resources::Create<HVBBUFFER>(L"DEBUGOCTAHEDRONVBBUFFER", ArrVtx, D3D11_USAGE_DYNAMIC);

		std::vector<DWORD> ArrIdx;
		ArrIdx.push_back(0);
		ArrIdx.push_back(1);
		ArrIdx.push_back(2);
		ArrIdx.push_back(0);
		ArrIdx.push_back(3);
		ArrIdx.push_back(2);
		ArrIdx.push_back(5);
		ArrIdx.push_back(3);
		ArrIdx.push_back(4);
		ArrIdx.push_back(5);
		ArrIdx.push_back(1);
		ArrIdx.push_back(4);
		ArrIdx.push_back(0);

		Resources::Create<HIBBUFFER>(L"DEBUGOCTAHEDRONIBBUFFER", ArrIdx, D3D11_USAGE_DYNAMIC);
	}

	{
		HPTR<HMESH> NEWMESH = Resources::Create<HMESH>(L"DEBUGOCTAHEDRON");
		NEWMESH->AddVB(L"DEBUGOCTAHEDRONVBBUFFER");
		NEWMESH->AddIB(L"DEBUGOCTAHEDRONIBBUFFER");
	}
#pragma endregion
#pragma region DEBUGCIRCLE

	{
		std::vector<EngineVertex::VertexColor> ArrVtx;
		std::vector<DWORD> ArrIdx;

		Vector4 Start = Vector4::UP;

		for (size_t i = 0; i < 24; i++)
		{
			ArrVtx.push_back(EngineVertex::VertexColor());

			ArrVtx[i].Pos = Vector4::CalDToDir2D((float)i * 15.0f) * 0.5f;

			ArrIdx.push_back((DWORD)i);
		}

		ArrIdx.push_back(0);

		Resources::Create<HVBBUFFER>(L"DEBUGCIRCLEIBBUFFER", ArrVtx, D3D11_USAGE_DYNAMIC);
		Resources::Create<HIBBUFFER>(L"DEBUGCIRCLEIBBUFFER", ArrIdx, D3D11_USAGE_DYNAMIC);
	}

	{
		HPTR<HMESH> NEWMESH = Resources::Create<HMESH>(L"DEBUGCIRCLE");
		NEWMESH->AddVB(L"DEBUGCIRCLEIBBUFFER");
		NEWMESH->AddIB(L"DEBUGCIRCLEIBBUFFER");
	}

#pragma endregion
#pragma region DEBUGSPHERE
	{
		std::vector<EngineVertex::VertexColor> ArrVtx;
		std::vector<DWORD> ArrIdx;

		Vector4 Start = Vector4::UP;

		for (size_t i = 0; i < 24; i++)
		{
			ArrVtx.push_back(EngineVertex::VertexColor());

			ArrVtx[i].Pos = Vector4::CalDToDir2D((float)i * 15.0f) * 0.5f;

			ArrIdx.push_back((DWORD)i);
		}

		ArrIdx.push_back(0);

		for (size_t i = 24; i < 24 + 19; i++)
		{
			ArrVtx.push_back(EngineVertex::VertexColor());

			ArrVtx[i].Pos = Vector4::CalDToDir2D((float)(i - 24) * 15.0f) * 0.5f;
			ArrVtx[i].Pos.z = ArrVtx[i].Pos.y;
			ArrVtx[i].Pos.y = 0.0f;

			ArrIdx.push_back((DWORD)i);
		}

		for (size_t i = 24 + 19; i < (24 + 19) + 24; i++)
		{
			ArrVtx.push_back(EngineVertex::VertexColor());

			ArrVtx[i].Pos = Vector4::CalDToDir2D((float)(i - (24 + 19)) * 15.0f) * 0.5f;
			ArrVtx[i].Pos.z = -ArrVtx[i].Pos.x;
			ArrVtx[i].Pos.x = 0.0f;

			ArrIdx.push_back((DWORD)i);
		}

		for (size_t i = (24 + 19) + 24; i < ((24 + 19) + 24) + 7; i++)
		{
			ArrVtx.push_back(EngineVertex::VertexColor());

			ArrVtx[i].Pos = Vector4::CalDToDir2D(((float)(i - (24 + 19)) * 15.0f) + (18 * 15.0f)) * 0.5f;
			ArrVtx[i].Pos.z = ArrVtx[i].Pos.y;
			ArrVtx[i].Pos.y = 0.0f;

			ArrIdx.push_back((DWORD)i);
		}
		ArrIdx.push_back(0);


		Resources::Create<HVBBUFFER>(L"DEBUGSPHEREIBBUFFER", ArrVtx, D3D11_USAGE_DYNAMIC);
		Resources::Create<HIBBUFFER>(L"DEBUGSPHEREIBBUFFER", ArrIdx, D3D11_USAGE_DYNAMIC);
	}

	{
		HPTR<HMESH> NEWMESH = Resources::Create<HMESH>(L"DEBUGSPHERE");
		NEWMESH->AddVB(L"DEBUGSPHEREIBBUFFER");
		NEWMESH->AddIB(L"DEBUGSPHEREIBBUFFER");
	}

#pragma endregion
}

void MeshInit()
{
#pragma region GRIDRECT
	{
		std::vector<EngineVertex::VertexGrid> ArrVtx;
		ArrVtx.resize(4);

		ArrVtx[0].Pos = Vector4(-0.5f, 0.0f, -0.5f, 1.0f);
		ArrVtx[1].Pos = Vector4(0.5f, 0.0f, -0.5f, 1.0f);
		ArrVtx[2].Pos = Vector4(0.5f, 0.0f, 0.5f, 1.0f);
		ArrVtx[3].Pos = Vector4(-0.5f, 0.0f, 0.5f, 1.0f);

		// 5번째 
		ArrVtx[0].vUv = Vector2(0.0f, 0.0f);
		ArrVtx[1].vUv = Vector2(1.0f, 0.0f);
		ArrVtx[2].vUv = Vector2(0.0f, 1.0f);
		ArrVtx[3].vUv = Vector2(1.0f, 1.0f);

		Resources::Create<HVBBUFFER>(L"GRIDVBBUFFER", ArrVtx, D3D11_USAGE_DYNAMIC);

		std::vector<EngineIndex::Index32> ArrIdx;
		ArrIdx.push_back({ 0, 2, 1 });
		ArrIdx.push_back({ 0, 3, 2 });

		Resources::Create<HIBBUFFER>(L"GRIDIBBUFFER", ArrIdx, D3D11_USAGE_DYNAMIC);
	}

	{
		HPTR<HMESH> NEWMESH = Resources::Create<HMESH>(L"GRIDMESH");
		NEWMESH->AddVB(L"GRIDVBBUFFER");
		NEWMESH->AddIB(L"GRIDIBBUFFER");
	}
#pragma endregion
#pragma region 2DTEXRECT
	{
		std::vector<EngineVertex::Vertex2D> ArrVtx;
		ArrVtx.resize(4);

		ArrVtx[0].Pos = Vector4(-0.5f, 0.5f, 0.0f, 1.0f);
		ArrVtx[1].Pos = Vector4(0.5f, 0.5f, 0.0f, 1.0f);
		ArrVtx[2].Pos = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);
		ArrVtx[3].Pos = Vector4(0.5f, -0.5f, 0.0f, 1.0f);

		// 5번째 
		ArrVtx[0].vUv = Vector2(0.0f, 0.0f);
		ArrVtx[1].vUv = Vector2(1.0f, 0.0f);
		ArrVtx[2].vUv = Vector2(0.0f, 1.0f);
		ArrVtx[3].vUv = Vector2(1.0f, 1.0f);


		ArrVtx[0].Color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		ArrVtx[1].Color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		ArrVtx[2].Color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		ArrVtx[3].Color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);

		Resources::Create<HVBBUFFER>(L"DTEXVBBUFFER", ArrVtx, D3D11_USAGE_DYNAMIC);

		std::vector<EngineIndex::Index32> ArrIdx;
		ArrIdx.push_back({ 0, 3, 2 });
		ArrIdx.push_back({ 0, 1, 3 });

		Resources::Create<HIBBUFFER>(L"DTEXIBBUFFER", ArrIdx, D3D11_USAGE_DYNAMIC);
	}

	{
		HPTR<HMESH> NEWMESH = Resources::Create<HMESH>(L"DTEXMESH");
		NEWMESH->AddVB(L"DTEXVBBUFFER");
		NEWMESH->AddIB(L"DTEXIBBUFFER");
	}
#pragma endregion

#pragma region TargetTEXRECT
	{
		std::vector<EngineVertex::Vertex2D> ArrVtx;
		ArrVtx.resize(4);

		ArrVtx[0].Pos = Vector4(-1.0f, 1.0f, 0.0f, 1.0f); // 왼쪽위
		ArrVtx[1].Pos = Vector4(1.0f, 1.0f, 0.0f, 1.0f); // 오른쪽위
		ArrVtx[2].Pos = Vector4(-1.0f, -1.0f, 0.0f, 1.0f); // 왼쪽 아래
		ArrVtx[3].Pos = Vector4(1.0f, -1.0f, 0.0f, 1.0f); // 오른쪽 아래

		// 5번째 
		ArrVtx[0].vUv = Vector2(0.0f, 0.0f);
		ArrVtx[1].vUv = Vector2(1.0f, 0.0f);
		ArrVtx[2].vUv = Vector2(0.0f, 1.0f);
		ArrVtx[3].vUv = Vector2(1.0f, 1.0f);


		ArrVtx[0].Color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		ArrVtx[1].Color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		ArrVtx[2].Color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		ArrVtx[3].Color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);

		Resources::Create<HVBBUFFER>(L"TargetVBBUFFER", ArrVtx, D3D11_USAGE_DYNAMIC);

		std::vector<EngineIndex::Index32> ArrIdx;
		ArrIdx.push_back({ 0, 3, 2 });
		ArrIdx.push_back({ 0, 1, 3 });

		Resources::Create<HIBBUFFER>(L"TargetIBBUFFER", ArrIdx, D3D11_USAGE_DYNAMIC);
	}

	{
		HPTR<HMESH> NEWMESH = Resources::Create<HMESH>(L"TargetMESH");
		NEWMESH->AddVB(L"TargetVBBUFFER");
		NEWMESH->AddIB(L"TargetIBBUFFER");
	}
#pragma endregion




#pragma region 3DTEXBOX
	{
		std::vector<EngineVertex::Vertex3D> ArrVtx;
		ArrVtx.resize(24);

		{
			// 뒷면
			{
				// HVEC4(-1.0f, 0.0f,  0.0f, 0.0f)
				// HVEC4(0.0f,  1.0f,  0.0f, 0.0f)
				// HVEC4(0.0f,  0.0f, -1.0f, 0.0f)
				// HVEC4(-0.5f, 0.5f, -0.5f, 1.0f)
				ArrVtx[0] = { Vector4(-0.5f, 0.5f, -0.5f, 1.0f)  ,Vector2(0.f, 0.f), Vector4::WHITE,Vector4(0.0f, 0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[1] = { Vector4(0.5f, 0.5f, -0.5f, 1.0f)   ,Vector2(1.f, 0.f), Vector4::WHITE,Vector4(0.0f, 0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[2] = { Vector4(0.5f, -0.5f, -0.5f, 1.0f)  ,Vector2(1.f, 1.f), Vector4::WHITE,Vector4(0.0f, 0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[3] = { Vector4(-0.5f, -0.5f, -0.5f, 1.0f) ,Vector2(0.f, 1.f), Vector4::WHITE,Vector4(0.0f, 0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
			}
			// 앞면
			{
				ArrVtx[4] = { Vector4(-0.5f, 0.5f, 0.5f, 1.0f)   , Vector2(1.f, 0.f), Vector4::WHITE,Vector4(0.0f, 0.0f, 1.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[5] = { Vector4(0.5f, 0.5f, 0.5f, 1.0f)    , Vector2(0.f, 0.f), Vector4::WHITE,Vector4(0.0f, 0.0f, 1.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[6] = { Vector4(0.5f, -0.5f, 0.5f, 1.0f)   ,Vector2(0.f, 1.f) , Vector4::WHITE,Vector4(0.0f, 0.0f, 1.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[7] = { Vector4(-0.5f, -0.5f, 0.5f, 1.0f)  ,Vector2(1.f, 1.f) , Vector4::WHITE,Vector4(0.0f, 0.0f, 1.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
			}
			// 오른쪽
			{
				ArrVtx[8] = { Vector4(0.5f, 0.5f, -0.5f, 1.0f)      ,Vector2(1.f, 0.f) ,  Vector4::WHITE, Vector4(1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[9] = { Vector4(0.5f, 0.5f, 0.5f, 1.0f)     ,Vector2(0.f, 0.f)   ,  Vector4::WHITE, Vector4(1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[10] = { Vector4(0.5f, -0.5f, 0.5f, 1.0f)      ,Vector2(0.f, 1.f) , Vector4::WHITE, Vector4(1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[11] = { Vector4(0.5f, -0.5f, -0.5f, 1.0f)      ,Vector2(1.f, 1.f), Vector4::WHITE, Vector4(1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
			}
			// 왼쪽
			{
				ArrVtx[12] = { Vector4(-0.5f, 0.5f, -0.5f, 1.0f)   ,Vector2(0.f, 0.f),  Vector4::WHITE,Vector4(-1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[13] = { Vector4(-0.5f, 0.5f, 0.5f, 1.0f)    ,Vector2(1.f, 0.f),  Vector4::WHITE,Vector4(-1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[14] = { Vector4(-0.5f, -0.5f, 0.5f, 1.0f)   ,Vector2(1.f, 1.f),  Vector4::WHITE,Vector4(-1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[15] = { Vector4(-0.5f, -0.5f, -0.5f, 1.0f)  ,Vector2(0.f, 1.f),  Vector4::WHITE,Vector4(-1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };

			}
			// 아랫면
			{
				ArrVtx[16] = { Vector4(-0.5f, -0.5f, -0.5f, 1.0f)   ,Vector2(0.f, 0.f) ,  Vector4::WHITE,Vector4(0.0f, -1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, -1.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[17] = { Vector4(0.5f, -0.5f, -0.5f, 1.0f)    ,Vector2(1.f, 0.f) ,  Vector4::WHITE,Vector4(0.0f, -1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, -1.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[18] = { Vector4(0.5f, -0.5f, 0.5f, 1.0f)      ,Vector2(1.f, 1.f),  Vector4::WHITE,Vector4(0.0f, -1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, -1.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[19] = { Vector4(-0.5f, -0.5f, 0.5f, 1.0f)     ,Vector2(0.f, 1.f),  Vector4::WHITE,Vector4(0.0f, -1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, -1.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
			}
			// 윗면
			{
				ArrVtx[20] = { Vector4(-0.5f, 0.5f, -0.5f, 1.0f)   ,Vector2(0.f, 0.f) , Vector4::WHITE,Vector4(0.0f, 1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[21] = { Vector4(0.5f, 0.5f, -0.5f, 1.0f)    ,Vector2(1.f, 0.f) , Vector4::WHITE,Vector4(0.0f, 1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[22] = { Vector4(0.5f, 0.5f, 0.5f, 1.0f)     ,Vector2(1.f, 1.f) , Vector4::WHITE,Vector4(0.0f, 1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[23] = { Vector4(-0.5f, 0.5f, 0.5f, 1.0f)    ,Vector2(0.f, 1.f) , Vector4::WHITE,Vector4(0.0f, 1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
			}

		}

		Resources::Create<HVBBUFFER>(L"BOXVBBUFFER", ArrVtx, D3D11_USAGE_DYNAMIC);

		std::vector<EngineIndex::Index32> ArrIdx;

		// 뒷면
		ArrIdx.push_back({ 0, 1, 2 });
		ArrIdx.push_back({ 0, 2, 3 });
		// 앞면
		ArrIdx.push_back({ 6, 5, 4 });
		ArrIdx.push_back({ 7, 6, 4 });

		// 왼쪽
		ArrIdx.push_back({ 8, 9, 10 });
		ArrIdx.push_back({ 8, 10, 11 });
		// 오른쪽
		ArrIdx.push_back({ 14, 13, 12 });
		ArrIdx.push_back({ 15, 14, 12 });

		// 아랫면
		ArrIdx.push_back({ 16, 17, 18 });
		ArrIdx.push_back({ 16, 18, 19 });

		// 윗면
		ArrIdx.push_back({ 22, 21, 20 });
		ArrIdx.push_back({ 23, 22, 20 });
		Resources::Create<HIBBUFFER>(L"BOXIBBUFFER", ArrIdx, D3D11_USAGE_DYNAMIC);
	}

	{
		HPTR<HMESH> NEWMESH = Resources::Create<HMESH>(L"BOXMESH");
		NEWMESH->AddVB(L"BOXVBBUFFER");
		NEWMESH->AddIB(L"BOXIBBUFFER");
	}
#pragma endregion

#pragma region 3DTEXPLANE
	{
		std::vector<EngineVertex::Vertex3D> ArrVtx;
		ArrVtx.resize(24);

		{
			// 뒷면
			{
				ArrVtx[0] = { Vector4(-0.5f, 0.01f, -0.5f, 1.0f)  ,Vector2(0.f, 0.f), Vector4::WHITE,Vector4(0.0f, 0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[1] = { Vector4(0.5f, 0.01f, -0.5f, 1.0f)   ,Vector2(1.f, 0.f), Vector4::WHITE,Vector4(0.0f, 0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[2] = { Vector4(0.5f, -0.01f, -0.5f, 1.0f)  ,Vector2(1.f, 1.f), Vector4::WHITE,Vector4(0.0f, 0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[3] = { Vector4(-0.5f, -0.01f, -0.5f, 1.0f) ,Vector2(0.f, 1.f), Vector4::WHITE,Vector4(0.0f, 0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
			}
			// 앞면
			{
				ArrVtx[4] = { Vector4(-0.5f, 0.01f, 0.5f, 1.0f)   , Vector2(1.f, 0.f), Vector4::WHITE,Vector4(0.0f, 0.0f, 1.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[5] = { Vector4(0.5f, 0.01f, 0.5f, 1.0f)    , Vector2(0.f, 0.f), Vector4::WHITE,Vector4(0.0f, 0.0f, 1.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[6] = { Vector4(0.5f, -0.01f, 0.5f, 1.0f)   ,Vector2(0.f, 1.f) , Vector4::WHITE,Vector4(0.0f, 0.0f, 1.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[7] = { Vector4(-0.5f, -0.01f, 0.5f, 1.0f)  ,Vector2(1.f, 1.f) , Vector4::WHITE,Vector4(0.0f, 0.0f, 1.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
			}
			// 오른쪽
			{
				ArrVtx[8] = { Vector4(0.5f, 0.01f, -0.5f, 1.0f)      ,Vector2(1.f, 0.f) ,  Vector4::WHITE, Vector4(1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[9] = { Vector4(0.5f, 0.01f, 0.5f, 1.0f)     ,Vector2(0.f, 0.f)   ,  Vector4::WHITE, Vector4(1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[10] = { Vector4(0.5f, -0.01f, 0.5f, 1.0f)      ,Vector2(0.f, 1.f) , Vector4::WHITE, Vector4(1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[11] = { Vector4(0.5f, -0.01f, -0.5f, 1.0f)      ,Vector2(1.f, 1.f), Vector4::WHITE, Vector4(1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
			}
			// 왼쪽
			{
				ArrVtx[12] = { Vector4(-0.5f, 0.01f, -0.5f, 1.0f)   ,Vector2(0.f, 0.f),  Vector4::WHITE,Vector4(-1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[13] = { Vector4(-0.5f, 0.01f, 0.5f, 1.0f)    ,Vector2(1.f, 0.f),  Vector4::WHITE,Vector4(-1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[14] = { Vector4(-0.5f, -0.01f, 0.5f, 1.0f)   ,Vector2(1.f, 1.f),  Vector4::WHITE,Vector4(-1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[15] = { Vector4(-0.5f, -0.01f, -0.5f, 1.0f)  ,Vector2(0.f, 1.f),  Vector4::WHITE,Vector4(-1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };

			}
			// 아랫면
			{
				ArrVtx[16] = { Vector4(-0.5f, -0.01f, -0.5f, 1.0f)   ,Vector2(0.f, 0.f) ,  Vector4::WHITE,Vector4(0.0f, -1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, -1.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[17] = { Vector4(0.5f, -0.01f, -0.5f, 1.0f)    ,Vector2(1.f, 0.f) ,  Vector4::WHITE,Vector4(0.0f, -1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, -1.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[18] = { Vector4(0.5f, -0.01f, 0.5f, 1.0f)      ,Vector2(1.f, 1.f),  Vector4::WHITE,Vector4(0.0f, -1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, -1.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[19] = { Vector4(-0.5f, -0.01f, 0.5f, 1.0f)     ,Vector2(0.f, 1.f),  Vector4::WHITE,Vector4(0.0f, -1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, -1.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
			}
			// 윗면
			{
				ArrVtx[20] = { Vector4(-0.5f, 0.01f, -0.5f, 1.0f)   ,Vector2(0.f, 0.f) , Vector4::WHITE,Vector4(0.0f, 1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[21] = { Vector4(0.5f, 0.01f, -0.5f, 1.0f)    ,Vector2(1.f, 0.f) , Vector4::WHITE,Vector4(0.0f, 1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[22] = { Vector4(0.5f, 0.01f, 0.5f, 1.0f)     ,Vector2(1.f, 1.f) , Vector4::WHITE,Vector4(0.0f, 1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
				ArrVtx[23] = { Vector4(-0.5f, 0.01f, 0.5f, 1.0f)    ,Vector2(0.f, 1.f) , Vector4::WHITE,Vector4(0.0f, 1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 0.0f),Vector4(1.0f, 0.0f, 0.0f, 0.0f) };
			}

		}

		Resources::Create<HVBBUFFER>(L"PLANEVBBUFFER", ArrVtx, D3D11_USAGE_DYNAMIC);

		std::vector<EngineIndex::Index32> ArrIdx;

		// 뒷면
		ArrIdx.push_back({ 0, 1, 2 });
		ArrIdx.push_back({ 0, 2, 3 });
		// 앞면
		ArrIdx.push_back({ 6, 5, 4 });
		ArrIdx.push_back({ 7, 6, 4 });

		// 왼쪽
		ArrIdx.push_back({ 8, 9, 10 });
		ArrIdx.push_back({ 8, 10, 11 });
		// 오른쪽
		ArrIdx.push_back({ 14, 13, 12 });
		ArrIdx.push_back({ 15, 14, 12 });

		// 아랫면
		ArrIdx.push_back({ 16, 17, 18 });
		ArrIdx.push_back({ 16, 18, 19 });

		// 윗면
		ArrIdx.push_back({ 22, 21, 20 });
		ArrIdx.push_back({ 23, 22, 20 });
		Resources::Create<HIBBUFFER>(L"PLANEIBBUFFER", ArrIdx, D3D11_USAGE_DYNAMIC);
	}

	{
		HPTR<HMESH> NEWMESH = Resources::Create<HMESH>(L"PLANEMESH");
		NEWMESH->AddVB(L"PLANEVBBUFFER");
		NEWMESH->AddIB(L"PLANEIBBUFFER");
	}
#pragma endregion

#pragma region 3DWATER
	{
		std::vector<EngineVertex::Vertex3D> ArrVtx;
		
		
		ArrVtx.resize(60000);

		float divCnt = 100.0f;
		size_t divCntSize = (size_t )divCnt;
		{
			float div = 1.0f / divCnt;
			UINT vtxIndex = 0;

			for (size_t z = 0; z < divCntSize; z++)
			{
				for (size_t x = 0; x < divCntSize; x++)
				{
					ArrVtx[vtxIndex + 0] = { Vector4(-0.5f + (x * (div)),		0.0f, -0.5f + (z * (div)), 1.0f)		,Vector2((x * (div)),		(z * (div))),		Vector4::WHITE,Vector4(0.0f, 0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
					ArrVtx[vtxIndex + 1] = { Vector4(-0.5f + ((x + 1) * (div)), 0.0f, -0.5f + (z * (div)), 1.0f)        ,Vector2(((x + 1) * (div)), (z * (div))),	Vector4::WHITE,Vector4(0.0f, 0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
					ArrVtx[vtxIndex + 2] = { Vector4(-0.5f + ((x + 1) * (div)), 0.0f, -0.5f + ((z + 1) * (div)), 1.0f)  ,Vector2(((x + 1) * (div)), ((z + 1) * (div))),	Vector4::WHITE,Vector4(0.0f, 0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
					ArrVtx[vtxIndex + 3] = { Vector4(-0.5f + (x * (div)),		0.0f,-0.5f + ((z + 1) * (div)), 1.0f)	,Vector2((x * (div)),		((z + 1) * (div))),		Vector4::WHITE,Vector4(0.0f, 0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f),Vector4(-1.0f, 0.0f, 0.0f, 0.0f) };
					vtxIndex += 4;
				}
			}
		}

		Resources::Create<HVBBUFFER>(L"WATERVBBUFFER", ArrVtx, D3D11_USAGE_DYNAMIC);

		std::vector<EngineIndex::Index32> ArrIdx;

		for (UINT z = 0; z < divCntSize; z++)
		{
			UINT divZ = UINT(divCntSize * 4 * z);
			
			for (UINT x = 0; x < divCntSize; x++)
			{
				ArrIdx.push_back({ (2 + (x * 4) + divZ), 1 + (x * 4) + divZ, (x * 4) + divZ });
				ArrIdx.push_back({ (3 + (x * 4) + divZ), 2 + (x * 4) + divZ, (x * 4) + divZ });
			}
		}
		
		Resources::Create<HIBBUFFER>(L"WATERIBBUFFER", ArrIdx, D3D11_USAGE_DYNAMIC);
	}

	{
		HPTR<HMESH> NEWMESH = Resources::Create<HMESH>(L"WATERMESH");
		NEWMESH->AddVB(L"WATERVBBUFFER");
		NEWMESH->AddIB(L"WATERIBBUFFER");
	}
#pragma endregion

#pragma region 3DSPHEREMESH
	{
		EngineVertex::Vertex3D V;
		std::vector<EngineVertex::Vertex3D> SphereVtx;
		std::vector<DWORD> SphereIdx;

		float fRadius = 0.5f;

		// 북극점
		V.Pos = Vector4{ 0.0f, fRadius, 0.0f, 1.0f };
		V.vUv = Vector2{ 0.5f, 0.0f };
		V.Color = Vector4{ 1.0f, 1.0f, 1.0f, 1.0f };
		V.Normal = V.Pos;
		V.Normal.Normalization();
		V.Normal.w = 0.0f;
		V.Tangent = Vector4{ 1.0f, 0.0f, 0.0f, 0.0f };
		V.BiNormal = Vector4{ 0.0f, 0.0f, 1.0f, 0.0f };

		SphereVtx.push_back(V);

		UINT iStackCount = 40; // 가로 분할 개수
		UINT iSliceCount = 40; // 세로 분할 개수

		float yRotAngle = DirectX::XM_PI / (float)iStackCount;
		float zRotAngle = DirectX::XM_2PI / (float)iSliceCount;

		float yUvRatio = 1.0f / (float)iSliceCount;
		float zUvRatio = 1.0f / (float)iStackCount;

		// PointToDir

		for (UINT y = 1; y < iStackCount; ++y)
		{
			float phi = y * yRotAngle;

			for (UINT z = 0; z < iSliceCount + 1; ++z)
			{
				float theta = z * zRotAngle;

				V.Pos = Vector4
				{
					fRadius * sinf(y * yRotAngle) * cosf(z * zRotAngle),
					fRadius * cosf(y * yRotAngle),
					fRadius * sinf(y * yRotAngle) * sinf(z * zRotAngle),
					1.0f
				};


				V.Pos.w = 1.0f;
				V.vUv = Vector2{ yUvRatio * z, zUvRatio * y };
				V.Color = Vector4{ 1.0f, 1.0f, 1.0f, 1.0f };
				V.Normal = V.Pos;
				V.Normal.Normalization();
				V.Normal.w = 0.0f;

				V.Tangent.x = -fRadius * sinf(phi) * sinf(theta);
				V.Tangent.y = 0.0f;
				V.Tangent.z = fRadius * sinf(phi) * cosf(theta);
				V.Tangent.Normalization();
				V.Tangent.w = 0.0f;

				V.BiNormal = Vector4::CrossProduct(V.Tangent, V.Normal);
				V.BiNormal.Normalization();
				V.BiNormal.w = 0.0f;

				SphereVtx.push_back(V);
			}
		}

		// 남극점
		V.Pos = Vector4{ 0.0f, -fRadius, 0.0f, 1.0f };
		V.vUv = Vector2{ 0.5f, 1.0f };
		V.Color = Vector4{ 1.0f, 1.0f, 1.0f, 1.0f };
		V.Normal = V.Pos;
		V.Normal.Normalization();
		V.Normal.w = 0.0f;
		SphereVtx.push_back(V);

		// 인덱스 버퍼 만들기
		SphereIdx.clear();
		for (UINT i = 0; i < iSliceCount; i++)
		{
			SphereIdx.push_back(0);
			SphereIdx.push_back(i + 2);
			SphereIdx.push_back(i + 1);
		}


		for (UINT y = 0; y < iStackCount - 2; ++y)
		{
			for (UINT z = 0; z < iSliceCount; ++z)
			{
				SphereIdx.push_back((iSliceCount + 1) * y + z + 1);
				SphereIdx.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
				SphereIdx.push_back((iSliceCount + 1) * (y + 1) + z + 1);

				SphereIdx.push_back((iSliceCount + 1) * y + z + 1);
				SphereIdx.push_back((iSliceCount + 1) * y + (z + 1) + 1);
				SphereIdx.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
			}
		}

		// 북극점 인덱스
		UINT iBotStart = (UINT)SphereVtx.size() - 1;

		for (UINT i = 0; i < iSliceCount; ++i)
		{
			SphereIdx.push_back(iBotStart);
			SphereIdx.push_back(iBotStart - (i + 2));
			SphereIdx.push_back(iBotStart - (i + 1));
		}

		Resources::Create<HVBBUFFER>(L"SPHEREVBBUFFER", SphereVtx, D3D11_USAGE_DYNAMIC);
		Resources::Create<HIBBUFFER>(L"SPHEREIBBUFFER", SphereIdx, D3D11_USAGE_DYNAMIC);


		{
			HPTR<HMESH> NEWMESH = Resources::Create<HMESH>(L"SPHEREMESH");
			NEWMESH->AddVB(L"SPHEREVBBUFFER");
			NEWMESH->AddIB(L"SPHEREIBBUFFER");
		}
	}
#pragma endregion
}

void ShaderInit()
{
	// 상수버퍼는 나눠서 사용할수 있다.
	// 두번만들필요는 없다.
	// 달라지는 점은 이게 있을수가 있다.
	// BaseCB
	{
		Resources::Create<HCB>(L"MATBUF", HSHADER::SHADERTYPE::SHT_VS, sizeof(Matrix), 0);
		Resources::Create<HCB>(L"CUTBUF", HSHADER::SHADERTYPE::SHT_VS, sizeof(Vector4), 1);
		Resources::Create<HCB>(L"RECTCOLOR", HSHADER::SHADERTYPE::SHT_PS, sizeof(Vector4), 1);

		Resources::Create<HCB>(L"ALLLIGHTDATAV", HSHADER::SHADERTYPE::SHT_VS, sizeof(Light::ALLLIGHTDATA), 10);
		Resources::Create<HCB>(L"ALLLIGHTDATAP", HSHADER::SHADERTYPE::SHT_PS, sizeof(Light::ALLLIGHTDATA), 10);
		Resources::Create<HCB>(L"ALLLIGHTDATAD", HSHADER::SHADERTYPE::SHT_DS, sizeof(Light::ALLLIGHTDATA), 10);
		Resources::Create<HCB>(L"ALLLIGHTDATAG", HSHADER::SHADERTYPE::SHT_GS, sizeof(Light::ALLLIGHTDATA), 10);
		Resources::Create<HCB>(L"ALLLIGHTDATAH", HSHADER::SHADERTYPE::SHT_HS, sizeof(Light::ALLLIGHTDATA), 10);
	}

#pragma region CAMSH
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\CAMSHADER.fx", 5, 0, "VS_CAM");

		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->EndLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0);
		HPTR<HPIXSHADER> PIXSH = Resources::Load<HPIXSHADER>(Path::FindPathToString(L"SHADER") + L"\\CAMSHADER.fx", 5, 0, "PS_CAM");

		// PIXSH->CreateTEXDATA(L"PS_MAPTEX", 0);
		PIXSH->CreateSMPDATA(L"PS_LSMP", 0);

	}
#pragma endregion

#pragma region DEBUGTEXSH
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\DEBUGTEXSHADER.fx", 5, 0, "VS_TEX");

		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->EndLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0);
		VTXSH->CreateCB<Matrix>(L"VS_MATDATA", 0);
		HPTR<HPIXSHADER> PIXSH = Resources::Load<HPIXSHADER>(Path::FindPathToString(L"SHADER") + L"\\DEBUGTEXSHADER.fx", 5, 0, "PS_TEX");
		PIXSH->CreateTEXDATA(L"PS_MAPTEX", 0);
		PIXSH->CreateSMPDATA(L"PS_LSMP", 0);

	}
#pragma endregion

#pragma region DEBUGSH
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\DEBUGMESH.fx", 5, 0, "VS_DEBUGMESH");
		VTXSH->EndLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->CreateCB<Matrix>(L"VS_MATDATA", 0);
		Resources::Load<HPIXSHADER>(Path::FindPathToString(L"SHADER") + L"\\DEBUGMESH.fx", 5, 0, "PS_DEBUGMESH");
	}
#pragma endregion
#pragma region CSH
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\COLORRECT.fx", 5, 0, "VS_COLORRECT");
		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0);
		VTXSH->EndLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->CreateCB<Matrix>(L"VS_MATDATA", 0);

		Resources::Load<HPIXSHADER>(Path::FindPathToString(L"SHADER") + L"\\COLORRECT.fx", 5, 0, "PS_COLORRECT");
		// 컬러 쉐이더 하나 만듬
	}
#pragma endregion

#pragma region UITEXSH
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\UITEXSHADER.fx", 5, 0, "VS_TEX");
		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0);
		VTXSH->EndLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		VTXSH->CreateCB<MatrixData>(L"VS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		VTXSH->CreateCB<Vector4>(L"VS_UICUTDATA", 1);
		VTXSH->CreateCB<Vector4>(L"UIDATA", 2);

		// 한번에 많은 데이터를 보내기 위한 용도로 사용되는 경우가 많다.
		HPTR<HPIXSHADER> PIXSH = Resources::Load<HPIXSHADER>(Path::FindPathToString(L"SHADER") + L"\\UITEXSHADER.fx", 5, 0, "PS_TEX");
		PIXSH->CreateTEXDATA(L"PS_MAPTEX", 0);
		PIXSH->CreateSMPDATA(L"PS_LSMP", 0);
		PIXSH->CreateSMPDATA(L"PS_PSMP", 1);

		HPTR<HPIXSHADER> PIXTRANSHALF = Resources::LoadToName<HPIXSHADER>(L"PS_TRANSHALF", Path::FindPathToString(L"SHADER") + L"\\UITEXSHADER.fx", 5, 0, "PS_HALFTRANS");
		PIXTRANSHALF->CreateTEXDATA(L"PS_MAPTEX", 0);
		PIXTRANSHALF->CreateSMPDATA(L"PS_LSMP", 0);
		PIXTRANSHALF->CreateSMPDATA(L"PS_PSMP", 1);
	}
#pragma endregion

#pragma region BASETEXSH

	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\TEXSHADER.fx", 5, 0, "VS_TEX");
		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0);
		VTXSH->EndLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		// 타입을 넣어주자.
		// 트랜스폼 같은것은 자동으로 
		// 상수버퍼 타입이 트랜스로 지정해주면
		// 특별취급당한다.
		VTXSH->CreateCB<MatrixData>(L"VS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		VTXSH->CreateCB<Vector4>(L"VS_CUTDATA", 1);

		// 한번에 많은 데이터를 보내기 위한 용도로 사용되는 경우가 많다.
		HPTR<HPIXSHADER> PIXSH = Resources::Load<HPIXSHADER>(Path::FindPathToString(L"SHADER") + L"\\TEXSHADER.fx", 5, 0, "PS_TEX");
		PIXSH->CreateTEXDATA(L"PS_MAPTEX", 0);
		PIXSH->CreateCB<Vector4>(L"PS_STATDATA", 2, HSHADER::CBTYPE::TRANS);

		PIXSH->CreateSMPDATA(L"PS_LSMP", 0);
		PIXSH->CreateSMPDATA(L"PS_PSMP", 1);

		HPTR<HPIXSHADER> PIXSHHP = Resources::LoadToName<HPIXSHADER>(L"PS_HPMESH",Path::FindPathToString(L"SHADER") + L"\\TEXSHADER.fx", 5, 0, "PS_HPTEX");
		PIXSHHP->CreateTEXDATA(L"PS_MAPTEX", 0);
		PIXSHHP->CreateCB<Vector4>(L"PS_STATDATA", 2, HSHADER::CBTYPE::TRANS);

		PIXSHHP->CreateSMPDATA(L"PS_LSMP", 0);
		PIXSHHP->CreateSMPDATA(L"PS_PSMP", 1);

		HPTR<HPIXSHADER> PIXSHMP = Resources::LoadToName<HPIXSHADER>(L"PS_MPMESH", Path::FindPathToString(L"SHADER") + L"\\TEXSHADER.fx", 5, 0, "PS_MPTEX");
		PIXSHMP->CreateTEXDATA(L"PS_MAPTEX", 0);
		PIXSHMP->CreateCB<Vector4>(L"PS_STATDATA", 2, HSHADER::CBTYPE::TRANS);

		PIXSHMP->CreateSMPDATA(L"PS_LSMP", 0);
		PIXSHMP->CreateSMPDATA(L"PS_PSMP", 1);

		HPTR<HPIXSHADER> PIXSHCOOL = Resources::LoadToName<HPIXSHADER>(L"PS_COOLMESH", Path::FindPathToString(L"SHADER") + L"\\TEXSHADER.fx", 5, 0, "PS_COOLTEX");
		PIXSHCOOL->CreateTEXDATA(L"PS_MAPTEX", 0);
		PIXSHCOOL->CreateCB<Vector4>(L"PS_STATDATA", 2, HSHADER::CBTYPE::TRANS);

		PIXSHCOOL->CreateSMPDATA(L"PS_LSMP", 0);
		PIXSHCOOL->CreateSMPDATA(L"PS_PSMP", 1);
	}
#pragma endregion
#pragma region GRIDSH
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\HGRID.fx", 5, 0, "VS_GRID");
		VTXSH->EndLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->CreateCB<MatrixData>(L"VS_MATDATA", 0, HSHADER::CBTYPE::TRANS);

		HPTR<HPIXSHADER> PIXSH = Resources::Load<HPIXSHADER>(Path::FindPathToString(L"SHADER") + L"\\HGRID.fx", 5, 0, "PS_GRID");
		// PIXSH->CreateCB<HMATDATA>(L"PS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		PIXSH->CreateCB<GridRenderer::GRIDDATA>(L"GRIDDATA", 1);
	}
#pragma endregion
#pragma region EFFECTLIGHTSH
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\LIGHTMESHSHADEREFFECT.fx", 5, 0, "VS_MESH");
		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->EndLayOut("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		VTXSH->CreateCB<MatrixData>(L"VS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		VTXSH->CreateCB<Vector4>(L"VS_CUTDATA", 1);


		HPTR<HPIXSHADER> PIXSH = Resources::LoadToName<HPIXSHADER>(L"PS_EFFECTMESH", Path::FindPathToString(L"SHADER") + L"\\LIGHTMESHSHADEREFFECT.fx", 5, 0, "PS_EFFECTMESH");
		PIXSH->CreateCB<MatrixData>(L"PS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		PIXSH->CreateTEXDATA(L"PS_MAPTEX", 0);
		PIXSH->CreateTEXDATA(L"PS_BUMPTEX", 1);
		PIXSH->CreateSMPDATA(L"PS_LSMP", 0);
		PIXSH->CreateSMPDATA(L"PS_PSMP", 1);

	}
#pragma endregion

#pragma region LIGHTSH
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\LIGHTMESHSHADER.fx", 5, 0, "VS_MESH");
		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->EndLayOut("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		VTXSH->CreateCB<MatrixData>(L"VS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		VTXSH->CreateCB<Vector4>(L"VS_CUTDATA", 1);

		HPTR<HPIXSHADER> PIXSH = Resources::LoadToName<HPIXSHADER>(L"PS_MESH", Path::FindPathToString(L"SHADER") + L"\\LIGHTMESHSHADER.fx", 5, 0, "PS_MESH");
		PIXSH->CreateCB<MatrixData>(L"PS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		PIXSH->CreateTEXDATA(L"PS_MAPTEX", 0);
		PIXSH->CreateTEXDATA(L"PS_BUMPTEX", 1);
		PIXSH->CreateSMPDATA(L"PS_LSMP", 0);
		PIXSH->CreateSMPDATA(L"PS_PSMP", 1);

		HPTR<HPIXSHADER> DPIXSH = Resources::LoadToName<HPIXSHADER>(L"PS_DMESH", Path::FindPathToString(L"SHADER") + L"\\LIGHTMESHSHADER.fx", 5, 0, "PS_DMESH");
		DPIXSH->CreateCB<MatrixData>(L"PS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		//DPIXSH->CreateCB<Vector4>(L"PS_SEIRENDATA", 2, HSHADER::CBTYPE::TRANS);
		/*Vector4 seirenData = Vector4{ 0.0f, 0.0f, 0.0f };
		DPIXSH->LinkCB(L"PS_SEIRENDATA", &seirenData);*/

		
		DPIXSH->CreateTEXDATA(L"PS_MAPTEX", 0);
		DPIXSH->CreateTEXDATA(L"PS_BUMPTEX", 1);
		DPIXSH->CreateSMPDATA(L"PS_LSMP", 0);
		DPIXSH->CreateSMPDATA(L"PS_PSMP", 1);

		HPTR<HPIXSHADER> FBXDPIXSH = Resources::LoadToName<HPIXSHADER>(L"PS_FBXDMESH", Path::FindPathToString(L"SHADER") + L"\\LIGHTMESHSHADER.fx", 5, 0, "PS_FBXDMESH");
		FBXDPIXSH->CreateCB<MatrixData>(L"PS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		FBXDPIXSH->CreateCB<Vector4>(L"PS_SEIRENDATA", 3, HSHADER::CBTYPE::TRANS);
		/*Vector4 seirenData = Vector4{ 0.0f, 0.0f, 0.0f };
		DPIXSH->LinkCB(L"PS_SEIRENDATA", &seirenData);*/


		FBXDPIXSH->CreateTEXDATA(L"PS_MAPTEX", 0);
		FBXDPIXSH->CreateTEXDATA(L"PS_BUMPTEX", 1);
		FBXDPIXSH->CreateTEXDATA(L"PS_TESTTEX", 2);

		FBXDPIXSH->CreateSMPDATA(L"PS_LSMP", 0);
		FBXDPIXSH->CreateSMPDATA(L"PS_PSMP", 1);

		HPTR<HPIXSHADER> SKYPIXSH = Resources::LoadToName<HPIXSHADER>(L"SKY_MESH", Path::FindPathToString(L"SHADER") + L"\\LIGHTMESHSHADER.fx", 5, 0, "SKY_MESH");
		SKYPIXSH->CreateCB<MatrixData>(L"PS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		SKYPIXSH->CreateTEXDATA(L"PS_MAPTEX", 0);
		SKYPIXSH->CreateTEXDATA(L"PS_BUMPTEX", 1);
		SKYPIXSH->CreateSMPDATA(L"PS_LSMP", 0);
		SKYPIXSH->CreateSMPDATA(L"PS_PSMP", 1);

		HPTR<HPIXSHADER> DPIXEFFECTSH = Resources::LoadToName<HPIXSHADER>(L"PS_EFFECTDMESH", Path::FindPathToString(L"SHADER") + L"\\LIGHTMESHSHADER.fx", 5, 0, "PS_EFFECTDMESH");
		DPIXEFFECTSH->CreateCB<MatrixData>(L"PS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		DPIXEFFECTSH->CreateTEXDATA(L"PS_MAPTEX", 0);
		DPIXEFFECTSH->CreateTEXDATA(L"PS_BUMPTEX", 1);
		DPIXEFFECTSH->CreateSMPDATA(L"PS_LSMP", 0);
		DPIXEFFECTSH->CreateSMPDATA(L"PS_PSMP", 1);

		HPTR<HPIXSHADER> DPIXFBXEFFECTSH = Resources::LoadToName<HPIXSHADER>(L"PS_FBXEFFECTDMESH", Path::FindPathToString(L"SHADER") + L"\\LIGHTMESHSHADER.fx", 5, 0, "PS_FBXEFFECTDMESH");
		DPIXFBXEFFECTSH->CreateCB<MatrixData>(L"PS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		DPIXFBXEFFECTSH->CreateTEXDATA(L"PS_MAPTEX", 0);
		DPIXFBXEFFECTSH->CreateTEXDATA(L"PS_BUMPTEX", 1);
		DPIXFBXEFFECTSH->CreateSMPDATA(L"PS_LSMP", 0);
		DPIXFBXEFFECTSH->CreateSMPDATA(L"PS_PSMP", 1);
	}
#pragma endregion

#pragma region WATERSH
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\WATERMESHSHADER.fx", 5, 0, "VS_MESH");
		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->EndLayOut("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		VTXSH->CreateCB<MatrixData>(L"VS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		VTXSH->CreateCB<Vector4>(L"VS_CUTDATA", 1);
		VTXSH->CreateCB<Vector4>(L"VS_TIMEDATA", 2, HSHADER::CBTYPE::TRANS);

		HPTR<HPIXSHADER> PIXSH = Resources::LoadToName<HPIXSHADER>(L"PS_WATERMESH", Path::FindPathToString(L"SHADER") + L"\\WATERMESHSHADER.fx", 5, 0, "PS_MESH");
		PIXSH->CreateCB<MatrixData>(L"PS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		PIXSH->CreateCB<Vector4>(L"PS_TIMEDATA", 2, HSHADER::CBTYPE::TRANS);

		HPTR<HPIXSHADER> DPIXSH = Resources::LoadToName<HPIXSHADER>(L"PS_WATERDMESH", Path::FindPathToString(L"SHADER") + L"\\WATERMESHSHADER.fx", 5, 0, "PS_DMESH");
		DPIXSH->CreateCB<MatrixData>(L"PS_MATDATA", 0, HSHADER::CBTYPE::TRANS);


		DPIXSH->CreateTEXDATA(L"PS_MAPTEX", 0);
		DPIXSH->CreateTEXDATA(L"PS_BUMPTEX", 1);
		DPIXSH->CreateSMPDATA(L"PS_LSMP", 0);
		DPIXSH->CreateSMPDATA(L"PS_PSMP", 1);
	}
#pragma endregion

#pragma region TERRAINSH
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\TERRAIN.fx", 5, 0, "VS_TERRAIN");
		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->EndLayOut("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		VTXSH->CreateCB<MatrixData>(L"VS_MATDATA", 0, HSHADER::CBTYPE::TRANS);

		HPTR<HPIXSHADER> PIXSH = Resources::LoadToName<HPIXSHADER>(L"PS_TERRAIN", Path::FindPathToString(L"SHADER") + L"\\TERRAIN.fx", 5, 0, "PS_TERRAIN");
		PIXSH->CreateCB<MatrixData>(L"PS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		PIXSH->CreateTEXDATA(L"PS_MAPTEX", 0);
		PIXSH->CreateTEXDATA(L"PS_BUMPTEX", 1);
		PIXSH->CreateSMPDATA(L"PS_LSMP", 0);
		PIXSH->CreateSMPDATA(L"PS_PSMP", 1);

		HPTR<HPIXSHADER> DPIXSH = Resources::LoadToName<HPIXSHADER>(L"PS_DTERRAIN", Path::FindPathToString(L"SHADER") + L"\\TERRAIN.fx", 5, 0, "PS_DTERRAIN");
		DPIXSH->CreateCB<MatrixData>(L"PS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		DPIXSH->CreateCB<MatrixData>(L"TERRAINDATA", 1);
		DPIXSH->CreateTEXDATA(L"F0Tex", 0);
		DPIXSH->CreateTEXDATA(L"F0Bump", 1);

		DPIXSH->CreateTEXDATA(L"F1Tex", 2);
		DPIXSH->CreateTEXDATA(L"F1Bump", 3);
		DPIXSH->CreateTEXDATA(L"F1Sp", 4);

		DPIXSH->CreateTEXDATA(L"F2Tex", 5);
		DPIXSH->CreateTEXDATA(L"F2Bump", 6);
		DPIXSH->CreateTEXDATA(L"F2Sp", 7);

		DPIXSH->CreateTEXDATA(L"F3Tex", 8);
		DPIXSH->CreateTEXDATA(L"F3Bump", 9);
		DPIXSH->CreateTEXDATA(L"F3Sp", 10);



		//Texture2D F1Tex : register(t0);
		//Texture2D F1Bump : register(t1);

		//Texture2D F2Tex : register(t2);
		//Texture2D F2Bump : register(t3);
		//Texture2D F2Sp : register(t4);

		//Texture2D F3Tex : register(t5);
		//Texture2D F3Bump : register(t6);
		//Texture2D F3Sp : register(t7);

		//Texture2D F4Tex : register(t8);
		//Texture2D F4Bump : register(t9);
		//Texture2D F4Sp : register(t10);

		DPIXSH->CreateSMPDATA(L"PS_LSMP", 0);
		DPIXSH->CreateSMPDATA(L"PS_PSMP", 1);


	}
#pragma endregion

#pragma region DEFERREDLIGHTSHADER
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\DEFERREDLIGHTING.fx", 5, 0, "VS_DLIGHTING");
		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->EndLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		HPTR<HPIXSHADER> DPIXSH = Resources::Load<HPIXSHADER>(Path::FindPathToString(L"SHADER") + L"\\DEFERREDLIGHTING.fx", 5, 0, "PS_DLIGHTING");
		DPIXSH->CreateCB<Vector4>(L"DEFERREDCAMPOS", 1);
		DPIXSH->CreateCB<Light::ShadowData>(L"SHADOWDATA", 2);

		DPIXSH->CreateTEXDATA(L"Pos", 0);
		DPIXSH->CreateTEXDATA(L"Nomal", 1);
		DPIXSH->CreateTEXDATA(L"Depth", 2);
		DPIXSH->CreateTEXDATA(L"Shadow", 3);
		DPIXSH->CreateTEXDATA(L"Test", 4);
		DPIXSH->CreateTEXDATA(L"Test2", 5);
		//DPIXSH->CreateTEXDATA(L"Diff", 0);
		//DPIXSH->CreateTEXDATA(L"Pos", 1);
		//DPIXSH->CreateTEXDATA(L"Nomal", 2);
		//DPIXSH->CreateTEXDATA(L"Depth", 3);
		DPIXSH->CreateSMPDATA(L"PS_LSMP", 0);
	}
#pragma endregion

#pragma region DEFERREDLIGHTSHADER
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\DEFERREDLIGHTINGMERGE.fx", 5, 0, "VS_LIGHTMERGE");
		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->EndLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		HPTR<HPIXSHADER> DPIXSH = Resources::Load<HPIXSHADER>(Path::FindPathToString(L"SHADER") + L"\\DEFERREDLIGHTINGMERGE.fx", 5, 0, "PS_LIGHTMERGE");
		DPIXSH->CreateTEXDATA(L"DiffTex", 0);
		DPIXSH->CreateTEXDATA(L"LightDiffTex", 1);
		DPIXSH->CreateTEXDATA(L"LightSpecTex", 2);
		DPIXSH->CreateTEXDATA(L"TestTex", 3);
		//이부분추가해줘야한다
		DPIXSH->CreateSMPDATA(L"PS_LSMP", 0);
	}
#pragma endregion

#pragma region SHADOWSHADER
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\SHADOW.fx", 5, 0, "VS_SHADOW");
		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("BLENDWEIGHTS", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->EndLayOut("BLENDINDICES", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		VTXSH->CreateCB<MatrixData>(L"VS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		


		HPTR<HPIXSHADER> DPIXSH = Resources::Load<HPIXSHADER>(Path::FindPathToString(L"SHADER") + L"\\SHADOW.fx", 5, 0, "PS_SHADOW");
	}
#pragma endregion

#pragma region SHADOWSHADERANI
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\SHADOWANI.fx", 5, 0, "VS_SHADOW");
		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("BLENDWEIGHTS", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->EndLayOut("BLENDINDICES", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		VTXSH->CreateCB<MatrixData>(L"VS_MATDATA", 0, HSHADER::CBTYPE::TRANS);



		HPTR<HPIXSHADER> DPIXSH = Resources::Load<HPIXSHADER>(Path::FindPathToString(L"SHADER") + L"\\SHADOWANI.fx", 5, 0, "PS_SHADOW");
	}
#pragma endregion

#pragma region FBXANIVTXSHADER
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\LIGHTMESHSHADERFBX.fx", 5, 0, "VS_ANIMESH");
		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("BLENDWEIGHTS", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->EndLayOut("BLENDINDICES", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		VTXSH->CreateCB<MatrixData>(L"VS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
	}
#pragma endregion

#pragma region FBXANIEFFECTVTXSHADER
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\LIGHTMESHSHADERFBXEFFECT.fx", 5, 0, "VS_ANIMESH");
		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("BLENDWEIGHTS", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->EndLayOut("BLENDINDICES", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		VTXSH->CreateCB<MatrixData>(L"VS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
	}
#pragma endregion

#pragma region FBXSTATICVTXSHADER
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\LIGHTMESHSHADERSTATIC.fx", 5, 0, "VS_ANIMESH");
		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("BLENDWEIGHTS", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->EndLayOut("BLENDINDICES", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		VTXSH->CreateCB<MatrixData>(L"VS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		//VTXSH->CreateCB<Vector4>(L"VS_CUTDATA", 1);
		VTXSH->CreateCB<Vector4>(L"VS_TIMEDATA", 2, HSHADER::CBTYPE::TRANS);
	}
#pragma endregion

#pragma region FBXSTATICEFFECTVTXSHADER
	{
		HPTR<HVTXSHADER> VTXSH = Resources::Load<HVTXSHADER>(Path::FindPathToString(L"SHADER") + L"\\LIGHTEFFECTMESHSHADERSTATIC.fx", 5, 0, "VS_ANIMESH");
		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->AddLayOut("BLENDWEIGHTS", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->EndLayOut("BLENDINDICES", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

		VTXSH->CreateCB<MatrixData>(L"VS_MATDATA", 0, HSHADER::CBTYPE::TRANS);
		//VTXSH->CreateCB<Vector4>(L"VS_CUTDATA", 1);
		VTXSH->CreateCB<Vector4>(L"VS_TIMEDATA", 2, HSHADER::CBTYPE::TRANS);
	}
#pragma endregion

#pragma region BLOOM
	{
		HPTR<HVTXSHADER> VTXSH = Resources::LoadToName<HVTXSHADER>(L"VS_BLOOMSMALL", Path::FindPathToString(L"SHADER") + L"\\BLOOM.fx", 5, 0, "VS_BLOOMSMALL");
		VTXSH->AddLayOut("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->EndLayOut("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		VTXSH->CreateCB<Vector4>(L"VS_FilterSize", 0);

		HPTR<HPIXSHADER> DPIXSH = Resources::LoadToName<HPIXSHADER>(L"PS_BLOOMSMALL", Path::FindPathToString(L"SHADER") + L"\\BLOOM.fx", 5, 0, "PS_BLOOMSMALL");
		DPIXSH->CreateTEXDATA(L"LightDiffTarget", 0);
		DPIXSH->CreateTEXDATA(L"LightSpecTarget", 1);
		DPIXSH->CreateSMPDATA(L"PS_LSMP", 0);
		DPIXSH->CreateCB<Vector4>(L"PS_FilterSize", 0);

		HPTR<HPIXSHADER> EPIXSH = Resources::LoadToName<HPIXSHADER>(L"PS_BLOOMEFFECT", Path::FindPathToString(L"SHADER") + L"\\BLOOM.fx", 5, 0, "PS_BLOOMEFFECT");
		EPIXSH->CreateTEXDATA(L"SmallTarget", 0);
		EPIXSH->CreateTEXDATA(L"OriTarget", 1);
		EPIXSH->CreateSMPDATA(L"PS_LSMP", 0);
		EPIXSH->CreateCB<Vector4>(L"PS_FilterSize", 0);
	}
#pragma endregion

}


void RenderPathInit()
{
	{
		// 변경된것이 없어도 클론을 한다.
		// 메테리얼
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"CAMPATH");

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"CAMDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"CAMBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		PATH->SetVtxShader(L"CAMSHADER.fx");
		PATH->SetPxlShader(L"CAMSHADER.fx");
	}

	// 라이트 머지 패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"DFFERDLIGHTMERGEPATH");

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"CAMDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		// 디퍼드 패스.
		PATH->SetVtxShader(L"DEFERREDLIGHTINGMERGE.fx");
		PATH->SetPxlShader(L"DEFERREDLIGHTINGMERGE.fx");
	}

	// 블룸 스몰패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"BLOOMSMALLPATH");
		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"CAMDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"DBLEND");

		PATH->SetVtxShader(L"VS_BLOOMSMALL");
		PATH->SetPxlShader(L"PS_BLOOMSMALL");
	}

	// 블룸 이펙트패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"BLOOMEFFECTPATH");
		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"BLOOMDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"DBLEND");

		PATH->SetVtxShader(L"VS_BLOOMSMALL");
		PATH->SetPxlShader(L"PS_BLOOMEFFECT");
	}

	{
		// 변경된것이 없어도 클론을 한다.
		// 메테리얼
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"GRIDPATH");

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"GRIDRSSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		PATH->SetVtxShader(L"HGRID.fx");
		PATH->SetPxlShader(L"HGRID.fx");
	}

	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"SKYBOXPATH");

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"SKYSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		PATH->SetVtxShader(L"TEXSHADER.fx");
		PATH->SetPxlShader(L"TEXSHADER.fx");
	}
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"BASESPRITEPATH");

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"GRIDRSSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		PATH->SetVtxShader(L"TEXSHADER.fx");
		PATH->SetPxlShader(L"TEXSHADER.fx");
	}
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"SPRITEHPPATH");

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"GRIDRSSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		PATH->SetVtxShader(L"TEXSHADER.fx");
		PATH->SetPxlShader(L"PS_HPMESH");
	}
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"SPRITEMPPATH");

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"GRIDRSSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		PATH->SetVtxShader(L"TEXSHADER.fx");
		PATH->SetPxlShader(L"PS_MPMESH");
	}
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"SPRITECOOLPATH");

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"GRIDRSSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		PATH->SetVtxShader(L"TEXSHADER.fx");
		PATH->SetPxlShader(L"PS_COOLMESH");
	}

	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"UISPRITEPATH");

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"UIBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		PATH->SetVtxShader(L"UITEXSHADER.fx");
		PATH->SetPxlShader(L"UITEXSHADER.fx");
	}

	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"UISPRITEPATH_HF");

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"UIBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		PATH->SetVtxShader(L"UITEXSHADER.fx");
		PATH->SetPxlShader(L"PS_TRANSHALF");
	}
	// 라이트 포워드 패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"LIGHTMESHPATH");
		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		PATH->SetVtxShader(L"LIGHTMESHSHADER.fx");
		PATH->SetPxlShader(L"PS_MESH");
	}
	// 워터  패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"WATERMESHPATH");
		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"WATERBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		// 디퍼드 패스.
		PATH->SetVtxShader(L"WATERMESHSHADER.fx");
		PATH->SetPxlShader(L"PS_WATERMESH");
	}

	// 워터  D패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"WATERMESHDPATH");
		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetRenderType(RT_DEFERRED);

		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"WATERBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		// 디퍼드 패스.
		PATH->SetVtxShader(L"WATERMESHSHADER.fx");
		PATH->SetPxlShader(L"PS_WATERDMESH");
	}


	// 스카이메쉬패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"SKYMESHPATH");
		PATH->SetRenderType(RT_DEFERRED);

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"SKYSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		// 디퍼드 패스.
		PATH->SetVtxShader(L"LIGHTMESHSHADER.fx");
		PATH->SetPxlShader(L"SKY_MESH");
	}




	// 라이트 디퍼드 패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"ROCKMESHDPATH");
		PATH->SetRenderType(RT_DEFERRED);

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"SKYSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		// 디퍼드 패스.
		PATH->SetVtxShader(L"LIGHTMESHSHADER.fx");
		PATH->SetPxlShader(L"PS_DMESH");
	}

	// 라이트 포워드 패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"ROCKMESHPATH");
		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"SKYSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		PATH->SetVtxShader(L"LIGHTMESHSHADER.fx");
		PATH->SetPxlShader(L"PS_MESH");
	}

	// 라이트 디퍼드 패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"LIGHTMESHDPATH");
		PATH->SetRenderType(RT_DEFERRED);

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		// 디퍼드 패스.
		PATH->SetVtxShader(L"LIGHTMESHSHADER.fx");
		PATH->SetPxlShader(L"PS_DMESH");
	}

	// 라이트 이펙트 디퍼드 패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"EFFECTDPATH");
		PATH->SetRenderType(RT_DEFERRED);

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"EFFECTBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		// 디퍼드 패스.
		PATH->SetVtxShader(L"LIGHTMESHSHADEREFFECT.fx");
		PATH->SetPxlShader(L"PS_EFFECTMESH");
	}

	// 터레인 포워드 패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"TERRAINPATH");
		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		PATH->SetVtxShader(L"TERRAIN.fx");
		PATH->SetPxlShader(L"PS_TERRAIN");
	}
	// 터레인 디퍼드 패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"TERRAINDPATH");
		PATH->SetRenderType(RT_DEFERRED);

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		// 디퍼드 패스.
		PATH->SetVtxShader(L"TERRAIN.fx");
		PATH->SetPxlShader(L"PS_DTERRAIN");
	}


	// 애니메이션 포워드 패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"ANILIGHTMESHPATH");

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		// 디퍼드 패스.
		PATH->SetVtxShader(L"LIGHTMESHSHADERFBX.fx");
		PATH->SetPxlShader(L"PS_MESH");
	}



	//ANILIGHTMESHDPATH
	// 애니메이션 디퍼드 패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"ANILIGHTMESHDPATH");
		PATH->SetRenderType(RT_DEFERRED);

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"3DANIBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		// 디퍼드 패스.
		PATH->SetVtxShader(L"LIGHTMESHSHADERFBX.fx");
		PATH->SetPxlShader(L"PS_FBXDMESH");
	}

	// 애니메이션 .이펙트 디퍼드 패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"ANILIGHTEFFECTMESHDPATH");
		PATH->SetRenderType(RT_DEFERRED);

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"EFFECTBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		// 디퍼드 패스.
		PATH->SetVtxShader(L"LIGHTMESHSHADERFBXEFFECT.fx");
		PATH->SetPxlShader(L"PS_FBXEFFECTDMESH");
	}

	// 스태틱 포워드 패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"STATCLIGHTMESHPATH");

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"STATICRSSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		// 디퍼드 패스.
		PATH->SetVtxShader(L"LIGHTMESHSHADERSTATIC.fx");
		PATH->SetPxlShader(L"PS_MESH");
	}
	//스태틱 애니메이션 디퍼드 패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"STATCLIGHTMESHDPATH");
		PATH->SetRenderType(RT_DEFERRED);

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"STATICRSSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		// 디퍼드 패스.
		PATH->SetVtxShader(L"LIGHTMESHSHADERSTATIC.fx");
		PATH->SetPxlShader(L"PS_DMESH");
	}
	//이펙트 스태틱 애니메이션 디퍼드 패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"EFFECTMESHDPATH");
		PATH->SetRenderType(RT_DEFERRED);

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"STATICRSSTATE");
		PATH->SetBlend(L"EFFECTBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		// 디퍼드 패스.
		PATH->SetVtxShader(L"LIGHTEFFECTMESHSHADERSTATIC.fx");
		PATH->SetPxlShader(L"PS_EFFECTMESH");
	}

	// 라이트 디퍼드 패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"DFFERDLIGHTINGPATH");

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DSNONE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"DBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		// 디퍼드 패스.
		PATH->SetVtxShader(L"DEFERREDLIGHTING.fx");
		PATH->SetPxlShader(L"DEFERREDLIGHTING.fx");
	}


	// 라이트 디퍼드 STATIC 패스
	//{
	//	HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"DFFERDSTATICLIGHTINGPATH");

	//	PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	PATH->SetDSState(L"DSNONE");
	//	PATH->SetRSState(L"DRSSTATE");
	//	PATH->SetBlend(L"DBLEND");
	//	// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
	//	// 디퍼드 패스.
	//	PATH->SetVtxShader(L"DEFERREDSTATICLIGHTING.fx");
	//	PATH->SetPxlShader(L"DEFERREDSTATICLIGHTING.fx");
	//}

	//쉐도우 패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"SHADOWPATH");

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"SHADOWBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		// 디퍼드 패스.
		PATH->SetVtxShader(L"SHADOW.fx");
		PATH->SetPxlShader(L"SHADOW.fx");
	}

	//쉐도우 패스
	{
		HPTR<RenderPath> PATH = Resources::Create<RenderPath>(L"SHADOWANIPATH");

		PATH->SetDrawMode(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PATH->SetDSState(L"DDSSTATE");
		PATH->SetRSState(L"DRSSTATE");
		PATH->SetBlend(L"SHADOWBLEND");
		// 상수버퍼 이름 겹치면 터트리는거 여기서 터트리는게 맞겠지???
		// 디퍼드 패스.
		PATH->SetVtxShader(L"SHADOWANI.fx");
		PATH->SetPxlShader(L"SHADOWANI.fx");
	}

}

void RenderTargetInit() {
	// HPTR<HRENDERTARGET> NEWTarget = HRES::Create<HRENDERTARGET>(L"TEST");

}