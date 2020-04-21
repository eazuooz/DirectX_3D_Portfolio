#pragma once
#include "GRAPHICDEVICE.h"
#include "COMPONENT.h"
#include <set>
#include "TRANS.h"

enum PROJMODE
{
	PM_PERS, // 원근
	PM_ORTH //  직교
};

class Light;
class RenderPath;
class HMESH;
class HRENDERTARGET;
class HDEPTHBUFFER;
class HMULTIRENDERTarget;
class Camera final : public Component
{
	// 임시로 만들어 놓겠습니다.
public:
	friend class RenderMgr;

private:
	static Camera* m_MainCam;
	static Camera* m_UICam;

public:
	static Camera* GetMainCam() 
	{
		return m_MainCam;
	}

	static Camera* GetUICam()
	{
		return m_UICam;
	}


	static void SetUICam(Camera* _Camera)
	{
		m_UICam = _Camera;
	}

private:
	PROJMODE m_Mode;
	Matrix m_WORLD;
	Matrix m_VIEW;
	Matrix m_PROJ;
	Matrix m_VIEWTRANSPOSE;
	Matrix m_PROJTRANSPOSE;
	Matrix m_VP;


	std::set<int> m_GroupList; // 1 2 3 5
	float m_Fov;
	float m_StartX;
	float m_StartY;
	float m_Width;
	float m_Height;
	float m_Near;
	float m_Far;

	D3D11_VIEWPORT m_CamViewPort;// 뷰포트행렬용 정보.
	// 이녀석은 0번 텍스처로 세팅해 주면 된다.
	HPTR<HDEPTHBUFFER> m_DepthBuffer;

	HPTR<HRENDERTARGET>			m_CamTarget;
	HPTR<HMULTIRENDERTarget>	m_DEFERREDTarget;
	HPTR<HMULTIRENDERTarget>	m_LightTarget;

	HPTR<HMESH> m_CamRenderMesh;

	HPTR<RenderPath> m_CamRenderPath;
	HPTR<RenderPath> m_DEFERREDLightPath;
	HPTR<RenderPath> m_LightMergePath;

	Vector4 m_ViewCamPos;

	MatrixData m_DATA;


public:
	HPTR<HRENDERTARGET> CamTarget() {
		return m_CamTarget;
	}
	HPTR<HMULTIRENDERTarget> GetDeferredTarget() {
		return m_DEFERREDTarget;
	}
	HPTR<HMULTIRENDERTarget> GetLightTarget() {
		return m_LightTarget;
	}

	void SetNearValue(float _Near) 
	{
		m_Near = _Near;
	}

	void SetFarValue(float _Far)
	{
		m_Far = _Far;
	}

	void SetViewPortSize(float _PosX, float _PosY, float _SizeX, float _SizeY)
	{
		m_CamViewPort.TopLeftX = _PosX;
		m_CamViewPort.TopLeftY = _PosY;
		m_CamViewPort.Width = _SizeX;
		m_CamViewPort.Height = _SizeY;
	}

	D3D11_VIEWPORT GetViewPort()
	{
		return m_CamViewPort;
	}

	void SetArea(float _Width, float _Height) {
		SetWidth(_Width);
		SetHeight(_Height);
	}

	void SetWidth(float _Width)
	{
		m_Width = _Width;
	}

	void SetHeight(float _Height)
	{
		m_Height = _Height;
	}

	void SetFov(float _Fov) {
		m_Fov = _Fov;
	}

	void SetProjMode(PROJMODE _Mode)
	{
		m_Mode = _Mode;
	}

	

private:
	bool m_IsAllView;
	bool IsAllView()
	{
		return m_IsAllView;
	}


public:
	int m_Order;
	int Order() 
	{
		return m_Order;
	}
	void Order(int _Order);

	void Update();
	

public:
	inline bool IsRender(int _Order) 
	{
		return m_GroupList.find(_Order) != m_GroupList.end();
	}

