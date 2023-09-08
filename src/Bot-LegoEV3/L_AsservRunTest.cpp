#include "L_AsservRunTest.hpp"

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

void L_AsservRunTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    //robot.getArgs().addArgument("nb", "nbr position", "1");
    robot.getArgs().addArgument("x", "x mm");
    robot.getArgs().addArgument("y", "y mm");
    robot.getArgs().addArgument("x2", "x2 mm", "0");
    robot.getArgs().addArgument("y2", "y2 mm", "0");
    //TODO mettre plus de points x,y

    Arguments::Option cOpt('+', "Coordinates x,y,a");
    cOpt.addArgument("coordx", "coord x mm", "0.0");
    cOpt.addArgument("coordy", "coord y mm", "300.0");
    cOpt.addArgument("coorda", "coord teta mm", "0.0");
    robot.getArgs().addOption(cOpt);

    //reparse again arguments for the specific test
    robot.parseConsoleArgs(argc, argv);

}

void L_AsservRunTest::run(int argc, char** argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv);

    utils::Chronometer chrono("L_AsservRunTest");

    float x = 0.0;
    float y = 0.0;
    float x2 = 0.0;
    float y2 = 0.0;
    float coordx = 0.0;
    float coordy = 0.0;
    float coorda_deg = 0.0;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    Arguments args = robot.getArgs();
//    if (args["nb"] != "0") {
//        nb = atoi(args["nb"].c_str());
//        logger().debug() << "Arg nb set " << args["nb"] << ", nb = " << nb << logs::end;
//    }

    if (args["x"] != "0") {
        x = atof(args["x"].c_str());
        logger().debug() << "Arg x set " << args["x"] << ", x = " << x << logs::end;
    }
    if (args["y"] != "0") {
        y = atof(args["y"].c_str());
        logger().debug() << "Arg y set " << args["y"] << ", y = " << y << logs::end;
    }

    if (args["x2"] != "0") {
        x2 = atof(args["x2"].c_str());
        logger().debug() << "Arg x2 set " << args["x2"] << ", x2 = " << x2 << logs::end;
    }
    if (args["y2"] != "0") {
        y2 = atof(args["y2"].c_str());
        logger().debug() << "Arg y2 set " << args["y2"] << ", y2 = " << y2 << logs::end;
    }

    coordx = atof(args['+']["coordx"].c_str());
    coordy = atof(args['+']["coordy"].c_str());
    coorda_deg = atof(args['+']["coorda"].c_str());

    robot.asserv().startMotionTimerAndOdo(true);

    logger().info() << "COORD avec x=" << coordx << " y=" << coordy << " a=" << coorda_deg << logs::end;

    robot.asserv().setPositionAndColor(coordx, coordy, coorda_deg, (robot.getMyColor() != PMXGREEN));

    robot.svgPrintPosition();

    //detection adverse
    robot.actions().start();
    robot.actions().sensors().addTimerSensors(200);
    chrono.start();

    robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, false);
    robot.actions().sensors().setIgnoreBackNearObstacle(false, true, false);

    logger().info() << "GOTO x=" << x << " y=" << y << logs::end;

    TRAJ_STATE ts = robot.ia().iAbyPath().whileMoveForwardTo(x, y, false, 500000, 3, 3);
    logger().info() << "END GOTO ts=" << ts << logs::end;

    if (x2 != 0 && y2 != 0) {
        sleep(3);
        logger().info() << "GOTO2 x2=" << x2 << " y2=" << y2 << logs::end;

        TRAJ_STATE ts = robot.ia().iAbyPath().whileMoveForwardTo(x2, y2, false, 500000, 3, 3);
        logger().info() << "END GOTO2 ts=" << ts << logs::end;
    }

    /*
     TRAJ_STATE ts;
     int f = 0;
     int c = 0;
     while ((ts = robot.asserv().doMoveForwardTo(x, y)) != TRAJ_FINISHED) {
     robot.svgPrintPosition();
     robot.displayTS(ts);

     if (ts == TRAJ_NEAR_OBSTACLE) {
     robot.logger().error() << " ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;

     f++;
     usleep(200000);
     if (f > 3)
     {
     robot.asserv().resetEmergencyOnTraj();
     }
     }
     if (ts == TRAJ_COLLISION) {
     robot.logger().error() << "===== COLLISION essai n°" << c << logs::end;

     c++;
     if (c > 3)
     {
     robot.asserv().resetEmergencyOnTraj();
     }
     }
     robot.resetDisplayTS();
     logger().info() << "AGAIN GOTO x=" << x << " y=" << y << logs::end;

     }

     left = robot.asserv().base()->encoders().getLeftEncoder();
     right = robot.asserv().base()->encoders().getRightEncoder();
     logger().info() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= " << right
     << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
     << robot.asserv().pos_getThetaInDegree() << logs::end;

     robot.svgPrintPosition();
     */

    logger().info() << "Happy End." << logs::end;
}
