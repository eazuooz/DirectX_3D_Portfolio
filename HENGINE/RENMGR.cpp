#include "RENMGR.h"
#include "GRAPHICDEVICE.h"
#include "GRAPHICDEBUG.h"
#include "SCENE.h"

#include "HMESH.h"
#include "HCB.h"
#include "HVTXSHADER.h"
#include "HPIXSHADER.h"
#include "HSMP.h"
#include "SPRITE.h"

std::map<std::wstring, HPTR<Instancing>>::iterator RenderMgr::FindInstIter;
std::map<std::wstring, HPTR<Instancing>>::iterator RenderMgr::StartInst;
std::map<std::wstring, HPTR<Instancing>>::iterator RenderMgr::EndInst;
std::map<std::wstring, HPTR<Instancing>> RenderMgr::m_InstMap;

void Instancing::SetMesh(const wchar_t* _Name)
{
	m_HMESH = Resources::Find<HMESH>(_Name);

	if (nullptr == m_HMESH)
	{
		BOOM;
	}

	if (nullptr == m_HMESH->INSTBUFFER())
	{
		BOOM;
	}
}


// 나눌꺼다.
// 이제 지훈쌤 
// 학원 그 누구보다도 Mfc를 많이 사용해봤다.

bool(*RenderMgr::SortFunc[RenderMgr::GT_MAX])(const HPTR<Renderer>&, const HPTR<Renderer>&);

bool RenderMgr::CamSort(const HPTR<Camera>& _Left, const HPTR<Camera>& _Right)
{
	return _Left->Order() < _Right->Order();
}
bool RenderMgr::ZSort(const HPTR<Renderer>& _Left, const HPTR<Renderer>& _Right) 
{
	return _Left->GetActor()->GetTransform()->GetPosition().z > _Right->GetActor()->GetTransform()->GetPosition().z;
}
bool RenderMgr::YSort(const HPTR<Renderer>& _Left, const HPTR<Renderer>& _Right) 
{
	return _Left->GetActor()->GetTransform()->GetPosition().y > _Right->GetActor()->GetTransform()->GetPosition().y;
}
bool RenderMgr::XSort(const HPTR<Renderer>& _Left, const HPTR<Renderer>& _Right) 
{
	return _Left->GetActor()->GetTransform()->GetPosition().x > _Right->GetActor()->GetTransform()->GetPosition().x;
}

// 그 시간이 빨라지고 실망하지 않고
// 버려.
// 실력은 둘째치고 프로에 가까워지는 것.


Light::ALLLIGHTDATA RenderMgr::m_ALLLIGHTDATA;
std::vector<HPTR<HCB>> RenderMgr::m_LIGHTCBLIST;


bool m_IsLightInit = false;
void RenderMgr::LightCBInit()
{
	if (true == m_IsLightInit)
	{
		return;
	}

	m_LIGHTCBLIST.resize(5);

	// 상수버퍼가 이미 만들어 져있으므로 되는것.
	m_LIGHTCBLIST[0] = Resources::Find<HCB>(L"ALLLIGHTDATAV");
	m_LIGHTCBLIST[1] = Resources::Find<HCB>(L"ALLLIGHTDATAP");
	m_LIGHTCBLIST[2] = Resources::Find<HCB>(L"ALLLIGHTDATAD");
	m_LIGHTCBLIST[3] = Resources::Find<HCB>(L"ALLLIGHTDATAG");
	m_LIGHTCBLIST[4] = Resources::Find<HCB>(L"ALLLIGHTDATAH");

	for (size_t i = 0; i < m_LIGHTCBLIST.size(); i++)
	{
		m_LIGHTCBLIST[i]->Link(&m_ALLLIGHTDATA, sizeof(Light::ALLLIGHTDATA));
	}
	m_IsLightInit = true;
}

RenderMgr::RenderMgr(Scene* _Scene) : m_Scene(_Scene)
{
	SortFunc[GT_Z] = ZSort;
	SortFunc[GT_Y] = ZSort;
	SortFunc[GT_X] = ZSort;
	SortFunc[GT_CAMZ] = nullptr;
	SortFunc[GT_CAMY] = nullptr;
	SortFunc[GT_CAMX] = nullptr;


	LightCBInit();
}


