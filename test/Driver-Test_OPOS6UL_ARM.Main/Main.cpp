#include <iostream>
#include "../Suite/UnitTestSuite.hpp"
#include "AsservDriverTest.hpp"
#include "LcdShieldDriverTest.hpp"

using namespace std;

int main()
{
	cout << "!!!Hello TEST World!!!" << endl; // prints !!!Hello World!!!

	//Assert/fail
	UnitTestSuite suite;

	suite.addTest(new test::LcdShieldDriverTest());
	suite.addTest(new test::AsservDriverTest());

	suite.run();

	return 0;
}
