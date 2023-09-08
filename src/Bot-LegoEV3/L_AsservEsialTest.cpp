#include "L_AsservEsialTest.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <string>

#include "../Common/Arguments.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_AsservEsialTest::configureConsoleArgs(int argc, char **argv) // a appeler dans le run ci-dessous
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    robot.getArgs().addArgument("step", "n step test");
    robot.getArgs().addArgument("dist", "dist mm", "40");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void L_AsservEsialTest::run(int argc, char **argv) {
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv);
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    int left = 0;
    int right = 0;
    int nb = 0;
    int step = 0;
    int dist = 0;

    Arguments args = robot.getArgs();

    if (args["step"] != "0") {
        step = atoi(args["step"].c_str());
        logger().debug() << "Arg step set " << args["step"] << ", step = " << step << logs::end;
    }
    if (args["dist"] != "0") {
        dist = atoi(args["dist"].c_str());
        logger().debug() << "Arg dist set " << args["dist"] << ", dist = " << dist << logs::end;
    }

    robot.asserv().startMotionTimerAndOdo(false);
    robot.asserv().setPositionAndColor(0.0, 0.0, 0.0, (robot.getMyColor() != PMXGREEN));

    robot.chrono().start();

    if (step == 0) {
        logger().info() << "ETAPE 0: TEST ASSERVDRIVER - test" << logs::end;

        while (1) {
            robot.asserv().freeMotion();
//            robot.asserv().runMotorLeft(dist, -1);
//            robot.asserv().runMotorRight(dist, -1);

            robot.asserv().runMotorLeft(dist, 3500);
            robot.asserv().runMotorRight(150, 3500);

            usleep(1000000);
            robot.asserv().getEncodersCounts(&right, &left); //accumulated encoders

            ROBOTPOSITION p = robot.asserv().pos_getPosition();
            logger().info() << "time= "
                    << robot.chrono().getElapsedTimeInMilliSec()
                    << "ms ; left= "
                    << left
                    << " ; right= "
                    << right
                    << " x="
                    << p.x
                    << " y="
                    << p.y
                    << " deg="
                    << p.theta * 180.0 / M_PI
                    << logs::end;

            nb++;
            if (nb > 0) break;
        }

    }
    if (step == 1) {
        logger().info() << "ETAPE 1: TEST CODEURS DIRECT - compter sur un mètre" << logs::end;
        //test0 les codeurs
        while (1) {
            robot.asserv().getEncodersCounts(&right, &left); //accumulated encoders
            ROBOTPOSITION p = robot.asserv().pos_getPosition();
            logger().info() << nb
                    << " time= "
                    << robot.chrono().getElapsedTimeInMilliSec()
                    << "ms ; left(2)= "
                    << left
                    << " ; right(1)= "
                    << right
                    << "\tx="
                    << p.x
                    << " y="
                    << p.y
                    << " deg="
                    << p.theta * 180.0 / M_PI

                    << logs::end;
            usleep(1000000);
            nb++;
        }
    }

    if (step == 2) {
        logger().info() << "ETAPE 2 : TEST MOTEURS ET CODEURS" << logs::end;
        //test2 moteurs et codeurs dans le bon sens
        while (1) {
            robot.asserv().runMotorLeft(dist, -1);
            robot.asserv().runMotorRight(dist, -1);

            robot.asserv().getEncodersCounts(&right, &left); //accumulated encoders
            ROBOTPOSITION p = robot.asserv().pos_getPosition();
            logger().info() << "time= "
                    << robot.chrono().getElapsedTimeInMilliSec()
                    << "ms ; left= "
                    << left
                    << " ; right= "
                    << right
                    << " x="
                    << p.x
                    << " y="
                    << p.y
                    << " deg="
                    << p.theta * 180.0 / M_PI
                    << logs::end;
            usleep(100000);
            nb++;
            if (nb > 20) break;
        }
    }

    //test2 quadramp desactivé on regle le P
    //distance avec    disableDistanceRegu = true; //faire le reglage en automatique dans cette fonction
    //puis angle
    if (step == 3) {
        logger().info() << "ETAPE 3 : assistedHandling pour regler P" << logs::end;
        while (1) {
            robot.asserv().disablePID(); //allow to deactivate QuadRamp
            robot.asserv().assistedHandling();

            sleep(3);
        }
    }

    //test3 on avance de 10cm pour regler le D
    //reste dans la boucle d'attente sir le quadramp n'est pas activée pour terminer
    if (step == 4) {
        logger().info() << "ETAPE 4 : on avance pour regler D" << logs::end;
        robot.asserv().disablePID(); //allow to deactivate QuadRamp
        robot.asserv().assistedHandling();
        robot.asserv().doLineAbs(dist);
        sleep(1);
    }

    if (step == 5) {
        logger().info() << "ETAPE 5 : on tourne pour regler D" << logs::end;
        robot.asserv().disablePID(); //allow deactivate QuadRamp
        robot.asserv().assistedHandling();
        robot.asserv().doRotateAbs(dist);
        sleep(1);
    }

    if (step == 6) {
        logger().info() << "ETAPE 6 : on avance avec QuadRamp de " << dist << logs::end;
        robot.asserv().assistedHandling();
        robot.asserv().doLineAbs(dist);
        sleep(1);
    }

    if (step == 7) {
        logger().info() << "ETAPE 7 : on tourne avec QuadRamp de " << dist << logs::end;
        robot.asserv().assistedHandling();
        robot.asserv().doRotateAbs(dist);
        sleep(1);
    }

    robot.asserv().getEncodersCounts(&right, &left);
    ROBOTPOSITION p = robot.asserv().pos_getPosition();
    logger().info() << "END time= "
            << robot.chrono().getElapsedTimeInMilliSec()
            << "ms ; left= "
            << left
            << " ; right= "
            << right
            << " x="
            << p.x
            << " y="
            << p.y
            << " deg="
            << p.theta * 180.0 / M_PI
            << logs::end;

    robot.svgPrintPosition();

    logger().info() << "Happy End." << logs::end;

}

