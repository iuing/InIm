#pragma once
#include "netio_queue_operator.h"

class netio_queue_operator_select
	: public netio_queue_operator
{
public:
	netio_queue_operator_select(netio_queue& rInstance);
	virtual ~netio_queue_operator_select();
public:
	virtual bool Start();
	virtual void Stop();

	virtual bool AddEvent(SOCKET s, bool bRead, bool bWrite);
	virtual bool RemoveEvent(SOCKET s);
	virtual bool ModifyEvent(SOCKET s, bool bRead, bool bWrite);

	virtual bool PollingEvents(void);
private:
	typedef std::map<SOCKET, SOCKET> MapSocket;
	MapSocket  m_reads;
	MapSocket  m_writes;
};

