#include "O_State_Init.hpp"

#include "../Common/Action/LcdShield.hpp"
#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

IAutomateState*
O_State_Init::execute(Robot&)
{
    logger().info() << "O_StateInit executing..." << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.actions().start();

    //BEGIN
    begin: robot.actions().lcd2x16().setBacklightOn();
    robot.actions().lcd2x16().clear();
    robot.actions().servo_init();

    robot.strategy("all");




    if (!robot.skipSetup()) {
        logger().info() << "METTRE LA TIRETTE ! " << logs::end;
        robot.actions().lcd2x16().home();
        robot.actions().lcd2x16().print("METTRE LA TIRETTE ! ");
        robot.actions().tirette().waitPressed();
        robot.actions().lcd2x16().clear();

        robot.actions().ledBar().startK2mil(50000, 50000, LED_GREEN, false);

        robot.actions().lcd2x16().clear();
        robot.actions().lcd2x16().home();
        robot.actions().lcd2x16().print("CHOISIR COULEUR + IA...");
        logger().info() << "CHOISIR COULEUR + IA..." << logs::end;
        ButtonTouch b = BUTTON_NONE;
        while (b != BUTTON_BACK_KEY || robot.getMyColor() == PMXNOCOLOR) {
            b = robot.actions().buttonBar().waitOneOfAllPressed();
            if (b == BUTTON_LEFT_KEY) {
                logger().info() << "BUTTON_LEFT_KEY - BLUE" << logs::end;
                robot.actions().lcd2x16().clear();
                robot.actions().lcd2x16().setCursor(0, 1);
                robot.actions().lcd2x16().print("BLUE  ");
                robot.setMyColor(PMXGREEN);
            }
            if (b == BUTTON_RIGHT_KEY) {
                logger().info() << "BUTTON_RIGHT_KEY - ORANGE" << logs::end;
                robot.actions().lcd2x16().clear();
                robot.actions().lcd2x16().setCursor(0, 1);
                robot.actions().lcd2x16().print("ORANGE");
                robot.setMyColor(PMXORANGE);
            }
            if (b == BUTTON_UP_KEY) {
                logger().info() << "BUTTON_UP_KEY - IA" << logs::end;
                //robot.strategy("strat5");
            }
            if (b == BUTTON_DOWN_KEY) {
                logger().info() << "BUTTON_DOWN_KEY - MECA" << logs::end;

            }
        }

        robot.actions().lcd2x16().clear();
        robot.actions().lcd2x16().home();

        robot.actions().ledBar().stopAndWait(true);
        //sortir pince
        if (robot.getMyColor() == PMXORANGE)
            robot.actions().servo_init_orange();
        else
            robot.actions().servo_init_green();

        //tirette
        robot.actions().ledBar().startAlternate(100000, 100000, 0x81, 0x3C, LED_GREEN, false);

        setPos();
        robot.waitForInit(true);


        robot.actions().lcd2x16().clear();
        robot.actions().lcd2x16().print("PMX...WAIT TIRETTE !");
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
                robot.actions().lcd2x16().clear();
                goto begin;
            }
            usleep(100000);
        }

    } else {
        logger().error() << "SKIP SETUP...." << logs::end;
        if (robot.getMyColor() == PMXNOCOLOR) {
            robot.actions().lcd2x16().home();
            robot.actions().lcd2x16().print("NO COLOR... => EXIT !!");
            exit(0);
        }

        robot.actions().lcd2x16().home();
        robot.actions().lcd2x16().print("Skip setup...");

        robot.actions().servo_init();

        robot.strategy("all");

        //sortir pince
        if (robot.getMyColor() == PMXORANGE)
            robot.actions().servo_init_orange();
        else
            robot.actions().servo_init_green();


        setPos();
        robot.waitForInit(true);
        usleep(500000); //simulation attente tirette pour avoir les logs sequentiels
    }

    robot.actions().ledBar().stopAndWait(true);

    robot.actions().ledBar().resetAll();
    robot.actions().lcd2x16().clear();
    robot.actions().lcd2x16().setBacklightOff();

    logger().info() << "O_StateInit executed" << logs::end;
    return this->getState("WaitEndOfMatch"); //return NULL; //finish all state

}

void O_State_Init::setPos()
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.actions().lcd2x16().clear();
    robot.actions().lcd2x16().print("SET POSITION...");

    robot.asserv().startMotionTimerAndOdo(false);
    robot.asserv().setPositionAndColor(70, 190, 0.0, (robot.getMyColor() != PMXORANGE));
    //robot.svgPrintPosition();

    robot.asserv().ignoreFrontCollision(true);
    robot.asserv().ignoreRearCollision(true);
    robot.asserv().assistedHandling();
    robot.asserv().doLineAbs(155);
    //robot.asserv().doLineAbs(-50);


    robot.actions().lcd2x16().print("SET POSITION : OK");
}
