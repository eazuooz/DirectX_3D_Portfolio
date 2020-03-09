#pragma once
#include <string>
#include <vector>
#include <map>
#include "REF.h"

#include "DEBUG.h"
// 눌렸다. 안눌렸다.
// 프레임으로 치면

// 눌린다 땐다.
// bool 지금 한순간 눌렸다. 1프레임 true가 된다.
// bool 계속 눌리고 있다. 지속적으로 누르고 있다.

// 자신이 필요한 것만 만들고
// bool 안눌려있다. -> 지속적으로 떼어져 있다.
// bool 땐순간. -> 1프레임만 땠다.

// bool 이 리턴된다.
// 236 LP
// 동시키도 가능하게. 'A' 'B'

// if(ture == HINPU::GetUpAway(L"Fire"))
// if(ture == HINPU::GetPress(L"Fire"))
// if(ture == HINPU::GetUp(L"Fire"))
// if(ture == HINPU::GetDown(L"Fire"))
// 총알 쏘는 코드 사용

class Input
{
private:
	//class Destructor
	//{
	//public:
	//	~Destructor() {
	//		HINPUT::InputEnd();
	//	}
	//};
	//static Destructor DES;
	//friend Destructor;

private:
	class Key : public Ref
	{
	private:
		friend Input;

	public:
		// float Time ; // 몇초간 눌렸다면.
		bool m_Up;
		bool m_UpAway; // 이녀석만 true
		bool m_Down;
		bool m_Press;
		std::vector<int> m_KeyArray;

	public:
		bool IsKeyCheck(); // 키체크하는 구간.
		void Update(); // 키체크하는 구간.

	private:
		Key();
		~Key();
	};

private:
	static std::map<std::wstring, HPTR<Key>>::iterator KeyStart;
	static std::map<std::wstring, HPTR<Key>>::iterator KeyEnd;
	static std::map<std::wstring, HPTR<Key>> m_AllKey;

	// static HPOS MousePos(HWND _Window);

	// static void InputEnd();

private:
	static HPTR<Key> FindKey(const wchar_t* _Name);

public:
	static void Update();

public:
	static HPTR<Key> NewKey;

	template<typename ...Rest>
	static void CreateKey(const wchar_t* _Name, Rest ..._Arg) 
	{
		NewKey = FindKey(_Name);

		if (nullptr != NewKey)
		{
			BOOM;
		}

		NewKey = new Key();
		// 벡터를 사용할때는 리저브나 리사이즈 해서
		// 쓸거 아니면 쓰지 말아라.
		NewKey->m_KeyArray.reserve(sizeof...(_Arg));
		PushKey(_Arg...);

		m_AllKey.insert(std::map<std::wstring, HPTR<Key>>::value_type(_Name, NewKey));
	}

	//template<typename T, typename ...Rest>
	//static void PushKey(T _Key, Rest ..._Arg)
	//{
	//  sizeof...(_Arg) // 몇개 처리되어서 몇개 남았다는 것을 알수 있다.
	//  if(&typeid(int) == typeid(_Key))
	//	NewKey->m_KeyArray.push_back(_Key);
	//	PushKey(_Arg...);
	//}

	template<typename ...Rest>
	static void PushKey(int _Key, Rest ..._Arg)
	{
		NewKey->m_KeyArray.push_back(_Key);
		PushKey(_Arg...);
	}

	static void PushKey()
	{
	}

public:
	static bool IsKey(const wchar_t* _Name);

	static bool Down(const wchar_t* _Name);
	static bool UpAway(const wchar_t* _Name);
	static bool Press(const wchar_t* _Name);
	static bool Up(const wchar_t* _Name);


public:
	Input();
	~Input();
};

