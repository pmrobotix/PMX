#include "Tirette.hpp"

#include <unistd.h>
#include <string>

#include "../../Log/Logger.hpp"

using namespace std;

Tirette::Tirette(Actions & actions) :
        AActionsElement(actions)

{
    switchdriver = ASwitchDriver::create("tirette");
}

Tirette::~Tirette()
{
}

bool Tirette::pressed()
{
    int temp = switchdriver->tirettePressed();
    logger().debug() << "pressed = " << temp << logs::end;

    return temp;
}

//pressed then released
void Tirette::monitor(int nb)
{
    logger().info() << "monitor" << logs::end;
    int n = 0;
    while (n <= nb) {
        while (!pressed()) {
            usleep(10000);
        }
        logger().info() << "pressed" << logs::end;

        while (pressed()) {
            usleep(10000);
        }
        logger().info() << "unpressed" << logs::end;
        n++;
    }
}

bool Tirette::waitPressed()
{
    while (!pressed()) {
        usleep(100000);
    }
    return true;
}

bool Tirette::waitUnpressed()
{
    while (pressed()) {
        usleep(100000);
    }
    return true;
}
