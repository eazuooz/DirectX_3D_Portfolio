#pragma once
#include <map>
#include <list>

#include <Name.h>
#include <JTIME.h>

#include "UPDATEBASE.h"
#include "TRANS.h"
#include "ACTOR.h"
#include "GRAPHICDEVICE.h"
#include "RENMGR.h"
#include "COLMGR.h"

// 상속 더이상 내리지마.
class Scene final : public Ref, public UpdateBase, public Name
{
private:
	friend class GameWindow;
	friend class RenderMgr;

public:
	class Updater : public Ref
	{
	public:
		friend Scene;

	private:
		Scene* pScene; // 자기를 가지고 있는 씬
		void SetScene(Scene* _pScene)
		{
			pScene = _pScene;
		}

	public:
		Scene* GetScene()
		{
			return pScene;
		}

	private:
		bool IsLoading;

	protected:
		Updater() : IsLoading(false) {}
		virtual void Loading() = 0 {} // 로딩될때
		virtual void SceneUpdate() {} // 씬 업데이트에서 만들어야할 내용.
		virtual void ChangeEnd() {} // 내가 이제 사라질때
		virtual void ChangeStart() {} // 내가 새로운 메인씬이 되었을때
		virtual void DebugRender() {} // 내가 새로운 메인씬이 되었을때

		bool MouseInTheBox(Actor* _Box);
	};

private:
	GameWindow* pWindow;
	void Window(GameWindow* _Window)	{		pWindow = _Window;	}

public:
	GameWindow* Window()	{		return pWindow;	}

private:
	// 동시에 두개를 집어넣을수 없다고 생각하고 만들겠습니다.
	std::list<HPTR<Updater>> m_UpdaterList;
	std::list<HPTR<Updater>>::iterator m_UpdaterBegin;
	std::list<HPTR<Updater>>::iterator m_UpdaterEnd;

private:
	virtual void LoadingCheck()
	{
		m_UpdaterBegin = m_UpdaterList.begin();
		m_UpdaterEnd = m_UpdaterList.end();

		for (; m_UpdaterBegin != m_UpdaterEnd; ++m_UpdaterBegin)
		{
			if (false == (*m_UpdaterBegin)->IsLoading)
			{
				(*m_UpdaterBegin)->Loading();
				(*m_UpdaterBegin)->IsLoading = true;
			}
		}

		// 시간 다시 재계산
		// 로딩시간을 고려한다.
		Time::Reset();
	} // 로딩될때

public:
	template<typename Com>
	void AddUpdater() 
	{
		Com* NewT = new Com();
		AddUpdater(NewT);
	}

private:
	void AddUpdater(HPTR<Updater> _pCom);

/////////////////////////////////////////////////// Actor

private:
	// 순서를 지정하려면
	std::list<HPTR<Actor>>::iterator m_ActorBegin;
	std::list<HPTR<Actor>>::iterator m_ActorEnd;
	std::list<HPTR<Actor>> m_ActorList;

public:
	HPTR<Actor> CreateActor(const wchar_t* _Name = L"GameObject");


	
	// HPTR<HACTOR> CreateNoneTransActor(const wchar_t* _Name = L"GameObject");


public:
	void ChangeEnd(); // 내가 이제 사라질때
	void ChangeStart(); // 내가 새로운 메인씬이 되었을때

private:
	void Progress();
	void SceneComUpdate();

	void ActorCheck();

	void UpdateBefore()override;
	void Update()override;
	void UpdateAfter()override;
	void FinalUpdate()override;
	void RenderBefore()override;
	void RenderAfter()override;
	void ColBefore()override;
	void ColAfter()override;
	void Release()override;
	void ReleaseAfter()override;
	void DebugRender() ;


///////////////////////////////////// Render
public:
	RenderMgr RMGR;

private:
	void SceneRender();

///////////////////////////////////// Col 
public:
	ColliderMgr CMGR;

private:
	void SceneCol();

	//////////////////////////////////// OverList
private:
	std::list<HPTR<Actor>> ExtractOverActor(const wchar_t* _NextSceneName);
	void OverActorPush(std::list<HPTR<Actor>>&& _List);

private:
	Scene();
	~Scene();
};