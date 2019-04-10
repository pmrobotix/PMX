#include "L_AsservEsialTest.hpp"

#include <unistd.h>
#include <string>

#include "../Common/Arguments.hpp"
#include "../Common/Asserv/ExtEncoderControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_AsservEsialTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void L_AsservEsialTest::run(int argc, char** argv)
{
    //logs::Logger::LoggerBuffer info = logger().info();

    logger().info() << "Executing - " << this->desc() << logs::end;
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    long left;
    long right;

    Arguments args = robot.getArgs();
    utils::Chronometer chrono("L_AsservEsialTest");

    robot.asserv().startMotionTimerAndOdo(false);
    robot.asserv().setPositionAndColor(0.0, 0.0, 0.0, (robot.getMyColor() != PMXVIOLET));


    LegoEV3AsservExtended asserv = robot.asserv();
    ExtEncoderControl extEncoders = asserv.base()->extEncoders();
int nb = 0;
    while (1) {

        left = extEncoders.getLeftEncoder();
        right = extEncoders.getRightEncoder();

        logger().info() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= " << right << " x="
                << asserv.pos_getX_mm() << " y=" << asserv.pos_getY_mm() << " a=" << asserv.pos_getThetaInDegree()
                << logs::end;
        usleep(200000);
        nb++;
        if (nb >10)
            break;
    }
    robot.svgPrintPosition();

    robot.stopAll();
    logger().info() << "Happy End." << logs::end;
}

