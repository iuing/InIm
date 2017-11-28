#pragma once
#include "INet_TcpServer.h"
#include "tcp_listener.h"
#include "IMsg_Handle.h"

class tcp_server_manager
	: public tcp_listener_notify
	, public IMsg_Handle
	, public Net_TcpServer
{
public:
	tcp_server_manager(INet_TcpServerNotify& rNETEC_TCPServerNotify);
	virtual ~tcp_server_manager();
public:
	bool Start(unsigned port);
	void Stop();

protected:
	virtual void HandleMsg(unsigned int ulMsgID);
	virtual void OnTcpListenerNotifyAccept(SOCKET hSocket, const std::string&strLocalIP, const std::string&strPeerIP);
protected:
	INet_TcpServerNotify&		m_rNetTcpServerNotify;
	tcp_listener*				m_pTcpListener;
	uint32_t					m_port;
};

