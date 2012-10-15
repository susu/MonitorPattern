#include <iostream>

#include <unistd.h>

#include "Monitor.hpp"

class Subject
{
  public:
    Subject()
      : m_threadInMethod(false)
      , m_twoThreadEntered(false)
    {}

    void method()
    {
      enter();
      sleep(1);
      leave();
    }

    bool hasTwoThreadEntered() const
    {
      return m_twoThreadEntered;
    }
  private:

    void enter()
    {
      if (m_threadInMethod)
      {
        m_twoThreadEntered = true;
      }
      m_threadInMethod = true;
    }

    void leave()
    {
      m_threadInMethod = true;
    }

    bool m_threadInMethod;
    bool m_twoThreadEntered;
};


int main()
{
  std::cout << "Create subject" << std::endl;
  //std::unique_ptr<Subject> subj(new Subject);
  Monitor<Subject> subj( std::move( std::unique_ptr<Subject>(new Subject) ) );

  std::cout << "start threads" << std::endl;
  
  std::function<void()> runnerFunc = [&subj]()
  {
    subj->method();
  };

  std::thread t1( runnerFunc );
  std::thread t2( runnerFunc );

  std::cout << "Wait threads to finish" << std::endl;
  t1.join();
  t2.join();

  std::cout << "\n\nRESULT:\n";
  if (subj->hasTwoThreadEntered())
  {
    std::cout << "OH-OH, CONCURRENT ACCESSS!!!!" << std::endl;
  }
  else
  {
    std::cout << "Nice, single thread a time!" << std::endl;
  }

}