RenderMgr::~RenderMgr()
{
	
}

void RenderMgr::Render()
{
	// 혹시나 순서가 변경된 캠들을 이동시킨다.
	// CHECKCAMORDER();

	// 기본 깊이버퍼는 한번 밀린다.
	//GraphicDevice::Inst()->ClearTarget();
	//GraphicDevice::Inst()->DefRenderTargetSetting();
	//GraphicDevice::Inst()->DefViewPortSetting();

	// 여기서 클리어 해버리면 안된다.
	m_ALLCAMSTART = m_ALLCAMLIST.begin();
	m_ALLCAMEND = m_ALLCAMLIST.end();
	for (; m_ALLCAMSTART != m_ALLCAMEND; ++m_ALLCAMSTART)
	{
		(*m_ALLCAMSTART)->CamClear();
	}

	RenderGStart = m_RenderList.begin();
	RenderGEnd = m_RenderList.end();

	for (; RenderGStart != RenderGEnd; ++RenderGStart)
	{
		RenderStart = RenderGStart->second.begin();
		RenderEnd = RenderGStart->second.end();

		for (; RenderStart != RenderEnd; ++RenderStart)
		{
			if (false == (*RenderStart)->IsUpdate())
			{
				continue;
			}

			(*RenderStart)->RenderUpdate();
		}
	}


	
	// 모든 랜더 데이터를 돌릴 준비를 한다.
	// ★ 1. 모든 랜더그룹을 돌릴 준비를 한다.
	// 포워드 부터 돌릴 준비를 한다.
	RenderForwardGStart = m_FORWARDRenderList.begin();
	RenderForwardGEnd = m_FORWARDRenderList.end();

	// 5번 랜더 그룹이 돈다.
	for (; RenderForwardGStart != RenderForwardGEnd; ++RenderForwardGStart)
	{
		if (0 >= RenderForwardGStart->second.size())
		{
			continue;
		}

		CamGroupFinder = m_CAMLIST.find(RenderForwardGStart->first);
		if (CamGroupFinder == m_CAMLIST.end() || 0 >= CamGroupFinder->second.size())
		{
			continue;
		}

		CamStart = CamGroupFinder->second.begin();
		CamEnd = CamGroupFinder->second.end();

		// ★ 3. 카메라들이 랜더 그룹에 속한 랜더러들을 모조리 랜더한다.
		for (; CamStart != CamEnd; ++CamStart)
		{
			GraphicDevice::MainReset();

			(*CamStart)->Update();
			(*CamStart)->FORWARDTargetSet();

			LightSetting(RenderForwardGStart->first, *CamStart);

			RenderDataStart = RenderForwardGStart->second.begin();
			RenderDataEnd = RenderForwardGStart->second.end();
			for (; RenderDataStart != RenderDataEnd; ++RenderDataStart)
			{
				if (false == (*RenderDataStart)->m_Parent->IsUpdate())
				{
					continue;
				}

				// 잘 생각해보면.
				(*RenderDataStart)->m_Parent->UpdateTransform();
				(*RenderDataStart)->m_Parent->UpdateSubTrans((*CamStart));
				(*RenderDataStart)->m_Parent->RenderDataUpdate((*CamStart), (*RenderDataStart));
				(*RenderDataStart)->m_Parent->m_MatData.RENDERDATA.x = (float)(*RenderDataStart)->m_Path->m_LIGHT;
				(*RenderDataStart)->m_Parent->m_MatData.RENDERDATA.y = (float)(*RenderDataStart)->m_Path->m_BUMP;



				(*RenderDataStart)->m_Mesh->UpdateVB((*RenderDataStart)->VBIndex);
				(*RenderDataStart)->m_Mesh->UpdateIB((*RenderDataStart)->IBIndex);
				(*RenderDataStart)->m_Path->Update();
				(*RenderDataStart)->m_Mesh->Render((*RenderDataStart)->IBIndex);
			}

		}
	}

	// 디퍼드 다 그리기.
	RenderDeferredGStart = m_DEFERREDRenderList.begin();
	RenderDeferredGEnd = m_DEFERREDRenderList.end();
	for (; RenderDeferredGStart != RenderDeferredGEnd; ++RenderDeferredGStart)
	{
		if (0 >= RenderDeferredGStart->second.size())
		{
			continue;
		}

		CamGroupFinder = m_CAMLIST.find(RenderDeferredGStart->first);
		if (CamGroupFinder == m_CAMLIST.end() || 0 >= CamGroupFinder->second.size())
		{
			continue;
		}

		CamStart = CamGroupFinder->second.begin();
		CamEnd = CamGroupFinder->second.end();

		// ★ 3. 카메라들이 랜더 그룹에 속한 랜더러들을 모조리 랜더한다.
		for (; CamStart != CamEnd; ++CamStart)
		{
			GraphicDevice::MainReset();

			(*CamStart)->Update();
			(*CamStart)->DEFERREDTargetSet();

			// LightSetting(RenderDeferredGStart->first, *CamStart);

			RenderDataStart = RenderDeferredGStart->second.begin();
			RenderDataEnd = RenderDeferredGStart->second.end();
			for (; RenderDataStart != RenderDataEnd; ++RenderDataStart)
			{
				if (false == (*RenderDataStart)->m_Parent->IsUpdate())
				{
					continue;
				}

				// 잘 생각해보면.
				(*RenderDataStart)->m_Parent->UpdateTransform();
				(*RenderDataStart)->m_Parent->UpdateSubTrans((*CamStart));
				(*RenderDataStart)->m_Parent->RenderDataUpdate((*CamStart), (*RenderDataStart));
				(*RenderDataStart)->m_Parent->m_MatData.RENDERDATA.x = (float)(*RenderDataStart)->m_Path->m_LIGHT;
				(*RenderDataStart)->m_Parent->m_MatData.RENDERDATA.y = (float)(*RenderDataStart)->m_Path->m_BUMP;

				(*RenderDataStart)->m_Mesh->UpdateVB((*RenderDataStart)->VBIndex);
				(*RenderDataStart)->m_Mesh->UpdateIB((*RenderDataStart)->IBIndex);
				(*RenderDataStart)->m_Path->Update();
				(*RenderDataStart)->m_Mesh->Render((*RenderDataStart)->IBIndex);
			}


			GraphicDevice::MainReset();
			ShadowRender(RenderDeferredGStart->first, *CamStart);
			LightSetting(RenderDeferredGStart->first, *CamStart);
			LightGroupFinder = m_LIGHTLIST.find(RenderDeferredGStart->first);
			HPTR<Light> ShadowLight = nullptr;
			
			if ((m_LIGHTLIST.end() != LightGroupFinder))
			{
				if (0 != LightGroupFinder->second.size())
				{
					ShadowLight = *LightGroupFinder->second.begin();
				}
			}


			//for (size_t i = 0; i < length; i++)
			//{
			//}
			// LIGHT->DEFERREDLightRender(ShadowLight);
			(*CamStart)->DEFERREDLightRender(ShadowLight);
		}
	}
	m_ALLCAMLIST.sort(CamSort);

	// 다 그러져있지.

	GraphicDevice::MainReset(); // 다 빼고
	m_ALLCAMSTART = m_ALLCAMLIST.begin();
	m_ALLCAMEND = m_ALLCAMLIST.end();

	for (; m_ALLCAMSTART != m_ALLCAMEND; ++m_ALLCAMSTART)
	{
		(*m_ALLCAMSTART)->DEFERREDMerge();
	}
	GraphicDebug::DebugCamRender();

	// GRAPHICDEVICE::HMainDevice()->ClearTarget();
	GraphicDevice::MainReset(); // 다 빼고
	GraphicDevice::Inst()->ClearTarget();
	GraphicDevice::Inst()->DefRenderTargetSetting();
	GraphicDevice::Inst()->DefViewPortSetting();
	// 깊이버퍼 끄고 그려야 한다.

	m_ALLCAMSTART = m_ALLCAMLIST.begin();
	m_ALLCAMEND = m_ALLCAMLIST.end();

	for (; m_ALLCAMSTART != m_ALLCAMEND; ++m_ALLCAMSTART)
	{
		(*m_ALLCAMSTART)->CamRender();
	}

	// 디버그 랜더를 여기서 돌린다.

	//GraphicDevice::Inst()->DefRenderTargetSetting();
	//GraphicDevice::Inst()->DefViewPortSetting();
	m_Scene->DebugRender();
	GraphicDebug::DebugRender();

	RenderGStart = m_RenderList.begin();
	RenderGEnd = m_RenderList.end();
	for (; RenderGStart != RenderGEnd; ++RenderGStart)
	{
		RenderStart = RenderGStart->second.begin();
		RenderEnd = RenderGStart->second.end();

		for (; RenderStart != RenderEnd; ++RenderStart)
		{
			if (false == (*RenderStart)->IsUpdate())
			{
				continue;
			}

			(*RenderStart)->TextRenderUpdate();
		}
	}

	GraphicDevice::MainReset();

	// ITERFOR(AStart, AEnd, m_ActorList, Render);
	GraphicDevice::Inst()->Present();

}


