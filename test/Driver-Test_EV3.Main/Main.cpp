#include <iostream>

#include "../Suite/UnitTestSuite.hpp"
#include "AsservDriverTest.hpp"
#include "SoundDriverTest.hpp"

#include "LcdDriverTest.hpp"
#include "LedDriverTest.hpp"
#include "SensorDriverTest.hpp"
#include "ServoDriverTest.hpp"

using namespace std;

int main()
{
	cout << "!!!Hello TEST World!!!" << endl; // prints !!!Hello World!!!

	UnitTestSuite suite;
	//suite.addTest(new test::LedDriverTest());
	//suite.addTest(new test::LcdDriverTest());
	//suite.addTest(new test::SensorDriverTest());
	//suite.addTest(new test::AsservDriverTest());


	//suite.addTest(new test::SoundDriverTest());
	suite.addTest(new test::ServoDriverTest());
	suite.run();


	//Assert/fail
	return 0;
}
