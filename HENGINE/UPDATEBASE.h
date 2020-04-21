#pragma once

// 디파인에서 \는 한줄 내리겠다는 뜻이 된다.
//#define PLUS(X) X + X; X + X;

#define ITERFOR(Start, End, List, FuncName) \
if (0 != List##.size()) \
{ \
	Start = List##.begin(); \
	End = List##.end(); \
	for (; Start != End; ++Start) \
	{ \
		if (true == (*##Start##)->##IsUpdate##()) \
		{ \
			(*##Start##)->##FuncName##(); \
		} \
	} \
}

// class Collider;
class UpdateBase
{
protected:
	bool m_IsUpdate;
	bool m_IsDeath;

public:
	// 함수는 함수체와
	// 함수 선언으로 구성되어 있다.

	// inline은 함수체로 치환시켜 버린다.
	// inline virtual bool IsUpdate() 함수 선언
	//	{
	//      return true == m_IsUpdate && false == m_IsDeath;
    //  }

	inline virtual bool UpdateOn()
	{
		return m_IsUpdate = true;
	}

	inline virtual bool UpdateOff()
	{
		return m_IsUpdate = false;
	}

	inline virtual bool IsUpdate()
	{
		return true == m_IsUpdate && false == m_IsDeath;
	}
	inline virtual bool IsDeath() {
		return m_IsDeath;
	}

	inline virtual void Die() {
		m_IsDeath = true;
	}
	
public:
	// 가상함수가 느리다고 하는이유.
	// 빈함수라도 실행은 해버린다.
	virtual void UpdateAfter();
	virtual void Update();
	virtual void UpdateBefore();
	virtual void FinalUpdate();
	virtual void RenderBefore();
	virtual void RenderAfter();
	virtual void ColBefore();
	virtual void ColAfter();
	virtual void Release();
	virtual void ReleaseAfter();

	// 지금 왜 안쓰려고 하는가??? 선생님은.
	// TRANS가 왜 COLENTER를 호출해야 하는가?
	//virtual void ColEnter(Collider* _This, Collider* _Other) {}
	//virtual void ColStay(Collider* _This, Collider* _Other) {}
	//virtual void ColExit(Collider* _This, Collider* _Other) {}

public:
	UpdateBase();
	virtual ~UpdateBase() = 0;
};

