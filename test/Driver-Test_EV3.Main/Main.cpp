#include <unistd.h>
#include <iostream>

#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTestSuite.hpp"
#include "AsservDriverTest.hpp"

using namespace std;

int main()
{
    utils::set_realtime_priority(); //priority MAX 99

    cout << "!!!Hello TEST World!!!" << endl; // prints !!!Hello World!!!

    UnitTestSuite suite;
    //suite.addTest(new test::LedDriverTest());
    //suite.addTest(new test::LcdDriverTest());
    //suite.addTest(new test::SensorDriverTest());
    suite.addTest(new test::AsservDriverTest());
    //suite.addTest(new test::SoundDriverTest());
    //suite.addTest(new test::ServoDriverTest());
    //sleep(2);
    suite.run();

    //Assert/fail
    return 0;
}
