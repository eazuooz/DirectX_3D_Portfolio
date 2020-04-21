#include "GRAPHICDEBUG.h"
#include "HDSSTATE.h"
#include "SCENE.h"

#include "HMESH.h"
#include "HCB.h"
#include "HVTXSHADER.h"
#include "HPIXSHADER.h"
#include "HBLEND.h"
#include "HDSSTATE.h"
#include "HRSSTATE.h"
#include "TEX.h"
#include "CAMERA.h"


#ifdef RELEASEMODE
bool GraphicDebug::IsDebug = false;
#else
bool GraphicDebug::IsDebug = true;
#endif

bool GraphicDebug::IsInit = false;

Vector4 GraphicDebug::StartTextPos = Vector4::ZERO;

std::vector<GraphicDebug::DDI> GraphicDebug::m_DebugRenderList;
std::vector<GraphicDebug::TDDI> GraphicDebug::m_DebugRenderText;
std::vector<GraphicDebug::DDITEX> GraphicDebug::m_DebugRenderTex;
int GraphicDebug::m_DrawCount = 0;
int GraphicDebug::m_DrawTextCount = 0;
int GraphicDebug::m_TexCount = 0;
HPTR<HMESH>			GraphicDebug::m_HMESH[DEBUGDRAWTYPE::MAX] = { nullptr };
HPTR<HCB>			GraphicDebug::m_HCB= nullptr;
HPTR<HCB>			GraphicDebug::m_HCBTEX = nullptr;
HPTR<HSMP>			GraphicDebug::m_TEXSMP = nullptr;
HPTR<HVTXSHADER>	GraphicDebug::m_HVTXSHADER= nullptr;
HPTR<HPIXSHADER>	GraphicDebug::m_HPIXSHADER= nullptr;
HPTR<HVTXSHADER>	GraphicDebug::m_HVTXTEXSHADER = nullptr;
HPTR<HPIXSHADER>	GraphicDebug::m_HPIXTEXSHADER = nullptr;
HPTR<HBLEND>		GraphicDebug::m_BLEND= nullptr;
HPTR<HDSSTATE>		GraphicDebug::m_DSSTATE= nullptr;
HPTR<HDSSTATE>		GraphicDebug::m_DSCAMSTATE = nullptr;
HPTR<HDSSTATE>		GraphicDebug::m_TEXDSSTATE = nullptr;
HPTR<HRSSTATE>		GraphicDebug::m_RSSTATE= nullptr;

GraphicDebug::GraphicDebug()
{
}


GraphicDebug::~GraphicDebug()
{
}


// 디바이스를 받아서 사용해야 한다.
// MainDevice

void GraphicDebug::Init() 
{
	if (true == IsInit)
	{
		return;
	}

	m_HCB = Resources::Find<HCB>(L"MATBUF");
	m_HCBTEX = Resources::Find<HCB>(L"CUTBUF");
	m_TEXSMP = Resources::Find<HSMP>(L"LSMP");
	m_TEXDSSTATE = Resources::Find<HDSSTATE>(L"DSNONE");
	m_DSCAMSTATE = Resources::Find<HDSSTATE>(L"DEBUGCAMDSSTATE");
	m_DSSTATE = Resources::Find<HDSSTATE>(L"DEBUGDSSTATE");
	m_RSSTATE = Resources::Find<HRSSTATE>(L"DEBUGRSSTATE");
	m_BLEND = Resources::Find<HBLEND>(L"DEBUGBLEND");
	m_HVTXSHADER = Resources::Find<HVTXSHADER>(L"DEBUGMESH.fx");
	m_HPIXSHADER = Resources::Find<HPIXSHADER>(L"DEBUGMESH.fx");
	m_HVTXTEXSHADER = Resources::Find<HVTXSHADER>(L"DEBUGTEXSHADER.fx");;
	m_HPIXTEXSHADER = Resources::Find<HPIXSHADER>(L"DEBUGTEXSHADER.fx");;
	m_DebugRenderList.resize(1000);
	m_DebugRenderText.resize(100);
	m_DebugRenderTex.resize(100);

	m_HMESH[DEBUGDRAWTYPE::RECT] = Resources::Find<HMESH>(L"DEBUGRECT");
	m_HMESH[DEBUGDRAWTYPE::CIRCLE] = Resources::Find<HMESH>(L"DEBUGCIRCLE");
	m_HMESH[DEBUGDRAWTYPE::BOX] = Resources::Find<HMESH>(L"DEBUGBOX");
	m_HMESH[DEBUGDRAWTYPE::OCTAHEDRON] = Resources::Find<HMESH>(L"DEBUGOCTAHEDRON");
	m_HMESH[DEBUGDRAWTYPE::SPHERE] = Resources::Find<HMESH>(L"DEBUGSPHERE");
	m_HMESH[DEBUGDRAWTYPE::TEX] = Resources::Find<HMESH>(L"DTEXMESH");

	IsInit = true;
}

