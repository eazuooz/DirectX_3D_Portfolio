#pragma once
#include "RENDER.h"
class Terrain : public Renderer
{
public:
	class FloorData
	{
	public:
		float m_OnOff;
		float m_Detail;
		float m_Bump;
		float m_Temp;
	};

	class TerrainData
	{
	public:
		Vector4 m_Size;
		FloorData m_ArrData[4];
	};

	HPTR<HMESH> NEWMESH;
	Vector4 m_PivotSize;
	std::vector<Vector4> ArrPos;

public:
	std::vector<Vector4> GetVtxArr()
	{
		return ArrPos;
	}


public:
	TerrainData m_Data;
	void CreateTerrain(int _X, int _Z, const wchar_t* _Name, const wchar_t* _HeightName, float _HRatio, float _Detail = 1.0f);

	void FloorTextureSetting(int _X, const wchar_t* _Name, const wchar_t* _SpName, float _Detail = 1.0f);
	void FloorBumpSetting(int _X, const wchar_t* _Name);


	//void Update() override;
	void RenderUpdate() override;
	void RenderDataUpdate(const HPTR<Camera>& _CAM, HRENDERDATA* _Data) override;
	//void Render(const HPTR<Camera>& _CAM) override;

	float WPosToY(Vector4 _Pos);
	Vector4 WPosToIndex(Vector4 Pos);

public:
	Terrain();
	~Terrain();
};