void RenderMgr::PushCamera(const HPTR<Camera>& _CAM)
{
	// 캠그룹이 없으면
	// 만들거다.

	//std::set<int>::iterator StartGroup = _CAM->m_GroupList.begin();
	//std::set<int>::iterator EndGroup = _CAM->m_GroupList.end();

	for (auto& var : _CAM->m_GroupList)
	{
		std::map<int, std::list<HPTR<Camera>>>::iterator FindIter
			= m_CAMLIST.find(var);

		if (FindIter == m_CAMLIST.end())
		{
			m_CAMLIST.insert(std::map<int, std::list<HPTR<Camera>>>::value_type(var, std::list<HPTR<Camera>>()));
			FindIter = m_CAMLIST.find(var);
		}

		FindIter->second.push_back(_CAM);
	}

	m_ALLCAMLIST.push_back(_CAM);
}

void RenderMgr::PushLight(const HPTR<Light>& _Light) 
{

	for (auto& var : _Light->m_GroupList)
	{
		std::map<int, std::list<HPTR<Light>>>::iterator FindIter
			= m_LIGHTLIST.find(var);

		if (FindIter == m_LIGHTLIST.end())
		{
			m_LIGHTLIST.insert(std::map<int, std::list<HPTR<Light>>>::value_type(var, std::list<HPTR<Light>>()));
			FindIter = m_LIGHTLIST.find(var);
		}

		if (LIGHTMAX <= FindIter->second.size())
		{
			BOOM;
			continue;
		}

		FindIter->second.push_back(_Light);
	}
}

