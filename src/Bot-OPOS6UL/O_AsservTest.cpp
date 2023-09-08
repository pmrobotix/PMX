#include "O_AsservTest.hpp"

#include <bits/basic_string.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Arguments.hpp"
#include "../Common/Asserv/EncoderControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Interface.Driver/AAsservDriver.hpp"
#include "../Common/IA/IAbyPath.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_IAExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_AsservTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    //robot.getArgs().addArgument("nb", "nbr position", "1");
    robot.getArgs().addArgument("x", "x mm");
    robot.getArgs().addArgument("y", "y mm");
    robot.getArgs().addArgument("x2", "x2 mm", "0");
    robot.getArgs().addArgument("y2", "y2 mm", "0");
    //TODO mettre plus de points x,y

    Arguments::Option cOpt('+', "Coordinates x,y,a");
    cOpt.addArgument("coordx", "coord x mm", "150.0");
    cOpt.addArgument("coordy", "coord y mm", "800.0");
    cOpt.addArgument("coorda", "coord teta mm", "0.0");
    robot.getArgs().addOption(cOpt);

    //reparse again arguments for the specific test
    robot.parseConsoleArgs(argc, argv);

}

void O_AsservTest::run(int argc, char** argv) {
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv);

    float x = 0.0;
    float y = 0.0;
    float x2 = 0.0;
    float y2 = 0.0;
    float coordx = 0.0;
    float coordy = 0.0;
    float coorda_deg = 0.0;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

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

    logger().info() << "COORD avec x=" << coordx << " y=" << coordy << " a=" << coorda_deg << logs::end;

    robot.asserv().startMotionTimerAndOdo(true); //reset et lancement du thread position, attente et moteur à 1 attente
    robot.setMyColor(PMXGREEN);
    while (robot.asserv().pos_getX_mm() < 10) {
        robot.asserv().setPositionAndColor(coordx, coordy, coorda_deg, (robot.getMyColor() != PMXGREEN));
    }


    logger().info() << "setposition done:"
            << " x="
            << robot.asserv().pos_getX_mm()
            << " y="
            << robot.asserv().pos_getY_mm()
            << " a="
            << robot.asserv().pos_getThetaInDegree()
            << " color="
            << robot.getMyColor()
            << logs::end;
    robot.svgPrintPosition();

//    robot.asserv().assistedHandling();
    robot.asserv().setLowSpeedForward(true, 40);


    //detection adverse
    robot.actions().start();
    robot.actions().sensors().addTimerSensors(200);
    robot.chrono().start();

    robot.actions().sensors().setIgnoreFrontNearObstacle(false, true, false);
    robot.actions().sensors().setIgnoreBackNearObstacle(false, true, false);

    logger().info() << "GOTO x=" << x << " y=" << y << logs::end;
/*
    robot.asserv().gotoChain(300, 800);
    robot.asserv().gotoChain(400, 800);
    robot.asserv().gotoChain(500, 800);
    robot.asserv().gotoChain(600, 800);
     TRAJ_STATE ts = robot.asserv().gotoXY(700, 800);


        if (ts != TRAJ_FINISHED) {
                robot.logger().error() << " whileMoveForwardTo  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
                robot.asserv().resetEmergencyOnTraj();

            }
*/




    TRAJ_STATE ts = robot.ia().iAbyPath().whileMoveForwardTo(x, y, false, 2000000, 5, 5, false);
    if (ts != TRAJ_FINISHED) {
        robot.logger().error() << " whileMoveForwardTo  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
        robot.asserv().resetEmergencyOnTraj();
    }
    logger().info() << "END GOTO ts=" << ts << logs::end;

    if (x2 != 0 && y2 != 0) {
        sleep(3);
        logger().info() << "GOTO2 x2=" << x2 << " y2=" << y2 << logs::end;

        TRAJ_STATE ts = robot.ia().iAbyPath().whileMoveForwardTo(x2, y2, false, 1000000, 3, 3, false);
        if (ts != TRAJ_FINISHED) {
            robot.logger().error() << " whileMoveForwardTo x2,y2  ===== PB COLLISION FINALE - Que fait-on? ts=" << ts << logs::end;
            robot.asserv().resetEmergencyOnTraj();
        }
        logger().info() << "END GOTO2 ts=" << ts << logs::end;
    }

    logger().info() << "time= "
            << robot.chrono().getElapsedTimeInMilliSec()
            << "ms "
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
