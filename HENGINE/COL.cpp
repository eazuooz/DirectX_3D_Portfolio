#include "COL.h"
#include "SCENE.h"


Collider::Collider()
{
}


Collider::~Collider()
{
}


void Collider::ColCheck(Collider* _Other)
{
	// 나와 너의 도형은 닿아있다.
	if (true == GeometryCheck(_Other))
	{
		// 이전 나랑 충돌한 애들을 보관하던 자료구조에서
		// 없었데
		if (m_OtherSet.end() == m_OtherSet.find(_Other))
		{
			m_OtherSet.insert(_Other);
			ColEnter(_Other);

			// _Other->Actor(
		}
		else 
		{
			ColStay(_Other);
		}
	}
	else 
	{
		if (m_OtherSet.end() != m_OtherSet.find(_Other))
		{
			ColExit(_Other);
			m_OtherSet.erase(_Other);
		}
	}


}

bool Collider::GeometryCheck(Collider* _Other) 
{
	BOOM;

	return false;
}


void Collider::Init()
{
	if (nullptr == GetActor()->GetTransform())
	{
		BOOM;
	}

	SetParentTrans(GetActor()->GetTransform());
}


void Collider::DataCreate(int _Order) 
{
	m_Order = _Order;
	GetScene()->CMGR.PushCol(this);
}

void Collider::ColEnter(Collider* _Other)
{
	m_FuncStart = m_EnterList.begin();
	m_FuncEnd = m_EnterList.end();

	for (; m_FuncStart != m_FuncEnd; ++m_FuncStart)
	{
		(*m_FuncStart)(this, _Other);
	}
}

void Collider::ColExit(Collider* _Other)
{
	m_FuncStart = m_ExitList.begin();
	m_FuncEnd = m_ExitList.end();

	for (; m_FuncStart != m_FuncEnd; ++m_FuncStart)
	{
		(*m_FuncStart)(this, _Other);
	}
}

void Collider::ColStay(Collider* _Other)
{
	m_FuncStart = m_StayList.begin();
	m_FuncEnd = m_StayList.end();

	for (; m_FuncStart != m_FuncEnd; ++m_FuncStart)
	{
		(*m_FuncStart)(this, _Other);
	}
}

void Collider::SceneChangeStart() 
{
	GetScene()->CMGR.PushCol(this);
}