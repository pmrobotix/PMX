//drivers...SIMU

#include "SensorsDriver.hpp"

#include <string>

#include "../Bot-OPOS6UL/OPOS6UL_AsservExtended.hpp"
#include "../Bot-OPOS6UL/OPOS6UL_RobotExtended.hpp"

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
    //TODO temp mettre un ifdef pour gros robot et petit robot
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    if (robot.asserv().pos_getX_mm() > 800)
    {
        robot.svgPrintPosition(3);
        return 300;
    }
    else
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