void GraphicDebug::DebugCamRender()
{
	for (int i = 0; i < m_DrawCount; i++)
	{
		Matrix WVP = m_DebugRenderList[i]._W * m_DebugRenderList[i]._V * m_DebugRenderList[i]._P;
		Matrix WVPT = WVP;

		WVPT.TransPose();

		// 최대한 덜 호출하는 것이 빠를것이다.
		m_HCB->DataSetting(&WVPT, sizeof(Matrix));
		m_HCB->Update();
		m_BLEND->Update();
		GraphicDevice::MainContext()->IASetPrimitiveTopology(m_DebugRenderList[i].m_MeshDrawType);
		m_DSCAMSTATE->Update();
		m_RSSTATE->Update();

		m_HVTXSHADER->Update();
		m_HPIXSHADER->Update();

		m_DebugRenderList[i].m_Cam->FORWARDTargetSet();
		m_HMESH[m_DebugRenderList[i].Type]->Update(0, 0);
		m_HMESH[m_DebugRenderList[i].Type]->Render(0);
	}

	m_DrawCount = 0;
}

void GraphicDebug::DebugRender() 
{


	// 택스처 찍는다.
	for (int i = 0; i < m_TexCount; i++)
	{
		Matrix WVP = m_DebugRenderTex[i]._W * m_DebugRenderTex[i]._V * m_DebugRenderTex[i]._P;
		Matrix WVPT = WVP;

		WVPT.TransPose();

		m_DebugRenderTex[i].m_Tex->Update(HSHADER::SHADERTYPE::SHT_PS, 0);
		m_TEXSMP->Update(HSHADER::SHADERTYPE::SHT_PS, 0);
		// 최대한 덜 호출하는 것이 빠를것이다.
		m_HCB->DataSetting(&WVPT, sizeof(Matrix));
		m_HCB->Update();
		m_BLEND->Update();
		GraphicDevice::MainContext()->IASetPrimitiveTopology(m_DebugRenderTex[i].m_MeshDrawType);

		m_DSSTATE->Update();
		m_RSSTATE->Update();
		// m_TEXDSSTATE->Update();

		m_HVTXTEXSHADER->Update();
		m_HPIXTEXSHADER->Update();

		m_HMESH[m_DebugRenderTex[i].Type]->Update(0, 0);
		m_HMESH[m_DebugRenderTex[i].Type]->Render(0);

		// m_DebugRenderTex[i].m_Tex = nullptr;
	}

	m_TexCount = 0;
	

	// 텍스트 찍는다.
	for (int i = 0; i < m_DrawTextCount; i++)
	{
		HPTR<HEFONT> Font = Resources::Find<HEFONT>(m_DebugRenderText[i].FontName);

		if (nullptr == Font)
		{
			BOOM;
			continue;
		}

		// 이녀석 하고 나면
		// 이녀석이 지오메트리쉐이더
		// 컴퓨트쉐이더 
		Font->DrawUI(m_DebugRenderText[i].Text.c_str(), 20.0f, StartTextPos + (Vector4::UP * 20.0F * (float)i), m_DebugRenderText[i].Color);
	}

	m_DrawTextCount = 0;
}

void GraphicDebug::DrawBox(HPTR<Camera> _Cam, Matrix _W)
{
	m_DebugRenderList[m_DrawCount].m_Cam = _Cam;
	m_DebugRenderList[m_DrawCount].Type = DEBUGDRAWTYPE::BOX;
	m_DebugRenderList[m_DrawCount]._W = _W;
	m_DebugRenderList[m_DrawCount]._V = _Cam->GetViewMatrix();
	m_DebugRenderList[m_DrawCount]._P = _Cam->PMAT();
	m_DebugRenderList[m_DrawCount].m_MeshDrawType = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
	++m_DrawCount;
}

void GraphicDebug::DrawCircle(HPTR<Camera> _Cam, Matrix _W) 
{
	m_DebugRenderList[m_DrawCount].m_Cam = _Cam;
	m_DebugRenderList[m_DrawCount].m_Cam = _Cam;
	m_DebugRenderList[m_DrawCount].Type = DEBUGDRAWTYPE::CIRCLE;
	m_DebugRenderList[m_DrawCount]._W = _W;
	m_DebugRenderList[m_DrawCount]._V = _Cam->GetViewMatrix();
	m_DebugRenderList[m_DrawCount]._P = _Cam->PMAT();
	m_DebugRenderList[m_DrawCount].m_MeshDrawType = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP;
	++m_DrawCount;
}

