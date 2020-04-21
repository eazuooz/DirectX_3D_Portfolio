#pragma once
#include "COMPONENT.h"

#include <d3d11.h>

#define LIGHTMAX 512


// 관리되어야 한다.
class HRENDERTARGET;
class HDEPTHBUFFER;
class RenderMgr;
class Light : public Component
{
public:
	enum LIGHTTYPE
	{
		LT_DIR,
		LT_POINT,
	};

public:
	friend RenderMgr;

public:
	struct LightData
	{
		// 빛을 전부다 표현하려고 만들었으므로
		Vector4 Color; // 빛의 색깔
		Vector4 LightPos; // 빛의 위치 (디렉션 라이트면 의미가 없다)
		Vector4 LightDir; // 빛이 바라보는 방향(포인트 라이트면 그때그때마다 재계산 디렉션이면 항상 똑같다)
		Vector4 LightDirInv; // -LightDir (람베르트 조명공식에서 L)
		//HVEC4 ViewLightPos; // 빛의 위치 (디렉션 라이트면 의미가 없다)
		//HVEC4 ViewLightDir; // 빛의 위치 (디렉션 라이트면 의미가 없다)
		//HVEC4 ViewLightDir; // 빛의 위치 (디렉션 라이트면 의미가 없다)
		float Range; // 범위
		float DiffPower; // 빛의 강도 // 전반적으로 밝아지고
		float SpecPower; // 빛의 강도 // 내가 바라보는 방향과 변이 
		int Type;
		int SpecPow;
		int temp1;
		int temp2;
		int temp3;
	};

	// 왠만하면 int 하나 넣어도 16바이트로 만드세요.
	struct ALLLIGHTDATA
	{
		LightData ArrData[LIGHTMAX];
		int LightSettingData[4];
	};


	// 이녀석이 만들어졌을때 받는다.
	unsigned int m_Index;
	std::set<int> m_GroupList;

	// 저는 2번 3번 5번 랜더 그룹을 비춥니다.
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
	void Init() override;

private:
	Light::LightData m_LightData;

public:
	void RenderBefore() override;
	void Update() override;

public:
	void SetLightType(LIGHTTYPE _Light)
	{
		m_LightData.Type = (int)_Light;
	}

	void SetRange(float _Value)
	{
		m_LightData.Range = _Value;
	}

	void SetSpecPow(int _Value)
	{
		m_LightData.SpecPow = _Value;
	}

	void SpecPower(float _Value)
	{
		m_LightData.SpecPower = _Value;
	}


	void SetDiffPow(float _Value)
	{
		m_LightData.DiffPower = _Value;
	}

	void SetDiffPowMul(float _Value)
	{
		m_LightData.DiffPower *= _Value;
	}

public:
	class ShadowData
	{
	public:
		Matrix LightVP;		//라이트 뷰포트행렬 
		Matrix CamInvView;
		Vector4 m_ShadowData;
	};

	HPTR<HRENDERTARGET> m_ShadowTarget;
	HPTR <HDEPTHBUFFER> m_DepthBuffer;

	ShadowData m_ShadowData;
	void ShadowUpdate();

	Matrix m_VIEW;
	Matrix m_PROJ;
	Matrix m_VIEWTRANSPOSE;
	Matrix m_PROJTRANSPOSE;
	Matrix m_VP;
	float m_Width, m_Height, m_Near, m_Far;

	//HPTR<Texture> m_BoneTex;
	

	D3D11_VIEWPORT m_LightViewPort;

	Matrix PMAT() { return m_PROJ; }
	Matrix VMAT() { return m_VIEW; }
	Matrix VPMAT() { return m_VP; }


public:
	Light();
	~Light();
};

