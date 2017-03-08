#include <iostream>

#include "../Suite/UnitTestSuite.hpp"
#include "AsservDriverTest.hpp"
#include "LedDriverTest.hpp"
#include "SoundDriverTest.hpp"

using namespace std;

int main()
{
	cout << "!!!Hello TEST World!!!" << endl; // prints !!!Hello World!!!

	UnitTestSuite suite;
	suite.addTest(new test::AsservDriverTest());
	//suite.addTest(new test::LedDriverTest());
	//suite.addTest(new test::SoundDriverTest());

	suite.run();


	//Assert/fail
	return 0;
}
