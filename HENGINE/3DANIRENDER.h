#pragma once
#include "RENDER.h"
#include "FBX.h"

class Fbx;
class Texture;
class FbxAnimation : public Renderer
{
public:
	class UserAniClip : public Ref
	{
	public:
		std::wstring Name;
		int m_Start; // fbx의 타임모드를 int화 시킨거
		int m_End; // fbx의 타임모드를 int화 시킨거
		float m_StartTime;
		float m_EndTime;
		float m_Speed;

		bool Loop;

		bool m_AniEnd;
		bool m_AniHalf;
		bool m_AniThreeQuarters;

		UserAniClip() : m_Speed(1.0f), m_AniHalf(false), m_AniThreeQuarters(false)
		{

		}
	};

	class ANIBONEDATA
	{
	public:
		Vector4 T;
		Vector4 S;
		Vector4 Q;
	};

	class BONEDATA
	{
	public:
		Vector4 LT;
		Vector4 LS;
		Vector4 LQ;
		Vector4 WT;
		Vector4 WS;
		Vector4 WQ;
		Matrix LMat;
		Matrix WMat;
	};

private:
	std::map<std::wstring, HPTR<UserAniClip>> m_Ani;

	int m_ClipIndex; // 애니메이션중
	int m_FrameCount; // fbx의 타임모드를 int화 시킨거
	
	float m_CurTime; // 현재 재생 시간
	float m_UpdateTime; // 업데이트
	int StartFrame;
	int EndFrame;
	
	bool isLock;
	Vector4 seirenData;

	Fbx::FbxAniData* CurAniData;

	HPTR<Fbx> m_FBXDATA;
	HPTR<Texture> m_BoneTex;

	std::vector<ANIBONEDATA> m_ANIBONEDATA;
	std::vector<Matrix> m_OffsetAniBoneData; // 애니메이션 행렬을 만들기 전의 본행렬
	std::vector<Matrix> m_CalAniBoneData; // 애니메이션된 행렬

	int iFrameIndex;
	int iNextFrameIndex;
	HPTR<UserAniClip> CurClip;


	HRENDERDATA* DATA;

public:
	bool IsLock()
	{
		return isLock;
	}

	void LockOn()
	{
		isLock = true;
	}
	void LockOff()
	{
		isLock = false;
	}

public:
	bool ChangeRockMaterialOn();
	bool ChangeRockMaterialOff();

public:
	UserAniClip* GetCurClip()
	{
		return CurClip.PTR;
	}

private:
	HPTR<UserAniClip> FindAni(std::wstring _Name);

public:
	void CreateAnimation(std::wstring _Name, int _Start, int _End, bool _Loop = false);
	void CreateAnimation(std::wstring _Name, int _Start, int _End, float _Speed ,bool _Loop = false);
	void ChangeAnimation(std::wstring _Name);


public:
	int GetFrameIndex() {
		return iFrameIndex;
	}

	float GetUpdateTime() 
	{
		return m_UpdateTime;
	}
	void SetFrameIndex(int _Number) 
	{
		m_UpdateTime = (float)_Number / (float)m_FrameCount;
	}




public:
	void FbxSetting(const wchar_t* _Name);
	void FbxEffectSetting(const wchar_t* _Name);

	BONEDATA FindBoneData(const wchar_t* _Name);

public:
	void Update() override;
	void UpdateBefore() override;
	void RenderDataUpdate(const HPTR<Camera>& _CAM, HRENDERDATA* _Data) override;
	void Render(const HPTR<Camera>& _CAM) override;


public:
	FbxAnimation();
	~FbxAnimation();
};

