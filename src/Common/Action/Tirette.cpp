#include "Tirette.hpp"

#include <unistd.h>
#include <string>

#include "../../Log/Logger.hpp"

using namespace std;

Tirette::Tirette(Actions & actions) :
        AActionsElement(actions)

{
    switchdriver_ = ASwitchDriver::create("tirette");
}

Tirette::~Tirette()
{
    delete switchdriver_;
}

int Tirette::pressed()
{
    int temp = switchdriver_->tirettePressed();
    logger().debug() << "pressed = " << temp << logs::end;
    if (temp == -1) {
        logger().error() << "pressed1 = " << temp << logs::end;
        temp = switchdriver_->tirettePressed();
    }
    if (temp == -1) {
        logger().error() << "pressed2 = " << temp << logs::end;
        temp = switchdriver_->tirettePressed();
    }

    return temp;
}

//pressed then released
void Tirette::monitor(int nb)
{
    logger().info() << "monitor" << logs::end;
    int n = 0;
    while (n <= nb) {
        while (!pressed()) {
            utils::sleep_for_micros(10000);
        }
        logger().info() << "pressed" << logs::end;

        while (pressed()) {
            utils::sleep_for_micros(10000);
        }
        logger().info() << "unpressed" << logs::end;
        n++;
    }
}

bool Tirette::waitPressed()
{
    while (!pressed()) {
        utils::sleep_for_micros(100000);
    }
    return true;
}

bool Tirette::waitUnpressed()
{
    while (pressed()) {
        utils::sleep_for_micros(100000);
    }
    return true;
}

void Tirette::setGPIO(int p, bool activate)
{
    switchdriver_->setGPIO(p, activate);
}

