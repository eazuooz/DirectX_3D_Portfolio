#pragma once
#include <REF.h>
#include <DEBUG.h>

#include <map>
#include <list>
// 무조건 충돌처리
// 화면 바깥으로 나가면 안해야할 경우가 있는데.
// 결국 누구를 알아야 하나?
// CAM
class Collider;
class Scene;
class ColliderMgr
{
public:
	friend Collider;
	friend Scene;

private:
	class ColliderGroup : public Ref
	{
	public:
		friend ColliderMgr;

	public:
		std::list<HPTR<Collider>>::iterator m_ThisColStart;
		std::list<HPTR<Collider>>::iterator m_ThisColEnd;
		std::list<HPTR<Collider>>::iterator m_OtherColStart;
		std::list<HPTR<Collider>>::iterator m_OtherColEnd;

	public:
		// 나랑 충돌할 다른 그룹들을 보관하고 있는것.
		std::list<ColliderGroup*>::iterator m_OtherStart;
		std::list<ColliderGroup*>::iterator m_OtherEnd;
		std::list<ColliderGroup*> m_OtherList;

	private:
		std::list<HPTR<Collider>> m_ColList;

		void PushCol(const HPTR<Collider>& _COL) 
		{
			if (nullptr == _COL) { BOOM; }
			m_ColList.push_back(_COL);
		}

		void PushOtherGroup(ColliderGroup* _OtherGroup) 
		{
			if (nullptr == _OtherGroup) { BOOM; }
			m_OtherList.push_back(_OtherGroup);
		}

	public:
		void Release();
		void OverRelease();
		void COL();
		void OTHERGROUPCOL(ColliderGroup* _OtherGroup);
		void THISGROUPCOL();
	};

private:
	std::map<int, HPTR<ColliderGroup>>::iterator ColOrderStart;
	std::map<int, HPTR<ColliderGroup>>::iterator ColOrderEnd;
	std::map<int, HPTR<ColliderGroup>> m_COLMAP;

private:
	HPTR<ColliderGroup> FindGroup(const int& _Key);

	void PushCol(const HPTR<Collider>& _COL);

private:
	void Col();
	void Release();
	void OverRelease();

public:
	// 이거 호출 안하면 그룹간 충돌 x
	void Link(const int& _Src, const int& _Dest);

public:
	ColliderMgr();
	~ColliderMgr();
};