#include "ACTOR.h"
#include "COMPONENT.h"
#include "TRANS.h"
#include "SCENE.h"

Actor::Actor() : m_IsChildReady(false), level(0), order(0)
{
}


Actor::~Actor()
{
}


void Actor::PushComponent(Component* _pCom)
{
	_pCom->SetActor(this);
	_pCom->TypeSetting();
	_pCom->ComponentInit();
	_pCom->Init();
	m_ComList.push_back(_pCom);
}

void Actor::GetScene(Scene* _Scene)
{ 
	m_pScene = _Scene; 

	ChildStart = m_ChildList.begin();
	ChildEnd = m_ChildList.end();

	for (; ChildStart != ChildEnd; ++ChildStart)
	{
		(*ChildStart)->GetScene(_Scene);
	}
}

void Actor::UpdateAfter() {
	ITERFOR(ComStart, ComEnd, m_ComList, UpdateAfter);

	ITERFOR(ChildStart, ChildEnd, m_ChildList, UpdateAfter);
}
void Actor::Update() 
{
	ComStart = m_ComList.begin();
	ComEnd = m_ComList.end();

	for (; ComStart != ComEnd; ++ComStart)
	{
		if (false == (*ComStart)->IsUpdate())
		{
			continue;
		}
		(*ComStart)->Update();
	}

	ChildStart = m_ChildList.begin();
	ChildEnd = m_ChildList.end();

	for (; ChildStart != ChildEnd; ++ChildStart)
	{
		if (false == (*ChildStart)->IsUpdate())
		{
			continue;
		}
		(*ChildStart)->Update();
	}

	//ITERFOR(ComStart, ComEnd, m_ComList, Update);
	//ITERFOR(ChildStart, ChildEnd, m_ChildList, Update);
}
void Actor::UpdateBefore() {
	ITERFOR(ComStart, ComEnd, m_ComList, UpdateBefore);

	ITERFOR(ChildStart, ChildEnd, m_ChildList, UpdateBefore);
}
void Actor::FinalUpdate() {
	ITERFOR(ComStart, ComEnd, m_ComList, FinalUpdate);

	ITERFOR(ChildStart, ChildEnd, m_ChildList, FinalUpdate);
}
void Actor::RenderBefore() 
{
	ITERFOR(ComStart, ComEnd, m_ComList, RenderBefore);
	ITERFOR(ChildStart, ChildEnd, m_ChildList, RenderBefore);
}

void Actor::RenderAfter()
{
	ITERFOR(ComStart, ComEnd, m_ComList, RenderAfter);
	ITERFOR(ChildStart, ChildEnd, m_ChildList, RenderAfter);
}
void Actor::ColBefore()
{
	ITERFOR(ComStart, ComEnd, m_ComList, ColBefore);
	ITERFOR(ChildStart, ChildEnd, m_ChildList, ColBefore);
}

void Actor::ColAfter()
{
	ITERFOR(ComStart, ComEnd, m_ComList, ColAfter);
	ITERFOR(ChildStart, ChildEnd, m_ChildList, ColAfter);
}
void Actor::Release()
{
	ComStart = m_ComList.begin();
	ComEnd = m_ComList.end();

	for (; ComStart != ComEnd; )
	{
		(*ComStart)->Release();
		if (true == (*ComStart)->IsDeath())
		{
			ComStart = m_ComList.erase(ComStart);
		}
		else {
			++ComStart;
		}
	}

	ChildStart = m_ChildList.begin();
	ChildEnd = m_ChildList.end();

	for (; ChildStart != ChildEnd; )
	{
		(*ChildStart)->Release();
		if (true == (*ChildStart)->IsDeath())
		{
			ChildStart = m_ChildList.erase(ChildStart);
		}
		else {
			++ChildStart;
		}
	}


	// 이녀석만 좀 특별.
	//ITERFOR(ChildStart, ChildEnd, ChildList, Release);
	//ITERFOR(CStart, CEnd, m_ComList, Release);
}
void Actor::ReleaseAfter()
{
	ITERFOR(ComStart, ComEnd, m_ComList, ReleaseAfter);
	ITERFOR(ChildStart, ChildEnd, m_ChildList, ReleaseAfter);
}

bool Actor::CheckFunc(Component* _P, const type_info* _TI)
{
	return _P->IsType(_TI);
}

void Actor::SceneChangeStart()
{
	ITERFOR(ComStart, ComEnd, m_ComList, SceneChangeStart);
	ITERFOR(ChildStart, ChildEnd, m_ChildList, SceneChangeStart);
}

void Actor::SetParent(HPTR<Actor>	_ParentActor) 
{
	if (nullptr == _ParentActor)
	{
		BOOM;
	}

	m_ParentActor = _ParentActor;
	if (nullptr != GetTransform() && m_ParentActor->GetTransform())
	{
		GetTransform()->SetParentTrans(m_ParentActor->GetTransform());
	}

	m_IsChildReady = true;
}

void Actor::ChildReadyUpdate()
{
	if (nullptr != m_ParentActor)
	{
		m_ParentActor->m_ChildList.push_back(this);
		m_IsChildReady = false;
	}
}