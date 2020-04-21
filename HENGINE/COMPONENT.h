#pragma once
#include <REF.h>
#include <NAME.h>
#include <DEBUG.h>
#include <MATHS.h>
#include <DATATYPE.h>

#include "UPDATEBASE.h"
#include "ACTOR.h"
// 각 컴포넌트에는 그래서.
// 나는 어떤 컴포넌트가 없으면 만들수 없는애야!
// 나는 하나의 오브젝트 안에 1개이상 있을수 없어.

class Transform;
class Component : public Ref, public UpdateBase, public DataType
{
private:
	friend Actor;

public:
	inline bool IsUpdate() override
	{
		return ((true == m_IsUpdate) && (false == m_IsDeath) && (true == GetActor()->IsUpdate()));
	}
	inline bool IsDeath() override
	{
		return (m_IsDeath || (true == GetActor()->IsDeath()));
	}

	//inline bool Death() override
	//{
	//	m_IsDeath = true;
	//}

private:
	Actor* m_pActor;
	void SetActor(Actor* _Actor) 	{ m_pActor = _Actor; }

public:
	Actor* GetActor()
	{ 
		return m_pActor; 
	}

public:
	Scene* GetScene()
	{
		return GetActor()->GetScene();
	}

private:
	Transform* m_pTrans;

public:
	Transform* GetTransform()
	{
		return m_pTrans;
	}

public:
	virtual void Init() {}
	void ComponentInit();

public:
	void DataCreate() {}

public:
	virtual void SceneChangeStart();

public:
	Component();
	~Component();
};

