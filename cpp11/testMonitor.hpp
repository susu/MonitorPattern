#include <cxxtest/TestSuite.h>

#include <thread>

#include <unistd.h>

#include "Monitor.hpp"

class Subject
{
  public:
    Subject()
      : methodIsRunning(false)
      , doubleEntrance(false)
    {}
    void method()
    {
      enter();
      sleep(1);
      leave();
    }
    bool hasDoubleEntrance() const
    {
      return doubleEntrance;
    }
  private:
    void enter()
    {
      if (methodIsRunning)
      {
        doubleEntrance = true;
      }
      methodIsRunning = true;
    }
    void leave()
    {
      methodIsRunning = false;
    }

    bool methodIsRunning;
    bool doubleEntrance;
};

class TestMonitor : public CxxTest::TestSuite
{
  public:
    void test_ctor()
    {
      monitor<Subject> m(std::unique_ptr<Subject>(new Subject));
    }

    void test_make_monitor()
    {
      auto m = make_monitor(new Subject);
    }

    void test_subject()
    {
      Subject s;
      std::function<void()> runnerFunction = [&s]()
      {
        s.method();
      };
      std::thread t1(runnerFunction);
      std::thread t2(runnerFunction);
      if (t1.joinable()) t1.join();
      if (t2.joinable()) t2.join();
      TS_ASSERT( s.hasDoubleEntrance() );
    }

    void test_monitor_single_entrance()
    {
      auto m = make_monitor(new Subject);
      std::function<void()> runnerFunction = [&m]()
      {
        m->method();
      };
      std::thread t1(runnerFunction);
      std::thread t2(runnerFunction);
      if (t1.joinable()) t1.join();
      if (t2.joinable()) t2.join();
      TS_ASSERT( !m->hasDoubleEntrance() );
    }
};
