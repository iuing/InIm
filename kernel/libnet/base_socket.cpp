#include "stdafx.h"
#include "base_socket.h"

#if defined(OS_WIN)
WinsockInit g_winsockInit;
#endif

base_socket::base_socket()
{
}


base_socket::~base_socket()
{
}

void base_socket::AttachSocket(SOCKET s)
{
	m_socket = s;
}

void base_socket::Detach()
{
	m_socket = INVALID_SOCKET;
}

void base_socket::Close()
{
#ifdef OS_WIN
	closesocket(m_socket);
#else
	close(m_socket);
#endif
	m_socket = INVALID_SOCKET;
}

bool base_socket::SetNonBlock(bool bNonBlock)
{
	if (m_socket != INVALID_SOCKET)
	{
#ifdef OS_WIN
		unsigned long lValue = bNonBlock ? 1 : 0;
		return 0 == ioctlsocket(m_socket, FIONBIO, &lValue);
#else
		//	TODO 
	
#endif

	}
}
