#ifndef MONITOR_HPP_INC
#define MONITOR_HPP_INC

#include <memory>
#include <mutex>
#include <thread>

template<class T, class M>
class LockedObject
{
  public:
    LockedObject(T * subject, M & mutex)
      : m_subject(subject)
      , m_mutex(mutex)
    {
      m_mutex.lock();
    }

    ~LockedObject()
    {
      m_mutex.unlock();
    }

    T* operator->()
    {
      return m_subject;
    }
  private:
    T * m_subject;
    M & m_mutex;
};

template<class T, class M = std::recursive_mutex>
class Monitor
{
  public:
    Monitor(std::unique_ptr<T> & subject)
      : m_subjectPtr(subject)
    {}

    ~Monitor()
    {}

    LockedObject<T,M> operator->()
    {
      return LockedObject<T,M>(m_subjectPtr.get(), m_mutex);
    }
  private:
    std::unique_ptr<T> m_subjectPtr;
    M m_mutex;
};

#endif
