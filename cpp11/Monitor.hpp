#ifndef CPP11_MONITOR_HPP_INC
#define CPP11_MONITOR_HPP_INC

#include <mutex>
#include <memory>

template<class T, class M>
class locked_object
{
  public:
    locked_object(T*,M&);
    locked_object(locked_object&) = delete;
    locked_object(locked_object&&);
    ~locked_object();
    T* operator->();
  private:
    T * m_subject;
    M & m_mutex;
};

template<class T, class M = std::recursive_mutex>
class monitor
{
  public:
    monitor(monitor&&);
    monitor(std::unique_ptr<T> subj);
    locked_object<T,M> operator->();
  private:
    std::unique_ptr<T> m_subject;
    M m_mutex;
};

template<class T>
monitor<T> make_monitor(T * subjPtr)
{
  return monitor<T>(std::unique_ptr<T>(subjPtr));
}

//
// monitor
//
template<class T,class M>
monitor<T,M>::monitor(std::unique_ptr<T> subj)
  : m_subject(std::move(subj))
{}

template<class T,class M>
locked_object<T,M> monitor<T,M>::operator->()
{
  return locked_object<T,M>(m_subject.get(), m_mutex);
}

//
// locked_object
//
template<class T,class M>
locked_object<T,M>::locked_object(T* subj, M & mutex)
  : m_subject(subj)
  , m_mutex(mutex)
{
  m_mutex.lock();
}

template<class T,class M>
locked_object<T,M>::~locked_object()
{
  m_mutex.unlock();
}

template<class T,class M>
T* locked_object<T,M>::operator->()
{
  return m_subject;
}

#endif
