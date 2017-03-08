#include <iostream>

#include "../Suite/UnitTestSuite.hpp"
#include "LcdShieldDriverTest.hpp"


using namespace std;

int main()
{
	cout << "!!!Hello TEST World!!!" << endl; // prints !!!Hello World!!!

	//Assert/fail
	UnitTestSuite suite;
	suite.addTest(new test::LcdShieldDriverTest());

	suite.run();

	return 0;
}
