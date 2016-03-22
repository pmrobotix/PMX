#include <iostream>

#include "../Suite/DevUnitTestSuite.hpp"
#include "ActionManagerTest.hpp"
#include "ChronometerTest.hpp"
#include "LoggerTest.hpp"

using namespace std;

int main()
{
	cout << "!!!Common-SIMU-Test!!!" << endl;

	DevUnitTestSuite suite;

	suite.addTest(new test::LoggerTest());
	suite.addTest(new test::ChronometerTest());
	suite.addTest(new test::ActionManagerTest());

	suite.run();

	return 0;
}
