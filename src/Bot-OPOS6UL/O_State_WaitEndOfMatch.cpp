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

    //TODO check ARU and adversary here ?
    while (robot.chrono().getElapsedTimeInSec() <= 100)
    {

        usleep(1000000);
        long time = robot.chrono().getElapsedTimeInSec();
        this->logger().info() << "O_State_Wait90SecAction::execute chrono " << time << logs::end;

        robot.actions().ledBar().flash(time, LED_GREEN);

    }

    this->logger().info() << "O_State_Wait90SecAction::execute end100s...stop... " << robot.chrono().getElapsedTimeInSec() << logs::end;
    robot.freeMotion();

    robot.end90s(true); //indique que l'action est effectuée au prog princ
    logger().info() << "cancel decisionmaker"  << logs::end;
    robot.decisionMaker_->cancel();
    //init robot for end
    robot.freeMotion(); //stop the robot

    robot.stopExtraActions(); //stop specific actions, can take time for servos...

    robot.svgPrintEndOfFile();


    logger().info() << "print lcd during sec" << logs::end;

    robot.actions().lcd2x16().clear();
    robot.actions().lcd2x16().setBacklightOn();
    robot.actions().lcd2x16().setCursor(0,0);
    //robot.actions().lcd2x16().print("40 Points ?");
    robot.actions().lcd2x16().print(robot.points);
    robot.actions().lcd2x16().print(" points ?");
    robot.actions().lcd2x16().setCursor(0,1);
    robot.actions().lcd2x16().print("Yeahhh OK");
    robot.actions().ledBar().flashAll(LED_GREEN);


    robot.actions().ledBar().k2mil(2, 50000, LED_GREEN);
    //robot.actions().ledBar().startK2mil(10, 50000, LED_GREEN, false); //TODO pb a corriger
    sleep(480);
    logger().info() << "O_State_WaitEndOfMatch executed " << robot.chrono().getElapsedTimeInSec() << " sec" << logs::end;

    robot.stopAll(); //stop asserv and actionManagerTimer
    return NULL; //finish all state
}
