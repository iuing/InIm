#include "stdafx.h"
#include "DataPool.h"

DataPool::DataPool(int32_t nMargin /*= 0*/)
	: m_nMargin(nMargin)
{
	if (nMargin < 0)
	{
		m_nMargin = 0;
	}
}

DataPool::~DataPool()
{
	FlushPool();
}

void DataPool::PushBack(void*pData, int32_t nLen)
{
	if (pData == NULL || nLen <= 0)
	{
		return;
	}

	CAutoLock l(&m_lock);
	XDataBuffer* pDataBuf = new XDataBuffer;
	if (pDataBuf)
	{
		pDataBuf->m_buffer = malloc(nLen + m_nMargin);
		if (pDataBuf->m_buffer)
		{
			memcpy(pDataBuf->m_buffer, pData, nLen);
			pDataBuf->m_len = nLen;
			m_LstDataBuffer.push_back(pDataBuf);
			return;
		}
		pDataBuf->Release();
		pDataBuf = NULL;
	}
	return;
}

void DataPool::LockPool(void)
{
	m_lock.lock();
}

void DataPool::UnlockPool(void)
{
	m_lock.unlock();
}

bool DataPool::get_front(void**ppData, int32_t&nLen)
{
	if (m_LstDataBuffer.size() > 0)
	{
		XDataBuffer* pBuffer = (XDataBuffer*)m_LstDataBuffer.front();
		*ppData = pBuffer->m_buffer;
		nLen = pBuffer->m_len;
		return true;
	}
	return false;
}

bool DataPool::get_back(void**ppData, int32_t&nLen)
{
	if (m_LstDataBuffer.size())
	{
		XDataBuffer*pXDataBuffer = (XDataBuffer*)m_LstDataBuffer.back();
		*ppData = pXDataBuffer->m_buffer;
		nLen = pXDataBuffer->m_len;
		return true;
	}
	return false;
}

void DataPool::PopFront(void)
{
	CAutoLock l(&m_lock);
	if (m_LstDataBuffer.size())
	{
		XDataBuffer*pXDataBuffer = (XDataBuffer*)m_LstDataBuffer.front();
		m_LstDataBuffer.pop_front();
		pXDataBuffer->Release();
		pXDataBuffer = NULL;
	}
}

int32_t DataPool::GetDataCount(void)
{
	CAutoLock l(&m_lock);
	return (m_LstDataBuffer.size());
}

void DataPool::FlushPool(void)
{
	CAutoLock l(&m_lock);
	while (m_LstDataBuffer.size() > 0)
	{
		XDataBuffer*pXDataBuffer = (XDataBuffer*)m_LstDataBuffer.front();
		m_LstDataBuffer.pop_front();
		pXDataBuffer->Release();
		pXDataBuffer = NULL;
	}
}

DataPool::XDataBuffer* DataPool::GetDataBuffer(void)
{
	CAutoLock l(&m_lock);
	if (m_LstDataBuffer.size())
	{
		XDataBuffer*pXDataBuffer = (XDataBuffer*)m_LstDataBuffer.front();
		m_LstDataBuffer.pop_front();
		return pXDataBuffer;
	}
	return NULL;
}
