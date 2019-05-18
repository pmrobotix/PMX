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
    //demarre le actionManagerTimer
    robot.actions().start();

    //BEGIN
    begin:

    robot.actions().lcd().clear();


    if (!robot.skipSetup()) {
        robot.actions().lcd().setCursor(1, 1); //to change the font

        //logger().info() << "METTRE LA TIRETTE ! " << logs::end;
        robot.actions().lcd().display_content_string("METTRE LA TIRETTE", 4);

        robot.actions().ledBar().startK2mil(50000, 50000, LED_GREEN, false);

        ButtonTouch b = BUTTON_NONE;
        while (!robot.actions().tirette().pressed()) {
            b = robot.actions().buttonBar().checkOneOfAllPressed();
            if (b == BUTTON_BACK_KEY) {
                logger().info() << "Exit by User request! " << logs::end;
                robot.actions().ledBar().resetAll();
                robot.actions().ledBar().stopAndWait(true);
                robot.actions().lcd().clear();
                //on quitte le programme!!
                exit(0);
            }
            if (b == BUTTON_ENTER_KEY) {
                break;
            }
            usleep(1000);
        }

        robot.actions().lcd().clear();
        robot.actions().ledBar().resetAll();
        robot.actions().lcd().display_content_string("COLOR ?", 3, 3);

        //logger().info() << "CHOISIR COULEUR + IA..." << logs::end;
        b = BUTTON_NONE;
        int mode = 1; //1,2,3
        int v = 1; //1,2,3 VRR, RVR, RRV
        int st = 1;
        while (b != BUTTON_ENTER_KEY || robot.getMyColor() == PMXNOCOLOR) {

            if (mode == 1) {
                robot.actions().lcd().display_content_string("* ", 3);
                robot.actions().lcd().display_content_string("  ", 4);
                robot.actions().lcd().display_content_string("  ", 5);
                if (b == BUTTON_LEFT_KEY) {
                    //logger().info() << "BUTTON_LEFT_KEY - YELLOW" << logs::end;
                    robot.actions().lcd().display_content_string("YELLOW", 3, 3);
                    robot.actions().ledBar().stopAndWait(true);
                    robot.actions().ledBar().set(1, LED_YELLOW);
                    robot.actions().ledBar().set(0, LED_OFF);
                    robot.setMyColor(PMXYELLOW);
                }
                if (b == BUTTON_RIGHT_KEY) {
                    //logger().info() << "BUTTON_RIGHT_KEY - VIOLET" << logs::end;
                    robot.actions().lcd().display_content_string("VIOLET", 3, 3);
                    robot.actions().ledBar().stopAndWait(true);
                    robot.actions().ledBar().set(0, LED_RED);
                    robot.actions().ledBar().set(1, LED_OFF);
                    robot.setMyColor(PMXVIOLET);
                }
            } else if (mode == 2) {
                logger().debug() << "MODE VRR selected" << logs::end;
                robot.actions().lcd().display_content_string("  ", 3);
                robot.actions().lcd().display_content_string("* ", 4);
                robot.actions().lcd().display_content_string("  ", 5);

                if (b == BUTTON_LEFT_KEY) {
                    logger().debug() << "BUTTON_LEFT_KEY - PREV" << logs::end;
                    v--;
                    if (v <= 0)
                        v = 3;
                }
                if (b == BUTTON_RIGHT_KEY) {
                    logger().debug() << "BUTTON_RIGHT_KEY - NEXT" << logs::end;
                    v++;
                    if (v >= 4)
                        v = 1;
                }

                if (v == 1) {
                    logger().debug() << "mode V R R selected" << logs::end;
                    robot.actions().lcd().display_content_string("V R R", 4, 4);
                    robot.configVRR("VRR");
                } else if (v == 2) {
                    logger().debug() << "mode R V R selected" << logs::end;
                    robot.actions().lcd().display_content_string("R V R", 4, 4);
                    robot.configVRR("RVR");
                } else if (v == 3) {
                    logger().debug() << "mode R R V selected" << logs::end;
                    robot.actions().lcd().display_content_string("R R V", 4, 4);
                    robot.configVRR("RRV");
                }
            } else if (mode == 3) {
                logger().debug() << "MODE STRAT selected" << logs::end;
                robot.actions().lcd().display_content_string("  ", 3);
                robot.actions().lcd().display_content_string("  ", 4);
                robot.actions().lcd().display_content_string("* ", 5);

                if (b == BUTTON_LEFT_KEY) {
                    logger().debug() << "STRAT - PREV" << logs::end;
                    st--;
                    if (st <= 0)
                        st = 3;
                }
                if (b == BUTTON_RIGHT_KEY) {
                    logger().debug() << "STRAT - NEXT" << logs::end;
                    st++;
                    if (st >= 4)
                        st = 1;
                }

                if (st == 1) {
                    logger().debug() << "strat1" << logs::end;
                    robot.strategy("strat1");
                } else if (st == 2) {
                    logger().debug() << "strat2" << logs::end;
                    robot.strategy("strat2");
                } else if (st == 3) {
                    logger().debug() << "strat3" << logs::end;
                    robot.strategy("strat3");
                }
                robot.actions().lcd().display_content_string(robot.strategy(), 5, 3);
            }
            b = robot.actions().buttonBar().waitOneOfAllPressed();
            if (b == BUTTON_UP_KEY) {
                //logger().info() << "BUTTON_UP_KEY - MECA" << logs::end;
                robot.actions().init_servos();
                usleep(300000);
            }
            if (b == BUTTON_DOWN_KEY) {
                //logger().info() << "BUTTON_DOWN_KEY - IA" << logs::end;
                mode++;
                if (mode > 3)
                    mode = 1;
            }
//            if (b == BUTTON_BACK_KEY) {
//                logger().info() << "Exit by User request! " << logs::end;
//                robot.actions().ledBar().resetAll();
//                robot.actions().ledBar().stopAndWait(true);
//                //on quitte le programme!!
//                exit(0);
//            }
        }

        //tirette
        if (robot.getMyColor() == PMXVIOLET)
            robot.actions().ledBar().startAlternate(100000, 100000, 0x81, 0x3C, LED_RED, false);
        else
            robot.actions().ledBar().startAlternate(100000, 100000, 0x81, 0x3C, LED_YELLOW, false);

        setPos();

        robot.waitForInit(true);

        robot.actions().lcd().clear();
        //logger().info() << "PMX...WAIT TIRETTE !";
        if (robot.getMyColor() == PMXVIOLET) {
            //logger().info() << " VIOLET";
            robot.actions().lcd().display_content_string("VIOLET", 3, 2);
        } else {
            //logger().info() << "YELLOW";
            robot.actions().lcd().display_content_string("YELLOW", 3, 2);
        }
        robot.actions().lcd().display_content_string(robot.configVRR(), 4, 2);
        robot.actions().lcd().display_content_string(robot.strategy(), 5, 2);

//        logger().info() << logs::end;
//        logger().info() << "VERIFICATION TIRETTE...(ne pas oublier de l'enlever!)" << logs::end;
//        sleep(1);
//        logger().error() << "AVANT TIRETTE !!!" << logs::end;
        bool bb = false;
        robot.actions().lcd().display_content_string("WAIT TIRETTE...", 6);

        while (robot.actions().tirette().pressed()) {
            bb = robot.actions().buttonBar().pressed(BUTTON_DOWN_KEY);
            if (bb) {
                robot.actions().ledBar().stopAndWait(true);
                robot.asserv().freeMotion();
                robot.actions().lcd().clear();
                goto begin;
            }
        }
    } else {

        robot.actions().lcd().home();
        logger().info() << "SKIP SETUP...." << logs::end;

        if (robot.getMyColor() == PMXNOCOLOR) {
            logger().error() << "PMXNOCOLOR !!!" << logs::end;
            exit(0);
        } else {
            logger().info() << "COLOR is " << (robot.getMyColor() == PMXVIOLET ? "VIOLET" : "YELLOW") << logs::end;
        }

        logger().info() << "ENLEVER TIRETTE !!!" << logs::end;
        //attente si la tirette est mise!
        robot.actions().tirette().waitUnpressed();

        logger().info() << "Strategy is " << robot.strategy() << logs::end;

        setPos();

        robot.waitForInit(true);
        usleep(500000); //simulation attente tirette pour avoir les logs sequentiels
    }
    robot.actions().lcd().clear();
    if (robot.getMyColor() == PMXVIOLET) {

        robot.actions().right_arm_take();
        robot.actions().conveyorBelt_Left_low();
    } else {

        robot.actions().left_arm_take();
        robot.actions().conveyorBelt_Right_low();
    }

    robot.actions().ledBar().stopAndWait(true);

    robot.actions().ledBar().resetAll();

    logger().debug() << "L_State_Init finished" << logs::end;
    return this->getState("WaitEndOfMatch"); //return NULL; to finish all state
}

void L_State_Init::setPos()
{
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    logger().debug() << "setPos() executing" << logs::end;
    robot.actions().lcd().clear();
    robot.actions().lcd().display_content_string("SET POSITION...", 5);

    //init des objects
    robot.actions().init_servos();
//    robot.actions().conveyorBelt_Left_center();
//    robot.actions().left_arm_retract();
//    robot.actions().right_arm_retract();

//    if (robot.getMyColor() == PMXVIOLET) {
//    } else {
//    }
    robot.asserv().startMotionTimerAndOdo(false);
    robot.asserv().setPositionAndColor(45, 771, 0.0, (robot.getMyColor() != PMXVIOLET)); //collé au vert, coté bleu
    robot.svgPrintPosition();

    robot.asserv().ignoreFrontCollision(false);
    robot.asserv().ignoreRearCollision(true);

    robot.asserv().assistedHandling();
    robot.svgPrintPosition();
    //init

    robot.asserv().doLineAbs(150);
    robot.asserv().doMoveForwardAndRotateTo(300, 740, 90.0);
    robot.svgPrintPosition();
    logger().debug() << "setPos() executed" << logs::end;
    robot.actions().lcd().display_content_string("SET POSITION Done.", 6);

}
