#include "L_Asserv_SquareTest.hpp"

#include <stdlib.h>
#include <string>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Arguments.hpp"
#include "../Common/Asserv/EncoderControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_Asserv_SquareTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    //match color
    robot.getArgs().addArgument("x", "x mm");
    robot.getArgs().addArgument("y", "y mm");
    robot.getArgs().addArgument("d", "segment mm");
    robot.getArgs().addArgument("nb", "nbre de tours", "1");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void L_Asserv_SquareTest::run(int argc, char** argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv); //on appelle les parametres specifiques pour ce test

    utils::Chronometer chrono("L_Asserv_SquareTest");

    float x = 0.0;
    float y = 0.0;
    float d = 0.0;
    int nb = 0;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    Arguments args = robot.getArgs();

    if (args["x"] != "0") {
        x = atof(args["x"].c_str());
        logger().info() << "Arg x set " << args["x"] << ", x = " << x << logs::end;
    }
    if (args["y"] != "0") {
        y = atof(args["y"].c_str());
        logger().info() << "Arg y set " << args["y"] << ", y = " << y << logs::end;
    }
    if (args["d"] != "0") {
        d = atof(args["d"].c_str());
        logger().info() << "Arg d set " << args["d"] << ", d = " << d << logs::end;
    }

    if (args["nb"] != "0") {
        nb = atoi(args["nb"].c_str());
        logger().info() << "Arg nb set " << args["nb"] << ", nb = " << nb << logs::end;
    }

    robot.asserv().startMotionTimerAndOdo(true);

    robot.asserv().setPositionAndColor(0.0, 300.0, 0.0, (robot.getMyColor() != PMXVIOLET));
    robot.svgPrintPosition();

    robot.actions().start();
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, false);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.actions().sensors().addTimerSensors(100);

    chrono.start();
    TRAJ_STATE ts = TRAJ_OK;
    int f = 0;
    for (int n = 1; n <= nb; n++) {
        logger().info() << "doMoveForwardTo " << " x=" << x << " y=" << y << logs::end;
        while ((ts = robot.asserv().doMoveForwardTo(x, y)) != TRAJ_FINISHED) {
            f++;
            logger().info() << f << " doMoveForwardTo " << " x=" << x + d << " y=" << y << logs::end;
            logger().info() << f << "pos " << " x=" << robot.asserv().pos_getX_mm() << " y="
                    << robot.asserv().pos_getY_mm() << " a=" << robot.asserv().pos_getThetaInDegree() << "ts =" << ts
                    << logs::end;

            robot.svgPrintPosition();
            if (f >= 2) {
                break;
            }
//            if (f >=3)
            //usleep(800000);

        }

        f = 0;
        logger().info() << "doMoveForwardTo " << " x=" << x + d << " y=" << y << logs::end;
        while ((ts = robot.asserv().doMoveForwardTo(x + d, y)) != TRAJ_FINISHED) {
            f++;
            logger().info() << f << " doMoveForwardTo " << " x=" << x + d << " y=" << y << logs::end;
            logger().info() << f << " pos " << " x=" << robot.asserv().pos_getX_mm() << " y="
                    << robot.asserv().pos_getY_mm() << " a=" << robot.asserv().pos_getThetaInDegree() << "ts =" << ts
                    << logs::end;
            robot.svgPrintPosition();

            if (f >= 2) {
                break;
            }
        }

        logger().info() << "pos " << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm()
                << " a=" << robot.asserv().pos_getThetaInDegree() << "ts =" << ts << logs::end;
        robot.svgPrintPosition();

        f = 0;
        logger().info() << "doMoveForwardTo " << " x=" << x + d << " y=" << y + d << logs::end;
        while ((ts = robot.asserv().doMoveForwardTo(x + d, y + d)) != TRAJ_FINISHED) {
            f++;
            logger().info() << f << " doMoveForwardTo " << " x=" << x + d << " y=" << y + d << logs::end;
            logger().info() << f << " pos " << " x=" << robot.asserv().pos_getX_mm() << " y="
                    << robot.asserv().pos_getY_mm() << " a=" << robot.asserv().pos_getThetaInDegree() << "ts =" << ts
                    << logs::end;
            if (f >= 2) {
                break;
            }
        }

        logger().info() << "pos " << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm()
                << " a=" << robot.asserv().pos_getThetaInDegree() << "ts =" << ts << logs::end;
        robot.svgPrintPosition();

        f = 0;
        logger().info() << "doMoveForwardTo " << " x=" << x << " y=" << y + d << logs::end;
        while ((ts = robot.asserv().doMoveForwardTo(x, y + d)) != TRAJ_FINISHED) {
            f++;
            logger().info() << f << " doMoveForwardTo " << " x=" << x + d << " y=" << y + d << logs::end;
            logger().info() << f << " pos " << " x=" << robot.asserv().pos_getX_mm() << " y="
                    << robot.asserv().pos_getY_mm() << " a=" << robot.asserv().pos_getThetaInDegree() << "ts =" << ts
                    << logs::end;
            if (f >= 2) {
                break;
            }
        }
        logger().info() << "pos " << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm()
                << " a=" << robot.asserv().pos_getThetaInDegree() << "ts =" << ts << logs::end;
        robot.svgPrintPosition();

        f = 0;
        logger().info() << "doMoveForwardTo " << " x=" << x << " y=" << y << " a=0.0" << logs::end;
        while ((ts = robot.asserv().doMoveForwardAndRotateTo(x, y, 0.0)) != TRAJ_FINISHED) {
            f++;
            logger().info() << f << " doMoveForwardTo " << " x=" << x + d << " y=" << y + d << logs::end;
            logger().info() << f << " pos " << " x=" << robot.asserv().pos_getX_mm() << " y="
                    << robot.asserv().pos_getY_mm() << " a=" << robot.asserv().pos_getThetaInDegree() << "ts =" << ts
                    << logs::end;
            if (f >= 2) {
                break;
            }
        }
        logger().info() << "pos " << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm()
                << " a=" << robot.asserv().pos_getThetaInDegree() << "ts =" << ts << logs::end;
        robot.svgPrintPosition();
    }

    logger().info() << "Happy End." << logs::end;
}

