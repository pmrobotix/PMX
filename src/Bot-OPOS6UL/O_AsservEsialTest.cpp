#include "O_AsservEsialTest.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <string>

#include "../Common/Arguments.hpp"
#include "../Common/Asserv/MotorControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_AsservEsialTest::configureConsoleArgs(int argc, char** argv) // a appeler dans le run ci-dessous
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    //step
    robot.getArgs().addArgument("step", "n step test");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void O_AsservEsialTest::run(int argc, char** argv) {
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv);
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    //OPOS6UL_AsservExtended &asserv = robot.asserv();
    int left = 0;
    int right = 0;
    int nb = 0;
    int step = 0;

    Arguments args = robot.getArgs();

    if (args["step"] != "0") {
        step = atoi(args["step"].c_str());
        logger().debug() << "Arg step set " << args["step"] << ", step = " << step << logs::end;
    }

    utils::Chronometer chrono("L_AsservEsialTest");

    robot.asserv().setPositionAndColor(100.0, 800.0, 0.0, (robot.getMyColor() != PMXGREEN));
    robot.asserv().startMotionTimerAndOdo(true);

    robot.svgPrintPosition();

    /*
     robot.asserv().doLineAbs(200);
     robot.svgPrintPosition();
     robot.asserv().doRelativeRotateBy(20);
     robot.svgPrintPosition();
     robot.asserv().doLineAbs(100);
     robot.svgPrintPosition();
     robot.asserv().doFaceTo(step, 1200);
     robot.svgPrintPosition();
     robot.asserv().doLineAbs(300);
     robot.svgPrintPosition();
     robot.asserv().gotoXY(1000,1000);
     robot.svgPrintPosition();
     robot.asserv().doAbsoluteRotateTo(-45);
     robot.svgPrintPosition();
     robot.asserv().gotoReverse(1200, 1400);
     robot.svgPrintPosition();
     utils::sleep_for_secs(2);
     */
//    OPOS6UL_AsservExtended asserv = robot.asserv();
//    EncoderControl extEncoders = asserv.base()->extEncoders();
    chrono.start();

    if (step == 0) {
        //set position
        logger().info() << "set position..." << logs::end;
        robot.asserv().setPositionAndColor(300, 500, 0.0, (robot.getMyColor() != PMXGREEN));

        while (1) {

            robot.asserv().getEncodersCounts(&right, &left); //accumulated encoders
            RobotPosition p = robot.asserv().pos_getPosition();
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
            utils::sleep_for_micros(100000);
            nb++;
        }
    }

    if (step == 1) {
        logger().info() << "ETAPE 1 : TEST CODEURS - compter sur un mètre" << logs::end;
        //test1 les codeurs sur 1m
        while (1) {

            robot.asserv().getEncodersCounts(&right, &left); //accumulated encoders
            RobotPosition p = robot.asserv().pos_getPosition();
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
            utils::sleep_for_micros(100000);
            nb++;
        }
    }

    if (step == 2) {
        logger().info() << "ETAPE 2 : TEST MOTEURS ET CODEURS" << logs::end;
        //test2 moteurs et codeurs dans le bon sens
        while (1) {
            robot.asserv().runMotorLeft(25, 0);
            robot.asserv().runMotorRight(25, 0);

            robot.asserv().getEncodersCounts(&right, &left); //accumulated encoders
            RobotPosition p = robot.asserv().pos_getPosition();
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
            if (nb > 50) break;
        }
    }
    //test2 quadramp desactivé on regle le P
    //distance avec    disableDistanceRegu = true; //faire le reglage en automatique dans cette fonction
    //puis angle
    if (step == 3) {
        logger().info() << "ETAPE 3 : assistedHandling pour regler P" << logs::end;
        while (1) {
            robot.asserv().assistedHandling();
            sleep(1);
        }
    }

    //test3 on avance de 10cm pour regler le D
    //reste dans la boucle d'attente sir le quadramp n'est pas activée pour terminer
    if (step == 4) {
        logger().info() << "ETAPE 4 : on avance pour regler D" << logs::end;
        robot.asserv().assistedHandling();
        robot.asserv().doLineAbs(100);
        sleep(1);
    }

    if (step == 5) {
        logger().info() << "ETAPE 5 : on tourne pour regler D" << logs::end;
        robot.asserv().assistedHandling();
        robot.asserv().doRotateAbs(90);
        sleep(1);

    }
    //test4 quadramp

//    logger().info() << "END t= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= " << right
//            << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a=" << robot.asserv().pos_getThetaInDegree()
//            << logs::end;
    //robot.svgPrintPosition();

    logger().info() << "Happy End." << logs::end;
}

