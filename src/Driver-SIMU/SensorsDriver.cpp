//drivers...SIMU

#include "SensorsDriver.hpp"

#include <unistd.h>
#include <string>

//#include "../Bot-OPOS6UL/OPOS6UL_AsservExtended.hpp"
//#include "../Bot-OPOS6UL/OPOS6UL_RobotExtended.hpp"

using namespace std;

ASensorsDriver * ASensorsDriver::create(std::string, Robot * robot)
{
    return new SensorsDriver(robot);
}

SensorsDriver::SensorsDriver(Robot * robot)
{
    robot_ = robot;
}

SensorsDriver::~SensorsDriver()
{
}

bool SensorsDriver::is_connected()
{
    return true;
}

int SensorsDriver::getAnalogPinData()
{

}
void SensorsDriver::displayNumber(int number)
{

}

ASensorsDriver::bot_positions SensorsDriver::getvPositionsAdv() {
    ASensorsDriver::bot_positions bot_pos;

    //coord table à transformer en coordonnées robot: 200,700 => position robot robot_



    RobotPos pos = {1000, 1000 , 0, 200, 1}; //TODO Fantome a virer
    //simu des positions adverses
    bot_pos = {pos};

    return bot_pos;
}
int SensorsDriver::sync() {
    usleep(100000); //temps de mise a jour des données //TODO temps à mesurer avec la balise ??
    return 0;
}

int SensorsDriver::rightSide()
{
    return 400;
}
int SensorsDriver::leftSide()
{
    return 400;
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
    //TODO temp mettre un ifdef pour simulation gros robot et petit robot
	/*
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    if (robot.asserv().pos_getX_mm() > 800)
    {
        robot.svgPrintPosition(3);
        return 300;
    }
    else*/
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
