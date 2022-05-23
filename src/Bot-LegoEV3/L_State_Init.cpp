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
    logger().debug() << "L_StateInit executing..." << logs::end;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
    //demarre le actionManagerTimer
    robot.actions().start();

    //BEGIN
    begin:

    robot.actions().lcd().clear();

    if (!robot.skipSetup()) {
        robot.actions().lcd().setCursor(1, 1); //to change the font

//        logger().info() << "METTRE LA TIRETTE ! " << logs::end;
//        robot.actions().lcd().display_content_string("METTRE LA TIRETTE", 4);
//
        robot.actions().ledBar().startTimerK2mil(50000, 50000, LED_GREEN, false);
//        int nb_tirette = 0;
        ButtonTouch b = BUTTON_NONE;
//        while (nb_tirette < 3) {
//            int tirette = robot.actions().tirette().pressed();
//            if (tirette == 1)
//                nb_tirette++;
//            else
//                nb_tirette = 0;
//
//            b = robot.actions().buttonBar().checkOneOfAllPressed();
//            if (b == BUTTON_BACK_KEY) {
//                logger().info() << "Exit by User request! " << logs::end;
//                robot.actions().ledBar().resetAll();
//                robot.actions().ledBar().stopAndWait(true);
//                robot.actions().lcd().clear();
//                //on quitte le programme!!
//                exit(0);
//            }
//            if (b == BUTTON_ENTER_KEY) {
//                break;
//            }
//            usleep(1000);
//        }

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
                    robot.actions().lcd().clear();
                    //logger().info() << "BUTTON_LEFT_KEY - YELLOW" << logs::end;
                    robot.actions().lcd().display_content_string("YELLOW ", 3, 3);

                    robot.actions().ledBar().stop(true);
                    robot.actions().ledBar().set(1, LED_YELLOW);
                    robot.actions().ledBar().set(0, LED_OFF);
                    robot.setMyColor(PMXYELLOW);
                }
                if (b == BUTTON_RIGHT_KEY) {

                    robot.actions().lcd().clear();
                    //logger().info() << "BUTTON_RIGHT_KEY - VIOLET" << logs::end;
                    robot.actions().lcd().display_content_string("VIOLET ", 3, 3);
                    robot.actions().lcd().setCursor(0, 0);
                    robot.actions().ledBar().stop(true);
                    robot.actions().ledBar().set(0, LED_RED);
                    robot.actions().ledBar().set(1, LED_OFF);
                    robot.setMyColor(PMXVIOLET);
                }
            }
            else if (mode == 2) {
                logger().debug() << "MODE VRR selected" << logs::end;
                robot.actions().lcd().display_content_string("  ", 3);
                robot.actions().lcd().display_content_string("* ", 4);
                robot.actions().lcd().display_content_string("  ", 5);

                if (b == BUTTON_LEFT_KEY) {
                    logger().debug() << "BUTTON_LEFT_KEY - PREV" << logs::end;
                    v--;
                    if (v <= 0) v = 3;
                }
                if (b == BUTTON_RIGHT_KEY) {
                    logger().debug() << "BUTTON_RIGHT_KEY - NEXT" << logs::end;
                    v++;
                    if (v >= 4) v = 1;
                }

                if (v == 1) {
                    logger().debug() << "mode V R R selected" << logs::end;
                    robot.actions().lcd().display_content_string("V R R", 4, 4);
                    robot.configVRR("VRR");
                }
                else if (v == 2) {
                    logger().debug() << "mode R V R selected" << logs::end;
                    robot.actions().lcd().display_content_string("R V R", 4, 4);
                    robot.configVRR("RVR");
                }
                else if (v == 3) {
                    logger().debug() << "mode R R V selected" << logs::end;
                    robot.actions().lcd().display_content_string("R R V", 4, 4);
                    robot.configVRR("RRV");
                }
            }
            else if (mode == 3) {
                logger().debug() << "MODE STRAT selected" << logs::end;
                robot.actions().lcd().display_content_string("  ", 3);
                robot.actions().lcd().display_content_string("  ", 4);
                robot.actions().lcd().display_content_string("* ", 5);

                if (b == BUTTON_LEFT_KEY) {
                    logger().debug() << "STRAT - PREV" << logs::end;
                    st--;
                    if (st <= 0) st = 3;
                }
                if (b == BUTTON_RIGHT_KEY) {
                    logger().debug() << "STRAT - NEXT" << logs::end;
                    st++;
                    if (st >= 4) st = 1;
                }

                if (st == 1) {
                    logger().debug() << "strat1" << logs::end;
                    robot.strategy("strat1 ");
                }
                else if (st == 2) {
                    logger().debug() << "strat2" << logs::end;
                    robot.strategy("strat2 ");
                }
                else if (st == 3) {
                    logger().debug() << "strat3" << logs::end;
                    robot.strategy("strat3 ");
                }
                robot.actions().lcd().display_content_string(robot.strategy(), 5, 3);
            }
            b = robot.actions().buttonBar().waitOneOfAllPressed();
            if (b == BUTTON_UP_KEY) {
                //logger().info() << "BUTTON_UP_KEY - MECA" << logs::end;

                if (robot.getMyColor() == PMXYELLOW) {
                    robot.actions().arm_right_deploy(0);
                }
                else {
                    robot.actions().arm_left_deploy(0);
                }

                if (robot.getMyColor() == PMXYELLOW) {
                    robot.actions().square_push_right(1500);
                    robot.actions().square_middle_init(1500);
                }
                else {
                    robot.actions().square_push_left(1500);
                    robot.actions().square_middle_init(1500);
                }

                if (robot.getMyColor() == PMXYELLOW) {
                    robot.actions().arm_right_init(0);
                }
                else {
                    robot.actions().arm_left_init(0);
                }

                robot.actions().init_servos();
                usleep(2000000);
                robot.actions().init_mettre_cube();
            }
            if (b == BUTTON_DOWN_KEY) {
                //logger().info() << "BUTTON_DOWN_KEY - IA" << logs::end;
                mode++;
                if (mode > 3) mode = 1;
            }
