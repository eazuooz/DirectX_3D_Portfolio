#pragma once
#include "RENDER.h"
#include "FBX.h"

class Fbx;
class Texture;
class StaticFbxRender : public Renderer
{
private:
	HPTR<Fbx> m_FBXDATA;
	HRENDERDATA* m_RenderData;


public:
	HRENDERDATA* GetRenderData()
	{
		return m_RenderData;
	}

public:
	void SetShadow(const bool _Shadow)
	{
		m_RenderData->m_bShadow = _Shadow;
	}

public:
	void FbxSetting(const wchar_t* _Name);
	void FbxEffectSetting(const wchar_t* _Name);

	void FbxSetting(const wchar_t* _Name, const wchar_t* _PathName);
	void FbxSetting(const wchar_t* _Name, const wchar_t* _PathName, bool _Shadow);


public:
	void Update() override;
	void UpdateBefore() override;
	void RenderDataUpdate(const HPTR<Camera>& _CAM, HRENDERDATA* _Data) override;
	void Render(const HPTR<Camera>& _CAM) override;

public:
	StaticFbxRender();
	~StaticFbxRender();
};

