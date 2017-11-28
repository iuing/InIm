#include "stdafx.h"
#include "socket_frame.h"

socket_frame::socket_frame()
	:m_pSendBuffer(NULL)
	, m_nSendBufferLength(0)
	, m_pSendFrameData(NULL)
	, m_nSendFrameDataLen(0)
	, m_pReceivedBuffer(NULL)
	, m_nReceivedBufferLength(0)
	, m_nReceivedDataLen(0)
{
}

socket_frame::~socket_frame()
{
	Reset();
}

bool socket_frame::SendHeader(uint32_t ulType, const void*pData, int32_t nLen)
{
	m_nSendFrameDataLen = 8 + nLen;
	m_nSendBufferLength = 1024;
	if (m_nSendFrameDataLen > m_nSendBufferLength)
		m_nSendBufferLength = m_nSendFrameDataLen;

	m_pSendBuffer = (char*)malloc(m_nSendBufferLength);
	if (m_pSendBuffer == NULL)
	{
		return false;
	}
	*((uint32_t*)(m_pSendBuffer)) = htonl(ulType);
	*((uint32_t*)(m_pSendBuffer + 4)) = htonl(nLen);
	if (nLen > 0)
	{
		memcpy((char*)m_pSendBuffer + 8, pData, nLen);
	}
	m_pSendFrameData = m_pSendBuffer;
	return true;
}

bool socket_frame::SendHeader(const char*pHeader, int32_t nHeaderLen)
{
	if (pHeader == NULL || nHeaderLen < 8)
	{
		return false;
	}

	m_nSendFrameDataLen = nHeaderLen;
	m_nSendBufferLength = 1024;
	if (m_nSendFrameDataLen > m_nSendBufferLength)
		m_nSendBufferLength = m_nSendFrameDataLen;

	m_pSendBuffer = (char*)malloc(m_nSendBufferLength);
	if (m_pSendBuffer == NULL)
	{
		return false;
	}

	memcpy(m_pSendBuffer, pHeader, nHeaderLen);
	m_pSendFrameData = m_pSendBuffer;

	return true;
}

void socket_frame::Reset(void)
{
	if (m_pSendBuffer)
	{
		free(m_pSendBuffer);
		m_pSendBuffer = NULL;
	}
	if (m_pReceivedBuffer)
	{
		free(m_pReceivedBuffer);
		m_pReceivedBuffer = NULL;
	}

	m_nSendBufferLength = (0);
	m_pSendFrameData = (NULL);
	m_nSendFrameDataLen = (0);
	m_pReceivedBuffer = (NULL);
	m_nReceivedBufferLength = (0);
	m_nReceivedDataLen = (0);
}

bool socket_frame::SendFrame(const char*pFrameData, int32_t nLen)
{
	//send left frame data
	while (m_nSendFrameDataLen > 0)
	{
		int32_t nBytes = SendData(m_pSendFrameData, m_nSendFrameDataLen);
		if (nBytes > 0)
		{
			m_nSendFrameDataLen -= nBytes;
			m_pSendFrameData += nBytes;
		}
		else
		{
			return false;//(nBytes==0);
		}
	}

	//send frame
	if (pFrameData != NULL && nLen > 0)
	{
		if (m_nSendBufferLength < 4 + nLen)
		{
			m_nSendBufferLength = 4 + nLen;
			m_pSendBuffer = (char*)realloc(m_pSendBuffer, m_nSendBufferLength);
			if (m_pSendBuffer == NULL)
			{
				m_nSendBufferLength = 0;
				return false;
			}
		}

		//init frame header
		*(uint32_t*)(m_pSendBuffer) = htonl(nLen);
		m_pSendFrameData = m_pSendBuffer;
		m_nSendFrameDataLen = 4 + nLen;
		memcpy(m_pSendBuffer + 4, pFrameData, nLen);

		//send left frame data
		while (m_nSendFrameDataLen > 0)
		{
			int32_t nBytes = SendData(m_pSendFrameData, m_nSendFrameDataLen);
			if (nBytes > 0)
			{
				m_nSendFrameDataLen -= nBytes;
				m_pSendFrameData += nBytes;
			}
			else
			{
				return false;//(nBytes==0);
			}
		}
	}

	return true;
}

