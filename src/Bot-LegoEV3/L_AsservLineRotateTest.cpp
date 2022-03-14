#include "L_AsservLineRotateTest.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <string>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Arguments.hpp"
#include "../Common/Asserv/EncoderControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/IA/IAbyPath.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3IAExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_AsservLineRotateTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    //distance
    robot.getArgs().addArgument("d", "distance mm");
    //coord
    robot.getArgs().addArgument("x", "x mm", "0");
    robot.getArgs().addArgument("y", "y mm", "0");
    //angle
    robot.getArgs().addArgument("a", "angle degre", "0");

    robot.getArgs().addArgument("nb", "nb of time", "1");

    Arguments::Option cOpt('+', "Coordinates x,y,a");
    cOpt.addArgument("coordx", "coord x mm", "0.0");
    cOpt.addArgument("coordy", "coord y mm", "1300.0");
    cOpt.addArgument("coorda", "coord teta mm", "0.0");
    robot.getArgs().addOption(cOpt);

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void L_AsservLineRotateTest::run(int argc, char** argv) {
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv);

    long left;
    long right;

    float d = 0.0;
    float x = 0.0;
    float y = 0.0;
    int nb = 0.0;
    float a = 0.0;
    float coordx = 0.0;
    float coordy = 0.0;
    float coorda_deg = 0.0;
    TRAJ_STATE ts = TRAJ_OK;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    Arguments args = robot.getArgs();

    if (args["d"] != "0") {
        d = atof(args["d"].c_str());
        logger().debug() << "Arg d set " << args["d"] << ", d = " << d << logs::end;
    }

    if (args["x"] != "0") {
        x = atof(args["x"].c_str());
        logger().debug() << "Arg x set " << args["x"] << ", x = " << x << logs::end;
    }

    if (args["y"] != "0") {
        y = atof(args["y"].c_str());
        logger().debug() << "Arg y set " << args["y"] << ", y = " << y << logs::end;
    }

    if (args["a"] != "0") {
        a = atof(args["a"].c_str());
        logger().debug() << "Arg a set " << args["a"] << ", a = " << a << logs::end;
    }

    if (args["nb"] != "0") {
        nb = atoi(args["nb"].c_str());
        logger().debug() << "Arg nb set " << args["nb"] << ", nb = " << nb << logs::end;
    }
    coordx = atof(args['+']["coordx"].c_str());
    coordy = atof(args['+']["coordy"].c_str());
    coorda_deg = atof(args['+']["coorda"].c_str());

    robot.asserv().startMotionTimerAndOdo(true); //true = assistedHandling


    logger().info() << "COORD avec x=" << coordx << " y=" << coordy << " a=" << coorda_deg << logs::end;

    robot.asserv().setPositionAndColor(coordx, coordy, coorda_deg, (robot.getMyColor() != PMXYELLOW));

    left = robot.asserv().base()->extEncoders().getLeftEncoder();
    right = robot.asserv().base()->extEncoders().getRightEncoder();
    logger().info() << "time= "
            << robot.chrono().getElapsedTimeInMilliSec()
            << "ms ; left= "
            << left
            << " ; right= "
            << right
            << " x="
            << robot.asserv().pos_getX_mm()
            << " y="
            << robot.asserv().pos_getY_mm()
            << " a="
            << robot.asserv().pos_getThetaInDegree()
            << logs::end;

    robot.svgPrintPosition();

    robot.actions().start();
    robot.actions().sensors().addTimerSensors(100);    //detection adverse
    robot.chrono().start();

//    robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
//    robot.actions().sensors().setIgnoreBackNearObstacle(false, false, false);
    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);


    for (int num = 1; num <= nb; num++) {
        ts = TRAJ_OK;
        if (d != 0) {
            logger().info() << "go..." << d << "mm" << logs::end;
            while (ts != TRAJ_FINISHED) {
                ts = robot.ia().iAbyPath().whileMoveForwardTo(d, robot.asserv().pos_getY_mm(), false, 1000000, 3, 3, false);
                if (ts == TRAJ_NEAR_OBSTACLE) {
                    logger().error() << "===== TRAJ_NEAR_OBSTACLE FINAL" << logs::end;
                    robot.asserv().resetEmergencyOnTraj();
                }
                if (ts == TRAJ_COLLISION) {
                    logger().error() << "===== COLLISION ASSERV FINAL" << logs::end;
                    robot.asserv().resetEmergencyOnTraj();
                }
                //sleep(3);
                break;
            }
        }

        if (!(x == 0 && y == 0)) {
            ts = TRAJ_OK;
            logger().info() << "go Forward... x=" << x << ", y=" << y << logs::end;
            while (ts != TRAJ_FINISHED) {
                ts = TRAJ_OK;
                ts = robot.ia().iAbyPath().whileMoveForwardTo(x, y, false, 2000000, 3, 3);

                if (ts == TRAJ_NEAR_OBSTACLE) {
                    logger().error() << "===== TRAJ_NEAR_OBSTACLE FINAL" << logs::end;
                    robot.asserv().resetEmergencyOnTraj("robot.ia().iAbyPath().whileMoveForwardTo FINAL TRAJ_NEAR_OBSTACLE");
                }
                if (ts == TRAJ_COLLISION) {
                    logger().error() << "===== COLLISION ASSERV FINAL" << logs::end;
                    robot.asserv().resetEmergencyOnTraj("robot.ia().iAbyPath().whileMoveForwardTo FINAL TRAJ_COLLISION");
                }
                //sleep(2);
                break;
            }
            robot.svgPrintPosition();
        }


        if (a != 0) {
            ts = TRAJ_OK;
            while (ts != TRAJ_FINISHED) {

                ts = robot.ia().iAbyPath().whileMoveRotateTo(a, 1000000, 2);

                if (ts == TRAJ_NEAR_OBSTACLE) {
                    logger().error() << "===== TRAJ_NEAR_OBSTACLE FINAL" << logs::end;
                    robot.asserv().resetEmergencyOnTraj();
                }
                if (ts == TRAJ_COLLISION) {
                    logger().error() << "===== COLLISION ASSERV FINAL" << logs::end;
                    robot.asserv().resetEmergencyOnTraj();
                }
                //sleep(3);
                break;
            }
        }
        d += d;
    }
    sleep(1);
    left = robot.asserv().base()->encoders().getLeftEncoder();
    right = robot.asserv().base()->encoders().getRightEncoder();
    logger().info() << "End time= "
            << robot.chrono().getElapsedTimeInMilliSec()
            << "ms ; left= "
            << left
            << " ; right= "
            << right
            << " x="
            << robot.asserv().pos_getX_mm()
            << " y="
            << robot.asserv().pos_getY_mm()
            << " a="
            << robot.asserv().pos_getThetaInDegree()
            << logs::end;

    robot.svgPrintPosition();

    logger().info() << "Happy End." << logs::end;
}

