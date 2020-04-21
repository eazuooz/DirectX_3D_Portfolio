#pragma once
#include "RENDER.h"


class HEFONT;
class TextRenderer : public Renderer
{
private:
	HEFONT*				m_FONT;
	class GRIDDATA
	{
	public:
		// ³Ö¾îÁÝ½Ã´Ù.
		float Step; // 100~ 1000 ~ 10000
		float Borader; // ±½±â
		float MinA; // ±½±â
		float MaxA; // ±½±â
		Vector4 Color;
		Vector4 CamPos;
	};

private:
	GRIDDATA m_Data;

	class TextInfo
	{
	public:
		std::wstring FontName;
		std::wstring Text;
		float Size;
		Vector4 Color;
	};


private:
	TextInfo m_Text;
public:
	void SetText(/*std::wstring _FontName,*/ std::wstring _Text, float _Size, Vector4 Color)
	{
		m_Text.Text = _Text;
		m_Text.Size = _Size;
		m_Text.Color = Color;
	}

	void SetFont(std::wstring _FontName);
	void SetColorAlpha(float _Value);
	void SetColorAlphaMul(float _Value);
public:
	void Init() override;
	void Render(const HPTR<Camera>& _CAM) override;
	void UpdateSubTrans(const HPTR<Camera>& _Cam) override;
	void Update() override;
	void RenderUpdate() override;
	void TextRenderUpdate() override;

public:
	TextRenderer();
	~TextRenderer();
};

