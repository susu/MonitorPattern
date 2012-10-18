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
    locked_object(locked_object&&) = delete;
    T* operator->();
  private:
    T * m_subject;
    M & m_mutex;
};

template<class T, class M = std::recursive_mutex>
class monitor
{
  public:
    monitor() = delete;
    monitor(monitor&) = delete;
    monitor(monitor&&) = default;
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
  //return 
}

template<class T,class M>
monitor<T,M>::monitor(std::unique_ptr<T> subj)
  : m_subject(std::move(subj))
{}

// template<class T,class M>

#endif