	// 탐색만으로 의미가 있을때 사용한다.
	// 어떤키가 있어.
	// map에 value가 빠진 자료구조.
	// map<int, 벨류> 1 에 있는 오브젝트를 줘.
	// set<int> 1번이 있어? 그럼 뭘해.

	// 1번 그룹을 랜더한다.

	// 카메라는 내가 어떤 랜더그룹을 비출 카메라인지 알아야 한다.

	// 랜더그룹 1번

	// std::set<std::wstring>
	// FOR 이터레이터로 FOR문 돌려보면 안다.
	// UNORDEREDMAP 정렬 안됨
	// 정렬됨

	// 정렬을 쓸일이 없다.
	// 여러분들 끝나면
	// 이론쪽도 다시다 잡을것이다.
	// 여러분들은 제가 따로.
	// 이력서 자소서 기술소개서 끝나면

	// 내걸 다 따라잡고 -> 이거부터 
	// 뭘만들거냐 <- 
	// 뭘 만들기 위해서 하지 않는 공부는
	// 내가 생각하기에는 다 쓸모없다.
	// ASTAR 이런거 지금 신경쓸 필요 없다.
	// 타일에서 길찾는게임들.

	// 포폴은 다다익선
	// 무조건 따라잡으세요.
	// 애들이 너무 많이 만든다.
	// 부족하다고 생각하면 만들면 된다.
	// 1년안에 취직해야 한다.
	// 한회사만 보면 정말 운이다.
	// 취직 못한 학생들의 특징이.
	// 위의 것을 다 안지킨다.

	// 캐릭터애니메이션 잡고
	// 애니메이션 FSM 까지 해야 한다.

	// 시간이 남아.
	// 당연하게 만들수 있을 정도
	// 퀵소트 직접 만들기
	// 벡터 직접만들기
	// 리스트 직접만들기
	// 알고리즘

	// 이녀석은 두번 호출해줘야 할수도 있네.
	template<class... Types>
	void DataCreate(Types&&... _Args)
	{
		PushRenderGroup(_Args...);
		return;
	}

	// 인풋매니저 식.
	template<class... Types>
	void PushRenderGroup(int _Order, Types&&... _Args)
	{
		if (m_GroupList.find(_Order) != m_GroupList.end())
		{
			BOOM;
			return;
		}

		m_GroupList.insert(_Order);

		PushRenderGroup(_Args...);
	}

	// 이거 만들어 놔야 한다.
	void PushRenderGroup();


public:
	Matrix PMAT() { return m_PROJ; }
	Matrix GetViewMatrix() { return m_VIEW; }
	Matrix VPMAT() {		return m_VP;	}
	//Matrix GetWorldMatrix() { return m_w; }

	// 오쏘일때만 가능
	Vector4 OrthMousePos();
	// 오쏘가 아니라면
	// 광선 만들기
	// HVEC4 WorldMousePos(float ZDis);

public:
	void Init() override;

	// 인자값이 하나도 없다면
	// 0은 넣어주겠다.
	// 1 2 3 4 5 6

public:
	void FORWARDTargetSet();
public:
	void DEFERREDTargetSet();

private:
	void DEFERREDMerge();

	void RenderBefore() override;
	void CamRender();
	void DEFERREDLightRender(HPTR<Light> _Light);

	void CamClear();

public:
	class Filter : public Ref
	{
	public:
		friend Camera;

	protected:
		Camera* m_Cam;

	private:
		void Init(Camera* _Cam) { m_Cam = _Cam; }

	public:
		virtual void InitFilter() = 0 {}
		virtual void MergeBeforeEffect() = 0 {}
		virtual void MergeAfterEffect() = 0 {}

	};

private:
	std::list<HPTR<Filter>> m_Filters;
	// HLIGHT::SHADOWRENDERDATA SData;
public:
	template<typename T>
	HPTR<T> CreateFilter()
	{
		T* NewFilter = new T();
		NewFilter->Init(this);
		NewFilter->InitFilter();
		m_Filters.push_back(NewFilter);
		return NewFilter;
	}

private:
	void MergeBefore();
	void MergeAfter();


public:
	Camera();
	~Camera();
};

