//drivers...SIMU

#include "ButtonDriver.hpp"

#include <cstdio>

#include "../Common/ConsoleKeyInput.hpp"

using namespace std;

AButtonDriver * AButtonDriver::create()
{
	return new ButtonDriver();
}

ButtonDriver::ButtonDriver()
{
	lindex = 0;
	cInput = 0;
}

ButtonDriver::~ButtonDriver()
{
}

bool ButtonDriver::pressed(ButtonTouch button)
{
	cInput = ConsoleKeyInput::mygetch();
	logger().debug() << "key=" << cInput << logs::end;
	switch (button)
	{
	case BUTTON_ENTER_KEY:
		if (cInput == 10)
		{
			logger().info() << "Enter key!" << logs::end;
			return true;
		}
		break;
	case BUTTON_BACK_KEY:
		if (cInput == 127)
		{
			logger().info() << "BACK key!" << logs::end;
			return true;
		}
		break;
	case BUTTON_UP_KEY:
		if (cInput == 65)
		{
			logger().info() << "UP arrow key!" << logs::end;
			return true;
		}
		break;
	case BUTTON_DOWN_KEY:
		if (cInput == 66)
		{
			logger().info() << "DOWN arrow key!" << logs::end;
			return true;
		}
		break;
	case BUTTON_LEFT_KEY:
		if (cInput == 68)
		{
			logger().info() << "LEFT arrow key!" << logs::end;
			return true;
		}
		break;
	case BUTTON_RIGHT_KEY:
		if (cInput == 67)
		{
			logger().info() << "RIGHT arrow key!" << logs::end;
			return true;
		}
		break;
	default:
		return false;
	}


	return false;
}
