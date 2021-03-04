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

void O_AsservEsialTest::run(int argc, char** argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;
    configureConsoleArgs(argc, argv);
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    long left;
    long right;
    int nb = 0;
    int step = 0;

    Arguments args = robot.getArgs();

    if (args["step"] != "0") {
        step = atoi(args["step"].c_str());
        logger().debug() << "Arg step set " << args["step"] << ", step = " << step << logs::end;
    }

    utils::Chronometer chrono("L_AsservEsialTest");

    robot.asserv().startMotionTimerAndOdo(false);
    robot.asserv().setPositionAndColor(0.0, 0.0, 0.0, (robot.getMyColor() != PMXVIOLET));

    OPOS6UL_AsservExtended asserv = robot.asserv();
    EncoderControl extEncoders = asserv.base()->extEncoders();
    chrono.start();

    if (step == 1) {
        logger().info() << "ETAPE 1 : TEST CODEURS - compter sur un mètre" << logs::end;
        //test1 les codeurs sur 1m
        while (1) {

            left = extEncoders.getLeftEncoder(); //TODO a coder pour asserv externe !
            right = extEncoders.getRightEncoder();//TODO a coder pour asserv externe !

            logger().info() << nb << " time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left
                    << " ; right= " << right << " x=" << asserv.pos_getX_mm() << " y=" << asserv.pos_getY_mm() << " a="
                    << asserv.pos_getThetaInDegree() << logs::end;
            usleep(100000);
            nb++;
        }
    }

    if (step == 2) {
        logger().info() << "ETAPE 2 : TEST MOTEURS ET CODEURS" << logs::end;
        //test2 moteurs et codeurs dans le bon sens
        while (1) {
            asserv.base()->motors().runMotorLeft(25, 0);
            asserv.base()->motors().runMotorRight(25, 0);

            left = extEncoders.getLeftEncoder();
            right = extEncoders.getRightEncoder();

            logger().info() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= "
                    << right << " x=" << asserv.pos_getX_mm() << " y=" << asserv.pos_getY_mm() << " a="
                    << asserv.pos_getThetaInDegree() << logs::end;
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
            robot.asserv().assistedHandling();
            sleep(1);
        }
    }

    //test3 on avance de 10cm pour regler le D
    //reste dans la boucle d'attente sir le quadramp n'est pas activée pour terminer
    if (step == 4) {
        logger().info() << "ETAPE 4 : on avance pour regler D" << logs::end;
        robot.asserv().assistedHandling();
        robot.asserv().doLineAbs(200);
        sleep(1);
    }

    if (step == 5) {
        logger().info() << "ETAPE 5 : on tourne pour regler D" << logs::end;
        robot.asserv().assistedHandling();
        robot.asserv().doRotateAbs(90);
        sleep(1);

    }
    //test4 quadramp

    logger().info() << "END t= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= " << right
            << " x=" << asserv.pos_getX_mm() << " y=" << asserv.pos_getY_mm() << " a=" << asserv.pos_getThetaInDegree()
            << logs::end;
    robot.svgPrintPosition();

    logger().info() << "Happy End." << logs::end;
}

