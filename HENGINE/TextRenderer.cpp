#include "TextRenderer.h"
#include "HMESH.h"
#include "HCB.h"
#include "HVTXSHADER.h"
#include "HPIXSHADER.h"
#include "HSMP.h"
#include "HBLEND.h"
#include "HDSSTATE.h"
#include "HRSSTATE.h"
#include "TRANS.h"
#include "HEFONT.h"




TextRenderer::TextRenderer()
{
}


TextRenderer::~TextRenderer()
{
}


void TextRenderer::Init()
{
	Renderer::Init();
	m_FONT = Resources::Find<HEFONT>(L"³ª´®°íµñ");
}
void TextRenderer::Render(const HPTR<Camera>& _CAM)
{
}
void TextRenderer::UpdateSubTrans(const HPTR<Camera>& _Cam)
{
}

void TextRenderer::SetFont(std::wstring _FontName)
{
	m_Text.FontName = _FontName;
	m_FONT = Resources::Find<HEFONT>(_FontName);
}

void TextRenderer::SetColorAlpha(float _Value)
{
	m_Text.Color.w = _Value;
}

void TextRenderer::SetColorAlphaMul(float _Value)
{
	m_Text.Color.w *= _Value;
}
void TextRenderer::Update()
{

}


void TextRenderer::RenderUpdate()
{

}

void TextRenderer::TextRenderUpdate()
{
	UpdateTransform();
	Matrix W = GetWorldMatrix();
	Vector4 CameraWorldPos = Camera::GetUICam()->GetTransform()->GetPosition();//Camera::UICam()->Trans()->WorldPos();

	CameraWorldPos.z = 0.0f;
	CameraWorldPos.w = 1.0f;
	Vector4 TextWorldPos = { W._30, W._31, 0.0f, 1.0f };
	Vector4 TextPos;
	TextPos.x = (1600.0f / 2) - ((CameraWorldPos.x - TextWorldPos.x) * (900.0f / 900.0f));
	TextPos.y = (900.0f / 2) + ((CameraWorldPos.y - TextWorldPos.y) * (900.0f / 900.0f));
	m_FONT->DrawUI(m_Text.Text.c_str(), m_Text.Size, TextPos, m_Text.Color, FW1_CENTER);
}