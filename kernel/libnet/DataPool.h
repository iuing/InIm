#pragma once
#include "XListPtr.h"
class DataPool
{
public:
	DataPool(int32_t nMargin = 0);
	~DataPool();
public:
	virtual void PushBack(void*pData, int32_t nLen);
	virtual void LockPool(void);
	virtual void UnlockPool(void);
	virtual bool get_front(void**ppData, int32_t&nLen);
	virtual bool get_back(void**ppData, int32_t&nLen);
	virtual void PopFront(void);
	virtual int32_t GetDataCount(void);
	virtual void FlushPool(void);
	
public:
	class XDataBuffer
	{
	public:
		XDataBuffer()
			: m_buffer(NULL)
			, m_len(0)
		{

		}
		~XDataBuffer()
		{
			if (m_buffer)
			{
				free(m_buffer);
				m_buffer = NULL;
			}
		}

		void Release(void)
		{
			if (m_buffer)
			{
				free(m_buffer);
				m_buffer = NULL;
			}
			delete this;
		}

		void*  m_buffer;
		int32_t	m_len;
	};

	virtual DataPool::XDataBuffer*GetDataBuffer(void);
protected:
	XListPtr						m_LstDataBuffer;
	CLock							m_lock;
	int32_t							m_nMargin;
};

