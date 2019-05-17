//drivers...SIMU

#include "SwitchDriver.hpp"

#include <string>

using namespace std;

ASwitchDriver * ASwitchDriver::create(std::string)
{
    static SwitchDriver *instance = new SwitchDriver();
    return instance;
}

SwitchDriver::SwitchDriver()
{
    state_ = 1;
}

SwitchDriver::~SwitchDriver()
{
}

int SwitchDriver::tirettePressed()
{
    return 0;
}
int SwitchDriver::backLeftPressed()
{
    return 0;
}
int SwitchDriver::backRightPressed()
{
    return 0;
}

int SwitchDriver::pressed(unsigned char pin)
{
    return 0;
}
