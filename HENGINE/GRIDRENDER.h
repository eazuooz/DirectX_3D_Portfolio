#pragma once
#include "RENDER.h"

class Camera;
class GridRenderer : public Renderer
{
public:
	// 바이트 패딩
	class GRIDDATA 
	{
	public:
		// 넣어줍시다.
		float Step; // 100~ 1000 ~ 10000
		float Borader; // 굵기
		float MinA; // 굵기
		float MaxA; // 굵기
		Vector4 Color;
		Vector4 CamPos;
	};

private:
	GRIDDATA m_Data;

public:
	void SetColor(Vector4 _Color) 
	{
		m_Data.Color = _Color;
	}



public:
	void RenderDataUpdate(const HPTR<Camera>& _CAM, HRENDERDATA* _Data) override;
	void Render(const HPTR<Camera>& _CAM) override;

public:
	void Init() override;
	void Update() override;


public:
	GridRenderer();
	~GridRenderer();
};

