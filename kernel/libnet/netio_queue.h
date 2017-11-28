#pragma once
#include "IEventsQueue.h"
#include "Lock.h"

class netio_event;
typedef std::map<SOCKET, netio_event*>	MapINetioEvent;

class netio_queue_operator;
class netio_queue
	:public IEventsQueue
{
public:
	netio_queue();
	virtual ~netio_queue();
public:
	bool Start();
	void Stop();

	void AddEvent(netio_event* pNetioEvent);
	void RemoveEvent(netio_event* pNetioEvent);
	void ModifyEvent(netio_event* pNetioEvent);
	bool PumpEvents(void);

	void OnReadEvent(SOCKET hSocket);
	void OnWriteEvent(SOCKET hSocket);
	void OnErrorEvent(SOCKET hSocket);
protected:
	CLock				m_mapLock;
	MapINetioEvent		m_mapNetioEvent;
	netio_queue_operator *m_netioQueueOperator;
};