//            if (b == BUTTON_BACK_KEY) {
//                logger().info() << "Exit by User request! " << logs::end;
//                robot.actions().ledBar().resetAll();
//                robot.actions().ledBar().stopAndWait(true);
//                //on quitte le programme!!
//                exit(0);
//            }
            usleep(100000);
        }

        setPos();

        logger().info() << "METTRE LA TIRETTE ! " << logs::end;
        robot.actions().lcd().display_content_string("METTRE LA TIRETTE", 4);

        //robot.actions().ledBar().startK2mil(50000, 50000, LED_GREEN, false);
        int nb_tirette = 0;
        b = BUTTON_NONE;
        while (nb_tirette < 3) {
            int tirette = robot.actions().tirette().pressed();
            if (tirette == 1) nb_tirette++;
            else nb_tirette = 0;

            b = robot.actions().buttonBar().checkOneOfAllPressed();
            if (b == BUTTON_BACK_KEY) {
                logger().info() << "Exit by User request! " << logs::end;
                robot.actions().ledBar().resetAll();
                robot.actions().ledBar().stop(true);
                robot.actions().lcd().clear();
                //on quitte le programme!!
                exit(0);
            }
            if (b == BUTTON_ENTER_KEY) {
                break;
            }
            usleep(1000);
        }

        //tirette
        if (robot.getMyColor() == PMXYELLOW) robot.actions().ledBar().startTimerAlternate(100000, 100000, 0x81, 0x3C, LED_YELLOW, false);
        else robot.actions().ledBar().startTimerAlternate(100000, 100000, 0x81, 0x3C, LED_RED, false);

        robot.waitForInit(true);

        robot.actions().lcd().clear();
        //logger().info() << "PMX...WAIT TIRETTE !";
        if (robot.getMyColor() == PMXYELLOW) {
            //logger().info() << "YELLOW ";
            robot.actions().lcd().display_content_string("YELLOW", 3, 2);
        }
        else {
            //logger().info() << "VIOLET ";
            robot.actions().lcd().display_content_string("VIOLET", 3, 2);
        }
        robot.actions().lcd().display_content_string(robot.configVRR(), 4, 2);
        robot.actions().lcd().display_content_string(robot.strategy(), 5, 2);