void GraphicDebug::DrawRect(HPTR<Camera> _Cam, Matrix _W) 
{
	m_DebugRenderList[m_DrawCount].m_Cam = _Cam;
	m_DebugRenderList[m_DrawCount].Type = DEBUGDRAWTYPE::RECT;
	m_DebugRenderList[m_DrawCount]._W = _W;
	m_DebugRenderList[m_DrawCount]._V = _Cam->GetViewMatrix();
	m_DebugRenderList[m_DrawCount]._P = _Cam->PMAT();
	m_DebugRenderList[m_DrawCount].m_MeshDrawType = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
	++m_DrawCount;
}

void GraphicDebug::DrawOctahedron(HPTR<Camera> _Cam, Matrix _W) 
{
	m_DebugRenderList[m_DrawCount].m_Cam = _Cam;
	m_DebugRenderList[m_DrawCount].Type = DEBUGDRAWTYPE::OCTAHEDRON;
	m_DebugRenderList[m_DrawCount]._W = _W;
	m_DebugRenderList[m_DrawCount]._V = _Cam->GetViewMatrix();
	m_DebugRenderList[m_DrawCount]._P = _Cam->PMAT();
	m_DebugRenderList[m_DrawCount].m_MeshDrawType = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP;
	++m_DrawCount;
}


void GraphicDebug::DrawSphere(HPTR<Camera> _Cam, Matrix _W)
{
	m_DebugRenderList[m_DrawCount].m_Cam = _Cam;
	m_DebugRenderList[m_DrawCount].Type = DEBUGDRAWTYPE::SPHERE;
	m_DebugRenderList[m_DrawCount]._W = _W;
	m_DebugRenderList[m_DrawCount]._V = _Cam->GetViewMatrix();
	m_DebugRenderList[m_DrawCount]._P = _Cam->PMAT();
	m_DebugRenderList[m_DrawCount].m_MeshDrawType = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP;
	++m_DrawCount;
}

void GraphicDebug::DrawRect(HPTR<Camera> _Cam, Vector4 _Pos, Vector4 _Rot, Vector4 _Scale, Vector4 _Color) 
{
	// m_DebugRenderList[m_DrawCount]._P = _Cam
	++m_DrawCount;
}

void GraphicDebug::DrawRect(Vector4 _Pos, Vector4 _Rot, Vector4 _Scale, Vector4 _Color) 
{
	++m_DrawCount;
}
void GraphicDebug::DrawCircle(HPTR<Camera> _Cam, Vector4 _Pos, Vector4 _Rot, Vector4 _Scale, Vector4 _Color) 
{
	++m_DrawCount;
}
void GraphicDebug::DrawCircle(Vector4 _Pos, Vector4 _Rot, Vector4 _Scale, Vector4 _Color) 
{
	++m_DrawCount;
}

void GraphicDebug::DrawFontUI(const wchar_t* _Text, Vector4 COLOR)
{
	m_DebugRenderText[m_DrawTextCount].FontName = L"굴림체";
	m_DebugRenderText[m_DrawTextCount].Text = _Text;
	m_DebugRenderText[m_DrawTextCount].Color = COLOR;
	++m_DrawTextCount;
}

void GraphicDebug::DrawTexture(HPTR<Texture> _Tex, Vector4 _Pos, Vector4 _Size, Vector4 _TexSize)
{
	Matrix PosMat;
	PosMat.Translation(_Pos);
	Matrix SizeMat;
	SizeMat.Scaling(_Size);

	Matrix View;
	View.SetViewMatrix(Vector4(0.0F, 0.0F, -10.0F), Vector4(0.0F, 0.0F, 1.0F), Vector4(0.0F, 1.0F, 0.0F));
	Matrix Porj;

	
	if (_TexSize == Vector4::ZERO)
	{
		Porj.SetOrthgonalMatrix(Window::MainWindow()->Size().x, Window::MainWindow()->Size().y, 0.1f, 1000.0f);
	}
	else 
	{
		Porj.SetOrthgonalMatrix(_TexSize.x, _TexSize.y, 0.1f, 1000.0f);
	}


	m_DebugRenderTex[m_TexCount].Type = DEBUGDRAWTYPE::TEX;
	m_DebugRenderTex[m_TexCount]._W =  SizeMat * PosMat;
	m_DebugRenderTex[m_TexCount]._V = View;
	m_DebugRenderTex[m_TexCount]._P = Porj;
	m_DebugRenderTex[m_TexCount].m_Tex = _Tex;
	m_DebugRenderTex[m_TexCount].m_MeshDrawType = D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	++m_TexCount;
}