#pragma once
#include "base_socket.h"


class tcp_socket
	: public base_socket
{
public:
	tcp_socket();
	virtual ~tcp_socket();
public:
	bool Bind(const char* serverIp,unsigned int port);
	bool Connect(const char* serverIp, unsigned int port,bool bNonBlock = true);
	SOCKET Accept(std::string& strLocalIp, std::string& strPeerIp);
	void Close();

	int32_t Send(const char* data, int len);
	int32_t Receive(char* data, int len);
};

