#include "ButtonBar.hpp"

#include "../Action.Driver/AButtonDriver.hpp"
#include "../../Log/Logger.hpp"
#include "Actions.hpp"
#include <unistd.h>

using namespace std;

ButtonBar::ButtonBar(Actions & actions) :
		AActionsElement(actions)

{
	buttondriver = AButtonDriver::create();
}

ButtonBar::~ButtonBar()
{
	delete buttondriver;
}

bool ButtonBar::pressed(ButtonTouch button)
{
	return buttondriver->pressed(button);
}

bool ButtonBar::waitPressed(ButtonTouch button)
{
	while(!pressed(button))
	{
		usleep(1000);
	}
	return true;
}
