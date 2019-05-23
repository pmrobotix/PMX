#include "L_MovingBaseTest.hpp"

#include <unistd.h>
#include <cstdlib>
#include <string>

#include "../Common/Arguments.hpp"
#include "../Common/Asserv/EncoderControl.hpp"
#include "../Common/Asserv/MotorControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_MovingBaseTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    robot.getArgs().addArgument("LR", "L(0) or R(1) or LR(2)", "2");
    robot.getArgs().addArgument("p", "power", "20");
    robot.getArgs().addArgument("d", "distance ticks", "500");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void L_MovingBaseTest::run(int argc, char** argv)
{
    logger().info() << this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv); //on appelle les parametres specifiques pour ce test

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    Arguments args = robot.getArgs();

    int LR = 0;
    int d = 0;
    int p = 0;
    if (args["LR"] != "0") {
        LR = atoi(args["LR"].c_str());
        logger().info() << "Arg LR set " << args["LR"] << ", LR = " << LR << logs::end;
    }
    if (args["p"] != "0") {
        p = atoi(args["p"].c_str());
        logger().info() << "Arg p set " << args["p"] << ", p = " << p << " power" << logs::end;
    }
    if (args["d"] != "0") {
        d = atoi(args["d"].c_str());
        logger().info() << "Arg d set " << args["d"] << ", d = " << d << " ticks" << logs::end;
    }

    //robot.asserv().startMotionTimerAndOdo(true); //just to give odometry

    robot.asserv().setPositionAndColor(0.0, 300.0, 0.0, (robot.getMyColor() != PMXVIOLET));

    robot.svgPrintPosition();

    utils::Chronometer chrono("L_MovingBaseTest");

    long left = 0;
    long right = 0;

    logger().info() << "runMotor(" << p << ", 1000) " << logs::end;
    if (LR == 0 || LR == 2)
        robot.asserv().base()->motors().runMotorLeft(p, 1000); //run (power 128) pendant 1s
    if (LR == 1 || LR == 2)
        robot.asserv().base()->motors().runMotorRight(p, 1000); //run (power 128) pendant 1s
    chrono.start();
    while (chrono.getElapsedTimeInMilliSec() < 500.0) {
        left = robot.asserv().base()->encoders().getLeftEncoder();
        right = robot.asserv().base()->encoders().getRightEncoder();
        logger().info() << "time= " << chrono.getElapsedTimeInMilliSec() << " Lticks= " << left << " Rticks= " << right
                << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
                << robot.asserv().pos_getThetaInDegree() << logs::end;
        usleep(200000);
    }

    logger().info() << "STOPPED time= " << chrono.getElapsedTimeInMilliSec() << " Lticks= "
            << robot.asserv().base()->encoders().getLeftEncoder() << " Rticks= "
            << robot.asserv().base()->encoders().getRightEncoder() << " x=" << robot.asserv().pos_getX_mm() << " y="
            << robot.asserv().pos_getY_mm() << " a=" << robot.asserv().pos_getThetaInDegree() << logs::end;
    robot.svgPrintPosition();

    usleep(1000000);

    logger().info() << "runMotor(-" << p << ", 1000) " << logs::end;
    if (LR == 0 || LR == 2)
        robot.asserv().base()->motors().runMotorLeft(-p, 1000); //run with power -100 pendant 1s
    if (LR == 1 || LR == 2)
        robot.asserv().base()->motors().runMotorRight(-p, 1000); //run with power -100 pendant 1s
    chrono.start();
    while (chrono.getElapsedTimeInMilliSec() < 500.0) {
        left = robot.asserv().base()->encoders().getLeftEncoder();
        right = robot.asserv().base()->encoders().getRightEncoder();
        logger().info() << "time= " << chrono.getElapsedTimeInMilliSec() << " Lticks= " << left << " Rticks= " << right
                << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
                << robot.asserv().pos_getThetaInDegree() << logs::end;
        usleep(200000);
    }

    logger().info() << "STOPPED time= " << chrono.getElapsedTimeInMilliSec() << " Lticks= "
            << robot.asserv().base()->encoders().getLeftEncoder() << " Rticks= "
            << robot.asserv().base()->encoders().getRightEncoder() << " x=" << robot.asserv().pos_getX_mm() << " y="
            << robot.asserv().pos_getY_mm() << " a=" << robot.asserv().pos_getThetaInDegree() << logs::end;

    robot.svgPrintPosition();
    /*
     usleep(1000000);

     //TODO setMotorLeftPosition ici c'est absolu, ajouter le relatif.

     int dist_ticks = 1000;
     logger().info() << "setMotorPosition(" << p << ", " << d << ") " << logs::end;
     if (LR == 0 || LR == 2)
     robot.asserv().base()->motors().setMotorLeftPosition(p, d); //run until dist_ticks ticks

     if (LR == 1 || LR == 2)
     robot.asserv().base()->motors().setMotorRightPosition(p, d); //run until dist_ticks ticks
     chrono.start();
     left = robot.asserv().base()->encoders().getLeftEncoder();
     right = robot.asserv().base()->encoders().getRightEncoder();
     while (left < dist_ticks)
     {
     left = robot.asserv().base()->encoders().getLeftEncoder();
     right = robot.asserv().base()->encoders().getRightEncoder();
     logger().info() << "time= "
     << chrono.getElapsedTimeInMilliSec()
     << " Lticks= "
     << left
     << " Rticks= "
     << right
     << " x="
     << robot.asserv().pos_getX_mm()
     << " y="
     << robot.asserv().pos_getY_mm()
     << " a="
     << robot.asserv().pos_getThetaInDegree()
     << logs::end;
     usleep(200000);
     }
     logger().info() << "STOPPED time= "
     << chrono.getElapsedTimeInMilliSec()
     << " Lticks= "
     << robot.asserv().base()->encoders().getLeftEncoder()
     << " Rticks= "
     << robot.asserv().base()->encoders().getRightEncoder()
     << " x="
     << robot.asserv().pos_getX_mm()
     << " y="
     << robot.asserv().pos_getY_mm()
     << " a="
     << robot.asserv().pos_getThetaInDegree()
     << logs::end;
     robot.svgPrintPosition();
     */
    /*
     usleep(1000000);

     int dist_ticks2 = 100;
     logger().info() << "setMotorPosition(-50, " << dist_ticks2 << ") " << logs::end;
     robot.asserv().base()->motors().setMotorLeftPosition(-500, dist_ticks2); //run until dist_ticks2 ticks
     robot.asserv().base()->motors().setMotorRightPosition(-500, dist_ticks2); //run until dist_ticks2 ticks
     chrono.start();
     while (left > dist_ticks - dist_ticks2)
     {
     left = robot.asserv().base()->encoders().getLeftEncoder();
     right = robot.asserv().base()->encoders().getRightEncoder();
     logger().info() << "time= "
     << chrono.getElapsedTimeInMilliSec()
     << " Lticks= "
     << left
     << " Rticks= "
     << right
     << " x="
     << robot.asserv().pos_getX_mm()
     << " y="
     << robot.asserv().pos_getY_mm()
     << " a="
     << robot.asserv().pos_getThetaInDegree()
     << logs::end;
     usleep(200000);
     }
     logger().info() << "STOPPED time= "
     << chrono.getElapsedTimeInMilliSec()
     << " Lticks= "
     << robot.asserv().base()->encoders().getLeftEncoder()
     << " Rticks= "
     << robot.asserv().base()->encoders().getRightEncoder()
     << " x="
     << robot.asserv().pos_getX_mm()
     << " y="
     << robot.asserv().pos_getY_mm()
     << " a="
     << robot.asserv().pos_getThetaInDegree()
     << logs::end;
     robot.svgPrintPosition();
     */

    logger().info() << "Happy End." << logs::end;
}

