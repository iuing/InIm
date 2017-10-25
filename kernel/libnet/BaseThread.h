#pragma once

#if defined(OS_WIN)
#include <process.h>
typedef unsigned int ThreadId;
typedef void*	ThreadHandle;
#endif

const ThreadId kInvalidThreadId = 0;

enum ThreadPriority
{
	kThreadPriorityDefault,
	kThreadPriorityLow,
	kThreadPriorityNormal,
	kThreadPriorityHigh,
	kThreadPriorityRealtime
};

class BaseThread
{
public:
	BaseThread();
	virtual ~BaseThread();

	// Create a thread with a option, if the option is null, thread will start with
	// a default option
	bool CreateThread(const char* threadName = "");

	// Close the thread
	void CloseThread();

	// Terminate the thread
	void Terminate();

	// Sleeps for the specified duration (units are milliseconds)
	static void Sleep(int duration_ms);

	// Yield the current thread so another thread can be scheduled
	static void YieldThread();

	// Gets the current thread id
	static ThreadId CurrentId();

	ThreadId thread_id() const { return m_ThreadId; }

	void set_thread_id(ThreadId thread_id) { m_ThreadId = thread_id; }

	// Gets the current thread handle
	ThreadHandle thread_handle() const  { return m_ThreadHandle; }

	// Set the thread's priority
	void set_thread_priority(ThreadPriority priority) { m_ThreadPriority = priority; }
protected:
	virtual void ThreadRun() = 0;
private:
#if defined(_WIN32)
	static unsigned int __stdcall ThreadProcFunc(void *arg);

	std::string m_threadName;
#endif

	ThreadId		m_ThreadId;
	ThreadHandle	m_ThreadHandle;
	ThreadPriority	m_ThreadPriority;
	
};

