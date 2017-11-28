#include "stdafx.h"
#include "netio_queue.h"
#include "netio_queue_operator.h"
#include "netio_queue_operator_select.h"
#include "netio_event.h"


static netio_queue g_NetioQueue;
static bool g_bNetioQueueStarted = false;

void INetio_Queue::Start()
{
	if (!g_bNetioQueueStarted)
	{
		g_bNetioQueueStarted = g_NetioQueue.Start();
		if (g_bNetioQueueStarted)
		{
			IEventsPump::AddIEventsQueue(&g_NetioQueue);
		}
	}
}

void INetio_Queue::Stop()
{
	if (g_bNetioQueueStarted)
	{
		g_bNetioQueueStarted = false;
		IEventsPump::RemoveIEventsQueue(&g_NetioQueue);
		g_NetioQueue.Stop();
	}
}

void INetio_Queue::AddEvent(netio_event* pNetIOEvent)
{
	g_NetioQueue.AddEvent(pNetIOEvent);
}

void INetio_Queue::RemoveEvent(netio_event* pNetIOEvent)
{
	g_NetioQueue.RemoveEvent(pNetIOEvent);
}

void INetio_Queue::ModifyEvent(netio_event* pNetIOEvent)
{
	g_NetioQueue.ModifyEvent(pNetIOEvent);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
netio_queue::netio_queue()
	:m_netioQueueOperator(NULL)
{
}

netio_queue::~netio_queue()
{
}

bool netio_queue::Start()
{
	CAutoLock lock(&m_mapLock);
	//TODO
#if defined(OS_WIN) || defined(OS_IOS)
	m_netioQueueOperator = new netio_queue_operator_select(*this);
#else
#endif

	if (!m_netioQueueOperator)
		return false;

	if (!m_netioQueueOperator->Start())
	{
		m_netioQueueOperator->Start();
		delete m_netioQueueOperator;
		m_netioQueueOperator = NULL;
		return false;
	}

	return true;
}

void netio_queue::Stop()
{
	CAutoLock lock(&m_mapLock);
	if (m_netioQueueOperator)
	{
		m_netioQueueOperator->Stop();
		delete m_netioQueueOperator;
		m_netioQueueOperator = NULL;
	}
}

void netio_queue::AddEvent(netio_event* pNetioEvent)
{
	CAutoLock lock(&m_mapLock);
	if (m_netioQueueOperator == NULL)
		return;

	bool bModify = false;
	MapINetioEvent::iterator itor = m_mapNetioEvent.find(SOCKET(*pNetioEvent));
	if (itor != m_mapNetioEvent.end())
	{
		if (itor->second != pNetioEvent)	//TODO 
		{
			bModify = true;
		}
	}

	SOCKET hSocket = SOCKET(*pNetioEvent);
	m_mapNetioEvent[hSocket] = pNetioEvent;

	bool bRead = ((pNetioEvent->GetEvents()&NETIOEVENT_READ) == NETIOEVENT_READ);
	bool bWrite = ((pNetioEvent->GetEvents()&NETIOEVENT_WRITE) == NETIOEVENT_WRITE);

	if (bModify)
	{
		m_netioQueueOperator->ModifyEvent(hSocket, bRead, bWrite);
	}
	else
	{
		m_netioQueueOperator->AddEvent(hSocket, bRead, bWrite);
	}
}

void netio_queue::RemoveEvent(netio_event* pNetioEvent)
{
	CAutoLock lock(&m_mapLock);
	if (m_netioQueueOperator == NULL)
		return;

	MapINetioEvent::iterator itor = m_mapNetioEvent.find(SOCKET(*pNetioEvent));
	if (itor != m_mapNetioEvent.end())
	{
		if (itor->second == pNetioEvent)
		{
			m_mapNetioEvent.erase(itor);
			m_netioQueueOperator->RemoveEvent(SOCKET(*pNetioEvent));
		}
	}

}

void netio_queue::ModifyEvent(netio_event* pNetioEvent)
{
	CAutoLock lock(&m_mapLock);
	if (m_netioQueueOperator == NULL)
		return;

	MapINetioEvent::iterator itor = m_mapNetioEvent.find(SOCKET(*pNetioEvent));
	if (itor != m_mapNetioEvent.end())
	{
		if (itor->second == pNetioEvent)
		{
			bool bRead = ((pNetioEvent->GetEvents()&NETIOEVENT_READ) == NETIOEVENT_READ);
			bool bWrite = ((pNetioEvent->GetEvents()&NETIOEVENT_WRITE) == NETIOEVENT_WRITE);
			m_netioQueueOperator->ModifyEvent(SOCKET(*pNetioEvent), bRead, bWrite);
		}
	}
}

bool netio_queue::PumpEvents(void)
{
	CAutoLock lock(&m_mapLock);
	if (m_netioQueueOperator == NULL)
		return false;

	return m_netioQueueOperator->PollingEvents();
}

void netio_queue::OnReadEvent(SOCKET hSocket)
{
	MapINetioEvent::iterator itor = m_mapNetioEvent.find(hSocket);
	if (itor != m_mapNetioEvent.end())
	{
		itor->second->OnRead();
	}
}

void netio_queue::OnWriteEvent(SOCKET hSocket)
{
	MapINetioEvent::iterator itor = m_mapNetioEvent.find(hSocket);
	if (itor != m_mapNetioEvent.end())
	{
		itor->second->OnWrite();
	}
}

void netio_queue::OnErrorEvent(SOCKET hSocket)
{
	if (m_netioQueueOperator == NULL)
		return;

	MapINetioEvent::iterator itor = m_mapNetioEvent.find(hSocket);
	if (itor != m_mapNetioEvent.end())
	{
		itor->second->OnError();
		m_mapNetioEvent.erase(itor);
		m_netioQueueOperator->RemoveEvent(hSocket);
	}
}

