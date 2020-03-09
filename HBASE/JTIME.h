#pragma once

#include <Windows.h>
// 단위 

// 1초에 RUN을 
// deltatime
// 컴퓨터의 프레임 사이 간격을 
// 현실시간으로 변경한 시간.
//  
// A 컴퓨터 2 (2 * deltatime(0.5)) = 1
// B 컴퓨터 1 (1 * deltatime(1.0)) = 1

// 컴퓨터에는 CPU에 물리적으로 
// 항상 정확히 1초마다 정확히 같은 수를 정확히 똑같이 반복적으로 세고 있는다.
// 1초에 100번을 센다.
// ConstTime = 100;

// ConstTime = 1초에얼마샐수있니();
// Start = 시간얼마니() 50;
// Prev = 시간얼마니();


// Start = 누적시간얼마니() 50;
//  50    -  0    / 100
// (Start - Prev) / ConstTime; 0.5 
// Prev = Start;

// Start = 누적시간얼마니() 100;
//  100    -  50    / 100
// (Start - Prev) / ConstTime; 0.5 
// Prev = Start;

// Start = 누적시간얼마니() 150;
//  150    -  100    / 100
// (Start - Prev) / ConstTime; 0.5 
// Prev = Start;

class Time
{
public:
	class Timer 
	{
	public:
		friend Time;

	private:
		// __int64
		// 64비트 인트
		LARGE_INTEGER m_CountingTime; // 1초에 몇을 셀수 있느냐?
		LARGE_INTEGER m_CurTime; // 현재 얼마냐?
		LARGE_INTEGER m_PrevTime; // 이전에 얼마였냐?
		double m_DDeltaTime;
		float m_DeltaTime;

	public:
		float Update();
		void Reset();

	private:
		Timer();
		~Timer();
	};

private:
	// 여러분들에게는 프로파일링을 보여주려고 한다.
	// 게임 루프의 시간을 재는 용도.
	static Timer MainTimer; 

public:
	// 너무 간단한 함수라 여기안에 뒀는데
	// 내부에 두어도 상관이 없다.
	// inline 함수에 대해서 이론공부해보세요.
	static float DeltaTime()
	{
		return MainTimer.m_DeltaTime;
	}

	static float SDeltaTime(float _Speed)
	{
		return MainTimer.m_DeltaTime * _Speed;
	}

public:
	static void Update();
	static void Reset();


private:
	Time() {}
	~Time() {}
};

