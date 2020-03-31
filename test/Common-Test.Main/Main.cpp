#include <iostream>

#include "../../src/Thread/Thread.hpp"
#include "../Suite/UnitTestSuite.hpp"
#include "ReadWriteTest.hpp"

using namespace std;

int main()
{
    cout << "!!!Common-SIMU-Test!!!" << endl;

    utils::set_realtime_priority(); //set priority MAX 99


    UnitTestSuite suite;

    //suite.addTest(new test::LoggerTest()); //utilise ici uniquement les logs avec Memory Appender
    //suite.addTest(new test::ThreadTest());
    //suite.addTest(new test::ChronometerTest());
    //suite.addTest(new test::ActionManagerTimerTest()); //utilise les chronos et les threads
    suite.addTest(new test::ReadWriteTest());

    suite.run();

    return 0;
}
