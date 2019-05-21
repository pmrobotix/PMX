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
    return 0;
}
int SensorsDriver::leftSide()
{
    return 0;
}

int SensorsDriver::frontLeft()
{
    return 0;
}
int SensorsDriver::frontCenter()
{
    return 0;
}
int SensorsDriver::frontRight()
{
    return 0;
}

int SensorsDriver::backLeft()
{
    return 0;
}
int SensorsDriver::backCenter()
{
    return 0;
}
int SensorsDriver::backRight()
{
    return 0;
}
