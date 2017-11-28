#pragma once

#define NETIOEVENT_READ		0x00000001
#define NETIOEVENT_WRITE		0x00000002

class netio_event
{
public:
	netio_event();
	virtual ~netio_event();
public:
	virtual void AddEvent(void);
	virtual void RemoveEvent(void);
	virtual void ModifyEvent(void);

	virtual void EnableRead(void);
	virtual void DisableRead(void);
	virtual void EnableWrite(void);
	virtual void DisableWrite(void);

	virtual int32_t GetEvents(void);
	virtual operator SOCKET(void) = 0;

	virtual void OnRead(void) = 0;
	virtual void OnWrite(void) = 0;
	virtual void OnError(void) = 0;
protected:
	int32_t		m_events;
};

class INetio_Queue
{
public:
	INetio_Queue(){};
	virtual ~INetio_Queue(){};
public:
	static void Start();
	static void Stop();

	static void AddEvent(netio_event* pNetIOEvent);
	static void RemoveEvent(netio_event* pNetIOEvent);
	static void ModifyEvent(netio_event* pNetIOEvent);

};