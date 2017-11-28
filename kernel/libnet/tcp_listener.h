#pragma once
#include "tcp_socket.h"
#include "netio_event.h"

class tcp_listener_notify
{
public:
	virtual void OnTcpListenerNotifyAccept(SOCKET hSocket, const std::string&strLocalIP, const std::string&strPeerIP) = 0;
};

class tcp_listener
	: public netio_event
{
public:
	tcp_listener(tcp_listener_notify& rInstance);
	virtual ~tcp_listener();
public:
	int32_t Open(const char* csServerIp, unsigned int port);
	void Close();

	operator SOCKET(void){ return m_tcpSocket; };
	void OnRead(void);
	void OnWrite(void){};
	void OnError(void){};
protected:
	tcp_listener_notify& m_rInstance;
	tcp_socket		m_tcpSocket;
	unsigned int	m_port;
};

