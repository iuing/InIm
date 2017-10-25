#pragma once

#if defined(OS_WIN)
class WinsockInit
{
public:
	WinsockInit() {
		WSADATA wsadata;
		WSAStartup(MAKEWORD(2, 2), &wsadata);
	}
	~WinsockInit() {
		WSACleanup();
	}
};
#endif

class base_socket
{
public:
	base_socket();
	virtual ~base_socket();
public:
	virtual void AttachSocket(SOCKET s);
	virtual void Detach();
	virtual void Close();
	
	virtual operator SOCKET(){ return m_socket; }

	bool SetNonBlock(bool bNonBlock);

protected:
	SOCKET	m_socket;
};