// 그룹 바꾸는거 왠만하면 안만듬.
// UI였다가 PLAY 그룹빠져야 한다. 이런이야기 인데...
// 모르겠다. 그럴일이 있을지.
// 이러면 매크로 쓰고 싶어진다.
void RenderMgr::PushRender(Renderer* _RENDER) 
{
	//if (_RENDER->IsDebugCheck())
	//{
	//	_RENDER->DebugCheck();
	//}
	//int a = _RENDER->Order();

	// 2번 정도만 그냥 하자... 
	std::map<int, std::list<HPTR<Renderer>>>::iterator FindIter
		= m_RenderList.find(_RENDER->GetOrder());

	if (FindIter == m_RenderList.end())
	{
		m_RenderList.insert(std::map<int, std::list<HPTR<Renderer>>>::value_type(_RENDER->GetOrder(), std::list<HPTR<Renderer>>()));
		FindIter = m_RenderList.find(_RENDER->GetOrder());
		m_SORTTYPE.insert(std::map<int, GSORTTYPE>::value_type(_RENDER->GetOrder(), GSORTTYPE::GT_Z));
	}

	FindIter->second.push_back(_RENDER);
}

void RenderMgr::PushForwardender(Renderer::HRENDERDATA* _RENDER)
{
	std::map<int, std::list<Renderer::HRENDERDATA*>>::iterator FindIter
		= m_FORWARDRenderList.find(_RENDER->m_Parent->GetOrder());

	if (FindIter == m_FORWARDRenderList.end())
	{
		m_FORWARDRenderList.insert(std::map<int, std::list<Renderer::HRENDERDATA*>>::value_type(_RENDER->m_Parent->GetOrder(), std::list<Renderer::HRENDERDATA*>()));
		FindIter = m_FORWARDRenderList.find(_RENDER->m_Parent->GetOrder());
		m_SORTTYPE.insert(std::map<int, GSORTTYPE>::value_type(_RENDER->m_Parent->GetOrder(), GSORTTYPE::GT_Z));
	}

	FindIter->second.push_back(_RENDER);
}

