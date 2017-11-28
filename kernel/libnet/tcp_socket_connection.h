#pragma once
#include "tcp_socket.h"
#include "DataPool.h"
#include "netio_event.h"
#include "IMsg_Handle.h"
#include "socket_frame.h"

class tcp_socket_connection
	: public netio_event
	, public socket_frame
	, public DataPool
	, public IMsg_Handle
{
public:
	tcp_socket_connection(SOCKET hSocket);
	virtual ~tcp_socket_connection();

	virtual void ReleaseConnections(void);
	virtual bool SendHeader(uint32_t ulType, const void*pData, int32_t nLen);
	virtual bool SendHeader(const char*pHeader, int32_t nHeaderLen);

	virtual int32_t SendData(const char*pData, int32_t nLen);
	virtual void OnDisconnected(void);
	virtual bool IsDisconnected(void){ return m_bDisconnected; };

	operator SOCKET(void){ return m_XSocketTCP; };
	virtual void OnRead(void);
	virtual void OnWrite(void);
	virtual void OnError(void);

	virtual void PushBack(void*pData, int32_t nLen);
	virtual bool HasPendingData(void);
	virtual void GetPendingData(void);
	virtual void SetPendingData(void*pData, int32_t nLen);

	virtual void HandleMsg(uint32_t ulMsgID);
protected:
	tcp_socket			m_XSocketTCP;
	bool				m_bNeedKeepAlive;
	bool				m_bDisconnected;
	uint32_t			m_ulWriteTimestamp;
	uint32_t			m_ulReadimestamp;
};

