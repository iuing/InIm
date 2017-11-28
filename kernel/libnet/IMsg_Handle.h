#pragma  once
#include "utils.h"
#include "stdafx.h"

class IMsg_Handle
{
public:
	virtual void RegisterHandle(void);
	virtual void UnRegisterHandle(void);

	virtual void HandleMsg(unsigned int ulMsgID) = 0;

	virtual void PostMsg(unsigned int ulMsgID, bool bOnce = true, unsigned int ulDelay = 0);
	virtual void ClearMsg(void);
	virtual void RemoveMsg(unsigned int ulMsgID);
	virtual bool CheckMsg(void);
protected:
	struct XMsg
	{
		unsigned int  m_msgId;
		bool m_bOnce;
		unsigned int m_nDelay;
		unsigned int m_nTimeStamp;

		XMsg(unsigned int msgId, bool bOnce, unsigned int nDelay)
			: m_msgId(msgId)
			, m_bOnce(bOnce)
			, m_nDelay(nDelay)
			, m_nTimeStamp(XGetTimestamp()){}

		XMsg() :m_msgId(0), m_bOnce(true), m_nDelay(0), m_nTimeStamp(XGetTimestamp()){}

		XMsg operator=(const XMsg& msg)
		{
			m_msgId = msg.m_msgId;
			m_bOnce = msg.m_bOnce;
			m_nDelay = msg.m_nDelay;
			m_nTimeStamp = msg.m_nTimeStamp;
			return *this;
		}
	};

	typedef std::map<unsigned int, XMsg> MapMsgs;
protected:
	CLock			m_lock;
	MapMsgs			m_MapMsgs;
};



class IMsg_Queue
{
public:
	static void AddMsgHandle(IMsg_Handle*pIXMsgHandle);
	static void RemoveMsgHandle(IMsg_Handle*pIXMsgHandle);

	static void Start();
	static void Stop();
};