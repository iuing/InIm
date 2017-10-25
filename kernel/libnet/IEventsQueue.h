#pragma once

class IEventsQueue
{
public:
	virtual bool PumpEvents() = 0;
};

class IEventsPump
{
public:
	IEventsPump(){};
	virtual ~IEventsPump(){};

	static void Start();
	static void Stop();

	static void AddIEventsQueue(IEventsQueue* pEventQueue);
	static void RemoveIEventsQueue(IEventsQueue* pEventQueue);
	static bool PumpEvents();
};




