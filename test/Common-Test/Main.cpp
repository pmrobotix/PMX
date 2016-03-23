#include <iostream>

#include "../Suite/UnitTestSuite.hpp"
#include "ActionManagerTest.hpp"
#include "ChronometerTest.hpp"
#include "LoggerTest.hpp"
#include "TimerTest.hpp"

using namespace std;

int main()
{
	cout << "!!!Common-SIMU-Test!!!" << endl;

	UnitTestSuite suite;

	//suite.addTest(new test::LoggerTest());
	//suite.addTest(new test::ChronometerTest());
	suite.addTest(new test::ActionManagerTest());
	suite.addTest(new test::TimerTest());
	suite.run();

	return 0;
}
