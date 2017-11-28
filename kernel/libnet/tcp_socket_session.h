#pragma once
#include "tcp_socket.h"
#include "netio_event.h"

class tcp_socket_session;
class tcp_socket_session_notify
{
public:
	virtual bool OnTcpSessionNotifyGetData(tcp_socket_session* pSession, SOCKET hSocket, const std::string&strLocalIP, const std::string&strPeerIP, uint32_t ulType, const void*pData, int32_t nLen) = 0;
	virtual void OnTcpSessionNotifyFailed(tcp_socket_session* pSession);
};

class tcp_socket_session
	: public netio_event
{
public:
	tcp_socket_session(tcp_socket_session_notify& rInstance);
	virtual ~tcp_socket_session();
public:
	bool Start(SOCKET hSocket, const std::string& strLocalIp, const std::string&strPeerIP);
	void Stop();

	bool IsExpired();
	operator SOCKET(){ return m_TcpSocket; }

	void OnRead(void);
	void OnWrite(void);
	void OnError(void);
protected:
	tcp_socket_session_notify&		m_rTcpSessionNotify;
	tcp_socket						m_TcpSocket;
	uint32_t						m_ulType;
	void*							m_pData;
	int32_t							m_nLen;
	int32_t							m_nBytes;
	uint32_t						m_ulTimestamp;

	std::string						m_strLocalIP;
	std::string						m_strPeerIP;
};

