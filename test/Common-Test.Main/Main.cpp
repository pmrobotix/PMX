#include <chrono>
#include <iostream>
#include <thread>

#include "../../src/Thread/Thread.hpp"
#include "../Suite/UnitTestSuite.hpp"
#include "ActionManagerTimerTest.hpp"
#include "TimerFactoryTest.hpp"

using namespace std;

int main()
{
    cout << "!!!Common-Test!!!" << endl;

    utils::set_realtime_priority(3); //set priority MAX 99

    UnitTestSuite suite;

    //suite.addTest(new test::LoggerTest()); //utilise ici uniquement les logs avec Memory Appender
    //suite.addTest(new test::ThreadTest());
    //suite.addTest(new test::ChronometerTest());
    suite.addTest(new test::ActionManagerTimerTest()); //utilise les chronos et les threads
    //suite.addTest(new test::ReadWriteTest());
    //suite.addTest(new test::TimerFactoryTest());

    //this_thread::sleep_for(2000ms);

    suite.run();
//    this_thread::sleep_for(1s);
//    cout << "!!! End main()" << endl;
    return 0;
}
