#pragma once
#include "stdafx.h"
#include "netio_queue.h"

class netio_queue_operator
{
public:
	netio_queue_operator(netio_queue& rInstance)
		:m_rNetioQueue(rInstance){};
	virtual ~netio_queue_operator(){};

	virtual bool Start() = 0;
	virtual void Stop() = 0;

	virtual bool AddEvent(SOCKET s, bool bRead, bool bWrite) = 0;
	virtual bool RemoveEvent(SOCKET s) = 0;
	virtual bool ModifyEvent(SOCKET s, bool bRead, bool bWrite) = 0;

	virtual bool PollingEvents(void) = 0;
	virtual void ResetEvents(void){};
protected:
	void OnReadEvent(SOCKET hSocket){ m_rNetioQueue.OnReadEvent(hSocket); };
	void OnWriteEvent(SOCKET hSocket){ m_rNetioQueue.OnWriteEvent(hSocket); };
	void OnErrorEvent(SOCKET hSocket){ m_rNetioQueue.OnErrorEvent(hSocket); };
protected:
	netio_queue&	m_rNetioQueue;
};