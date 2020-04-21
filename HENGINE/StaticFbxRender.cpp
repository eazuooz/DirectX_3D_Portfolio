#include "StaticFbxRender.h"
#include <JTIME.h>

StaticFbxRender::StaticFbxRender()
{
}


StaticFbxRender::~StaticFbxRender()
{
}


void StaticFbxRender::Update()
{

}

void StaticFbxRender::UpdateBefore()
{

}

void StaticFbxRender::RenderDataUpdate(const HPTR<Camera>& _CAM, HRENDERDATA* _Data)
{

}

void StaticFbxRender::Render(const HPTR<Camera>& _CAM)
{
}

void StaticFbxRender::FbxSetting(const wchar_t* _Name)
{
	m_FBXDATA = Resources::Find<Fbx>(_Name);

	if (nullptr == m_FBXDATA)
	{
		BOOM;
	}


	// RenderData()
	// ANILIGHTMESHDPATH

	for (size_t i = 0; i < m_FBXDATA->MeshData().size(); i++)
	{
		for (size_t j = 0; j < m_FBXDATA->MeshData()[i]->vecIndices.size(); j++)
		{
			std::wstring MeshName = _Name;
			MeshName += L'0' + (wchar_t)j;
			m_RenderData = RenderData(MeshName.c_str(), L"STATCLIGHTMESHDPATH");
			m_RenderData->m_bShadow = true;
			m_RenderData->m_Path->m_BUMP = BUMPOK;

			

			std::wstring DifTexName = IOPath::PathToFileName(m_FBXDATA->MaterialData()[i][j]->DifTex.c_str());
			if (L"" != DifTexName)
			{
				LinkTexture((int)j, L"PS_MAPTEX", DifTexName.c_str());
			}
						
			std::wstring BumpTexName = IOPath::PathToFileName(m_FBXDATA->MaterialData()[i][j]->BumpTex.c_str());
			if (L"" != BumpTexName)
			{
				LinkTexture((int)j, L"PS_BUMPTEX", BumpTexName.c_str());
			}
			LinkSampler((int)j, L"PS_LSMP", L"LSMP");
			LinkSampler((int)j, L"PS_PSMP", L"PSMP");
		}
	}
}

void StaticFbxRender::FbxEffectSetting(const wchar_t* _Name)
{
	m_FBXDATA = Resources::Find<Fbx>(_Name);

	if (nullptr == m_FBXDATA)
	{
		BOOM;
	}


	// RenderData()
	// ANILIGHTMESHDPATH

	for (size_t i = 0; i < m_FBXDATA->MeshData().size(); i++)
	{
		for (size_t j = 0; j < m_FBXDATA->MeshData()[i]->vecIndices.size(); j++)
		{
			std::wstring MeshName = _Name;
			MeshName += L'0' + (wchar_t)j;
			m_RenderData = RenderData(MeshName.c_str(), L"EFFECTMESHDPATH");
			m_RenderData->m_bShadow = false;
			m_RenderData->m_Path->m_BUMP = BUMPOK;

			std::wstring DifTexName = IOPath::PathToFileName(m_FBXDATA->MaterialData()[i][j]->DifTex.c_str());
			if (L"" != DifTexName)
			{
				LinkTexture((int)j, L"PS_MAPTEX", DifTexName.c_str());
			}

			std::wstring BumpTexName = IOPath::PathToFileName(m_FBXDATA->MaterialData()[i][j]->BumpTex.c_str());
			if (L"" != BumpTexName)
			{
				LinkTexture((int)j, L"PS_BUMPTEX", BumpTexName.c_str());
			}
	
			LinkSampler((int)j, L"PS_LSMP", L"LSMP");
			LinkSampler((int)j, L"PS_PSMP", L"PSMP");
		}
	}
}

void StaticFbxRender::FbxSetting(const wchar_t* _Name, const wchar_t* _PathName)
{
	m_FBXDATA = Resources::Find<Fbx>(_Name);

	if (nullptr == m_FBXDATA)
	{
		BOOM;
	}


	// RenderData()
	// ANILIGHTMESHDPATH

	for (size_t i = 0; i < m_FBXDATA->MeshData().size(); i++)
	{
		for (size_t j = 0; j < m_FBXDATA->MeshData()[i]->vecIndices.size(); j++)
		{
			std::wstring MeshName = _Name;
			MeshName += L'0' + (wchar_t)j;
			m_RenderData = RenderData(MeshName.c_str(), _PathName);
			m_RenderData->m_bShadow = false;
			m_RenderData->m_Path->m_BUMP = BUMPOK;

			std::wstring DifTexName = IOPath::PathToFileName(m_FBXDATA->MaterialData()[i][j]->DifTex.c_str());
			if (L"" != DifTexName)
			{
				LinkTexture((int)j, L"PS_MAPTEX", DifTexName.c_str());
			}

			std::wstring BumpTexName = IOPath::PathToFileName(m_FBXDATA->MaterialData()[i][j]->BumpTex.c_str());
			if (L"" != BumpTexName)
			{
				LinkTexture((int)j, L"PS_BUMPTEX", BumpTexName.c_str());
			}
			LinkSampler((int)j, L"PS_LSMP", L"LSMP");
			LinkSampler((int)j, L"PS_PSMP", L"PSMP");
		}
	}
}

void StaticFbxRender::FbxSetting(const wchar_t* _Name, const wchar_t* _PathName, bool _Shadow)
{
	m_FBXDATA = Resources::Find<Fbx>(_Name);

	if (nullptr == m_FBXDATA)
	{
		BOOM;
	}
	//m_RenderData->m_Path->m_BUMP = BUMPOK;

	// RenderData()
	// ANILIGHTMESHDPATH

	for (size_t i = 0; i < m_FBXDATA->MeshData().size(); i++)
	{
		for (size_t j = 0; j < m_FBXDATA->MeshData()[i]->vecIndices.size(); j++)
		{
			std::wstring MeshName = _Name;
			MeshName += L'0' + (wchar_t)j;
			m_RenderData = RenderData(MeshName.c_str(), _PathName);
			m_RenderData->m_bShadow = _Shadow;


			std::wstring DifTexName = IOPath::PathToFileName(m_FBXDATA->MaterialData()[i][j]->DifTex.c_str());
			if (L"" != DifTexName)
			{
				LinkTexture((int)j, L"PS_MAPTEX", DifTexName.c_str());
			}

			std::wstring BumpTexName = IOPath::PathToFileName(m_FBXDATA->MaterialData()[i][j]->BumpTex.c_str());
			if (L"" != BumpTexName)
			{
				LinkTexture((int)j, L"PS_BUMPTEX", BumpTexName.c_str());
			}
			LinkSampler((int)j, L"PS_LSMP", L"LSMP");
			LinkSampler((int)j, L"PS_PSMP", L"PSMP");
		}
	}
}


