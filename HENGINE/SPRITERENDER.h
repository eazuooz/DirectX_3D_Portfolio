#pragma once
#include "RENDER.h"

// 2D스프라이트에 특화된 렌더러를 만든다.
class HMESH;
class HCB;
class HVTXSHADER;
class HPIXSHADER;
class HSMP;
class Sprite;
class HBLEND;
class HDSSTATE;
class HRSSTATE;
class SpriteRenderer : public Renderer
{
public:
	friend Actor;

private:
	bool m_BillBoard;
	int m_SpriteIndex;

	HPTR<Sprite> m_Sprite;
	Vector4 m_Vec;

	Vector4 m_UIData;
	
	int m_LastIdx;

public:
	void BillBoardOn()
	{
		m_BillBoard = true;
		//FrontOn();
	}

	void BillBoardOff()
	{
		m_BillBoard = false;
	}

	void FrontOn()
	{
		m_Vec.x = 1.0f;
	}

	void FrontOff()
	{
		m_Vec.x = 0.0f;
	}

	Vector4& GetCutData()
	{
		return m_Vec;
	}

	Vector4& GetUIData()
	{
		return m_UIData;
	}

	int GetLastIdx()
	{
		return m_LastIdx;
	}

public:
	void Init() override;
	void DataCreate(int _Order);

public:
	void UpdateSubTrans(const HPTR<Camera>& _Cam) override;
	
	void SetSprite(const wchar_t* _SpriteName, int _SpriteIndex = 0);
	void SetSpriteIdx(int _SpriteIndex);
	void CalCutData(int _SpriteIndex);

public:
	SpriteRenderer();
	~SpriteRenderer();
};