void RenderMgr::PushDeferredRender(Renderer::HRENDERDATA* _RENDER) 
{

	std::map<int, std::list<Renderer::HRENDERDATA*>>::iterator FindIter
		= m_DEFERREDRenderList.find(_RENDER->m_Parent->GetOrder());

	if (FindIter == m_DEFERREDRenderList.end())
	{
		m_DEFERREDRenderList.insert(std::map<int, std::list<Renderer::HRENDERDATA*>>::value_type(_RENDER->m_Parent->GetOrder(), std::list<Renderer::HRENDERDATA*>()));
		FindIter = m_DEFERREDRenderList.find(_RENDER->m_Parent->GetOrder());
		m_SORTTYPE.insert(std::map<int, GSORTTYPE>::value_type(_RENDER->m_Parent->GetOrder(), GSORTTYPE::GT_Z));
	}

	FindIter->second.push_back(_RENDER);
}

void RenderMgr::PopForwardender(Renderer::HRENDERDATA* _RENDER)
{
	std::map<int, std::list<Renderer::HRENDERDATA*>>::iterator FindIter
		= m_FORWARDRenderList.find(_RENDER->m_Parent->GetOrder());

	std::map<int, GSORTTYPE>::iterator FindSortIter 
		= m_SORTTYPE.find(_RENDER->m_Parent->GetOrder());

	if (FindIter == m_FORWARDRenderList.end())
	{
		FindIter = m_FORWARDRenderList.find(_RENDER->m_Parent->GetOrder());

		m_FORWARDRenderList.erase(FindIter);
		m_SORTTYPE.erase(FindSortIter);
	}

	//FindIter->second.push_back(_RENDER);
}

void RenderMgr::Release()
{
	// 만들어 놓을께요

	RenderForwardGStart = m_FORWARDRenderList.begin();
	RenderForwardGEnd = m_FORWARDRenderList.end();

	for (; RenderForwardGStart != RenderForwardGEnd; ++RenderForwardGStart)
	{
		RenderDataStart = RenderForwardGStart->second.begin();
		RenderDataEnd = RenderForwardGStart->second.end();

		for (; RenderDataStart != RenderDataEnd; )
		{
			if (true == (*RenderDataStart)->m_Parent->IsDeath())
			{
				RenderDataStart = RenderForwardGStart->second.erase(RenderDataStart);
			}
			else {
				++RenderDataStart;
			}

		}
	}

	RenderDeferredGStart = m_DEFERREDRenderList.begin();
	RenderDeferredGEnd = m_DEFERREDRenderList.end();

	for (; RenderDeferredGStart != RenderDeferredGEnd; ++RenderDeferredGStart)
	{
		RenderDataStart = RenderDeferredGStart->second.begin();
		RenderDataEnd = RenderDeferredGStart->second.end();

		for (; RenderDataStart != RenderDataEnd; )
		{
			if (true == (*RenderDataStart)->m_Parent->IsDeath())
			{
				RenderDataStart = RenderDeferredGStart->second.erase(RenderDataStart);
			}
			else {
				++RenderDataStart;
			}

		}
	}

	RenderGStart = m_RenderList.begin();
	RenderGEnd = m_RenderList.end();

	for (; RenderGStart != RenderGEnd; ++RenderGStart)
	{
		RenderStart = RenderGStart->second.begin();
		RenderEnd = RenderGStart->second.end();

		for (; RenderStart != RenderEnd; )
		{
			if (true == (*RenderStart)->IsDeath())
			{
				RenderStart = RenderGStart->second.erase(RenderStart);
			}
			else {
				++RenderStart;
			}
		}
	}
}

