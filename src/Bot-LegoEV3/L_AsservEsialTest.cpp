#include "L_AsservEsialTest.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <string>

#include "../Common/Arguments.hpp"
#include "../Common/Asserv/ExtEncoderControl.hpp"
#include "../Common/Asserv/MotorControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_AsservEsialTest::configureConsoleArgs(int argc, char** argv) // a appeler dans le run ci-dessous
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    //step
    robot.getArgs().addArgument("step", "n step test");

    //reparse arguments
    robot.parseConsoleArgs(argc, argv);
}

void L_AsservEsialTest::run(int argc, char** argv)
{
    logger().info() << "N° "<< this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv);
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    long left;
    long right;
    int nb = 0;
    int step = 0;

    Arguments args = robot.getArgs();

    if (args["step"] != "0") {
        step = atoi(args["step"].c_str());
        logger().debug() << "Arg step set " << args["step"] << ", step = " << step << logs::end;
    }

    //utils::Chronometer chrono("L_AsservEsialTest");

    robot.asserv().startMotionTimerAndOdo(false);
    robot.asserv().setPositionAndColor(0.0, 0.0, 0.0, (robot.getMyColor() != PMXVIOLET));

    //LegoEV3AsservExtended asserv = robot.asserv();
    //ExtEncoderControl extEncoders = asserv.base()->extEncoders();
    robot.chrono().start();

    if (step == 1) {
        logger().info() << "ETAPE 1 : TEST CODEURS - compter sur un mètre" << logs::end;
        //test1 les codeurs sur 1m
        while (1) {

            left = robot.asserv().base()->extEncoders().getLeftEncoder();
            right = robot.asserv().base()->extEncoders().getRightEncoder();

            logger().info() << nb << " time= " << robot.chrono().getElapsedTimeInMilliSec() << "ms ; left= " << left
                    << " ; right= " << right << " x=" << robot.asserv().pos_getX_mm() << " y="
                    << robot.asserv().pos_getY_mm() << " a=" << robot.asserv().pos_getThetaInDegree() << logs::end;
            usleep(100000);
            nb++;
        }
    }

    if (step == 2) {
        logger().info() << "ETAPE 2 : TEST MOTEURS ET CODEURS" << logs::end;
        //test2 moteurs et codeurs dans le bon sens
        while (1) {
            robot.asserv().base()->motors().runMotorLeft(25, 0);
            robot.asserv().base()->motors().runMotorRight(25, 0);

            left = robot.asserv().base()->extEncoders().getLeftEncoder();
            right = robot.asserv().base()->extEncoders().getRightEncoder();

            logger().info() << "time= " << robot.chrono().getElapsedTimeInMilliSec() << "ms ; left= " << left
                    << " ; right= " << right << " x=" << robot.asserv().pos_getX_mm() << " y="
                    << robot.asserv().pos_getY_mm() << " a=" << robot.asserv().pos_getThetaInDegree() << logs::end;
            usleep(100000);
            nb++;
            if (nb > 50)
                break;
        }
    }
    //test2 quadramp desactivé on regle le P
    //distance avec    disableDistanceRegu = true; //faire le reglage en automatique dans cette fonction
    //puis angle
    if (step == 3) {
        logger().info() << "ETAPE 3 : assistedHandling pour regler P" << logs::end;
        while (1) {
            robot.asserv().disablePID(); //TODO deactivate QuadRamp
            robot.asserv().assistedHandling();
            sleep(1);
        }
    }

    //test3 on avance de 10cm pour regler le D
    //reste dans la boucle d'attente sir le quadramp n'est pas activée pour terminer
    if (step == 4) {
        logger().info() << "ETAPE 4 : on avance pour regler D" << logs::end;
        robot.asserv().disablePID(); //deactivate QuadRamp
        robot.asserv().assistedHandling();
        robot.asserv().doLineAbs(200);
        sleep(1);
    }

    if (step == 5) {
        logger().info() << "ETAPE 5 : on tourne pour regler D" << logs::end;
        robot.asserv().disablePID(); //deactivate QuadRamp
        robot.asserv().assistedHandling();
        robot.asserv().doRotateAbs(90);
        sleep(1);

    }

    left = robot.asserv().base()->extEncoders().getLeftEncoder();
    right = robot.asserv().base()->extEncoders().getRightEncoder();
    logger().info() << "END t= " << robot.chrono().getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= "
            << right << " x=" << robot.asserv().pos_getX_mm() << " y=" << robot.asserv().pos_getY_mm() << " a="
            << robot.asserv().pos_getThetaInDegree() << logs::end;

    robot.svgPrintPosition();

    logger().info() << "Happy End." << logs::end;

}

