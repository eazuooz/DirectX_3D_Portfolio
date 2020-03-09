#pragma once
  
// HPTR로 관리하지 않을거면 
// 왜 상속받으세요?
class Ref
{
// 프렌드는 잘쓰면
// 오히려 객체지향과 캡슐화를 지원해줄수 있다.
private:
	// 빌드시간이 늘어나는 것이다.
	// 왜이렇게 템플릿 
	template<typename T>
	friend class HPTR;

private:
	int REFCOUNT;

	// 중단점.
	// 메모리덤프등을 할때.
// #ifdef DEBUGMODE
private:
	bool m_bDebug;

public:
	inline bool IsDebugCheck() 
	{
		return m_bDebug;
	}

	inline void DebugOn() {
		m_bDebug = true;
	}

	void DebugCheck();
// #endif

private:
	// 더한다
	// 기억안나면 다시 공부.
	void HADDREF();
	void HDECREF();

public:
	Ref();

	// 가상함수 테이블
	// 4바이트.
	// 가상함수 테이블 당연히 느리다.
	// 객체지향의 핵심적인 요소라.
	// 인간적인 코드를 칠수가 없다.
	// 메모리 더쓴다.
	// 업캐스팅 되었을때.
	// 자식 소멸자가 호출이 안될수가 있다.
	// 소멸자 virtual 

protected:
	virtual ~Ref();
};

template<typename P>
class HPTR 
{
	// 여러가지 잡다한 기능을 만들어 볼수 있다.
	// 

public:
	P* PTR;

public:
	//// 내가 PARENT의 자식인가?
	//template<typename PARENT> 
	//bool IsChild()	{	}

	//// 내가 CHILD 의 부모인가????
	//template<typename CHILD>
	//bool IsParent() {	}

	operator P*()
	{
		return PTR;
	}


	bool operator <(const std::nullptr_t _PTR) const
	{
		return PTR < _PTR;
	}
	bool operator >(const std::nullptr_t _PTR) const
	{
		return PTR > _PTR;
	}

	// 다른 PTR
	bool operator <(const HPTR& _PTR) const
	{
		return PTR < _PTR.PTR;
	}
	bool operator >(const HPTR& _PTR) const
	{
		return PTR > _PTR.PTR;
	}

	// 다른 PTR
	bool operator <(P* const _PTR) const
	{
		return PTR < _PTR;
	}
	bool operator >(P* const _PTR) const
	{
		return PTR > _PTR;
	}

	// 1000 int
	// nullptr
	bool operator !=(const std::nullptr_t _PTR) const
	{
		return PTR != _PTR;
	}
	bool operator ==(const std::nullptr_t _PTR) const
	{
		return PTR == _PTR;
	}

// 다른 PTR
	bool operator !=(const HPTR& _PTR) const
	{
		return PTR != _PTR.PTR;
	}
	bool operator ==(const HPTR& _PTR) const
	{
		return PTR == _PTR.PTR;
	}

// 다른 PTR
	bool operator !=(P* const _PTR) const
	{
		return PTR != _PTR;
	}
	bool operator ==(P* const _PTR) const
	{
		return PTR == _PTR;
	}

// ->연산자
	P* const operator->() const
	{
		return PTR;
	}

	P& operator*() const
	{
		return (*PTR);
	}

// 대입연산자들.
	HPTR& operator =(P* const _PTR)
	{
		CheckDEC();
		PTR = _PTR;
		CheckADD();
		return *this;
	}

	HPTR& operator =(const std::nullptr_t _PTR)
	{
		CheckDEC();
		PTR = nullptr;
		return *this;
	}

	HPTR& operator =(const HPTR& _PTR)
	{
		// 원래 내가 들고있던 PTR의 소유권을 
		// PTR
		// 내가 원래 들고있던 녀석의 소유권을 포기한다.
		CheckDEC();
		PTR = _PTR.PTR;
		// 내가 새롭게 들게될 녀석의 소유권을 주장한다.
		CheckADD();
		return *this;
	}

public:
	void CheckADD() 
	{
		if (nullptr == PTR)
		{
			return;
		}

		((Ref*)PTR)->HADDREF();
	}

	void CheckDEC()
	{
		if (nullptr == PTR)
		{
			return;
		}

		((Ref*)PTR)->HDECREF();
	}

	template<typename CON>
	CON* Convert() 
	{
		return dynamic_cast<CON*>(PTR);
	}

public:
	HPTR() : PTR(nullptr) {}
	HPTR(const std::nullptr_t _PTR) : PTR(_PTR) {}
	HPTR(P* const _PTR) : PTR(_PTR) 
	{
		CheckADD();
	}

	HPTR(const HPTR& _PTR) : PTR(_PTR.PTR)
	{
		CheckADD();
	}

public:
	virtual ~HPTR()
	{
		CheckDEC();
	}
};

// 전역 연산자 오버로딩
template<typename P>
typename bool operator==(const std::nullptr_t _PTR, const HPTR<P>& _HPTR) 
{
	return _PTR == _HPTR.PTR;
}
template<typename P>
typename bool operator!=(const std::nullptr_t _PTR, const HPTR<P>& _HPTR)
{
	return _PTR != _HPTR.PTR;
}
