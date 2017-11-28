#include "stdafx.h"
#include "msg_queue.h"
#include "IMsg_Handle.h"
#include "IEventsQueue.h"

static msg_queue	g_MsgQueue;
static bool			g_MsgQueueStarted = false;


void IMsg_Handle::RegisterHandle(void)
{
	IMsg_Queue::AddMsgHandle(this);
}

void IMsg_Handle::UnRegisterHandle(void)
{
	IMsg_Queue::RemoveMsgHandle(this);
}

void IMsg_Handle::PostMsg(unsigned int ulMsgID, bool bOnce /*= true*/, unsigned int ulDelay /*= 0*/)
{
	if (ulDelay < 1)
	{
		ulDelay = 1;
	}

	CAutoLock l(&m_lock);
	m_MapMsgs[ulMsgID] = XMsg(ulMsgID, bOnce, ulDelay);
}

void IMsg_Handle::ClearMsg(void)
{
	CAutoLock l(&m_lock);
	m_MapMsgs.clear();
}

void IMsg_Handle::RemoveMsg(unsigned int ulMsgID)
{
	CAutoLock l(&m_lock);
	m_MapMsgs.erase(ulMsgID);
}

bool IMsg_Handle::CheckMsg(void)
{
	bool bRet = false;
	unsigned int ulMsgID = 0;
	{
		CAutoLock l(&m_lock);
		uint32_t ulTimestamp = XGetTimestamp();
		MapMsgs::iterator iter = m_MapMsgs.begin();
		while (iter != m_MapMsgs.end())
		{
			if (ulTimestamp - iter->second.m_nTimeStamp >= iter->second.m_nDelay)
			{
				ulMsgID = iter->second.m_msgId;

				if (iter->second.m_bOnce)
				{
					m_MapMsgs.erase(iter++);
				}
				else
				{
					iter->second.m_nTimeStamp = ulTimestamp;
				}
				bRet = true;
				HandleMsg(ulMsgID);
			}
			else
			{
				++iter;
			}
		}
	}

	return bRet;
}

void IMsg_Queue::AddMsgHandle(IMsg_Handle*pIMsgHandle)
{
	g_MsgQueue.AddMsgHandle(pIMsgHandle);
}

void IMsg_Queue::RemoveMsgHandle(IMsg_Handle*pIMsgHandle)
{
	g_MsgQueue.RemoveMsgHandle(pIMsgHandle);
}

void IMsg_Queue::Start()
{
	if (!g_MsgQueueStarted)
	{
		g_MsgQueueStarted = true;
		IEventsPump::AddIEventsQueue(&g_MsgQueue);
	}
}

void IMsg_Queue::Stop()
{
	if (g_MsgQueueStarted)
	{
		g_MsgQueueStarted = false;
		IEventsPump::RemoveIEventsQueue(&g_MsgQueue);
	}
}

msg_queue::msg_queue()
{
}


msg_queue::~msg_queue()
{
}

void msg_queue::AddMsgHandle(IMsg_Handle*pIXMsgHandle)
{
	CAutoLock lock(&m_lock);
	m_mapMsgHandles[pIXMsgHandle] = pIXMsgHandle;
}

void msg_queue::RemoveMsgHandle(IMsg_Handle*pIXMsgHandle)
{
	CAutoLock lock(&m_lock);
	m_mapMsgHandles.erase(pIXMsgHandle);
}

bool msg_queue::PumpEvents()
{
	bool bRet = false;
	CAutoLock lock(&m_lock);
	for (MapMsgHandle::iterator itor = m_mapMsgHandles.begin(); itor != m_mapMsgHandles.end();itor++)
	{
		bRet |= itor->second->CheckMsg();
	}

	return bRet;
}
