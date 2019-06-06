#include "L_AsservRunTest.hpp"

#include <stdio.h>
#include <stdlib.h>

#include "../Common/Action/Actions.hpp"
#include "../Common/Arguments.hpp"
#include "../Common/Asserv/EncoderControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Robot.hpp"
#include "../Common/State/Automate.hpp"
#include "L_State_WaitEndOfMatch.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_AsservRunTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    robot.getArgs().addArgument("nb", "nbr position", "1");
    robot.getArgs().addArgument("x", "x mm");
    robot.getArgs().addArgument("y", "y mm");

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
    long left;
    long right;

    int nb = 0;
    float x = 0.0;
    float y = 0.0;
    float coordx = 0.0;
    float coordy = 0.0;
    float coorda_deg = 0.0;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    Arguments args = robot.getArgs();
    if (args["nb"] != "0") {
        nb = atoi(args["nb"].c_str());
        logger().debug() << "Arg nb set " << args["nb"] << ", nb = " << nb << logs::end;
    }

    if (args["x"] != "0") {
        x = atof(args["x"].c_str());
        logger().debug() << "Arg x set " << args["x"] << ", x = " << x << logs::end;
    }
    if (args["y"] != "0") {
        y = atof(args["y"].c_str());
        logger().debug() << "Arg y set " << args["y"] << ", y = " << y << logs::end;
    }



    coordx = atof(args['+']["coordx"].c_str());
    coordy = atof(args['+']["coordy"].c_str());
    coorda_deg = atof(args['+']["coorda"].c_str());

    robot.asserv().startMotionTimerAndOdo(true);

    logger().info() << "COORD avec x=" << coordx << " y=" << coordy << " a=" << coorda_deg << logs::end;

    robot.asserv().setPositionAndColor(coordx, coordy, coorda_deg, (robot.getMyColor() != PMXVIOLET));

    robot.svgPrintPosition();

    chrono.start();

    robot.actions().start();
    robot.actions().sensors().addTimerSensors(50);

    logger().info() << "GOTO x=" << x << " y=" << y << logs::end;

    TRAJ_STATE ts;
    int f = 0;
    while ((ts = robot.asserv().doMoveForwardTo(x, y)) != TRAJ_OK) {
        robot.svgPrintPosition();
        robot.asserv().displayTS(ts);

        if (ts == TRAJ_NEAR_OBSTACLE) {
            robot.logger().error() << " ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;

            f++;
            usleep(200000);
        }
        if (ts == TRAJ_COLLISION) {
            robot.logger().error() << "===== COLLISION essai n°" << f << logs::end;

            f++;
        }
        robot.asserv().resetDisplayTS();

    }

    left = robot.asserv().base()->encoders().getLeftEncoder();
    right = robot.asserv().base()->encoders().getRightEncoder();
    logger().info() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= " << right
            << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
            << robot.asserv().pos_getThetaInDegree() << logs::end;

    robot.svgPrintPosition();

    logger().info() << "Happy End." << logs::end;
}
