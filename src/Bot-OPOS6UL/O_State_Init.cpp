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
    begin:

    robot.actions().lcd2x16().setBacklightOn();
    robot.actions().lcd2x16().clear();

    if (!robot.skipSetup()) {
        //robot.strategy("all");//defaut si BEGIN //TODO utile ?
        ButtonTouch b = BUTTON_NONE;

//        logger().info() << "METTRE LA TIRETTE ! " << logs::end;
//        robot.actions().lcd2x16().home();
//        robot.actions().lcd2x16().print("METTRE LA TIRETTE ! ");
//
//        while (!robot.actions().tirette().pressed()) {
//            b = robot.actions().buttonBar().checkOneOfAllPressed();
//            if (b == BUTTON_BACK_KEY) {
//                robot.actions().lcd2x16().home();
//                robot.actions().lcd2x16().clear();
//                robot.actions().lcd2x16().print("EXIIIIIIT !");
//                robot.actions().ledBar().resetAll();
//                robot.actions().ledBar().stopAndWait(true);
//                usleep(200000);
//                logger().info() << "Exit by User request! " << logs::end;
//                robot.actions().lcd2x16().clear();
//                //on quitte le programme!!
//                exit(0);
//            }
//            if (b == BUTTON_ENTER_KEY) {
//                break;
//            }
//            usleep(1000);
//        }
//
//        robot.actions().lcd2x16().clear();

//robot.actions().ledBar().startK2mil(50000, 50000, LED_GREEN, false);

        robot.actions().lcd2x16().clear();
        robot.actions().lcd2x16().home();
        robot.actions().lcd2x16().print("  COLOR ?");
        logger().info() << "CHOISIR COULEUR + IA..." << logs::end;
        b = BUTTON_NONE;
        int mode = 1; //1,2,3
        int v = 1; //1,2,3 VRR, RVR, RRV
        int st = 1;
        while (b != BUTTON_BACK_KEY || robot.getMyColor() == PMXNOCOLOR) {

            if (mode == 1) {
                logger().info() << "MODE COULEUR selected" << logs::end;
                robot.actions().lcd2x16().setCursor(0, 0);
                robot.actions().lcd2x16().print("* ");
                robot.actions().lcd2x16().setCursor(9, 0);
                robot.actions().lcd2x16().print("  ");
                robot.actions().lcd2x16().setCursor(0, 1);
                robot.actions().lcd2x16().print("  ");

                if (b == BUTTON_LEFT_KEY) {
                    logger().info() << "BUTTON_LEFT_KEY - YELLOW selected" << logs::end;

                    robot.actions().lcd2x16().setCursor(2, 0);
                    robot.actions().lcd2x16().print("YELLOW ");
                    robot.setMyColor(PMXYELLOW);
                }
                if (b == BUTTON_RIGHT_KEY) {
                    logger().info() << "BUTTON_RIGHT_KEY - BLUE selected" << logs::end;

                    robot.actions().lcd2x16().setCursor(2, 0);
                    robot.actions().lcd2x16().print("BLUE ");
                    robot.setMyColor(PMXBLUE);
                }
            } else if (mode == 2) {
                logger().info() << "MODE VRR selected" << logs::end;
                robot.actions().lcd2x16().setCursor(0, 0);
                robot.actions().lcd2x16().print("  ");
                robot.actions().lcd2x16().setCursor(9, 0);
                robot.actions().lcd2x16().print("* ");
                robot.actions().lcd2x16().setCursor(0, 1);
                robot.actions().lcd2x16().print("  ");
                if (b == BUTTON_LEFT_KEY) {
                    logger().info() << "BUTTON_LEFT_KEY - PREV" << logs::end;
                    v--;
                    if (v <= 0)
                        v = 3;
                }
                if (b == BUTTON_RIGHT_KEY) {
                    logger().info() << "BUTTON_RIGHT_KEY - NEXT" << logs::end;
                    v++;
                    if (v >= 4)
                        v = 1;
                }
                robot.actions().lcd2x16().setCursor(11, 0);
                if (v == 1) {
                    logger().info() << "mode V R R selected" << logs::end;
                    robot.actions().lcd2x16().print("V R R");
                    robot.configVRR("VRR");
                } else if (v == 2) {
                    logger().info() << "mode R V R selected" << logs::end;
                    robot.actions().lcd2x16().print("R V R");
                    robot.configVRR("RVR");
                } else if (v == 3) {
                    logger().info() << "mode R R V selected" << logs::end;
                    robot.actions().lcd2x16().print("R R V");
                    robot.configVRR("RRV");
                }

            } else if (mode == 3) {
                logger().info() << "MODE STRAT selected" << logs::end;
                robot.actions().lcd2x16().setCursor(0, 0);
                robot.actions().lcd2x16().print("  ");
                robot.actions().lcd2x16().setCursor(9, 0);
                robot.actions().lcd2x16().print("  ");
                robot.actions().lcd2x16().setCursor(0, 1);
                robot.actions().lcd2x16().print("* ");
                robot.actions().lcd2x16().setCursor(1, 1);

                if (b == BUTTON_LEFT_KEY) {
                    logger().info() << "STRAT - PREV" << logs::end;
                    st--;
                    if (st <= 0)
                        st = 3;
                }
                if (b == BUTTON_RIGHT_KEY) {
                    logger().info() << "STRAT - NEXT" << logs::end;
                    st++;
                    if (st >= 4)
                        st = 1;
                }

                robot.actions().lcd2x16().setCursor(2, 1);
                if (st == 1) {
                    logger().info() << "tabletest" << logs::end;
                    robot.actions().lcd2x16().print("tabletest");
                    robot.strategy("tabletest");

                } else if (st == 2) {
                    logger().info() << "strat2" << logs::end;
                    robot.actions().lcd2x16().print("strat2");
                    robot.strategy("strat2");
                } else if (st == 3) {
                    logger().info() << "strat3" << logs::end;
                    robot.actions().lcd2x16().print("strat3");
                    robot.strategy("strat3");
                }

            } else if (mode == 4) {
//                logger().info() << "ejecter les balles" << logs::end;
//                robot.actions().lancer_les_balles(30);
//                robot.actions().aspiration_lacher_les_balles();
//                usleep(6000000);
//                robot.actions().aspiration_closed_init(-1);
//                robot.actions().stopper_lanceur_de_balles();
            }
            b = robot.actions().buttonBar().waitOneOfAllPressed();

            if (b == BUTTON_UP_KEY) {
                logger().info() << "BUTTON_UP_KEY - MECA" << logs::end;

                robot.actions().ax12_init();

                //activation de la turbine pour mettre les balles
                //robot.actions().turbine_aspiration(true);

                utils::sleep_for_micros(300000);
            }
            if (b == BUTTON_DOWN_KEY) {
                //arret de la turbine
                //robot.actions().turbine_aspiration(false);
                mode++;
                if (mode > 4)
                    mode = 1;
                logger().info() << "BUTTON_DOWN_KEY - MODE changed to " << mode << logs::end;

            }
            utils::sleep_for_micros(10000);
        }
        robot.actions().lcd2x16().clear();
        robot.actions().lcd2x16().home();
        if (robot.getMyColor() == PMXNOCOLOR) {
            robot.actions().lcd2x16().home();
            robot.actions().lcd2x16().print("NO COLOR... => EXIIIIT !!");

            robot.actions().ledBar().resetAll();
            robot.actions().ledBar().stop(true);
            utils::sleep_for_micros(060000);
            logger().info() << "Exit user - No color! " << logs::end;
            robot.actions().lcd2x16().clear();
            //on quitte le programme!!
            exit(0);
        }
        robot.actions().ledBar().stop(true);

        //position et init servo
        setPos();

//TODO TESTER pk ca pose problème ???
        //robot.actions().ledBar().startAlternate(100000, 100000, 0x81, 0x3C, LED_GREEN, false);

        logger().info() << "METTRE LA TIRETTE ! " << logs::end;
        robot.actions().lcd2x16().home();
        robot.actions().lcd2x16().print("METTRE LA TIRETTE ! ");

        b = BUTTON_NONE;
        while (!robot.actions().tirette().pressed()) {
            b = robot.actions().buttonBar().checkOneOfAllPressed();
            if (b == BUTTON_BACK_KEY) {
                robot.actions().lcd2x16().home();
                robot.actions().lcd2x16().clear();

//                robot.actions().lcd2x16().print("RESTART !");
//                robot.actions().ledBar().resetAll();
//                robot.actions().ledBar().stopAndWait(true);
//                goto begin;

                //version on quitte le prog
                robot.actions().lcd2x16().print("EXIIIIIIT !");
                robot.actions().ledBar().resetAll();
                robot.actions().ledBar().stop(true);
                utils::sleep_for_micros(200000);
                logger().info() << "Exit by User request! " << logs::end;
                robot.actions().lcd2x16().clear();
                //on quitte le programme!!
                exit(0);
            }
            if (b == BUTTON_ENTER_KEY) {
                break;
            }
            utils::sleep_for_micros(500);
        }

        //ATTENTE TIRETT

        robot.waitForInit(true);
        robot.actions().lcd2x16().clear();
        robot.actions().lcd2x16().setCursor(0, 1);
        robot.actions().lcd2x16().print("WAITING TIRETT...");
        logger().info() << "PMX...WAIT TIRETTE !!!!!!!!!!!!!!!";
        if (robot.getMyColor() == PMXBLUE) {
            robot.actions().lcd2x16().setCursor(0, 0);
            robot.actions().lcd2x16().print("BLU");
            logger().info() << "BLUE  ";
        } else {
            robot.actions().lcd2x16().setCursor(0, 0);
            robot.actions().lcd2x16().print("YEL");
            logger().info() << "YELLOW";
        }
        logger().info() << logs::end;

        robot.actions().lcd2x16().setCursor(4, 0);

        robot.actions().lcd2x16().print(robot.configVRR());

        robot.actions().lcd2x16().setCursor(8, 0);
        robot.actions().lcd2x16().print(robot.strategy());

        //ATTENTE TIRETTE LOOP !!!!
        bool bb = false;
        while (robot.actions().tirette().pressed()) { //WARNING renvoi toujours 0 en SIMU
            bb = robot.actions().buttonBar().pressed(BUTTON_DOWN_KEY);
            if (bb) {
                robot.actions().ledBar().stop(true);
                robot.actions().lcd2x16().clear();
                robot.asserv().freeMotion();
                goto begin;
            }
        }

    } else {
        logger().info() << "SKIP SETUP...." << logs::end;
        if (robot.getMyColor() == PMXNOCOLOR) {
            robot.actions().lcd2x16().home();
            robot.actions().lcd2x16().print("NO COLOR... => EXIIIIT !!");

            robot.actions().ledBar().resetAll();
            robot.actions().ledBar().stop(true);

            utils::sleep_for_micros(1000000);
            logger().info() << "Exit by No color! " << logs::end;
            robot.actions().lcd2x16().clear();
            //on quitte le programme!!
            exit(0);
        } else {
            logger().info() << "COLOR is " << (robot.getMyColor() == PMXBLUE ? "BLUE" : "YELLOW ") << logs::end;
        }

        robot.actions().lcd2x16().home();
        robot.actions().lcd2x16().print("Skip setup...");

        logger().info() << "Strategy is " << robot.strategy() << logs::end;

        setPos();

        logger().info() << "END SETUP !" << logs::end;
        robot.waitForInit(true);
        //usleep(2000000); //simulation attente tirette pour avoir les logs sequentiels
    }

    robot.actions().ledBar().stop(true);

    robot.actions().ledBar().resetAll();
    robot.actions().lcd2x16().clear();
    robot.actions().lcd2x16().setBacklightOn();
    robot.actions().lcd2x16().setCursor(0, 0);
    robot.actions().lcd2x16().home();
    robot.actions().lcd2x16().print("GO...");

    logger().info() << "ax12" << logs::end;
