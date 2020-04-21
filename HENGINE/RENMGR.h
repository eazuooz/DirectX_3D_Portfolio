#pragma once
#include "LIGHT.h"
#include "CAMERA.h"
#include "RENDER.h"
#include <map>
#include <vector>

class HMESH;
class RenderMgr;
class Instancing : public Ref
{
public:
	friend RenderMgr;

private:
	RenderPath			m_RenderPath;
	HPTR<HMESH>			m_HMESH;

public:
	void SetRenderPath(const RenderPath& _RenderPath) {
		m_RenderPath = _RenderPath;
	}

	RenderPath& GetRenderPath() 
	{
		return m_RenderPath;
	}

	template<typename T>
	void PushData(const T& _Data) 
	{
		m_HMESH->INSTBUFFER()->PushData(_Data);
	}

	void UpdateInstData() 
	{
		m_HMESH->INSTBUFFER()->UpdateInstData();
	}

	void Render() 
	{
		m_RenderPath.Update();
		m_HMESH->UpdateAllVB();
		m_HMESH->UpdateIB(0);
		m_HMESH->RenderInstancing(0);
		m_HMESH->INSTBUFFER()->ResetDataCount();
	}



public:
	void SetMesh(const wchar_t* _Name);
};

// HCAM의 기능중 나는 어떤 랜더러 부류들만 비춘다.
class Camera;
class Light;
class Scene;
class Renderer;
class RenderMgr
{
public:
	static void LightCBInit();

public:
	enum GSORTTYPE
	{
		// 카메라를 고려해서 처리해야 한다.
		// 카메라가 움직이면 의미가 없다.
		GT_NONE, // 쉐이더에서 직접 처리 하겠다.
		GT_Z, // Z로 정렬해라.
		GT_Y,
		GT_X, 
		GT_CAMZ, // VIEW행렬이 곱해진 상태에서의 비교.
		GT_CAMY,
		GT_CAMX,
		GT_MAX,
	};

public:
	friend Camera;
	friend Light;
	// 하위 녀석들을 여기에 FRIEND
	// 3DMESHRENDER같은 녀석들을 FRIEND걸면 또 잘못.
	// SOLID 중 의존성 역전법칙을 제대로 어기는것.
	friend Renderer;
	friend Scene;

private:
	Scene* m_Scene;

private:
	// CAM
	// 맵과 리스트가 합쳐져 있는 맵이 멀티맵이라고
	// 키에 리스트
	// 멀티맵은 키가 겹칠수 있는 맵이다.
	// 순서는 업데이트든 랜더든 충돌도 마찬가지
	// 문이랑 몬스터가 있다.
	// 몬스터랑 충돌해 있는 도중에는
	// 문에 못들어가게 하고 싶다.

	//      
	std::map<int, std::list<HPTR<Camera>>>::iterator CamGroupFinder;
	std::map<int, std::list<HPTR<Camera>>> m_CAMLIST;
	std::map<int, GSORTTYPE> m_SORTTYPE;

	std::list<HPTR<Camera>>::iterator m_ALLCAMSTART;
	std::list<HPTR<Camera>>::iterator m_ALLCAMEND;
	std::list<HPTR<Camera>> m_ALLCAMLIST;


	std::list<HPTR<Camera>>::iterator CamStart;
	std::list<HPTR<Camera>>::iterator CamEnd;

	std::map<int, std::list<HPTR<Light>>>::iterator LightGroupFinder;
	std::map<int, std::list<HPTR<Light>>> m_LIGHTLIST;

	static Light::ALLLIGHTDATA m_ALLLIGHTDATA;
	static std::vector<HPTR<HCB>> m_LIGHTCBLIST;

	std::map<int, std::list<HPTR<Renderer>>>::iterator RenderGStart;
	std::map<int, std::list<HPTR<Renderer>>>::iterator RenderGEnd;

	std::list<HPTR<Renderer>>::iterator RenderStart;
	std::list<HPTR<Renderer>>::iterator RenderEnd;
	std::map<int, std::list<HPTR<Renderer>>> m_RenderList;

	std::list<Renderer::HRENDERDATA*>::iterator RenderDataStart;
	std::list<Renderer::HRENDERDATA*>::iterator RenderDataEnd;

	std::map<int, std::list<Renderer::HRENDERDATA*>>::iterator RenderForwardGStart;
	std::map<int, std::list<Renderer::HRENDERDATA*>>::iterator RenderForwardGEnd;
	std::map<int, std::list<Renderer::HRENDERDATA*>> m_FORWARDRenderList;

