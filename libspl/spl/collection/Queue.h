#ifndef _queue_h
#define _queue_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/Memory.h>
#include <spl/threading/Event.h>
#include <spl/threading/Mutex.h>
#include <spl/collection/List.h>

namespace spl
{
	/** @brief A reader / writer pipe-like class.
	 */
	template<class T>
	class Queue : public IMemoryValidate
	{
	protected:
		List<T> m_q;
		Mutex m_lock;
		Event m_event;

		inline T _Get()
		{
			T t = m_q.Tail();
			m_q.RemoveTail();
			return t;
		}

	public:
		inline Queue()
		{
		}

		virtual ~Queue()
		{
		}

		void Put(T t)
		{
			m_lock.Lock();
			m_q.Add(t);
			m_lock.Unlock();
			m_event.Notify();
		}

		T Get()
		{
			while(true)
			{
				m_lock.Lock();
				if (m_q.Count() > 0)
				{
					T t = _Get();
					m_lock.Unlock();
					return t;
				}
				m_lock.Unlock();
				m_event.Wait();
			}
		}

	#ifdef DEBUG
		virtual void ValidateMem() const
		{
			m_q.ValidateMem();
		}

		virtual void CheckMem() const
		{
			m_q.CheckMem();
		}
	#endif
	};
}
#endif
