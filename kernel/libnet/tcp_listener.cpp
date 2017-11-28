#include "stdafx.h"
#include "tcp_listener.h"


tcp_listener::tcp_listener(tcp_listener_notify& rInstance)
	:m_rInstance(rInstance)
	, m_port(0)
{
}

tcp_listener::~tcp_listener()
{
}

int32_t tcp_listener::Open(const char* csServerIp, unsigned int port)
{
	m_port = port;
	if (!m_tcpSocket.Bind(csServerIp,port))
	{
		return -1;
	}

	AddEvent();
	EnableRead();
	return 0;
}

void tcp_listener::Close()
{
	RemoveEvent();
	m_tcpSocket.Close();
}

void tcp_listener::OnRead(void)
{
	std::string strPeerIp = "";
	std::string strLocalIp = "";
	SOCKET hSocket = m_tcpSocket.Accept(strLocalIp, strPeerIp);
	if ((hSocket != INVALID_SOCKET))
	{
		m_rInstance.OnTcpListenerNotifyAccept(hSocket, strLocalIp, strPeerIp);
	}
}
