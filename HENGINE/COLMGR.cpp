#include "COLMGR.h"
#include "COL.h"

// 몬스터의 이동
// 길찾기 알고리즘...
// 한번만 찾는다.
// 모드 체인지도 해야한다.

void ColliderMgr::ColliderGroup::COL() 
{
	m_OtherStart = m_OtherList.begin();
	m_OtherEnd = m_OtherList.end();

	for (; m_OtherStart != m_OtherEnd; ++m_OtherStart)
	{
		if (this != *m_OtherStart)
		{
			OTHERGROUPCOL(*m_OtherStart);
		}
		else 
		{
			THISGROUPCOL();
		}
	}
}

void ColliderMgr::ColliderGroup::OTHERGROUPCOL(ColliderGroup* _OtherGroup) 
{
	// 절대로 지우지 않는다.
	m_ThisColStart = m_ColList.begin();
	m_ThisColEnd = m_ColList.end();

	for (; m_ThisColStart != m_ThisColEnd; ++m_ThisColStart)
	{
		if (false == (*m_ThisColStart)->IsUpdate())
		{
			continue;
		}

		m_OtherColStart = _OtherGroup->m_ColList.begin();
		m_OtherColEnd = _OtherGroup->m_ColList.end();

		for (; m_OtherColStart != m_OtherColEnd; ++m_OtherColStart)
		{
			if (false == (*m_OtherColStart)->IsUpdate())
			{
				continue;
			}

			(*m_ThisColStart)->ColCheck((*m_OtherColStart));
		}
	}
}

void ColliderMgr::ColliderGroup::THISGROUPCOL() 
{
	m_ThisColStart = m_ColList.begin();
	m_ThisColEnd = m_ColList.end();

	for (; m_ThisColStart != m_ThisColEnd; ++m_ThisColStart)
	{
		if (false == (*m_ThisColStart)->IsUpdate())
		{
			continue;
		}

		m_OtherColStart = m_ThisColStart;
		++m_OtherColStart;
		// end를 바로 가리키게 되면
		m_OtherColEnd = m_ColList.end();

		for (; m_OtherColStart != m_OtherColEnd; ++m_OtherColStart)
		{
			if (false == (*m_OtherColStart)->IsUpdate())
			{
				continue;
			}

			(*m_ThisColStart)->ColCheck((*m_OtherColStart));
		}
	}
}

void ColliderMgr::ColliderGroup::Release()
{
	m_ThisColStart = m_ColList.begin();
	m_ThisColEnd = m_ColList.end();

	for (; m_ThisColStart != m_ThisColEnd; )
	{
		if (true == (*m_ThisColStart)->IsDeath())
		{
			m_ThisColStart = m_ColList.erase(m_ThisColStart);
		}
		else {
			++m_ThisColStart;
		}
	}
}

void ColliderMgr::ColliderGroup::OverRelease()
{
	m_ThisColStart = m_ColList.begin();
	m_ThisColEnd = m_ColList.end();

	for (; m_ThisColStart != m_ThisColEnd; )
	{
		if (true == (*m_ThisColStart)->GetActor()->IsOverReady())
		{
			m_ThisColStart = m_ColList.erase(m_ThisColStart);
		}
		else {
			++m_ThisColStart;
		}
	}
}

/////////////////////////////////////// CMGR

ColliderMgr::ColliderMgr()
{
}


ColliderMgr::~ColliderMgr()
{
}


void ColliderMgr::Col() 
{
	ColOrderStart = m_COLMAP.begin();
	ColOrderEnd = m_COLMAP.end();

	for (; ColOrderStart != ColOrderEnd; ++ColOrderStart)
	{
		ColOrderStart->second->COL();
	}
}

void ColliderMgr::PushCol(const HPTR<Collider>& _COL)
{
	// 2번 정도만 그냥 하자... 
	std::map<int, HPTR<ColliderGroup>>::iterator FindIter
		= m_COLMAP.find(_COL->GetOrder());

	if (FindIter == m_COLMAP.end())
	{
		m_COLMAP.insert(std::map<int, HPTR<ColliderGroup>>::value_type(_COL->GetOrder(), new ColliderGroup()));
		FindIter = m_COLMAP.find(_COL->GetOrder());
	}

	FindIter->second->PushCol(_COL);
}

HPTR<ColliderMgr::ColliderGroup> ColliderMgr::FindGroup(const int& _Key) 
{
	std::map<int, HPTR<ColliderGroup>>::iterator FindIter
		= m_COLMAP.find(_Key);

	if (FindIter == m_COLMAP.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

void ColliderMgr::Link(const int& _Src, const int& _Dest) 
{
	ColliderGroup* SRC = FindGroup(_Src);
	ColliderGroup* DEST = FindGroup(_Dest);

	if (nullptr == SRC)
	{
		SRC = new ColliderGroup();
		m_COLMAP.insert(std::map<int, HPTR<ColliderGroup>>::value_type(_Src, SRC));
	}

	if (nullptr == DEST)
	{
		DEST = new ColliderGroup();
		m_COLMAP.insert(std::map<int, HPTR<ColliderGroup>>::value_type(_Dest, DEST));
	}

	SRC->PushOtherGroup(DEST);
}

void ColliderMgr::Release()
{
	ColOrderStart = m_COLMAP.begin();
	ColOrderEnd = m_COLMAP.end();

	for (; ColOrderStart != ColOrderEnd; ++ColOrderStart)
	{
		ColOrderStart->second->Release();
	}
}

void ColliderMgr::OverRelease() 
{
	ColOrderStart = m_COLMAP.begin();
	ColOrderEnd = m_COLMAP.end();

	for (; ColOrderStart != ColOrderEnd; ++ColOrderStart)
	{
		ColOrderStart->second->Release();
	}
}