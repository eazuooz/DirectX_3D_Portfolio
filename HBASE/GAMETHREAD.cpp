#include "GAMETHREAD.h"
#include "DEBUG.h"

std::map<std::wstring, HPTR<Thread>> Thread::m_AllThread;

//HGAMETHREAD::StaticConAndDes HGAMETHREAD::SCDInst;
//
//HGAMETHREAD::StaticConAndDes::StaticConAndDes() {
//	int a = 0;
//}
//HGAMETHREAD::StaticConAndDes::~StaticConAndDes() 
//{
//	HGAMETHREAD::AllThreadReset();
//}

unsigned Thread::HGAMETHREADFUNC(void* _Arg)
{
	Thread* THREAD = (Thread*)_Arg;
	unsigned int Result = THREAD->m_ThreadFunc(THREAD, THREAD->m_Arg);
	DestroyThread(THREAD->m_Name.c_str());
	return Result;
}

void Thread::AllThreadReset()
{
	std::map<std::wstring, HPTR<Thread>>::iterator Start = Thread::m_AllThread.begin();
	std::map<std::wstring, HPTR<Thread>>::iterator End = Thread::m_AllThread.end();
	for (; Start != End; ++Start)
	{
		Start->second->Death();

		// 쓰레드가 안끝나면?

		if (WAIT_TIMEOUT == WaitForSingleObject(Start->second->m_HANDLE, 100))
		{
			// 0.1초안에 꺼지지가 않았어.
			TerminateThread(Start->second->m_HANDLE, -1);
		}

		Start = Thread::m_AllThread.begin();
		End = Thread::m_AllThread.end();

		if (Start == End)
		{
			break;
		}
	}
	m_AllThread.clear();
}

void Thread::CreateThread(const wchar_t* _Name, unsigned(__stdcall* _ThreadFunc)(Thread*, void*), void* _Arg, int _Size, THREADARGMODE _Mode)
{
	if (nullptr != FindThread(_Name))
	{
		BOOM;
		return;
	}

	HPTR<Thread> NewThread = new Thread();
	NewThread->m_Name = _Name;

	switch (_Mode)
	{
	case Thread::TAM_NEW:
		NewThread->m_Arg = new char[_Size];
		memcpy_s(NewThread->m_Arg, _Size, _Arg, _Size);
		break;
	case Thread::TAM_LINK:
		NewThread->m_Arg = _Arg;
		break;
	default:
		break;
	}

	NewThread->Start(_ThreadFunc, NewThread->m_Arg);
	m_AllThread.insert(std::map<std::wstring, HPTR<Thread>>::value_type(_Name, NewThread));
}

HPTR<Thread> Thread::FindThread(const wchar_t* _Name) 
{
	std::map<std::wstring, HPTR<Thread>>::iterator FindIter = m_AllThread.find(_Name);

	if (FindIter == m_AllThread.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

// 델리트를 못하는게 좀 그렇다.
void Thread::DestroyThread(const wchar_t* _Name) 
{
	std::map<std::wstring, HPTR<Thread>>::iterator FindIter = m_AllThread.find(_Name);

	if (FindIter == m_AllThread.end())
	{
		BOOM;
		return;
	}

	// 쓰레드가 종료되지 않았으므로...
	m_AllThread.erase(FindIter);
}

void Thread::DeathThread(const wchar_t* _Name) 
{
	HPTR<Thread> Thread = FindThread(_Name);

	if (nullptr == Thread)
	{
		BOOM;
		return;
	}

	Thread->Death();
}

//// member


Thread::Thread() : m_IsEnd(false)
{
}


Thread::~Thread()
{
	// WaitForSingleObject()

	if (nullptr != m_Arg && m_Mode == THREADARGMODE::TAM_NEW)
	{
		delete[] (char*)m_Arg;
	}

	CloseHandle(m_HANDLE);
}


void Thread::Start(unsigned(__stdcall* _ThreadFunc)(Thread*, void*), void* _Arg)
{
	m_ThreadFunc = _ThreadFunc;
	m_HANDLE = (HANDLE)_beginthreadex(nullptr, 0, HGAMETHREADFUNC, this, 0, &ID);
}