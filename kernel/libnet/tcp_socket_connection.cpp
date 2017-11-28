#include "stdafx.h"
#include "tcp_socket_connection.h"
#include "utils.h"

enum {
	XSOCKET_CONNECTION_CHECK = 0,
	XSOCKET_CONNECTION_WRITE_EVENT,
};

tcp_socket_connection::tcp_socket_connection(SOCKET hSocket)
	: m_bDisconnected(false)
	, m_ulWriteTimestamp(XGetTimestamp())
	, m_ulReadimestamp(XGetTimestamp())
	, m_bNeedKeepAlive(false)
{
	m_XSocketTCP.AttachSocket(hSocket);
	m_XSocketTCP.SetNonBlock(true);

	AddEvent();
	EnableRead();

	RegisterHandle();
	PostMsg(XSOCKET_CONNECTION_CHECK, false, 1000);
}


tcp_socket_connection::~tcp_socket_connection()
{
}

void tcp_socket_connection::ReleaseConnections(void)
{
	ClearMsg();
	UnRegisterHandle();

	RemoveEvent();

	m_XSocketTCP.Close();
}

bool tcp_socket_connection::SendHeader(uint32_t ulType, const void*pData, int32_t nLen)
{
	bool bRet = socket_frame::SendHeader(ulType, pData, nLen);
	if (bRet)
	{
		PostMsg(XSOCKET_CONNECTION_WRITE_EVENT);
	}
	return bRet;
}

bool tcp_socket_connection::SendHeader(const char*pHeader, int32_t nHeaderLen)
{
	bool bRet = socket_frame::SendHeader(pHeader, nHeaderLen);
	if (bRet)
	{
		PostMsg(XSOCKET_CONNECTION_WRITE_EVENT);
	}
	return bRet;
}

int32_t tcp_socket_connection::SendData(const char*pData, int32_t nLen)
{
	m_bNeedKeepAlive = false;
	int32_t nBytes = m_XSocketTCP.Send(pData, nLen);
	if (nBytes < 0)
	{
		m_bDisconnected = true;
		OnDisconnected();
	}
	return nBytes;
}

void tcp_socket_connection::OnDisconnected(void)
{

}

void tcp_socket_connection::OnRead(void)
{
	char*pReadBuf = NULL;
	int32_t nReadBufLen = 0;
	char pBuffer[65535];
	pReadBuf = pBuffer;
	nReadBufLen = 65535;
	if (pReadBuf != NULL)
	{
		int32_t nBytes = m_XSocketTCP.Receive(pReadBuf, nReadBufLen);
		if (nBytes > 0)
		{
			OnReceivedData(pReadBuf, nBytes);

			m_ulReadimestamp = XGetTimestamp();
			//TRACE("m_ulReadimestamp=%u\n",m_ulReadimestamp);
		}
		else
		{
			if (nBytes < 0)
			{
				m_bDisconnected = true;
				OnDisconnected();
				RemoveEvent();
			}
			else if (!HasPendingData())
			{
				m_bNeedKeepAlive = true;
				PostMsg(XSOCKET_CONNECTION_WRITE_EVENT);
			}
		}
	}
}

void tcp_socket_connection::OnWrite(void)
{
	m_ulWriteTimestamp = XGetTimestamp();

	while (HasPendingData())
	{
		while (HasSendFrameData())
		{
			if (SendFrame(NULL, 0) == false)
			{
				return;
			}
		}

		while (GetDataCount())
		{
			void*pData = NULL;
			int32_t nLen = 0;
			bool bSend = true;

			LockPool();
			if (get_front(&pData, nLen))
			{
				bSend = SendFrame((const char*)pData, nLen);
				UnlockPool();
			}
			DataPool::PopFront();

			if (!bSend)
			{
				return;
			}
		}

		GetPendingData();
	}

	if (m_bNeedKeepAlive)
	{
		m_bNeedKeepAlive = false;
		SendNullFrame();
	}

	if (!HasPendingData())
	{
		DisableWrite();
	}
}

void tcp_socket_connection::OnError(void)
{
	m_bDisconnected = true;
	OnDisconnected();
}

void tcp_socket_connection::PushBack(void*pData, int32_t nLen)
{
	DataPool::PushBack(pData, nLen);

	PostMsg(XSOCKET_CONNECTION_WRITE_EVENT);
}

bool tcp_socket_connection::HasPendingData(void)
{
	return (HasSendFrameData() || GetDataCount()>0);
}

void tcp_socket_connection::GetPendingData(void)
{

}

void tcp_socket_connection::SetPendingData(void*pData, int32_t nLen)
{
	DataPool::PushBack(pData, nLen);
}

void tcp_socket_connection::HandleMsg(uint32_t ulMsgID)
{
	switch (ulMsgID)
	{
	case XSOCKET_CONNECTION_CHECK:
	{
		uint32_t ulCurrentTimestamp = XGetTimestamp();
		//#ifndef _WIN32
		{
			if (ulCurrentTimestamp < m_ulReadimestamp)
			{
				m_ulReadimestamp = ulCurrentTimestamp;
			}
			else if (ulCurrentTimestamp - m_ulReadimestamp > 60000)
			{
				m_bDisconnected = true;
				OnDisconnected();
				return;
			}
		}
		//#endif

		if (ulCurrentTimestamp < m_ulWriteTimestamp)
		{
			m_ulWriteTimestamp = ulCurrentTimestamp;
		}
		else if (ulCurrentTimestamp - m_ulWriteTimestamp > 10000)
		{
			m_ulWriteTimestamp = ulCurrentTimestamp;

			m_bNeedKeepAlive = true;

			EnableWrite();
		}
	}
	break;
	case XSOCKET_CONNECTION_WRITE_EVENT:
	{
		EnableWrite();
	}
	break;
	}
}