//        logger().info() << logs::end;
//        logger().info() << "VERIFICATION TIRETTE...(ne pas oublier de l'enlever!)" << logs::end;
//        sleep(1);
//        logger().error() << "AVANT TIRETTE !!!" << logs::end;
        bool bb = false;
        robot.actions().lcd().display_content_string("WAIT TIRETTE...", 6);

        nb_tirette = 0;
        while (nb_tirette < 3) {
            int tirette = robot.actions().tirette().pressed();
            if (tirette == 0) nb_tirette++;
            else nb_tirette = 0;

            //TODO ajouter le test de servo si bouton up pendant l'attente tirette
            //robot.actions().init_servos();

            bb = robot.actions().buttonBar().pressed(BUTTON_DOWN_KEY);
            if (bb) {
                robot.actions().ledBar().stop(true);
                robot.asserv().freeMotion();
                robot.actions().lcd().clear();
                goto begin;
            }
        }
    }
    else {

        robot.actions().lcd().home();
        logger().info() << "SKIP SETUP...." << logs::end;

        if (robot.getMyColor() == PMXNOCOLOR) {
            logger().error() << "PMXNOCOLOR !!!" << logs::end;
            exit(0);
        }
        else {
            logger().info() << "COLOR is " << (robot.getMyColor() == PMXYELLOW ? "YELLOW" : "VIOLET  ") << logs::end;
        }

        logger().info() << "ENLEVER TIRETTE !!!" << logs::end;
        //attente si la tirette est mise!
        robot.actions().tirette().waitUnpressed();

        logger().info() << "Strategy is " << robot.strategy() << logs::end;

        logger().info() << "setPos() executing" << logs::end;

        setPos();

        robot.waitForInit(true);
        usleep(500000); //simulation attente tirette pour avoir les logs sequentiels
    }
    robot.actions().lcd().clear();

    robot.actions().ledBar().stop(true);

    robot.actions().ledBar().resetAll();

    logger().debug() << "WaitEndOfMatch launched" << logs::end;
    return this->getState("WaitEndOfMatch"); //return NULL; to finish all state
}

void L_State_Init::setPos() {
    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    //largeur robot 247mm (sans les petites roues noires +5mm)

    logger().debug() << "setPos() executing" << logs::end;
    robot.actions().lcd().clear();
    robot.actions().lcd().display_content_string("SET POSITION...", 5);

    //init des objects
    robot.actions().init_servos();

    //reset encoders
    robot.asserv().resetEncoders();

    robot.asserv().startMotionTimerAndOdo(false); //demarrage asserv sans assistedhandling
    robot.asserv().setPositionAndColor(126, 1590 - 247 / 2.0, 0.0, (robot.getMyColor() != PMXYELLOW)); //au coin du distributeur
    robot.svgPrintPosition();

    //active l'asservissement
    robot.asserv().assistedHandling();
    robot.svgPrintPosition();

    robot.asserv().doLineAbs(100);

    robot.asserv().doMoveForwardTo(260, 1150);

//Faire un faceTo sur le point de destination
    //    if (robot.getMyColor() == PMXYELLOW) {
    //    //robot.asserv().doFaceTo(700, 1600); //TODO, pas de transcription de jaune et violet ?
    //    }else
    //    {
    //        //robot.asserv().doFaceTo(700, 1600);
    //    }

    robot.svgPrintPosition();
    logger().debug() << "setPos() executed" << logs::end;
    robot.actions().lcd().display_content_string("SET POS Done.", 6);

}
