#pragma once
#include <ACTOR.h>
#include <SPRITERENDER.h>


class GameManager 
{
private:
	static GameManager m_inst;

public:
	static GameManager& Inst()
	{
		return m_inst;
	}

public:
	enum MGRSTATE
	{
		WAIT,
		WAR,
	};
private:
	HPTR<Actor> actor;
	HPTR<SpriteRenderer> renderer1;
	HPTR<SpriteRenderer> renderer2;

	Scene* curScene;

private:
	MGRSTATE state; //0 -> 대기상태 , 1 -> 전투상태
	bool isWarStart;


	float stayTime;
	float gameTIme;
	float bossTime;

	bool bossOn;
	int curTime;
	int number;
	int numberOfTen;
	int warRound;

	std::list<Actor*> deadAtorList;

public:
	 void PushDeadList(Actor* _Acttor)
	{
		deadAtorList.push_back(_Acttor);
	}

	 void DeadListAllDie()
	 {
		 std::list<Actor*>::iterator begin = deadAtorList.begin();
		 std::list<Actor*>::iterator end = deadAtorList.end();

		 for (; begin != end ; ++begin)
		 {
			 (*begin)->Die();
			 (*begin) = nullptr;
		 }

		 deadAtorList.clear();
	 }

public:
	void SkipState();
	

public:
	MGRSTATE GetState()
	{
		return state;
	}
	
public:
	void Init(Scene* _Scene) ;
	void Update() ;

public:
	int GetCurTime();
	void UpdateTimeTable();


public:
	GameManager();
	~GameManager();
};

