#include "stdafx.h"
#include "netio_queue_operator_select.h"

netio_queue_operator_select::netio_queue_operator_select(netio_queue& rInstance)
	: netio_queue_operator(rInstance)
{

}

netio_queue_operator_select::~netio_queue_operator_select()
{
}



bool netio_queue_operator_select::Start()
{
	return true;
}

void netio_queue_operator_select::Stop()
{

}

bool netio_queue_operator_select::AddEvent(SOCKET s, bool bRead, bool bWrite)
{
	if (bRead)
	{
		m_reads[s] = s;
	}
	if (bWrite)
	{
		m_writes[s] = s;
	}

	return true;
}

bool netio_queue_operator_select::RemoveEvent(SOCKET s)
{
	m_reads.erase(s);
	m_writes.erase(s);
	return true;
}

bool netio_queue_operator_select::ModifyEvent(SOCKET s, bool bRead, bool bWrite)
{
	if (bRead)
	{
		m_reads[s] = s;
	}
	else {
		m_reads.erase(s);
	}

	if (bWrite)
	{
		m_writes[s] = s;
	}
	else {
		m_writes.erase(s);
	}

	return true;
}

bool netio_queue_operator_select::PollingEvents(void)
{
	fd_set read_set;
	fd_set write_set;
	fd_set except_set;
	SOCKET Max_Socket = 0;

	if (m_reads.size() == 0 && m_writes.size() == 0)
		return false;
	
	FD_ZERO(&read_set);
	FD_ZERO(&write_set);
	FD_ZERO(&except_set);

	for (MapSocket::iterator itor = m_reads.begin(); itor != m_reads.end();++itor)
	{
		SOCKET s = itor->first;
		if (s > Max_Socket)
		{
			Max_Socket = s;
		}
		FD_SET(s, &read_set);
		FD_SET(s, &except_set);
	}

	for (MapSocket::iterator itor = m_writes.begin(); itor != m_writes.end(); ++itor)
	{
		SOCKET s = itor->first;
		if (s > Max_Socket)
		{
			Max_Socket = s;
		}
		FD_SET(s, &write_set);
		FD_SET(s, &except_set);
	}

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1000;

	int32_t nRet = select(Max_Socket + 1,
		(fd_set*)&read_set,
		(fd_set*)&write_set,
		(fd_set*)&except_set, &tv);

	if (nRet < 0)
	{
		return false;
	}

	MapSocket tMapSocket = m_reads;
	for (MapSocket::iterator iter = tMapSocket.begin(); iter != tMapSocket.end(); ++iter)
	{
		SOCKET s = iter->second;
		if (FD_ISSET(s,&read_set))
		{
			OnReadEvent(s);
		}
		if (FD_ISSET(s, &except_set))
		{
			OnErrorEvent(s);
		}
	}

	tMapSocket = m_writes;
	for (MapSocket::iterator iter = tMapSocket.begin(); iter != tMapSocket.end(); ++iter)
	{
		SOCKET hSocket = iter->second;
		if (FD_ISSET(hSocket, &except_set))
		{
			OnErrorEvent(hSocket);
		}
		else if (FD_ISSET(hSocket, &write_set))
		{
			OnWriteEvent(hSocket);
		}
	}

	return true;
}
