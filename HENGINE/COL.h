#pragma once
#include "SUBTRANS.h"
#include <set>
#include <list>
#include <functional>

// 디버깅 사각형이 화면에 떠야지.
// AABBCOL DirectX::BoundingBox
// OBBCOL DirectX::BoundingOrientedBox
// SUB

class ColliderMgr;
class Collider : public SubTransform
{
private:
	enum MODE
	{
		COL2D,
		COL3D,
	};

public:
	friend ColliderMgr;

private:
	int m_Order;

	int GetOrder()
	{
		return m_Order;
	}

	void SetOrder(int _Order) {
		m_Order = _Order;
	}

private:
	// 이름을 붙여서 쓸수 있으니ㅏ까.
	// void(*Ptr)();
	std::list<std::function<void(Collider*, Collider*)>>::iterator m_FuncStart;
	std::list<std::function<void(Collider*, Collider*)>>::iterator m_FuncEnd;
	std::list<std::function<void(Collider*, Collider*)>> m_EnterList;
	std::list<std::function<void(Collider*, Collider*)>> m_StayList;
	std::list<std::function<void(Collider*, Collider*)>> m_ExitList;

public:
	template<typename T>
	void RegEnterFuncPush(void(T::*pFunc)(Collider*, Collider*), T* _Obj)
	{
		std::function<void(Collider*, Collider*)> Func 
			= std::bind(pFunc, _Obj
			, std::placeholders::_1
			, std::placeholders::_2);
		if (nullptr == Func) { BOOM; }
		m_EnterList.push_back(Func);
	}

	// 전역함수
	void RegEnterFuncPush(void(*_Ptr)(Collider*, Collider*))
	{
		std::function<void(Collider*, Collider*)> Func = _Ptr;
		if (nullptr == Func) { BOOM; }
		m_EnterList.push_back(Func);
	}

	template<typename T>
	void RegStayFuncPush(void(T::*pFunc)(Collider*, Collider*), T* _Obj)
	{
		std::function<void(Collider*, Collider*)> Func
			= std::bind(pFunc, _Obj
				, std::placeholders::_1
				, std::placeholders::_2);
		if (nullptr == Func) { BOOM; }
		m_StayList.push_back(Func);
	}

	// 전역함수
	void RegStayFuncPush(void(*_Ptr)(Collider*, Collider*))
	{
		std::function<void(Collider*, Collider*)> Func = _Ptr;
		if (nullptr == Func) { BOOM; }
		m_StayList.push_back(Func);
	}

	template<typename T>
	void RegExitFuncPush(void(T::*pFunc)(Collider*, Collider*), T* _Obj)
	{
		std::function<void(Collider*, Collider*)> Func
			= std::bind(pFunc, _Obj
				, std::placeholders::_1
				, std::placeholders::_2);
		if (nullptr == Func) { BOOM; }
		m_ExitList.push_back(Func);
	}

	// 전역함수
	void RegExitFuncPush(void(*_Ptr)(Collider*, Collider*))
	{
		std::function<void(Collider*, Collider*)> Func = _Ptr;
		if (nullptr == Func) { BOOM; }
		m_ExitList.push_back(Func);
	}

private:
	void ColEnter(Collider* _Other);
	void ColExit(Collider* _Other);
	void ColStay(Collider* _Other);

private:
	// > < 바이너리 서치트리
	/*, std::greater<Collider*>*/
	std::set<Collider*>::iterator FindIter;
	// 나랑 충돌한 녀석들을 보관해 놓는것
	// 도형끼리 닿아있으면
	// 충돌이 시작되면
	// 보관한다.
	// 충돌이 끝나면
	// 버린다.
	std::set<Collider*> m_OtherSet;

private:
	void Init() override;

public:
	void DataCreate(int _Order = 0);

private:
	void ColCheck(Collider* _Other);

public:
	// 다운 캐스팅
	virtual bool GeometryCheck(Collider* _Other) = 0;

private:
	// 함수 포인터를 받는다.
	// 함수 포인터로 

public:
	void SceneChangeStart() override;

public:
	Collider();
	~Collider();
};

