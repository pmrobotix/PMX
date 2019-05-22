#include "O_Asserv_CalageTest.hpp"

#include <cmath>
#include <cstdlib>
#include <string>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Arguments.hpp"
#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_Asserv_CalageTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.getArgs().addArgument("d", "dist en mm");
    robot.getArgs().addArgument("type", "type de calage", "B");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void O_Asserv_CalageTest::run(int argc, char** argv)
{
    logger().info() << "Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv); //on appelle les parametres specifiques pour ce test
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    Arguments args = robot.getArgs();

    float d = 0.0;
    if (args["d"] != "0") {
        d = atof(args["d"].c_str());
        logger().info() << "Arg d set " << args["d"] << ", d = " << d << logs::end;
    }

    std::string type;
    if (args["type"] != "0") {
        type = args["v"];
        logger().info() << "Arg type set " << args["type"] << ", type = " << type << logs::end;
    }
    if (type == "B") {
        logger().info() << "Start Asserv " << logs::end;
        robot.setMyColor(PMXVIOLET);
        robot.asserv().startMotionTimerAndOdo(true);
        robot.asserv().setPositionAndColor(0.0, 0.0, 0.0, (robot.getMyColor() != PMXVIOLET));
        RobotPosition p = robot.asserv().pos_getPosition();
        logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° "
                << p.asservStatus << logs::end;
        robot.svgPrintPosition();

        logger().info() << "GO distance calage mm=" << d << logs::end;
        robot.asserv().doCalage(d, 4, 35);
        logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° "
                << p.asservStatus << logs::end;
        robot.svgPrintPosition();

    }
    if (type == "R") {

        robot.setMyColor(PMXVIOLET);
        robot.asserv().startMotionTimerAndOdo(true);
        robot.asserv().setPositionAndColor(70, 450.0, 0.0, (robot.getMyColor() != PMXVIOLET));

        robot.asserv().doMoveForwardTo(1680, 255);

        float calage = robot.actions().sensors().rightSide();
        robot.asserv().calculateDriftRightSideAndSetPos(100, calage, 70, 450);

    }

    logger().info() << "Happy End." << logs::end;
}

