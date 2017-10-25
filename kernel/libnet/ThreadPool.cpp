#include "stdafx.h"
#include "ThreadPool.h"


ThreadPool::ThreadPool(ThreadPoolCallack& callback)
	: m_callback(callback)
	, m_bIsRuning(false)
{

}

ThreadPool::~ThreadPool()
{
}

bool ThreadPool::Open()
{
	m_bIsRuning = true;
	return CreateThread();
}

void ThreadPool::Close()
{
	m_bIsRuning = false;
	CloseThread();
}

void ThreadPool::ThreadRun()
{
	while (m_bIsRuning)
	{
		if (!m_callback.OnXThreadPoolCallbackRunOnce())
		{
			::Sleep(1);
		}
	}
}
