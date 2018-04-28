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

    //démarrage du chrono
    logger().info() << "Start Chronometer" << logs::end;
    robot.chrono().start();
    /*
     //TODO check ARU and adversary here ?
     while (robot.chrono().getElapsedTimeInSec() <= 100) //todo mettre en parametre
     {

     usleep(1000000);
     long time = robot.chrono().getElapsedTimeInSec();
     this->logger().info() << "O_State_Wait90SecAction::execute chrono "
     << time << logs::end;

     robot.actions().ledBar().flash(time, LED_GREEN);

     }

     this->logger().info() << "O_State_Wait90SecAction::execute end100s...stop... "
     << robot.chrono().getElapsedTimeInSec() << logs::end;
     robot.asserv_default->freeMotion();

     robot.end90s(true); //indique que l'action est effectuée au prog princ

     */

    /*
     robot.actions().servoObjects().releaseAll();

     robot.freemotion();
     usleep(500000);
     robot.stopAll();
     exit(0);*/

    int i = 0;
    while (1) {
        i++;

        if (i == 4) {
            logger().info() << "cancel decisionmaker" << i << logs::end;
            robot.decisionMaker_->cancel();
            //init robot for end
            robot.freeMotion(); //stop the robot
            robot.stopActions(); //stop specific actions, can take time for servos...

        }

        if (i == 10)
            break;

        //logger().info() << "O_State_WaitEndOfMatch ---- " << i << logs::end;
        sleep(1);
    }


    logger().info() << "print lcd during sec" << logs::end;
    robot.actions().lcd2x16().clear();
    robot.actions().lcd2x16().print("Yeahhh OK");
    robot.actions().ledBar().flashAll(LED_GREEN);

    robot.actions().ledBar().startK2mil(10, 50000,LED_GREEN, false);
    sleep(3);
    logger().info() << "O_State_WaitEndOfMatch executed " << robot.chrono().getElapsedTimeInSec() << " sec" << logs::end;

    robot.stopAll(); //stop asserv and actionManagerTimer
    return NULL; //finish all state
}
