#pragma once
#include "BaseThread.h"

class ThreadPoolCallack
{
public:
	virtual bool OnXThreadPoolCallbackRunOnce(void) = 0;
};

class ThreadPool
	:public BaseThread
{
public:
	ThreadPool(ThreadPoolCallack& callback);
	virtual ~ThreadPool();

	bool		Open();
	void		Close();
protected:
	virtual void ThreadRun();
protected:
	ThreadPoolCallack & m_callback;
	bool	m_bIsRuning;
};

