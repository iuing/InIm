#include "stdafx.h"
#include "event_pump.h"
#include "IEventsQueue.h"

static event_pump g_eventPump;
static bool g_eventPumpStarted = false;

void IEventsPump::Start()
{
	if (!g_eventPumpStarted)
	{
		g_eventPumpStarted = g_eventPump.Start();
	}
}

void IEventsPump::Stop()
{
	if (g_eventPumpStarted)
	{
		g_eventPumpStarted = false;
		g_eventPump.Stop();
	}
}

void IEventsPump::AddIEventsQueue(IEventsQueue* pEventQueue)
{
	g_eventPump.AddIEventsQueue(pEventQueue);
}

void IEventsPump::RemoveIEventsQueue(IEventsQueue* pEventQueue)
{
	g_eventPump.RemoveIEventQueue(pEventQueue);
}

bool IEventsPump::PumpEvents()
{
	return g_eventPump.PumpEvents();
}


/////   event_pump
event_pump::event_pump()
{
	for (size_t i = 0; i < THREAD_COUNT; i++)
	{
		m_ThreadPool[i] = NULL;
	}
}

event_pump::~event_pump()
{
}

bool event_pump::Start()
{
	bool bRet = false;
	for (size_t i = 0; i < THREAD_COUNT; i++)
	{
		m_ThreadPool[i] = new ThreadPool(*this);
		bRet = m_ThreadPool[i]->Open();
	}

	return bRet;
}

void event_pump::Stop()
{
	for (size_t i = 0; i < THREAD_COUNT; i++)
	{
		if (!m_ThreadPool[i])
		{
			m_ThreadPool[i]->Close();
		}
		delete m_ThreadPool[i];
		m_ThreadPool[i] = NULL;
	}
}

void event_pump::AddIEventsQueue(IEventsQueue* pEventsQueue)
{
	CAutoLock lock(&m_mapLock);
	m_mapIEventsQueue[pEventsQueue] = pEventsQueue;
}

void event_pump::RemoveIEventQueue(IEventsQueue* pEventsQueue)
{
	CAutoLock lock(&m_mapLock);
	m_mapIEventsQueue.erase(pEventsQueue);
}

bool event_pump::PumpEvents()
{
	CAutoLock lock(&m_mapLock);
	bool bRet = false;
	for (MapIEventsQueue::iterator it = m_mapIEventsQueue.begin(); it != m_mapIEventsQueue.end();++it)
	{
		bRet = it->second->PumpEvents();
	}

	return bRet;
}

bool event_pump::OnXThreadPoolCallbackRunOnce()
{
	return PumpEvents();
}
