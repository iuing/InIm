#include "stdafx.h"
#include "utils.h"
#include "tcp_socket_session.h"

tcp_socket_session::tcp_socket_session(tcp_socket_session_notify& rInstance)
	: m_rTcpSessionNotify(rInstance)
	, m_ulType(-1)
	, m_ulTimestamp(XGetTimestamp())
	, m_pData(NULL)
{

}

tcp_socket_session::~tcp_socket_session()
{
}

bool tcp_socket_session::Start(SOCKET hSocket, const std::string& strLocalIp, const std::string&strPeerIP)
{
	m_strLocalIP = strLocalIp;
	m_strPeerIP = strPeerIP;

	m_TcpSocket.AttachSocket(hSocket);

	AddEvent();
	EnableRead();

	return true;
}

void tcp_socket_session::Stop()
{
	RemoveEvent();
	m_TcpSocket.Close();
	if (m_pData)
	{
		delete m_pData;
		m_pData = NULL;
	}
}

bool tcp_socket_session::IsExpired()
{
	uint32_t ulCurrentTimestamp = XGetTimestamp();
	if (ulCurrentTimestamp < m_ulTimestamp)
	{
		m_ulTimestamp = ulCurrentTimestamp;
	}
	else
	{
		if (ulCurrentTimestamp - m_ulTimestamp > 60000)
		{
			return true;
		}
	}
	return false;
}

void tcp_socket_session::OnRead(void)
{

}

void tcp_socket_session::OnWrite(void)
{

}

void tcp_socket_session::OnError(void)
{
	m_rTcpSessionNotify.OnTcpSessionNotifyFailed(this);
}
