#include "L_AsservPidTest.hpp"

#include <stdlib.h>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Arguments.hpp"
#include "../Common/Interface.Driver/AAsservDriver.hpp"
#include "../Common/IA/IAbyPath.hpp"
#include "../Common/Robot.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3IAExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_AsservPidTest::configureConsoleArgs(int argc, char **argv) //surcharge
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    robot.getArgs().addArgument("p", "P");
    robot.getArgs().addArgument("i", "I");
    robot.getArgs().addArgument("d", "D");



    //reparse again arguments for the specific test
    robot.parseConsoleArgs(argc, argv);

}

void L_AsservPidTest::run(int argc, char **argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv);

    utils::Chronometer chrono("L_AsservRunTest");

    float p = 0.0;
    float i = 0.0;
    float d = 0.0;


    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    Arguments args = robot.getArgs();

    if (args["p"] != "0") {
        p = atof(args["p"].c_str());
        logger().debug() << "Arg p set " << args["x"] << ", p = " << p << logs::end;
    }
    if (args["i"] != "0") {
        i = atof(args["i"].c_str());
        logger().debug() << "Arg i set " << args["y"] << ", i = " << i << logs::end;
    }
    if (args["d"] != "0") {
        d = atof(args["d"].c_str());
        logger().debug() << "Arg d set " << args["y"] << ", d = " << d << logs::end;
    }

    robot.asserv().startMotionTimerAndOdo(true);
    robot.asserv().setPositionAndColor(400, 400, 0.0, (robot.getMyColor() != PMXGREEN));
    robot.svgPrintPosition();
    chrono.start();

    robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
    robot.actions().sensors().setIgnoreBackNearObstacle(false, false, false);



    logger().info() << "Happy End." << logs::end;
}
