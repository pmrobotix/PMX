//drivers...EV3

#include "ButtonDriver.hpp"
#include "ev3dev.h"

using namespace std;
using namespace ev3dev;

AButtonDriver * AButtonDriver::create()
{
	static ButtonDriver *instance = new ButtonDriver();
	return instance;
}

ButtonDriver::ButtonDriver()
{

	//TODO test process and onclick funtions
}

ButtonDriver::~ButtonDriver()
{
}

bool ButtonDriver::pressed(ButtonTouch button)
{
	switch (button)
	{
	case BUTTON_ENTER_KEY:
		return button::enter.pressed();
		break;
	case BUTTON_BACK_KEY:
		return button::back.pressed();
		break;
	case BUTTON_UP_KEY:
		return button::up.pressed();
		break;
	case BUTTON_DOWN_KEY:
		return button::down.pressed();
		break;
	case BUTTON_LEFT_KEY:
		return button::left.pressed();
		break;
	case BUTTON_RIGHT_KEY:
		return button::right.pressed();
		break;
	}
	return 0;
}
/*
bool ButtonDriver::process(ButtonTouch button)
{
	switch (button)
	{
	case BUTTON_ENTER_KEY:
		return button::enter.process();
		break;
	case BUTTON_BACK_KEY:
		return button::back.process();
		break;
	case BUTTON_UP_KEY:
		return button::up.process();
		break;
	case BUTTON_DOWN_KEY:
		return button::down.process();
		break;
	case BUTTON_LEFT_KEY:
		return button::left.process();
		break;
	case BUTTON_RIGHT_KEY:
		return button::right.process();
		break;
	}
	return 0;
}*/
