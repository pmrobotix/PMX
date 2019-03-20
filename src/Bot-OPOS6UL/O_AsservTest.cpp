#include "O_AsservTest.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <string>

#include "../Common/Arguments.hpp"
#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_AsservTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.getArgs().addArgument("d", "dist en mm");
    robot.getArgs().addArgument("a", "angle en degrees", "0");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void O_AsservTest::run(int argc, char** argv)
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
    float a = 0.0;
    if (args["a"] != "0") {
        a = atof(args["a"].c_str());
        logger().info() << "Arg a set " << args["a"] << ", a = " << a << logs::end;
    }

    logger().info() << "Start Asserv " << logs::end;
    robot.setMyColor(PMXVIOLET);
    robot.asserv().startMotionTimerAndOdo(true); //assistedHandling is enabled with "true" !
    robot.asserv().setPositionAndColor(200.0, 300.0, 0.0, (robot.getMyColor() != PMXVIOLET));
    RobotPosition p = robot.asserv().pos_getPosition();
    logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° " << p.asservStatus << logs::end;
    robot.svgPrintPosition();

    //robot.asserv().freeMotion();
    //robot.asserv().assistedHandling();

    logger().info() << "GO distance mm=" << d << logs::end;
    TRAJ_STATE tr = robot.asserv().doLineAbs(d);
    if (tr != TRAJ_OK) {
        //logger().info() << robot.actions().sensors().front() << logs::end;
        logger().info() << "Interruption TRAJ_STATE=!! " << tr << logs::end;
    }

    p = robot.asserv().pos_getPosition();
    logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° " << p.asservStatus << logs::end;
    robot.svgPrintPosition();

    logger().info() << "GO turn angle=" << a << logs::end;
    if (robot.asserv().doRotateAbs(a) != TRAJ_OK) {
        logger().info() << "Interruption !!" << logs::end;
    }

    p = robot.asserv().pos_getPosition();
    logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° " << p.asservStatus << logs::end;
    sleep(1);
    p = robot.asserv().pos_getPosition();
    logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° " << p.asservStatus << logs::end;

    robot.svgPrintPosition();

    robot.asserv().freeMotion();
    logger().info() << "Stopping Robot... " << logs::end;
    robot.stopAll();
    logger().info() << "Happy End." << logs::end;
}

