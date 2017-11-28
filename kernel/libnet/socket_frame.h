#pragma once

class socket_frame
{
public:
	socket_frame();
	virtual ~socket_frame();
public:
	virtual bool SendHeader(uint32_t ulType, const void*pData, int32_t nLen);
	virtual bool SendHeader(const char*pHeader, int32_t nHeaderLen);
	virtual void Reset(void);
	virtual bool SendFrame(const char*pFrameData, int32_t nLen);

	virtual bool SendNullFrame(void);

	virtual bool HasSendFrameData(void);
	virtual void OnReceivedData(const char*pData, int32_t nLen);
	virtual int32_t SendData(const char*pData, int32_t nLen) = 0;
	virtual void OnReceivedFrame(const char*pFrameData, int32_t nLen) = 0;
protected:
	char*		m_pSendBuffer;
	int32_t			m_nSendBufferLength;
	char*		m_pSendFrameData;
	int32_t			m_nSendFrameDataLen;

	char*		m_pReceivedBuffer;
	int32_t			m_nReceivedBufferLength;
	int32_t			m_nReceivedDataLen;
};

