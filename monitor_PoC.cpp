#include <iostream>
#include <boost/thread/recursive_mutex.hpp>

template<class T, class Mutex>
class TempLock
{
  public:
    TempLock(T* subj, Mutex & m) : m_subject(subj), m_mutex(m)
    {
      std::cout << "LOCK" << typeid(m_mutex).name() << std::endl;
      m_mutex.lock();
    }
    ~TempLock()
    {
      std::cout << "And unlocking here" << std::endl;
      m_mutex.unlock();
    }
    T* operator->()
    {
      std::cout << "TempLock" << std::endl;
      return m_subject;
    }
  private:
    T* m_subject;
    Mutex & m_mutex;
};

template<class T, class Mutex = boost::recursive_mutex>
class Monitor
{
  public:
    Monitor( T* subject ) : m_subject(subject) {}
    TempLock<T,Mutex> operator->()
    {
      std::cout << __PRETTY_FUNCTION__ << std::endl;
      return TempLock<T, Mutex>(m_subject, m_mutex);
    }
  private:
    T* m_subject;
    Mutex m_mutex;
};

struct Subject
{
  void f()
  {
    std::cout << "Subject::f() -- CRITICAL PLACE HERE!" << a << std::endl;
  }
  int get() { return a; }
  int a;
};

int main()
{
  Subject s; s.a = 12;
  Monitor<Subject> m(&s);

  std::cout << m->get() + m->get() << std::endl;
}
