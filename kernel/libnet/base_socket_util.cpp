#include "stdafx.h"
#include "base_socket_util.h"



base_socket_util::base_socket_util()
{
}


base_socket_util::~base_socket_util()
{
}

bool base_socket_util::SetReuseAddr(SOCKET s, int value)
{
	return setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&value,sizeof(value));
}

bool base_socket_util::SetSendBuffer(SOCKET s, int32_t bufSize)
{
	return setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char*)&bufSize, sizeof(bufSize));
}

bool base_socket_util::SetRecvBuffer(SOCKET s, int32_t bufSize)
{
	return setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char*)&bufSize, sizeof(bufSize));
}
