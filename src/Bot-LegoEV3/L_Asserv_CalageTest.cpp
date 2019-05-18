#include "L_Asserv_CalageTest.hpp"

#include <stdlib.h>
#include <cmath>
#include <string>

#include "../Common/Arguments.hpp"
#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_Asserv_CalageTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.getArgs().addArgument("d", "dist en mm");
    //robot.getArgs().addArgument("a", "angle en degrees", "0");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void L_Asserv_CalageTest::run(int argc, char** argv)
{
    logger().info() << "Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv); //on appelle les parametres specifiques pour ce test
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    Arguments args = robot.getArgs();

    float d = 0.0;
    if (args["d"] != "0") {
        d = atof(args["d"].c_str());
        logger().info() << "Arg d set " << args["d"] << ", d = " << d << logs::end;
    }
    /*
     float a = 0.0;
     if (args["a"] != "0") {
     a = atof(args["a"].c_str());
     logger().info() << "Arg a set " << args["a"] << ", a = " << a << logs::end;
     }*/

    logger().info() << "Start Asserv " << logs::end;
    robot.setMyColor(PMXVIOLET);
    robot.asserv().startMotionTimerAndOdo(true);
    robot.asserv().setPositionAndColor(0.0, 0.0, 0.0, (robot.getMyColor() != PMXVIOLET));
    RobotPosition p = robot.asserv().pos_getPosition();
    logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° "
            << p.asservStatus << logs::end;
    robot.svgPrintPosition();

    logger().info() << "GO distance calage mm=" << d << logs::end;
    TRAJ_STATE ts = robot.asserv().doCalage(d, 4);

    logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° "
            << p.asservStatus << " ts=" << ts<<  logs::end;
    robot.svgPrintPosition();
    /*
     robot.asserv().setPositionAndColor(200.0, 200.0, 45.0, (robot.getMyColor() != PMXORANGE));

     logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° " << p.asservStatus << logs::end;
     robot.svgPrintPosition();

     logger().info() << "GO distance mm=" << d << logs::end;
     if (robot.asserv().doLineAbs(100) != TRAJ_OK) {
     logger().info() << "Interruption !!" << logs::end;
     }

     p = robot.asserv().pos_getPosition();
     logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° " << p.asservStatus << logs::end;
     sleep(1);
     p = robot.asserv().pos_getPosition();
     logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° " << p.asservStatus << logs::end;

     robot.svgPrintPosition();*/

    logger().info() << "Happy End." << logs::end;
}

