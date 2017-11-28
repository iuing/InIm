#include "stdafx.h"
#include "netio_event.h"

netio_event::netio_event()
{
	m_events = 0;
}


netio_event::~netio_event()
{
}

void netio_event::AddEvent(void)
{
	INetio_Queue::AddEvent(this);
}

void netio_event::RemoveEvent(void)
{
	INetio_Queue::RemoveEvent(this);
}

void netio_event::ModifyEvent(void)
{
	INetio_Queue::ModifyEvent(this);
}

void netio_event::EnableRead(void)
{
	if ((m_events & NETIOEVENT_READ) != NETIOEVENT_READ)
	{
		m_events |= NETIOEVENT_READ;
		ModifyEvent();
	}
}

void netio_event::DisableRead(void)
{
	if ((m_events & NETIOEVENT_READ) == NETIOEVENT_READ)
	{
		m_events &= ~NETIOEVENT_READ;
		ModifyEvent();
	}
}

void netio_event::EnableWrite(void)
{
	if ((m_events & NETIOEVENT_WRITE) != NETIOEVENT_WRITE)
	{
		m_events |= NETIOEVENT_WRITE;
		ModifyEvent();
	}
}

void netio_event::DisableWrite(void)
{
	if ((m_events & NETIOEVENT_WRITE) == NETIOEVENT_WRITE)
	{
		m_events &= ~NETIOEVENT_WRITE;
		ModifyEvent();
	}
}

int32_t netio_event::GetEvents(void)
{
	return m_events;
}

