#include "L_AsservLineRotateTest.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <string>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Arguments.hpp"
#include "../Common/Interface.Driver/AAsservDriver.hpp"
#include "../Common/IA/IAbyPath.hpp"
#include "../Common/Robot.hpp"
#include "../Common/State/Automate.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "L_State_WaitEndOfMatch.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3IAExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_AsservLineRotateTest::configureConsoleArgs(int argc, char **argv) //surcharge
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    //speed
    robot.getArgs().addArgument("s", "speed en %");
    //distance
    robot.getArgs().addArgument("d", "distance mm");
    //coord
    robot.getArgs().addArgument("x", "x mm", "0");
    robot.getArgs().addArgument("y", "y mm", "0");
    //angle
    robot.getArgs().addArgument("a", "angle degre", "0");

    robot.getArgs().addArgument("back", "back:0,1", "0");

    robot.getArgs().addArgument("nb", "nb of time", "1");

    Arguments::Option cOpt('+', "Coordinates x,y,a");
    cOpt.addArgument("coordx", "coord x mm", "324.0");
    cOpt.addArgument("coordy", "coord y mm", "126.0");
    cOpt.addArgument("coorda", "coord teta deg", "90.0"); //TODO ATTENTION ERROR SI (OPOSUL uniquement different de ZERO !!!
    robot.getArgs().addOption(cOpt);

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}
void L_AsservLineRotateTest::execute()
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    //arret d'urgence
    Automate automate_;
    IAutomateState *stateWaitEndOfMatch = new L_State_WaitEndOfMatch();
    automate_.run(robot, stateWaitEndOfMatch);

    logger().info() << "END L_AsservLineRotateTest::execute()" << logs::end;

    this->cancel();
    //exit(0);
}

