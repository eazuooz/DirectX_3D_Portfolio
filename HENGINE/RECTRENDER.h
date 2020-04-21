#pragma once
#include "RENDER.h"

class HMESH;
class HCB;
class HVTXSHADER;
class HPIXSHADER;
class HSMP;
class Sprite;
class HBLEND;
class HDSSTATE;
class HRSSTATE;
class HRECTRENDER :
	public Renderer
{
private:
	Vector4 m_COLOR;
	HPTR<HMESH> m_HMESH;
	HPTR<HCB> m_HCB;
	HPTR<HCB> m_HCBCOLOR;
	HPTR<HVTXSHADER> m_HVTXSHADER;
	HPTR<HPIXSHADER> m_HPIXSHADER;
	HPTR<HSMP> m_SMP;
	HPTR<HBLEND> m_BLEND;
	HPTR<HDSSTATE> m_DSSTATE;
	HPTR<HRSSTATE> m_RSSTATE;

public:
	void Init() override;
	void Render(const HPTR<Camera>& _CAM) override;

public:
	// ¼÷Á¦·Î ºÓÀº»ö ±â¿îÀÌ ¶çµµ·Ï
	void SetColor(const Vector4& _COLOR)
	{
		m_COLOR = _COLOR;
	}


public:
	HRECTRENDER();
	~HRECTRENDER();
};

