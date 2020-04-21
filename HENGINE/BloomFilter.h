#pragma once
#include "CAMERA.h"


class BloomFilter : public Camera::Filter
{
public:
	Vector4 FilterSize;
	Vector4 SFilterSize;

	D3D11_VIEWPORT m_SmallViewPort;// 뷰포트행렬용 정보.
	D3D11_VIEWPORT m_SSmallViewPort;// 뷰포트행렬용 정보.
	HPTR<HRENDERTARGET> SmallTaregt;
	HPTR<HRENDERTARGET> SSmallTaregt;
	HPTR<HRENDERTARGET> SpecCopyTarget;
	HPTR<RenderPath> m_BloomSmallPath;
	HPTR<RenderPath> m_BloomSmallEffectPath;
	HPTR<RenderPath> m_BloomEffectPath;
	HPTR<HMESH> m_Mesh;

public:
	void InitFilter() override;

private:
	void MergeBeforeEffect() override;
	void MergeAfterEffect() override;

public:
	BloomFilter();
	~BloomFilter();
};

