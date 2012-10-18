#include <cxxtest/TestSuite.h>

#include "Monitor.hpp"

struct Subject
{
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
};
