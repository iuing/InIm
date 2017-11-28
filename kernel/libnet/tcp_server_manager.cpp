#include "stdafx.h"
#include "tcp_server_manager.h"
#include "tcp_listener.h"

#define DO_CHECK	1

Net_TcpServer* Net_TcpServerCreate(INet_TcpServerNotify& rInstance, unsigned int port)
{
	tcp_server_manager *pTcpServerManager = new tcp_server_manager(rInstance);
	if (pTcpServerManager && pTcpServerManager->Start(port) == false)
	{
		pTcpServerManager->Stop();
		delete pTcpServerManager;
		pTcpServerManager = NULL;
	}

	return pTcpServerManager;
}

tcp_server_manager::tcp_server_manager(INet_TcpServerNotify& rNetTcpServerNotify)
	: m_rNetTcpServerNotify(rNetTcpServerNotify)
	, m_pTcpListener(NULL)
{

}

tcp_server_manager::~tcp_server_manager()
{
}

bool tcp_server_manager::Start(unsigned port)
{
	m_pTcpListener = new tcp_listener(*this);
	if (m_pTcpListener->Open("",port) != 0)
	{
		m_pTcpListener->Close();
		delete m_pTcpListener;
		m_pTcpListener = NULL;
	}

	m_port = port;
	RegisterHandle();
	PostMsg(DO_CHECK, false, 1000);

	return true;
}

void tcp_server_manager::Stop()
{
	ClearMsg();
	UnRegisterHandle();

	if (m_pTcpListener)
	{
		m_pTcpListener->Close();
		delete m_pTcpListener;
		m_pTcpListener = NULL;
	}
}

void tcp_server_manager::HandleMsg(unsigned int ulMsgID)
{
	switch (ulMsgID)
	{
	case DO_CHECK:
	{
		//DoCheck();
	}
	break;
	}
}

void tcp_server_manager::OnTcpListenerNotifyAccept(SOCKET hSocket, const std::string&strLocalIP, const std::string&strPeerIP)
{
	bool bRet = m_rNetTcpServerNotify.OnNETEC_TCPServerNotifyConnected(hSocket, strLocalIP.c_str(), strPeerIP.c_str());
	if (!bRet)
	{
		tcp_socket tXSocketTCP;
		tXSocketTCP.AttachSocket(hSocket);
		tXSocketTCP.Close();
	}
}
