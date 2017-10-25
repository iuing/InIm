#pragma once
class base_socket_util
{
public:
	base_socket_util();
	~base_socket_util();
public:
	static bool SetReuseAddr(SOCKET s,int value);

	static bool SetSendBuffer(SOCKET s, int32_t bufSize);

	static bool SetRecvBuffer(SOCKET s, int32_t bufSize);
};

