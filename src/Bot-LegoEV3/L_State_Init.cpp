#include "L_State_Init.hpp"

#include <unistd.h>
#include <cstdlib>

#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/LedBar.hpp"
#include "../Common/Action/Tirette.hpp"
#include "../Common/Action.Driver/AButtonDriver.hpp"
#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

IAutomateState*
L_State_Init::execute(Robot&)
{
    logger().info() << "O_StateInit executing..." << logs::end;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    robot.actions().start();

    //BEGIN
    begin: robot.strategy("all");

    if (!robot.skipSetup()) {
        logger().info() << "METTRE LA TIRETTE ! " << logs::end;

        robot.actions().ledBar().startK2mil(50000, 50000, LED_GREEN, false);
        robot.actions().tirette().waitPressed();
        robot.actions().ledBar().resetAll();

        logger().info() << "CHOISIR COULEUR + IA..." << logs::end;
        ButtonTouch b = BUTTON_NONE;
        while (b != BUTTON_BACK_KEY || robot.getMyColor() == PMXNOCOLOR) {
            b = robot.actions().buttonBar().waitOneOfAllPressed();
            if (b == BUTTON_LEFT_KEY) {
                logger().info() << "BUTTON_LEFT_KEY - GREEN" << logs::end;
                robot.actions().ledBar().stopAndWait(true);
                robot.actions().ledBar().set(1, LED_GREEN);
                robot.actions().ledBar().set(0, LED_OFF);
                robot.setMyColor(PMXGREEN);
            }
            if (b == BUTTON_RIGHT_KEY) {
                logger().info() << "BUTTON_RIGHT_KEY - ORANGE" << logs::end;
                robot.actions().ledBar().stopAndWait(true);
                robot.actions().ledBar().set(0, LED_ORANGE);
                robot.actions().ledBar().set(1, LED_OFF);

                robot.setMyColor(PMXORANGE);
            }
            if (b == BUTTON_UP_KEY) {
                logger().info() << "BUTTON_UP_KEY - IA" << logs::end;

            }
            if (b == BUTTON_DOWN_KEY) {
                logger().info() << "BUTTON_DOWN_KEY - MECA" << logs::end;
            }
        }

        //tirette
        if (robot.getMyColor() == PMXORANGE)
            robot.actions().ledBar().startAlternate(100000, 100000, 0x81, 0x3C, LED_ORANGE, false);
        else
            robot.actions().ledBar().startAlternate(100000, 100000, 0x81, 0x3C, LED_GREEN, false);

        //setPos();
        robot.waitForInit(true);

        logger().info() << "PMX...WAIT TIRETTE !";
        if (robot.getMyColor() == PMXORANGE)
            logger().info() << " ORANGE";
        else
            logger().info() << "GREEN";
        logger().info() << logs::end;

        bool bb = false;
        while (robot.actions().tirette().pressed()) {
            bb = robot.actions().buttonBar().pressed(BUTTON_DOWN_KEY);
            if (bb) {
                robot.actions().ledBar().stopAndWait(true);

                goto begin;
            }
            usleep(100000);
        }
    } else {
        logger().error() << "SKIP SETUP...." << logs::end;
        if (robot.getMyColor() == PMXNOCOLOR) {

            exit(0);
        }

        robot.strategy("all");

        setPos();
        robot.waitForInit(true);
        usleep(500000); //simulation attente tirette pour avoir les logs sequentiels
    }


    robot.actions().ledBar().stopAndWait(true);

    robot.actions().ledBar().resetAll();

    logger().info() << "O_StateInit executed" << logs::end;
    //return NULL; //finish all state
    return this->getState("WaitEndOfMatch"); //return NULL; //finish all state
}

void L_State_Init::setPos()
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    /*
     robot.asserv().startMotionTimerAndOdo(false);*/
    robot.asserv().setPositionAndColor(70, 210, 0.0, (robot.getMyColor() != PMXORANGE));
    robot.svgPrintPosition();

    robot.asserv().ignoreFrontCollision(false);
    robot.asserv().ignoreRearCollision(true);
    /*
     robot.asserv().assistedHandling();
     robot.asserv().doLineAbs(145);*/

}
