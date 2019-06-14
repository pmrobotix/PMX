#include "O_Asserv_SquareTest.hpp"

#include <cstdlib>
#include <string>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Arguments.hpp"
#include "../Common/Asserv/EncoderControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_Asserv_SquareTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    robot.getArgs().addArgument("x", "x mm");
    robot.getArgs().addArgument("y", "y mm");
    robot.getArgs().addArgument("d", "segment mm");
    robot.getArgs().addArgument("nb", "nbre de tours", "1");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void O_Asserv_SquareTest::run(int argc, char** argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv); //on appelle les parametres specifiques pour ce test

    utils::Chronometer chrono("O_Asserv_SquareTest");
    long left;
    long right;

    float x = 0.0;
    float y = 0.0;
    float d = 0.0;
    int nb = 0;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

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

    robot.setMyColor(PMXVIOLET);

    robot.asserv().startMotionTimerAndOdo(true);

    robot.asserv().setPositionAndColor(0.0, 300.0, 0.0, (robot.getMyColor() != PMXVIOLET));
    robot.svgPrintPosition();

    robot.actions().start();
    robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
    robot.actions().sensors().addTimerSensors(50);

    chrono.start();

    for (int n = 1; n <= nb; n++) {
        logger().info() << "doMoveForwardTo(" << x << ", " << y << ")" << logs::end;
        robot.asserv().doMoveForwardTo(x, y);

        left = robot.asserv().base()->encoders().getLeftEncoder();
        right = robot.asserv().base()->encoders().getRightEncoder();
        logger().info() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= "
                << right << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
                << robot.asserv().pos_getThetaInDegree() << logs::end;

        robot.svgPrintPosition();

        logger().info() << "doMoveForwardTo(" << x + d << ", " << y << ")" << logs::end;
        robot.asserv().doMoveForwardTo(x + d, y);

        left = robot.asserv().base()->encoders().getLeftEncoder();
        right = robot.asserv().base()->encoders().getRightEncoder();
        logger().info() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= "
                << right << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
                << robot.asserv().pos_getThetaInDegree() << logs::end;

        robot.svgPrintPosition();

        logger().info() << "doMoveForwardTo(" << x + d << ", " << y + d << ")" << logs::end;
        robot.asserv().doMoveForwardTo(x + d, y + d);

        left = robot.asserv().base()->encoders().getLeftEncoder();
        right = robot.asserv().base()->encoders().getRightEncoder();
        logger().info() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= "
                << right << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
                << robot.asserv().pos_getThetaInDegree() << logs::end;

        robot.svgPrintPosition();

        logger().info() << "doMoveForwardTo(" << x << ", " << y + d << ")" << logs::end;
        robot.asserv().doMoveForwardTo(x, y + d);

        left = robot.asserv().base()->encoders().getLeftEncoder();
        right = robot.asserv().base()->encoders().getRightEncoder();
        logger().info() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " right= " << right
                << " ; x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
                << robot.asserv().pos_getThetaInDegree() << logs::end;

        robot.svgPrintPosition();

        logger().info() << "doMoveForwardAndRotateTo(" << x << ", " << y << ", 0.0)" << logs::end;
        robot.asserv().doMoveForwardAndRotateTo(x, y, 0.0);

        left = robot.asserv().base()->encoders().getLeftEncoder();
        right = robot.asserv().base()->encoders().getRightEncoder();
        logger().info() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " right= " << right
                << " ; x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
                << robot.asserv().pos_getThetaInDegree() << logs::end;

        robot.svgPrintPosition();
    }

    logger().info() << "End time= " << chrono.getElapsedTimeInMilliSec() << " ; x=" << robot.asserv().pos_getX_mm()
            << " y=" << robot.asserv().pos_getY_mm() << " degrees=" << robot.asserv().pos_getThetaInDegree()
            << logs::end;

    logger().info() << "Happy End." << logs::end;
}