void L_AsservLineRotateTest::run(int argc, char **argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv);

    int left;
    int right;
    int s = 0;
    float d = 0.0;
    float x = 0.0;
    float y = 0.0;
    int nb = 0.0;
    bool back = false;
    float a = 0.0;
    float coordx = 0.0;
    float coordy = 0.0;
    float coorda_deg = 0.0;
    TRAJ_STATE ts = TRAJ_OK;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    Arguments args = robot.getArgs();
    if (args["s"] != "0") {
        s = atoi(args["s"].c_str());
        logger().info() << "Arg s set " << args["s"] << ", s = " << s << logs::end;
    }
    if (args["d"] != "0") {
        d = atof(args["d"].c_str());
        logger().info() << "Arg d set " << args["d"] << ", d = " << d << logs::end;
    }

    if (args["x"] != "0") {
        x = atof(args["x"].c_str());
        logger().info() << "Arg x set " << args["x"] << ", x = " << x << logs::end;
    }

    if (args["y"] != "0") {
        y = atof(args["y"].c_str());
        logger().info() << "Arg y set " << args["y"] << ", y = " << y << logs::end;
    }

    if (args["a"] != "0") {
        a = atof(args["a"].c_str());
        logger().debug() << "Arg a set " << args["a"] << ", a = " << a << logs::end;
    }

    if (args["nb"] != "0") {
        nb = atoi(args["nb"].c_str());
        logger().info() << "Arg nb set " << args["nb"] << ", nb = " << nb << logs::end;
    }
    coordx = atof(args['+']["coordx"].c_str());
    coordy = atof(args['+']["coordy"].c_str());
    coorda_deg = atof(args['+']["coorda"].c_str());

    if (args["back"] != "0") {
        back = atoi(args["back"].c_str());
        logger().debug() << "Arg back set " << args["back"] << ", back = " << back << logs::end;
    }

    robot.asserv().startMotionTimerAndOdo(true); //true = assistedHandling

    logger().info() << "COORD avec x=" << coordx << " y=" << coordy << " a=" << coorda_deg << logs::end;

    robot.asserv().setPositionAndColor(coordx, coordy, coorda_deg, (robot.getMyColor() != PMXGREEN));
    //utils::sleep_for_micros(300000); //pause pour bien prendre en compte la position ?

    robot.asserv().getEncodersCounts(&right, &left);

    logger().info() << "time= " << robot.chrono().getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= "
            << right << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
            << robot.asserv().pos_getThetaInDegree() << logs::end;

    robot.svgPrintPosition();

    //this->start("L_AsservLineRotateTest"); //ARU

    robot.actions().start();
    robot.actions().sensors().addTimerSensors(200);    //detection adverse toutes les 200ms
    robot.chrono().start();

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

    robot.asserv().setLowSpeedForward(true, s);
    robot.asserv().setLowSpeedBackward(true, s);

    for (int num = 1; num <= nb; num++) {
        ts = TRAJ_OK;
        if (d != 0) {
            //logger().info() << "go...d=" << d << "mm" << logs::end;
            //calcul de coord
            float x_dest = robot.asserv().pos_getX_mm() + cos(robot.asserv().pos_getTheta()) * d;
            float y_dest = robot.asserv().pos_getY_mm() + sin(robot.asserv().pos_getTheta()) * d;
            logger().info() << "go...d=" << d << "mm" << " xdest=" << x_dest << " ydest=" << y_dest << logs::end;
//            if (d >= 0) {
            if (back == 0) {
                robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
                robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

                while (ts != TRAJ_FINISHED) {
                    ts = robot.ia().iAbyPath().whileMoveForwardTo(x_dest, y_dest, false, 3000000, 4, 4, false);
                    if (ts == TRAJ_NEAR_OBSTACLE) {
                        logger().info() << "===== TRAJ_NEAR_OBSTACLE CONFIRMED" << logs::end;
                        robot.asserv().resetEmergencyOnTraj();
                    }
                    if (ts == TRAJ_COLLISION) {
                        logger().info() << "===== COLLISION ASSERV CONFIRMED" << logs::end;
                        robot.asserv().resetEmergencyOnTraj();
                    }
                    //sleep(3);
                    break;
                }

            } else {
                robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
                robot.actions().sensors().setIgnoreBackNearObstacle(true, false, true);
                while (ts != TRAJ_FINISHED) {
                    ts = robot.ia().iAbyPath().whileMoveBackwardTo(x_dest, y_dest, false, 3000000, 4, 4, false);
                    if (ts == TRAJ_NEAR_OBSTACLE) {
                        logger().info() << "===== TRAJ_NEAR_OBSTACLE CONFIRMED" << logs::end;
                        robot.asserv().resetEmergencyOnTraj();
                    }
                    if (ts == TRAJ_COLLISION) {
                        logger().info() << "===== COLLISION ASSERV CONFIRMED" << logs::end;
                        robot.asserv().resetEmergencyOnTraj();
                    }
                    //sleep(3);
                    break;
                }
            }

//            } else if (back == 1) {
//                robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
//                robot.actions().sensors().setIgnoreBackNearObstacle(true, false, true);
//                while (ts != TRAJ_FINISHED) {
//                    ts = robot.ia().iAbyPath().whileMoveBackwardTo(robot.asserv().pos_getX_mm(),
//                            robot.asserv().pos_getY_mm() + d, false, 2000000, 4, 4, false);
//                    if (ts == TRAJ_NEAR_OBSTACLE) {
//                        logger().info() << "===== TRAJ_NEAR_OBSTACLE CONFIRMED" << logs::end;
//                        robot.asserv().resetEmergencyOnTraj();
//                    }
//                    if (ts == TRAJ_COLLISION) {
//                        logger().info() << "===== COLLISION ASSERV CONFIRMED" << logs::end;
//                        robot.asserv().resetEmergencyOnTraj();
//                    }
//                    //sleep(3);
//                    break;
//                }
//            }
        }

        if (!(x == 0 && y == 0)) {
            if (!back) {
                ts = TRAJ_OK;
                logger().info() << "go Forward... x=" << x << ", y=" << y << logs::end;
                robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
                robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);
                while (ts != TRAJ_FINISHED) {
                    //ts = TRAJ_OK;
                    ts = robot.ia().iAbyPath().whileMoveForwardTo(x, y, false, 1000000, 4, 4, true);

                    if (ts == TRAJ_NEAR_OBSTACLE) {
                        logger().error() << "===== TRAJ_NEAR_OBSTACLE CONFIRMED" << logs::end;
                        robot.asserv().resetEmergencyOnTraj(
                                "robot.ia().iAbyPath().whileMoveForwardTo FINAL TRAJ_NEAR_OBSTACLE");
                    }
                    if (ts == TRAJ_COLLISION) {
                        logger().error() << "===== COLLISION ASSERV CONFIRMED" << logs::end;
                        robot.asserv().resetEmergencyOnTraj(
                                "robot.ia().iAbyPath().whileMoveForwardTo FINAL TRAJ_COLLISION");
                    }
                    //sleep(2);
                    break;
                }
                robot.svgPrintPosition();
            } else {
                ts = TRAJ_OK;
                logger().info() << "go Backward... x=" << x << ", y=" << y << logs::end;
                robot.actions().sensors().setIgnoreFrontNearObstacle(true, true, true);
                robot.actions().sensors().setIgnoreBackNearObstacle(true, false, true);
                while (ts != TRAJ_FINISHED) {
                    //ts = TRAJ_OK;
                    ts = robot.ia().iAbyPath().whileMoveBackwardTo(x, y, false, 1000000, 3, 3);

                    //TODO TRAJ_NEAR_OBSTACLE or TRAJ_NEAR_OBSTACLE_REAR ???
                    if (ts == TRAJ_NEAR_OBSTACLE) {
                        logger().error() << "===== TRAJ_NEAR_OBSTACLE CONFIRMED" << logs::end;
                        robot.asserv().resetEmergencyOnTraj(
                                "robot.ia().iAbyPath().whileMoveForwardTo FINAL TRAJ_NEAR_OBSTACLE");
                    }
                    if (ts == TRAJ_COLLISION) {
                        logger().error() << "===== COLLISION ASSERV CONFIRMED" << logs::end;
                        robot.asserv().resetEmergencyOnTraj(
                                "robot.ia().iAbyPath().whileMoveForwardTo FINAL TRAJ_COLLISION");
                    }
                    //sleep(2);
                    break;
                }
                robot.svgPrintPosition();
            }
        }

