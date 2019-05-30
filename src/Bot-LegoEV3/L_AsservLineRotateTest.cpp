#include "L_AsservLineRotateTest.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <string>

#include "../Common/Arguments.hpp"
#include "../Common/Asserv/EncoderControl.hpp"
#include "../Common/Asserv/ExtEncoderControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_AsservLineRotateTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    //distance
    robot.getArgs().addArgument("d", "distance mm");
    //angle
    robot.getArgs().addArgument("a", "angle degre", "0");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void L_AsservLineRotateTest::run(int argc, char** argv)
{
    logger().info() << this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv);

    utils::Chronometer chrono("L_AsservLineRotateTest");
    long left;
    long right;

    float d = 0.0;
    //float y = 0.0;
    float a = 0.0;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    Arguments args = robot.getArgs();

    if (args["d"] != "0") {
        d = atof(args["d"].c_str());
        logger().error() << "Arg d set " << args["d"] << ", d = " << d << logs::end;
    }

    if (args["a"] != "0") {
        a = atof(args["a"].c_str());
        logger().error() << "Arg a set " << args["a"] << ", a = " << a << logs::end;
    }

    robot.setMyColor(PMXVIOLET);
    robot.asserv().startMotionTimerAndOdo(true);
    robot.asserv().setPositionAndColor(0.0, 300.0, 0.0, (robot.getMyColor() != PMXVIOLET));

    left = robot.asserv().base()->extEncoders().getLeftEncoder();
    right = robot.asserv().base()->extEncoders().getRightEncoder();
    logger().info() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= " << right
            << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
            << robot.asserv().pos_getThetaInDegree() << logs::end;

    robot.svgPrintPosition();
    /*
     robot.actions().start();

     robot.actions().tirette().waitPressed();
     logger().error() << "waitPressed() done. wait tirette..."  << logs::end;
     while (robot.actions().tirette().pressed()) {

     usleep(100000);
     }
     logger().error() << "go..."  << logs::end;
     */

    //detection adverse
    robot.actions().start();
    robot.actions().sensors().addTimerSensors(100);
    chrono.start();

    robot.actions().sensors().setIgnoreFrontNearObstacle(false, false, false);
    robot.actions().sensors().setIgnoreBackNearObstacle(false, false, false);

    TRAJ_STATE ts;
    int f = 0;
    int c = 0;
    if (d != 0) {
        logger().info() << "go..." << d << "mm" << logs::end;

        while ((ts = robot.asserv().doMoveForwardTo(d, 300)) != TRAJ_OK) {

            logger().info() << "doMoveForwardTo Interruption dist TRAJ_STATE=" << ts << logs::end;
            robot.svgPrintPosition();
            robot.asserv().displayTS(ts);
            if (ts == TRAJ_NEAR_OBSTACLE) {
                logger().error() << "===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;

                if (f >= 5) {
                    logger().error() << "===== TRAJ_NEAR_OBSTACLE BREAK essai n°" << f << logs::end;
                    break; //return
                }
                f++;

                usleep(1000000);
            }

            if (ts == TRAJ_COLLISION) {
                logger().error() << "===== COLLISION ASSERV essai n°" << c << logs::end;

                if (c >= 2) {
                    logger().error() << "===== COLLISION ASSERV essai BREAK n°" << c << logs::end;
                    break; // ou return;
                }
                printf("c=%d\n", c);
                usleep(1000000);
            }
            usleep(1000000);
            robot.asserv().resetDisplayTS();
        }
        //logger().error() << "===== COLLISION ASSERV essai END WHILE on  ts:" << ts << logs::end;


        left = robot.asserv().base()->encoders().getLeftEncoder();
        right = robot.asserv().base()->encoders().getRightEncoder();
        logger().info() << "1 time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= "
                << right << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
                << robot.asserv().pos_getThetaInDegree() << logs::end;

        robot.svgPrintPosition();
    }
    /*
     if (a != 0) {
     ts = robot.asserv().doRotateAbs(a);
     if (ts != TRAJ_OK) //TRAJ_OK, TRAJ_ERROR, TRAJ_COLLISION, TRAJ_NEAR_OBSTACLE, TRAJ_CANCELLED, TRAJ_INTERRUPTED
     logger().info() << "TURN COLLISION !!!  ts=" << ts << logs::end;

     left = robot.asserv().base()->encoders().getLeftEncoder();
     right = robot.asserv().base()->encoders().getRightEncoder();
     logger().info() << "2 time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= "
     << right << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
     << robot.asserv().pos_getThetaInDegree() << logs::end;

     robot.svgPrintPosition();

     if (d != 0) {
     //2ème ligne droite
     ts = robot.asserv().doLineAbs(d);
     if (ts != TRAJ_OK) //TRAJ_OK, TRAJ_ERROR, TRAJ_COLLISION, TRAJ_NEAR_OBSTACLE, TRAJ_CANCELLED, TRAJ_INTERRUPTED
     logger().info() << "MOVE2 COLLISION !!!  ts=" << ts << logs::end;

     left = robot.asserv().base()->encoders().getLeftEncoder();
     right = robot.asserv().base()->encoders().getRightEncoder();
     logger().info() << "3 time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= "
     << right << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
     << robot.asserv().pos_getThetaInDegree() << logs::end;
     robot.svgPrintPosition();
     }
     }
     sleep(1);*/
    left = robot.asserv().base()->encoders().getLeftEncoder();
    right = robot.asserv().base()->encoders().getRightEncoder();
    logger().info() << "End time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= "
            << right << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
            << robot.asserv().pos_getThetaInDegree() << logs::end;

    robot.svgPrintPosition();

    logger().info() << "Happy End." << logs::end;
}

