#pragma once
#include <set>
#include <list>
#include <vector>
#include <REF.h>
#include <Name.h>

#include "UPDATEBASE.h"

class Component;
class Transform;
class Scene;
class Actor final : public Ref, public UpdateBase, public Name
{
	// 부모자식이 들어가 있는 이상.
	// 부모의 회전이 자식에게 영향을 주는것을 항상 생객해야 한다.
	// 업 라이트 포워드는 3가지 백터 곧 -> 회전행렬
	// 최종적으로 월드에서 뽑아내는것이 가능하냐?
	// 크자이 공부가 다 곱해진 녀석에서
	// 업 라이트 포워드 
	// 로컬 나의 업 라이트 포워드 => 회전행렬에서 뽑아내면 된다.
	// 월드에서의 나의 업 라이트 포워드

private:
	friend Scene;
	friend Transform;

private:
	// 넘나들어야 하는 씬들 리스트
	bool OverReady;
	std::set<std::wstring> m_OverSceneSet;

public:
	bool IsOverReady() 
	{
		return OverReady;
	}

private:
	void OverReadyOn() {
		OverReady = true;
	}

	void OverReadyOff() 
	{
		OverReady = false;
	}

public:
	inline bool OverCheck(const wchar_t* _NextScene)
	{
		return m_OverSceneSet.end() != m_OverSceneSet.find(_NextScene);
	}

	template<typename ...Rest>
	void OverScene(Rest ..._Arg)
	{
		PushOver(_Arg...);
	}

private:
	template<typename ...Rest>
	void PushOver(std::wstring _OverName, Rest ..._Arg)
	{
		m_OverSceneSet.insert(_OverName);
		PushOver(_Arg...);
	}

	void PushOver() {}


public:
	bool IsOver() {
		return 0 != m_OverSceneSet.size();
	}

private:
	// 여행을 하는 액터가 생겼다.
	// 자신을 현재 들고 있는 씬.
	Scene* m_pScene;
	void GetScene(Scene* _Scene);

public:
	Scene* GetScene()
	{
		//if (nullptr != m_ParentActor)
		//{
		//	return m_ParentActor->Scene();
		//}
		return m_pScene; 
	}

private:
	Transform* m_pTrans;
	void SetTransform(Transform* _Trans) { m_pTrans = _Trans; }

public:
	Transform* GetTransform() { return m_pTrans; }


private: //////////////////////////////////////// 컴포넌트 관리
	std::list<HPTR<Component>> m_ComList;
	std::list<HPTR<Component>>::iterator ComStart;
	std::list<HPTR<Component>>::iterator ComEnd;

	std::list<HPTR<Component>>::iterator FindStart;
	std::list<HPTR<Component>>::iterator FindEnd;

public:
	template<typename COM, class... Types>
	HPTR<COM> AddComponent(Types&&... _Args)
	{
		COM* NewCom = new COM();
		PushComponent(NewCom);
		NewCom->DataCreate(_Args...);
		return NewCom;
	}

	template<typename COM>
	HPTR<COM> GetComponent()
	{
		FindStart = m_ComList.begin();
		FindEnd = m_ComList.end();

		for (; FindStart != FindEnd; ++FindStart)
		{
			// 헤더에서 생기는 대부분의 문제는
			// 모른다고 하는거.
			// 헤더에서는 선언 만 두면
			// 헤더가 꼬일 일이 없다.
			// 꼬인다.
			// 템플릿.

			// 여기서 h
			// 이녀석을 빌드했더니 모른다는 사람들이 많았다.


			// 이 작업을 수행하려면 무엇이 필요한가? 
			// (*FindStart)->IsType<COM>()
			// 전방선언 만으로 쓸수 있냐?
			// typeid헤더와 상관없이 사용할수 있다.
			Component* P = (*FindStart);

			if (true == CheckFunc(P, &typeid(COM)))
			{
				// 단순한 포인터의 대입과 리턴연산.
				// 이런건 그냥 사실 헤더를 몰라도 된다.
				// 그녀석의 함수의 구현을 알아야 하는 경우가 아니면
				// 사실 헤더가 필요없다.
				// 포인터를 쓰거나 대입한다는 이유로 
				// 전방선언 만으로 

				// HPTR<HCOM> -> HPTR<HRENDER>
				return (COM*)P;
			}
		}
		return nullptr;
	}


	template<typename COM>
	std::vector<HPTR<COM>> GetComToList()
	{
		std::vector< HPTR<COM>> ReturnList;

		FindStart = m_ComList.begin();
		FindEnd = m_ComList.end();

		for (; FindStart != FindEnd; ++FindStart)
		{
			if (true == CheckFunc((*FindStart), &typeid(COM)))
			{
				ReturnList.push_back((COM*)(*FindStart).PTR);
			}
		}
		return ReturnList;
	}

private:
	bool CheckFunc(Component* _P, const type_info* _TI);

private:
	void PushComponent(Component* _pCom);



// 씬 넘나다니는 구조 
// 컴포넌트 관리
// 기능관리 구조


public:
	void UpdateAfter() override;
	void Update() override;
	void UpdateBefore() override;
	void FinalUpdate() override;
	void RenderBefore() override;
	void RenderAfter() override;
	void ColBefore() override;
	void ColAfter() override;
	void Release() override;
	void ReleaseAfter() override;

public:
	void SceneChangeStart();

////////////////// 부모자식 구조.
private:
	bool							  m_IsChildReady;

	Actor*							  m_ParentActor;
	std::list<HPTR<Actor>>::iterator ChildStart;
	std::list<HPTR<Actor>>::iterator ChildEnd;
	std::list<HPTR<Actor>>			  m_ChildList;

private:
	bool IsChildReady() 
	{
		return m_IsChildReady;
	}
	void ChildReadyUpdate();

public:
	void SetParent(HPTR<Actor>	_ParentActor);

private:
	int order;

public:
	void SetOrder(int _Order)
	{
		order = _Order;
	}

private:
	int level;

public:
	void SetLevel(int _Value)
	{
		level = _Value;
	}

	int GetLevel()
	{
		return level;
	}


private:
	Actor();
	~Actor();
};