//        if (a != 0) {
//            ts = TRAJ_OK;
//            while (ts != TRAJ_FINISHED) {
//                //ts = TRAJ_OK;
//                ts = robot.ia().iAbyPath().whileMoveRotateTo(a, 1000000, 2);
//
//                if (ts == TRAJ_NEAR_OBSTACLE) {
//                    logger().error() << "===== TRAJ_NEAR_OBSTACLE CONFIRMED" << logs::end;
//                    robot.asserv().resetEmergencyOnTraj();
//                }
//                if (ts == TRAJ_COLLISION) {
//                    logger().error() << "===== COLLISION ASSERV CONFIRMED" << logs::end;
//                    robot.asserv().resetEmergencyOnTraj();
//                }
//                if (ts != TRAJ_FINISHED) {
//                    logger().error() << "===== OTHERS TS=" << ts << logs::end;
//                    robot.asserv().resetEmergencyOnTraj();
//                }
//                //sleep(3);
//                break;
//            }
//            robot.svgPrintPosition();
//        }
        d += d;
    }
    //utils::sleep_for_micros(3000000);

    ts = TRAJ_OK;
    //robot.asserv().resetEmergencyOnTraj();
//    ts = robot.asserv().doLineAbs(400);
//    logger().info() << "===== doLineAbs(400) TS=" << ts << logs::end;
//    robot.svgPrintPosition();

    //utils::sleep_for_micros(5000000);

    ts = robot.asserv().doAbsoluteRotateTo(-90);
    logger().info() << "===== doAbsoluteRotateTo(-90) TS=" << ts << logs::end;
    robot.svgPrintPosition();

    //utils::sleep_for_micros(300000);
    /*
    ts = robot.asserv().doLineAbs(200);
    logger().info() << "===== doLineAbs(200) TS=" << ts << logs::end;
    robot.svgPrintPosition();
    */
/*
    ts = robot.asserv().doFaceTo(1500, 1500);
    logger().info() << "===== doFaceTo(1500, 1500) TS=" << ts << logs::end;
    robot.svgPrintPosition();
*/
    //utils::sleep_for_micros(300000);
    ts = robot.asserv().doLineAbs(300);
    logger().info() << "===== doLineAbs(300) TS=" << ts << logs::end;
    robot.svgPrintPosition();

    robot.asserv().getEncodersCounts(&right, &left);

    logger().info() << "End time= " << robot.chrono().getElapsedTimeInMilliSec() << "ms ; left= " << left
            << " ; right= " << right << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm()
            << " a=" << robot.asserv().pos_getThetaInDegree() << logs::end;

    robot.svgPrintPosition();
    utils::sleep_for_micros(3000000);
    logger().info() << "Happy End." << logs::end;
}

