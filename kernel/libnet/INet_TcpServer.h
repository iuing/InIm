#pragma once
#include "stdafx.h"

class INet_TcpServerNotify
{
public:
	virtual bool OnNETEC_TCPServerNotifyConnected(SOCKET hSocket, const char*cszLocalIP, const char*cszPeerIP) = 0;
};

class Net_TcpServer
{
public:
	virtual void Stop() = 0;
};

NET_EXPORT Net_TcpServer* Net_TcpServerCreate(INet_TcpServerNotify& rInstance, unsigned int port);