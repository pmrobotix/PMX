#include <iostream>

#include "../Suite/DevUnitTestSuite.hpp"
#include "LedDriverTest.hpp"

using namespace std;

int main()
{
	cout << "!!!Driver-SIMU-Test!!!" << endl;

	DevUnitTestSuite suite;

	suite.addTest(new test::LedDriverTest());

	suite.run();

	return 0;
}
