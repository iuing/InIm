#pragma once
#include "ThreadPool.h"
#include "Lock.h"

#define THREAD_COUNT 2

class IEventsQueue;
typedef std::map<void*, IEventsQueue*> MapIEventsQueue;

class event_pump
	: public ThreadPoolCallack
{
public:
	event_pump();
	virtual ~event_pump();
public:
	bool Start();
	void Stop();

	void AddIEventsQueue(IEventsQueue* pEventsQueue);
	void RemoveIEventQueue(IEventsQueue* pEventsQueue);

	bool PumpEvents();

	virtual bool OnXThreadPoolCallbackRunOnce();
protected:
	ThreadPool*			m_ThreadPool[THREAD_COUNT];
	MapIEventsQueue		m_mapIEventsQueue;
	CLock				m_mapLock;
};

