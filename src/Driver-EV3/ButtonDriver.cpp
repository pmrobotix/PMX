//drivers...EV3

#include "ButtonDriver.hpp"

#include <unistd.h>

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
}

ButtonDriver::~ButtonDriver()
{
}

bool ButtonDriver::pressed(ButtonTouch button)
{
    usleep(20000);
    switch (button) {
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
    case BUTTON_NONE:
        break;
    }
    return 0;
}

