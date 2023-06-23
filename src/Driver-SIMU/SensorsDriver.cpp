//drivers...SIMU

#include "SensorsDriver.hpp"

#include <unistd.h>
#include <initializer_list>
#include <string>
#include <vector>

#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/Robot.hpp"

//#include "../Bot-OPOS6UL/OPOS6UL_AsservExtended.hpp"
//#include "../Bot-OPOS6UL/OPOS6UL_RobotExtended.hpp"

using namespace std;

ASensorsDriver* ASensorsDriver::create(std::string, Robot *robot)
{
    return new SensorsDriver(robot);
}

SensorsDriver::SensorsDriver(Robot *robot)
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

void SensorsDriver::displayNumber(int number)
{

}

RobotPos SensorsDriver::transformPosTableToPosRobot(int nb, int x_table, int y_table)
{

    //coord table à transformer en coordonnées robot: 200,700 => position robot robot_

    RobotPosition p = robot_->asserv().pos_getPosition();

    float d = std::sqrt(square(y_table-p.y) + square(x_table - p.x));

    float x_rep_robot = 0;
    float y_rep_robot = 0;

    float b_rad = std::asin((y_table - p.y) / d);
    //pb quadrant car on ne connait pas les angles
    if (x_table <= p.x) {
        if (y_table <= p.y)
            b_rad = -M_PI - b_rad;
        else
            b_rad = M_PI - b_rad;
//    } else {
//        if (y_table <= p.y)
//        {
//            //b_rad = b_rad;
//        }
//        else
//        {
//            //b_rad = -b_rad;
//        }
    }
    float alpha_rad = b_rad - p.theta;

    std::fmod(alpha_rad, 2 * M_PI);
    if (alpha_rad < -M_PI)
        alpha_rad += M_PI;
    if (alpha_rad > M_PI)
        alpha_rad -= M_PI;

    //angle entre l'axe devant le robot et le segment entre les milieu des 2 robots
    float alpha_deg = (alpha_rad * 180.0 / M_PI);

    x_rep_robot = -d * std::sin(alpha_rad);
    y_rep_robot = d * std::cos(alpha_rad);

    logger().info() << __FUNCTION__ << " x_rep_robot=" << x_rep_robot << " y_rep_robot=" << y_rep_robot
            << " a_deg_rep_robot=" << alpha_deg << logs::end;

    RobotPos pos = { nb, (int) x_rep_robot, (int) y_rep_robot, alpha_deg, (int)d };

    return pos;
}

ASensorsDriver::bot_positions SensorsDriver::getvPositionsAdv()
{
    ASensorsDriver::bot_positions bot_pos;

    //coord table à transformer en coordonnées robot: 200,700 => position robot robot_
//    int x_table = 1000.0;
//    int y_table = 1500.0;

    int nb = 2;
    RobotPos pos1 = transformPosTableToPosRobot(nb, 1000, 1200);
    //RobotPos pos2 = transformPosTableToPosRobot(nb, 1300, 300);
    /*

     RobotPosition p = robot_->asserv().pos_getPosition();

     int d = std::sqrt(square(y_table-p.y) + square(x_table - p.x));

     float x_rep_robot = 0;
     float y_rep_robot = 0;

     float alpha_rad = std::asin((y_table - p.y) / d) - p.theta;

     std::fmod(alpha_rad, 2 * M_PI);
     if (alpha_rad < -M_PI)
     alpha_rad += M_PI;
     if (alpha_rad > M_PI)
     alpha_rad -= M_PI;

     float alpha_deg = (alpha_rad * 180.0 / M_PI);

     x_rep_robot = -d * std::sin(alpha_rad);
     y_rep_robot = d * std::cos(alpha_rad);

     RobotPos pos = { 1, (int) x_rep_robot, (int) y_rep_robot, alpha_deg, d };
     */
    //simu des positions adverses
    bot_pos = { pos1 };
    //bot_pos = { pos1, pos2 };

    return bot_pos;
}
int SensorsDriver::sync()
{
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
