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
    logger().info() << "L_StateInit executing..." << logs::end;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    robot.actions().start();

    //BEGIN
    begin: robot.strategy("all");
    logger().info() << "robot.skipSetup()= " << robot.skipSetup() << logs::end;

    if (!robot.skipSetup()) {
        logger().info() << "METTRE LA TIRETTE ! " << logs::end;

        robot.actions().ledBar().startK2mil(50000, 50000, LED_GREEN, false);
        //robot.actions().tirette().waitPressed();
        ButtonTouch b = BUTTON_NONE;
        while (!robot.actions().tirette().pressed()) {
            usleep(10000);
            b = robot.actions().buttonBar().checkOneOfAllPressed();
            if (b == BUTTON_BACK_KEY) {
                logger().info() << "Exit by User request! " << logs::end;
                robot.actions().ledBar().resetAll();
                robot.actions().ledBar().stopAndWait(true);
                //on quitte le programme!!
                exit(0);
            }
        }
        robot.actions().ledBar().resetAll();

        logger().info() << "CHOISIR COULEUR + IA..." << logs::end;
        b = BUTTON_NONE;
        while (b != BUTTON_ENTER_KEY || robot.getMyColor() == PMXNOCOLOR) {
            b = robot.actions().buttonBar().waitOneOfAllPressed();
            if (b == BUTTON_LEFT_KEY) {
                logger().info() << "BUTTON_LEFT_KEY - YELLOW" << logs::end;
                robot.actions().ledBar().stopAndWait(true);
                robot.actions().ledBar().set(1, LED_YELLOW);
                robot.actions().ledBar().set(0, LED_OFF);
                robot.setMyColor(PMXYELLOW);
            }
            if (b == BUTTON_RIGHT_KEY) {
                logger().info() << "BUTTON_RIGHT_KEY - VIOLET" << logs::end;
                robot.actions().ledBar().stopAndWait(true);
                robot.actions().ledBar().set(0, LED_RED);
                robot.actions().ledBar().set(1, LED_OFF);
                robot.setMyColor(PMXVIOLET);
            }
            if (b == BUTTON_UP_KEY) {
                logger().info() << "BUTTON_UP_KEY - IA" << logs::end;

            }
            if (b == BUTTON_DOWN_KEY) {
                logger().info() << "BUTTON_DOWN_KEY - MECA" << logs::end;
            }
            if (b == BUTTON_BACK_KEY) {
                logger().info() << "Exit by User request! " << logs::end;
                robot.actions().ledBar().resetAll();
                robot.actions().ledBar().stopAndWait(true);
                //on quitte le programme!!
                exit(0);
            }
        }

        //tirette
        if (robot.getMyColor() == PMXVIOLET)
            robot.actions().ledBar().startAlternate(100000, 100000, 0x81, 0x3C, LED_RED, false);
        else
            robot.actions().ledBar().startAlternate(100000, 100000, 0x81, 0x3C, LED_YELLOW, false);

        setPos();
        robot.waitForInit(true);

        logger().info() << "PMX...WAIT TIRETTE !";
        if (robot.getMyColor() == PMXVIOLET)
        {
            logger().info() << " VIOLET";
        }
        else
        {
            logger().info() << "YELLOW";
        }
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

        logger().info() << "SKIP SETUP...." << logs::end;
        if (robot.getMyColor() == PMXNOCOLOR) {
            logger().info() << "PMXNOCOLOR !!!" << logs::end;
            printf("PMXNOCOLOR !!!\n");
            exit(0);
        }

        //attente si la tirette est mise!
        logger().info() << "VERIFICATION TIRETTE...(ne pas oublier de l'enlever!)" << logs::end;
        robot.actions().tirette().waitUnpressed();

        robot.strategy("all");

        setPos();
        robot.waitForInit(true);
        usleep(500000); //simulation attente tirette pour avoir les logs sequentiels
    }

    robot.actions().ledBar().stopAndWait(true);

    robot.actions().ledBar().resetAll();

    logger().info() << "execute is executed" << logs::end;
    //return NULL; //finish all state
    return this->getState("WaitEndOfMatch"); //return NULL; //finish all state
}

void L_State_Init::setPos()
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    logger().info() << "setPos() executing" << logs::end;

    //TODO int des objects


    robot.asserv().startMotionTimerAndOdo(false);
    robot.asserv().setPositionAndColor(45, 718, 0.0, (robot.getMyColor() != PMXVIOLET));
    robot.svgPrintPosition();

    robot.asserv().ignoreFrontCollision(false);
    robot.asserv().ignoreRearCollision(true);

    robot.asserv().assistedHandling();
    robot.svgPrintPosition();
    //init

    robot.asserv().doLineAbs(150);
    robot.asserv().doMoveForwardAndRotateTo(300, 750, 90.0);
    robot.svgPrintPosition();
    logger().info() << "setPos() executed" << logs::end;

}
