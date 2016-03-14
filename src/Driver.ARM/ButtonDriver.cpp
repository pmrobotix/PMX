//drivers...SIMU

#include "ButtonDriver.hpp"

using namespace std;

AButtonDriver * AButtonDriver::create()
{
	return new ButtonDriver();
}

ButtonDriver::ButtonDriver()
{

}

ButtonDriver::~ButtonDriver()
{
}

bool ButtonDriver::pressed(ButtonTouch button)
{
	switch (button)
	{
	case BUTTON_ENTER_KEY:
		break;
	case BUTTON_BACK_KEY:
			break;
	case BUTTON_UP_KEY:
			break;
	case BUTTON_DOWN_KEY:
			break;
	case BUTTON_LEFT_KEY:
			break;
	case BUTTON_RIGHT_KEY:
			break;
	}

	return 0;
}
