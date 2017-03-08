#include <iostream>

#include "../Suite/UnitTestSuite.hpp"
#include "ActionManagerTimerTest.hpp"
#include "ChronometerTest.hpp"
#include "LoggerTest.hpp"
#include "ThreadTest.hpp"

using namespace std;

int main() {
	cout << "!!!Common-SIMU-Test!!!" << endl;

	UnitTestSuite suite;

	//suite.addTest(new test::LoggerTest()); //utilise ici uniquement les logs avec Memory Appender
	//suite.addTest(new test::ThreadTest());
	//suite.addTest(new test::ChronometerTest());
	suite.addTest(new test::ActionManagerTimerTest()); //utilise les chronos et les threads


	suite.run();

	return 0;
}
