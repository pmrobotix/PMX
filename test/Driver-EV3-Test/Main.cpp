#include <iostream>
#include <unistd.h>
#include "../../src/Driver.EV3/LedDriver.hpp"
#include "../../src/Driver.EV3/ev3dev.h"
using namespace std;
using namespace ev3dev;

int main()
{
	cout << "!!!Hello TEST World!!!" << endl; // prints !!!Hello World!!!



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
