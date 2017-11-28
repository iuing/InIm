#pragma once

class NET_EXPORT XListPtr
{
public:
	XListPtr();
	virtual ~XListPtr();

public:
	class NET_EXPORT iterator
	{
	public:
		iterator(void* pMenber = NULL)
		{
			m_pMember = pMenber;
			m_pNext = this;
			m_pPrev = this;
		}
		virtual ~iterator(){

		}

		void operator ++()
		{
			*this = *m_pNext;
		}
		void operator --(){
			*this = *m_pPrev;
		}
		void* operator *(){
			return m_pMember;
		}
		iterator &operator =(iterator itor){
			m_pMember = itor.m_pMember;
			m_pNext = itor.m_pNext;
			m_pPrev = itor.m_pPrev;
			return (*this);
		}
		bool operator ==(iterator titerator)
		{
			return (m_pMember == titerator.m_pMember &&
				m_pNext == titerator.m_pNext &&
				m_pPrev == titerator.m_pPrev);
		}
		bool operator !=(iterator titerator)
		{
			return (m_pMember != titerator.m_pMember ||
				m_pNext != titerator.m_pNext ||
				m_pPrev != titerator.m_pPrev);
		}
	protected:
		void*	m_pMember;
		iterator* m_pNext;
		iterator* m_pPrev;

		friend class XListPtr;
	};
public:
	XListPtr::iterator begin();
	XListPtr::iterator end(void);
	void*front(void);
	void pop_front(void);
	void push_front(void*ptr);
	void*back(void);
	void pop_back(void);
	void push_back(void*ptr);
	void insert(XListPtr::iterator titerator, void*ptr);
	void erase(XListPtr::iterator titerator);
	uint32_t size(void);
	void clear(void);
	void remove(void*pPtr);
	XListPtr::iterator find(void*pPtr);
protected:
	iterator		m_iterator;
	uint32_t		m_size;
};

