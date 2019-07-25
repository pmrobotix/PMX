//drivers...SIMU

#include <Driver.SIMUCOCOS/SensorsDriver.hpp>
#include <string>

using namespace std;

ASensorsDriver* ASensorsDriver::create(std::string)
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
    return 99;
}
int SensorsDriver::leftSide()
{
    return 99;
}

int SensorsDriver::frontLeft()
{
    return 99;
}
int SensorsDriver::frontCenter()
{
    return 99;
}
int SensorsDriver::frontRight()
{
    return 99;
}

int SensorsDriver::backLeft()
{
    return 99;
}
int SensorsDriver::backCenter()
{
    return 99;
}
int SensorsDriver::backRight()
{
    return 99;
}