void RenderMgr::OverRelease() 
{
	RenderGStart = m_RenderList.begin();
	RenderGEnd = m_RenderList.end();

	for (; RenderGStart != RenderGEnd; ++RenderGStart)
	{
		RenderStart = RenderGStart->second.begin();
		RenderEnd = RenderGStart->second.end();

		for (; RenderStart != RenderEnd; )
		{
			if (true == (*RenderStart)->GetActor()->IsOverReady())
			{
				RenderStart = RenderGStart->second.erase(RenderStart);
			}
			else {
				++RenderStart;
			}
		}
	}
}

void RenderMgr::ShadowRender(int _Order, HPTR<Camera>& _Cam)
{
	LightGroupFinder = m_LIGHTLIST.find(_Order);




	HPTR<RenderPath> PATH = nullptr;
	if (LightGroupFinder != m_LIGHTLIST.end())
	{
		for (auto& CurLight : LightGroupFinder->second)
		{
			CurLight->ShadowUpdate();

			RenderDataStart = RenderDeferredGStart->second.begin();
			RenderDataEnd = RenderDeferredGStart->second.end();


			for (; RenderDataStart != RenderDataEnd; ++RenderDataStart)
			{
				if (true == (*RenderDataStart)->m_bShadow)
				{
					if ((*RenderDataStart)->m_bShadowAni == true)
					{
						PATH = Resources::Find<RenderPath>(L"SHADOWANIPATH");
					}
					else
					{
						PATH = Resources::Find<RenderPath>(L"SHADOWPATH");						
					}

					if (nullptr == PATH)
					{
						BOOM;
					}

					PATH->Init((*RenderDataStart)->m_Parent);
					(*RenderDataStart)->m_Parent->UpdateTransform();
					// 라이트 시점으로 그림자를 그려야 하니까 빛의 시점을 넣어주는것.
					(*RenderDataStart)->m_Parent->UpdateShadowTrans(CurLight);
					(*RenderDataStart)->m_Parent->RenderDataUpdate((*CamStart), (*RenderDataStart));

					(*RenderDataStart)->m_Mesh->UpdateVB((*RenderDataStart)->VBIndex);
					(*RenderDataStart)->m_Mesh->UpdateIB((*RenderDataStart)->IBIndex);
					PATH->Update();
					(*RenderDataStart)->m_Mesh->Render((*RenderDataStart)->IBIndex);
				}
			}
		}
	}
}

void RenderMgr::LightRender(HPTR<Camera>& _Cam) 
{

}

void RenderMgr::LightSetting(int _Order, HPTR<Camera>& _Cam) 
{
	// Light를 준비
	LightGroupFinder = m_LIGHTLIST.find(_Order);

	if (LightGroupFinder != m_LIGHTLIST.end())
	{
		int LightCount = 0;
		for (auto& CurLight : LightGroupFinder->second)
		{
			m_ALLLIGHTDATA.ArrData[LightCount] = CurLight->m_LightData;
			m_ALLLIGHTDATA.ArrData[LightCount].LightPos = _Cam->GetViewMatrix().MulOne(m_ALLLIGHTDATA.ArrData[LightCount].LightPos);
			// m_ALLLIGHTDATA.ArrData[LightCount].LightPos.Normal3D();
			m_ALLLIGHTDATA.ArrData[LightCount].LightDir = _Cam->GetViewMatrix().MulZero(m_ALLLIGHTDATA.ArrData[LightCount].LightDir);
			m_ALLLIGHTDATA.ArrData[LightCount].LightDir.Normal3DToWZero();
			m_ALLLIGHTDATA.ArrData[LightCount].LightDirInv = _Cam->GetViewMatrix().MulZero(m_ALLLIGHTDATA.ArrData[LightCount].LightDirInv);
			m_ALLLIGHTDATA.ArrData[LightCount].LightDirInv.Normal3DToWZero();
			++LightCount;
		}

		m_ALLLIGHTDATA.LightSettingData[0] = LightCount;


		for (size_t i = 0; i < m_LIGHTCBLIST.size(); i++)
		{
			m_LIGHTCBLIST[i]->DataSetting();
			m_LIGHTCBLIST[i]->Update();
		}
	}

}