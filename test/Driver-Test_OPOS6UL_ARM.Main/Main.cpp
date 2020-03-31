#include <iostream>

#include "../Suite/UnitTestSuite.hpp"
#include "AsservDriverTest.hpp"
#include "ColorDriverTest.hpp"
#include "LcdShieldDriverTest.hpp"
#include "LedDriverTest.hpp"
#include "SwitchDriverTest.hpp"
#include "SensorDriverTest.hpp"
#include "ServoDriverTest.hpp"

using namespace std;

int main()
{
    utils::set_realtime_priority(); //priority MAX 99

    cout << "!!!Hello TEST World!!!" << endl; // prints !!!Hello World!!!

    //Assert/fail
    UnitTestSuite suite;

    //suite.addTest(new test::ColorDriverTest());
    //suite.addTest(new test::LcdShieldDriverTest());
    //suite.addTest(new test::LedDriverTest());
    suite.addTest(new test::AsservDriverTest());
    //suite.addTest(new test::SwitchDriverTest());
    //suite.addTest(new test::SensorDriverTest());
    //suite.addTest(new test::ServoDriverTest());

    suite.run();

    return 0;
}
