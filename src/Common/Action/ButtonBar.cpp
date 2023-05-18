#include "ButtonBar.hpp"

#include "../Action.Driver/AButtonDriver.hpp"
#include "../../Log/Logger.hpp"
#include "Actions.hpp"
#include <unistd.h>

using namespace std;

ButtonBar::ButtonBar(Actions & actions) :
        AActionsElement(actions)

{
    buttondriver_ = AButtonDriver::create();
}

ButtonBar::~ButtonBar()
{
    delete buttondriver_;
}

bool ButtonBar::pressed(ButtonTouch button)
{
    return buttondriver_->pressed(button);
}

bool ButtonBar::waitPressed(ButtonTouch button)
{
    while (!pressed(button)) {
        utils::sleep_for_micros(500);
        std::this_thread::yield();
    }
    return true;
}

ButtonTouch ButtonBar::waitOneOfAllPressed() //TODO create same with actionmanager
{
    ButtonTouch bt = BUTTON_NONE;

    while (bt == BUTTON_NONE) {
        bt = checkOneOfAllPressed();
        std::this_thread::yield();
    }
    return bt;
}

ButtonTouch ButtonBar::checkOneOfAllPressed() //TODO create same with actionmanager
{
    ButtonTouch bt = BUTTON_NONE;
    for (int b = BUTTON_ENTER_KEY; b < BUTTON_NONE; b++) {

        if (pressed((ButtonTouch) b)) {
            bt = (ButtonTouch) b;
        }
    }
    return bt;
}