bool socket_frame::SendNullFrame(void)
{
	//init frame header
	if (m_pSendBuffer == NULL)
	{
		m_nSendBufferLength = 1024;
		m_pSendBuffer = (char*)malloc(m_nSendBufferLength);
		if (m_pSendBuffer == NULL)
		{
			return false;
		}
	}

	*(uint32_t*)(m_pSendBuffer) = htonl(0);
	m_pSendFrameData = m_pSendBuffer;
	m_nSendFrameDataLen = 4;

	while (m_nSendFrameDataLen > 0)
	{
		int32_t nBytes = SendData(m_pSendFrameData, m_nSendFrameDataLen);
		if (nBytes > 0)
		{

			m_nSendFrameDataLen -= nBytes;
			m_pSendFrameData += nBytes;
		}
		else
		{
			return false;//(nBytes==0);
		}
	}

	return true;
}

bool socket_frame::HasSendFrameData(void)
{
	if ((m_nSendFrameDataLen > 0 && m_pSendFrameData != NULL))
	{
		return true;
	}

	return false;
}

void socket_frame::OnReceivedData(const char*pData, int32_t nLen)
{
	if (pData == NULL || nLen <= 0)
	{
		return;
	}
	//alloc buffer
	if (m_pReceivedBuffer == NULL)
	{
		m_nReceivedBufferLength = nLen * 2;
		if (m_nReceivedBufferLength < 1024)
			m_nReceivedBufferLength = 1024;

		m_pReceivedBuffer = (char*)malloc(m_nReceivedBufferLength);
		if (m_pReceivedBuffer == NULL)
		{
			return;
		}
	}

	//alloc more buffer
	if (m_nReceivedDataLen + nLen > m_nReceivedBufferLength)
	{
		m_nReceivedBufferLength = (m_nReceivedDataLen + nLen) * 2;
		char*pTempBuffer = (char*)malloc(m_nReceivedBufferLength);
		if (pTempBuffer == NULL)
		{
			free(m_pReceivedBuffer);
			m_pReceivedBuffer = NULL;
			return;
		}
		memcpy(pTempBuffer, m_pReceivedBuffer, m_nReceivedDataLen);
		free(m_pReceivedBuffer);
		m_pReceivedBuffer = pTempBuffer;
	}

	//save data
	memcpy(m_pReceivedBuffer + m_nReceivedDataLen, pData, nLen);
	m_nReceivedDataLen += nLen;

	//read header
	char*pFrameData = m_pReceivedBuffer;
	int32_t nReceivedDataLen = m_nReceivedDataLen;
	while (nReceivedDataLen >= 4)
	{
#if defined(NETEC_ARM) || defined(NETEC_ARM)
		uint32_t nTemp;
		memcpy(&nTemp, pFrameData, 4);
		int32_t nFrameDataLen = ntohl(nTemp);
#else
		int32_t nFrameDataLen = ntohl(*(uint32_t*)(pFrameData));
#endif

		if (nFrameDataLen < 0 || nFrameDataLen>100000000)
		{
			m_nReceivedDataLen = 0;
			return;
		}

		if (nFrameDataLen + 4 <= nReceivedDataLen)
		{
			//skip header
			pFrameData += 4;
			nReceivedDataLen -= 4;

			if (nFrameDataLen > 0)
			{
				OnReceivedFrame(pFrameData, nFrameDataLen);

				//skip this frame
				pFrameData += nFrameDataLen;
				nReceivedDataLen -= nFrameDataLen;
			}
		}
		else
		{
			break;
		}
	}

	//save left received data
	if (nReceivedDataLen > 0)
	{
		memmove(m_pReceivedBuffer, pFrameData, nReceivedDataLen);
		m_nReceivedDataLen = nReceivedDataLen;
	}
	else
	{
		m_nReceivedDataLen = 0;
		if (m_nReceivedBufferLength > 2000000)
		{
			free(m_pReceivedBuffer);
			m_pReceivedBuffer = NULL;

			m_nReceivedBufferLength = 0;
		}
	}
}