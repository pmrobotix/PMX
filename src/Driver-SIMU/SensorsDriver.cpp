//drivers...SIMU

#include "SensorsDriver.hpp"

using namespace std;

ASensorsDriver * ASensorsDriver::create(std::string)
{

    return new SensorsDriver();
}

SensorsDriver::SensorsDriver()
{

}

SensorsDriver::~SensorsDriver()
{
}

int SensorsDriver::rightSide()
{
    return 100;
}
int SensorsDriver::leftSide()
{
    return 100;
}

int SensorsDriver::frontLeft()
{
    return 999;
}
int SensorsDriver::frontCenter()
{
    return 999;
}
int SensorsDriver::frontRight()
{
    return 999;
}

int SensorsDriver::backLeft()
{
    return 999;
}
int SensorsDriver::backCenter()
{
    return 999;
}
int SensorsDriver::backRight()
{
    return 999;
}