//    robot.actions().ax12_left_cil(0);
//    robot.actions().ax12_right_cil();

    //robot.points += 5; //init
    robot.displayPoints();

    logger().info() << "O_StateInit executed" << logs::end;
    return this->getState("WaitEndOfMatch"); //return NULL; //finish all state

}

void O_State_Init::setPos()
{
    logger().info() << "O_State_Init::setPos() executing" << logs::end;
    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.actions().lcd2x16().clear();
    robot.actions().lcd2x16().print("SET POSITION...");

    robot.asserv().startMotionTimerAndOdo(true);
    if (robot.strategy() == "tabletest")
        robot.asserv().setPositionAndColor(130, 900 + 130, 0.0, (robot.getMyColor() != PMXBLUE));
    else
        robot.asserv().setPositionAndColor(130, 900 + 130, 0.0, (robot.getMyColor() != PMXBLUE));
    logger().info() << "O_State_Init::setPos() svgPrintPosition x=" << robot.asserv().pos_getX_mm() << " y="
            << robot.asserv().pos_getY_mm() << " a=" << robot.asserv().pos_getThetaInDegree() << logs::end;
    robot.svgPrintPosition();

    robot.actions().lcd2x16().clear();

    robot.asserv().setLowSpeedForward(false, 0); //au cas où par les sensors (si pas de ARU) //a voir si on ne peut pas le mettre ailleurs à l'init

    robot.actions().sensors().setIgnoreFrontNearObstacle(true, false, true);
    robot.actions().sensors().setIgnoreBackNearObstacle(true, true, true);

    //robot.asserv().resetDisplayTS();
    robot.asserv().assistedHandling();
    TRAJ_STATE ts = TRAJ_OK;
    robot.asserv().setLowSpeedForward(true, 50); //35 battery et 50 secteur
    ts = robot.asserv().doLineAbs(100);
    robot.asserv().setLowSpeedForward(true, 50);
    //robot.asserv().setLowSpeedForward(true, 100);

    //if (robot.strategy() != "tabletest")
    ts = robot.asserv().doMoveForwardTo(300, 1100);
    ts = robot.asserv().doFaceTo(850, 850);
    robot.actions().ax12_init();

    /*
     ts = robot.asserv().doLineAbs(200);
     robot.actions().ax12_init();
     ts = robot.asserv().doMoveForwardTo(600, 450);
     robot.actions().ax12_init();
     ts = robot.asserv().doMoveBackwardAndRotateTo(70 + 150, 450 + 13, 0.0);
     */
    //robot.asserv().displayTS(ts);
    robot.svgPrintPosition();
    robot.actions().lcd2x16().println("SET POSITION : OK");

    /*
     robot.actions().ax12_bras_droit(0);
     robot.actions().ax12_bras_gauche(-1);
     robot.actions().ax12_bras_droit_init(0);
     robot.actions().ax12_bras_gauche_init(-1);*/
   /* robot.actions().ax12_bras_droit(0);
    robot.actions().ax12_bras_gauche(-1);
    robot.actions().ax12_bras_droit_init(0);
    robot.actions().ax12_bras_gauche_init(-1);
    //utils::sleep_for_secs(3);
    robot.actions().ax12_bras_droit(0);
    robot.actions().ax12_bras_gauche(-1);
    robot.actions().ax12_bras_droit_init(0);
    robot.actions().ax12_bras_gauche_init(-1);
    //utils::sleep_for_secs(2);*/
    /*

    robot.actions().ax12_bras_droit(0);
    robot.actions().ax12_bras_gauche(-1);
    robot.actions().ax12_bras_droit_init(0);
    robot.actions().ax12_bras_gauche_init(-1);
    //utils::sleep_for_secs(1);
    robot.actions().ax12_bras_droit(0);
    robot.actions().ax12_bras_gauche(-1);
    robot.actions().ax12_bras_droit_init(0);
    robot.actions().ax12_bras_gauche_init(-1);
    */
}
