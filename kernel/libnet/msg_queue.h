#pragma once
#include "IEventsQueue.h"
#include "Lock.h"

class IMsg_Handle;
typedef std::map<void*, IMsg_Handle*> MapMsgHandle;

class msg_queue
	: public IEventsQueue
{
public:
	msg_queue();
	virtual ~msg_queue();
public:
	void AddMsgHandle(IMsg_Handle*pIXMsgHandle);
	void RemoveMsgHandle(IMsg_Handle*pIXMsgHandle);

	virtual bool PumpEvents();
protected:
	MapMsgHandle	m_mapMsgHandles;
	CLock			m_lock;
};

