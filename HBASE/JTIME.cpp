#include "JTIME.h"

///////////////////////////////////// Timer

Time::Timer::Timer() : m_DeltaTime(0.0f)
{
	Reset();
}


Time::Timer::~Timer()
{
}

// 일단 델타타임을 구하고 리턴해준다.
float Time::Timer::Update() 
{
	// 이때 샌다면 
	QueryPerformanceCounter(&m_CurTime);

	//1000 == 1초
	//   0 == 0초
	// 500 == 0.5초

	m_DDeltaTime = ((double)m_CurTime.QuadPart - (double)m_PrevTime.QuadPart) / ((double)m_CountingTime.QuadPart);
	m_DeltaTime = (float)m_DDeltaTime;
	m_PrevTime = m_CurTime;

	return m_DeltaTime;
}

// 체크시작전 각 맴버함수들의 초기에 있어야할 상태로 돌린다. 
void Time::Timer::Reset() 
{
	// 절대로 바뀌지 않을 
	// 100
	QueryPerformanceFrequency(&m_CountingTime);
	// 나는 현재까지 얼마를 숫자를 카운팅 했다.
	// 38
	QueryPerformanceCounter(&m_CurTime);
	QueryPerformanceCounter(&m_PrevTime);

	// m_CountingTime; // 1초에 몇을 셀수 있느냐?
	// m_CurTime; // 현재 얼마냐?
	// m_PrevTime; // 이전에 얼마였냐?

}

///////////////////////////////////// Time
Time::Timer Time::MainTimer;

void Time::Update() 
{
	MainTimer.Update();
}
void Time::Reset()
{
	MainTimer.Reset();
}