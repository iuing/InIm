#include "stdafx.h"
#include "tcp_socket.h"
#include "base_socket_util.h"

#pragma warning(disable:4996)

#define SOCK_BUFF_SIZE 512*1024		

tcp_socket::tcp_socket()
{
}


tcp_socket::~tcp_socket()
{
}

bool tcp_socket::Bind(const char* serverIp , unsigned int port)
{
	if (!serverIp)
		return false;

	m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
		return false;

	{
		base_socket_util::SetReuseAddr(m_socket, 1);
		base_socket_util::SetSendBuffer(m_socket, SOCK_BUFF_SIZE);
		base_socket_util::SetRecvBuffer(m_socket, SOCK_BUFF_SIZE);
	}

	sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);
	servAddr.sin_addr.S_un.S_addr = inet_addr(serverIp);

	if (0 != ::bind(m_socket, (sockaddr*)&servAddr, sizeof(sockaddr_in)))
		return false;

	if (0 != ::listen(m_socket, 200))
		return false;

	return true;
}

bool tcp_socket::Connect(const char* serverIp, unsigned int port, bool bNonBlock /*= true*/)
{
	if (!serverIp)
		return false;

	m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
		return false;

	{
		SetNonBlock(bNonBlock);
		base_socket_util::SetReuseAddr(m_socket, 1);
		base_socket_util::SetSendBuffer(m_socket, SOCK_BUFF_SIZE);
		base_socket_util::SetRecvBuffer(m_socket, SOCK_BUFF_SIZE);
	}
	
	sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);
	servAddr.sin_addr.S_un.S_addr = inet_addr(serverIp);

	if (-1 == connect(m_socket, (sockaddr*)&servAddr, sizeof(sockaddr_in)))
	{
		if (!bNonBlock)
			return false;
#ifdef OS_WIN
		DWORD dwError = GetLastError();
		if (WSAEWOULDBLOCK != dwError || EINTR != dwError || EAGAIN != dwError && dwError != WSAEINTR && dwError != WSAETIMEDOUT)
		{
			return false;
		}
#else
		// TODO
#endif
	}

	return true;
}

SOCKET tcp_socket::Accept(std::string& strLocalIp, std::string& strPeerIp)
{
	SOCKET client_socket;
	sockaddr_in client_sa;
	socklen_t AddrLenth = sizeof(sockaddr_in);
	memset(&client_sa, 0, AddrLenth);

	client_socket = ::accept(m_socket, (sockaddr*)&client_sa,&AddrLenth);
	if (client_socket != INVALID_SOCKET)
	{
		strPeerIp = inet_ntoa(client_sa.sin_addr);
	}

	return client_socket;
}

void tcp_socket::Close()
{
	base_socket::Close();
}

int32_t tcp_socket::Send(const char* data, int len)
{
	int32_t nRet = send(m_socket, data, len, 0);
	if (nRet < 0)
	{
#ifdef OS_WIN
		DWORD dwError = WSAGetLastError();
		if (WSAEWOULDBLOCK != dwError && EINTR != dwError && EAGAIN != dwError && dwError != WSAEINTR && dwError != WSAETIMEDOUT && dwError != 0)
		{
			return -1;
		}
		else
		{
			return 0;
		}
#else
		if (errno != 0 && errno != EAGAIN && errno != EWOULDBLOCK  && errno != ETIMEDOUT && EINTR != errno)
		{
			return -1;
		}
		else
		{
			return 0;
		}
#endif

	}

	return nRet;
}

int32_t tcp_socket::Receive(char* data, int len)
{
	int32_t nRet = ::recv(m_socket, data, len, 0);
	if (nRet < 0)
	{
#ifdef OS_WIN
		DWORD dwError = WSAGetLastError();
		if (WSAEWOULDBLOCK != dwError && EINTR != dwError && EAGAIN != dwError && dwError != WSAEINTR && dwError != WSAETIMEDOUT && dwError != 0)
		{
			return -1;
		}
		else
		{
			return 0;
		}
#else
		if (errno!=0 && errno!=EAGAIN && errno!=EWOULDBLOCK  && errno!=ETIMEDOUT && EINTR!=errno)
		{
			return -1;
		}
		else
		{
			return 0;
		}
#endif

	}

	return nRet;
}
