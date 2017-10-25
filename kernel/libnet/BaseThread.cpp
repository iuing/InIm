#include "stdafx.h"
#include "BaseThread.h"
//设置线程名称
#if defined(_WIN32)
const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void SetThreadName(DWORD thread_id, const char* name)
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = name;
	info.dwThreadID = thread_id;
	info.dwFlags = 0;

	__try {
		RaiseException(MS_VC_EXCEPTION,
			0,
			sizeof(info) / sizeof(ULONG_PTR),
			(ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		//
	}
}

#endif

BaseThread::BaseThread()
{
	m_ThreadId = 0;
	m_ThreadHandle = NULL;
	m_ThreadPriority = kThreadPriorityDefault;
}


BaseThread::~BaseThread()
{
	CloseThread();
}

bool BaseThread::CreateThread(const char* threadName)
{
	// create thread first
	m_ThreadHandle = (HANDLE)_beginthreadex(NULL,
		0, ThreadProcFunc, this, 0, (unsigned*)&m_ThreadId);

	if (m_ThreadHandle < (HANDLE)2)
	{
		m_ThreadHandle = NULL;
		m_ThreadId = 0;
	}

	m_threadName = threadName;
	// if thread need set priority
	if (m_ThreadPriority > kThreadPriorityDefault)
	{
		// set the thread priority
		int pri = THREAD_PRIORITY_BELOW_NORMAL;
		if (m_ThreadPriority == kThreadPriorityLow)
			pri = THREAD_PRIORITY_LOWEST;
		else if (m_ThreadPriority == kThreadPriorityNormal)
			pri = THREAD_PRIORITY_BELOW_NORMAL;
		else if (m_ThreadPriority == kThreadPriorityHigh)
			pri = THREAD_PRIORITY_HIGHEST;
		else if (m_ThreadPriority == kThreadPriorityRealtime)
			pri = THREAD_PRIORITY_TIME_CRITICAL;
		SetThreadPriority(m_ThreadHandle, pri);
	}

	SetThreadName(m_ThreadId, threadName);

	return m_ThreadHandle > (HANDLE)1;
}

void BaseThread::CloseThread()
{
	if (m_ThreadHandle)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(m_ThreadHandle, 1000))
		{
			::CloseHandle(m_ThreadHandle);
			m_ThreadHandle = NULL;
			m_ThreadId = 0;
		}
		else
		{
			Terminate();
		}
	}
}

void BaseThread::Terminate()
{
	if (m_ThreadHandle)
	{
		::TerminateThread(m_ThreadHandle, 0);
		::CloseHandle(m_ThreadHandle);
		m_ThreadHandle = NULL;
		m_ThreadId = 0;
	}
}

void BaseThread::Sleep(int duration_ms)
{
	::Sleep(duration_ms);
}

void BaseThread::YieldThread()
{
	::Sleep(0);
}

ThreadId BaseThread::CurrentId()
{
	return GetCurrentThreadId();
}

unsigned int __stdcall BaseThread::ThreadProcFunc(void *arg)
{
	BaseThread* the_thread = (BaseThread*)arg;
	if (the_thread != NULL)
	{
		the_thread->ThreadRun();
	}
	_endthreadex(0);
	return 0;
}
