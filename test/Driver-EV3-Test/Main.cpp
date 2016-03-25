#include <iostream>

#include "../Suite/UnitTestSuite.hpp"
#include "LedDriverTest.hpp"
#include "SoundDriverTest.hpp"

using namespace std;

int main()
{
	cout << "!!!Hello TEST World!!!" << endl; // prints !!!Hello World!!!

	UnitTestSuite suite;

	suite.addTest(new test::LedDriverTest());
	suite.addTest(new test::SoundDriverTest());

	suite.run();

	/*
	 //TEST BUTTON
	 bool up = false, down = false, left = false, right = false, enter = false,
	 escape = false;

	 while (escape == 0)
	 {
	 up = button::up.pressed ();
	 down = button::down.pressed ();
	 left = button::left.pressed ();
	 right = button::right.pressed ();
	 enter = button::enter.pressed ();
	 escape = button::back.pressed ();

	 printf ("up:%d down:%d left:%d right:%d enter:%d esc:%d\n", up, down,
	 left, right, enter, escape);
	 usleep (100000);
	 }
	 */

	//Assert/fail
	return 0;
}
