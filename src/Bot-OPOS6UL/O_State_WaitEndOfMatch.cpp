#include "O_State_WaitEndOfMatch.hpp"

#include <unistd.h>

#include "../Common/Action/LcdShield.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "../Thread/Thread.hpp"
#include "O_State_DecisionMakerIA.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

IAutomateState* O_State_WaitEndOfMatch::execute(Robot&)
{

    logger().info() << "O_State_WaitEndOfMatch executing..." << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    //démarrage du chrono et des taches du decision maker
    logger().info() << "Start Chronometer" << logs::end;
    robot.chrono().start();

    while (robot.chrono().getElapsedTimeInSec() <= 95) {


        std::this_thread::sleep_for(std::chrono::seconds(1));

        long time = robot.chrono().getElapsedTimeInSec();
        this->logger().info() << "O_State_Wait90SecAction::execute chrono " << time << logs::end;
        robot.actions().lcd2x16().setCursor(0, 1);
        robot.actions().lcd2x16().print(time);
        robot.actions().lcd2x16().print(" sec");
        //indique le temps en binaire
        //robot.actions().ledBar().flash(time, LED_GREEN);
    }

    //release dans tous les cas
//    robot.actions().ax12_left_cil_release(-1);
//    robot.actions().ax12_left_cil_release(-1);
//    robot.actions().ax12_left_cil_release(-1);

    robot.actions().ax12_drapeaux();

    robot.points += 10;
    robot.displayPoints();

/*
    //TODO danse de fin ?
    robot.actions().ax12_bras_droit(0);
    robot.actions().ax12_bras_gauche(-1);
    robot.actions().ax12_bras_droit_init(0);
    robot.actions().ax12_bras_gauche_init(-1);
    robot.actions().ax12_bras_droit(0);
    robot.actions().ax12_bras_gauche(-1);

    robot.actions().ax12_bras_droit(0);
    robot.actions().ax12_bras_gauche(-1);
    robot.actions().ax12_bras_droit_init(0);
    robot.actions().ax12_bras_gauche_init(-1);
    robot.actions().ax12_bras_droit(0);
    robot.actions().ax12_bras_gauche(-1);
    robot.actions().ax12_bras_droit_init(0);
    robot.actions().ax12_bras_gauche_init(-1);*/

    this->logger().info() << "O_State_Wait90SecAction::stopping... "
            << robot.chrono().getElapsedTimeInSec() << logs::end;
    robot.freeMotion();

    robot.end90s(true); //indique que l'action est effectuée au prog princ
    logger().info() << "cancel decisionmaker" << logs::end;
    robot.decisionMaker_->cancel();

    robot.actions().releaseAll();
    robot.freeMotion();

    robot.stopExtraActions(); //stop specific actions, sensors, can take time for servos...
    robot.svgPrintEndOfFile();

    logger().info() << "Display Points after 100sec" << logs::end;

    robot.actions().lcd2x16().clear();

    robot.actions().lcd2x16().setBacklightOn();
    robot.actions().lcd2x16().setCursor(0, 0);
    robot.actions().lcd2x16().print(robot.points);
    robot.actions().lcd2x16().print(" points ?");

    robot.actions().lcd2x16().setCursor(0, 1);
    robot.actions().lcd2x16().print("ooooxxyyyy !");

    robot.actions().ledBar().flashAll(LED_GREEN);

    robot.actions().ledBar().k2mil(2, 50000, LED_GREEN);


    ButtonTouch b = BUTTON_NONE;
    while (1) {
        b = robot.actions().buttonBar().checkOneOfAllPressed();

        if (b == BUTTON_BACK_KEY) {
            logger().info() << "BUTTON_BACK_KEY" << logs::end;
            break;
        }
        if (b == BUTTON_ENTER_KEY) {
            logger().info() << "BUTTON_BACK_KEY" << logs::end;
            break;
        }


        std::this_thread::sleep_for(std::chrono::microseconds(1000));
        //usleep(1000);
    }

    logger().info() << "O_State_WaitEndOfMatch executed " << robot.chrono().getElapsedTimeInSec() << " sec"
            << logs::end;

    return NULL; //finish all state
}