	std::map<int, std::list<Renderer::HRENDERDATA*>>::iterator RenderDeferredGStart;
	std::map<int, std::list<Renderer::HRENDERDATA*>>::iterator RenderDeferredGEnd;
	std::map<int, std::list<Renderer::HRENDERDATA*>> m_DEFERREDRenderList;



	///////////////////////////////////////////// CAM
private:
	void PushCamera(const HPTR<Camera>& _CAM);

	/////////////////////////////////////////////// LIGHT
private:
	void PushLight(const HPTR<Light>& _Light);

private:
	// 오더
	// std::list<HPTR<HRENDER>> Z값에 따라서 정렬해주려고 하는것. 

private:
	void PushRender(Renderer* _RENDER);
	void PushForwardender(Renderer::HRENDERDATA* _RENDER);
	void PushDeferredRender(Renderer::HRENDERDATA* _RENDER);

	void PopForwardender(Renderer::HRENDERDATA* _RENDER);

public:
	void Render();
	void Release();
	void OverRelease();
	

	// 정렬함수.
public:

	static bool CamSort(const HPTR<Camera>& _Left, const HPTR<Camera>& _Right);
	static bool ZSort(const HPTR<Renderer>& _Left, const HPTR<Renderer>& _Right);
	static bool YSort(const HPTR<Renderer>& _Left, const HPTR<Renderer>& _Right);
	static bool XSort(const HPTR<Renderer>& _Left, const HPTR<Renderer>& _Right);
	static bool(*SortFunc[GT_MAX])(const HPTR<Renderer>&, const HPTR<Renderer>&);

/////////////////////////// Inst;

private:
	// 인스턴싱은 씬에 구속되지 않는다.
	// 그래서 stati
	static std::map<std::wstring, HPTR<Instancing>>::iterator FindInstIter;

	static std::map<std::wstring, HPTR<Instancing>>::iterator StartInst;
	static std::map<std::wstring, HPTR<Instancing>>::iterator EndInst;
	static std::map<std::wstring, HPTR<Instancing>> m_InstMap;

public:
	template<typename T>
	static void PushData(const wchar_t* _Name, const T& _Data)
	{
		// 매번 데이터 넣을때마다 찾아야 하하는게.
		HPTR<Instancing> pInst = FindInstancing(_Name);

		if (nullptr != pInst)
		{
			BOOM;
		}
		pInst->PushData<T>(_Data);
	}

	static HPTR<Instancing> FindInstancing(const wchar_t* _Name)
	{
		FindInstIter = m_InstMap.find(_Name);

		if (FindInstIter == m_InstMap.end())
		{
			return nullptr;
		}
		return FindInstIter->second;
	}


	static HPTR<Instancing> CreateInstancing(const wchar_t* _Name
		, const wchar_t* _Mesh
		, unsigned int _InstCount = 1000)
		// 자동확장하게 만들겠다.
	{
		HPTR<Instancing> pInst = FindInstancing(_Name);

		if (nullptr != pInst)
		{
			BOOM;
		}

		pInst = new Instancing();
		pInst->SetMesh(_Mesh);

		if (nullptr == pInst->m_HMESH->INSTBUFFER())
		{
			BOOM;
		}

		m_InstMap.insert(std::map<std::wstring, HPTR<Instancing>>::value_type(_Name, pInst));

		return pInst;
	}

	static HPTR<Instancing> CreateInstancing(const wchar_t* _Name
		, const wchar_t* _Mesh
		, const RenderPath& _Path
		, unsigned int _InstCount = 1000)
		// 자동확장하게 만들겠다.
	{
		HPTR<Instancing> pInst = FindInstancing(_Name);

		if (nullptr != pInst)
		{
			BOOM;
		}

		pInst = new Instancing();
		pInst->SetMesh(_Mesh);
		pInst->SetRenderPath(_Path);

		if (nullptr == pInst->m_HMESH->INSTBUFFER())
		{
			BOOM;
		}

		m_InstMap.insert(std::map<std::wstring, HPTR<Instancing>>::value_type(_Name, pInst));

		return pInst;
	}

public:
	void LightSetting(int _Order, HPTR<Camera>& _Cam);
	void ShadowRender(int _Order, HPTR<Camera>& _Cam);
	void LightRender(HPTR<Camera>& _Cam);
	

public:
	RenderMgr(Scene* _Scene);
	~RenderMgr();
};