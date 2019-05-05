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
    while (!pressed(button)) {
        usleep(200000);
    }
    return true;
}

ButtonTouch ButtonBar::waitOneOfAllPressed() //TODO create same with actionmanager
{
    ButtonTouch bt = BUTTON_NONE;

    while (bt == BUTTON_NONE) {
        bt = checkOneOfAllPressed();
        /*
        for (int b = BUTTON_ENTER_KEY; b < BUTTON_NONE; b++) {

            if (pressed((ButtonTouch) b)) {
                //return (ButtonTouch)b;
                bt = (ButtonTouch) b;
            }
        }*/
        //return BUTTON_NONE;
        usleep(100000);
    }

    return bt;

}

ButtonTouch ButtonBar::checkOneOfAllPressed() //TODO create same with actionmanager
{
    ButtonTouch bt = BUTTON_NONE;

    for (int b = BUTTON_ENTER_KEY; b < BUTTON_NONE; b++) {

        if (pressed((ButtonTouch) b)) {
            //return (ButtonTouch)b;
            bt = (ButtonTouch) b;
        }
    }

    return bt;

}
