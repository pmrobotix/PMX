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
    begin: robot.strategy("all");
    robot.actions().lcd2x16().setBacklightOn();
    robot.actions().lcd2x16().clear();

//    robot.actions().servo_init();

    if (!robot.skipSetup()) {
        logger().info() << "METTRE LA TIRETTE ! " << logs::end;
        robot.actions().lcd2x16().home();
        robot.actions().lcd2x16().print("METTRE LA TIRETTE ! ");
        //robot.actions().tirette().waitPressed();
        ButtonTouch b = BUTTON_NONE;
        while (!robot.actions().tirette().pressed()) {
            usleep(10000);
            b = robot.actions().buttonBar().checkOneOfAllPressed();
            if (b == BUTTON_BACK_KEY) {
                robot.actions().lcd2x16().home();
                robot.actions().lcd2x16().clear();
                robot.actions().lcd2x16().print("EXIIIIIIT !");
                robot.actions().ledBar().resetAll();
                robot.actions().ledBar().stopAndWait(true);
                usleep(500000);
                logger().info() << "Exit by User request! " << logs::end;
                //on quitte le programme!!
                exit(0);
            }
        }

        robot.actions().lcd2x16().clear();

        robot.actions().ledBar().startK2mil(50000, 50000, LED_GREEN, false);

        robot.actions().lcd2x16().clear();
        robot.actions().lcd2x16().home();
        robot.actions().lcd2x16().print("CHOISIR COULEUR + IA...");
        logger().info() << "CHOISIR COULEUR + IA..." << logs::end;
        b = BUTTON_NONE;
        while (b != BUTTON_BACK_KEY || robot.getMyColor() == PMXNOCOLOR) {
            b = robot.actions().buttonBar().waitOneOfAllPressed();
            if (b == BUTTON_LEFT_KEY) {
                logger().info() << "BUTTON_LEFT_KEY - YELLOW" << logs::end;
                robot.actions().lcd2x16().clear();
                robot.actions().lcd2x16().setCursor(0, 1);
                robot.actions().lcd2x16().print("YELLOW ");
                robot.setMyColor(PMXYELLOW);
            }
            if (b == BUTTON_RIGHT_KEY) {
                logger().info() << "BUTTON_RIGHT_KEY - VIOLET" << logs::end;
                robot.actions().lcd2x16().clear();
                robot.actions().lcd2x16().setCursor(0, 1);
                robot.actions().lcd2x16().print("VIOLET");
                robot.setMyColor(PMXVIOLET);
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
//        if (robot.getMyColor() == PMXVIOLET)
//            robot.actions().servo_init_orange();
//        else
//            robot.actions().servo_init_green();

//tirette
        robot.actions().ledBar().startAlternate(100000, 100000, 0x81, 0x3C, LED_GREEN, false);

        setPos();
        robot.waitForInit(true);

        robot.actions().lcd2x16().clear();
        robot.actions().lcd2x16().print("WAIT TIRETTE...");
        logger().info() << "PMX...WAIT TIRETTE !";
        if (robot.getMyColor() == PMXVIOLET) {
            robot.actions().lcd2x16().setCursor(0, 1);
            robot.actions().lcd2x16().print("=>  VIOLET !");
            logger().info() << " VIOLET";
        } else {
            robot.actions().lcd2x16().setCursor(0, 1);
            robot.actions().lcd2x16().print("=>  YELLOW !");
            logger().info() << "YELLOW";
        }
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

//        robot.actions().servo_init();

        robot.strategy("all");

        setPos();

        //sortir pince
//        if (robot.getMyColor() == PMXVIOLET)
//            robot.actions().servo_init_orange();
//        else
//            robot.actions().servo_init_green();

        robot.waitForInit(true);
        usleep(500000); //simulation attente tirette pour avoir les logs sequentiels
    }

    robot.actions().ledBar().stopAndWait(true);

    robot.actions().ledBar().resetAll();
    robot.actions().lcd2x16().clear();
    //robot.actions().lcd2x16().setBacklightOff();
    robot.actions().lcd2x16().setBacklightOn();
    robot.actions().lcd2x16().setCursor(0, 0);
    robot.actions().lcd2x16().home();
    robot.actions().lcd2x16().print("GO...");

    logger().info() << "on ferme les cils" << logs::end;
    robot.actions().ax12_left_cil(0);
    robot.actions().ax12_right_cil();

    robot.points += 40; //35 +5
    robot.displayPoints(robot.points);

    logger().info() << "O_StateInit executed" << logs::end;
    return this->getState("WaitEndOfMatch"); //return NULL; //finish all state

}

void O_State_Init::setPos()
{
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.actions().lcd2x16().clear();
    robot.actions().lcd2x16().print("SET POSITION...");

    robot.actions().ax12_leftHand();
    robot.actions().ax12_rightHand();
    robot.actions().ax12_left_cil_retract();
    robot.actions().ax12_right_cil_retract(-1);

    robot.actions().ax12_leftHand_retract();
    robot.actions().ax12_rightHand_retract();
    robot.actions().ax12_left_cil();
    robot.actions().ax12_right_cil(-1);

    robot.actions().ax12_left_cil_retract();
    robot.actions().ax12_right_cil_retract();

    //demi largeur 150
    robot.asserv().startMotionTimerAndOdo(false);

    robot.actions().lcd2x16().clear();
    robot.asserv().setPositionAndColor(70, 450, 0.0, (robot.getMyColor() != PMXVIOLET));
    robot.svgPrintPosition();

    robot.asserv().setLowSpeed(false); //au cas où par les sensors (si pas de ARU) //a voir si on ne peut pas le mettre ailleurs à l'init

    robot.asserv().ignoreFrontCollision(true);
    robot.asserv().ignoreRearCollision(true);
    robot.asserv().resetDisplayTS();
    robot.asserv().assistedHandling();
    TRAJ_STATE ts = robot.asserv().doLineAbs(130);
    robot.asserv().displayTS(ts);
    robot.svgPrintPosition();
    robot.actions().lcd2x16().println("SET POSITION : OK");
}
