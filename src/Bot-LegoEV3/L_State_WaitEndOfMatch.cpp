#include "L_State_WaitEndOfMatch.hpp"

#include <unistd.h>

#include "../Common/Action/LedBar.hpp"
#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "../Thread/Thread.hpp"
#include "L_State_DecisionMakerIA.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3RobotExtended.hpp"


IAutomateState* L_State_WaitEndOfMatch::execute(Robot&)
{

    logger().info() << "executing..." << logs::end;

   LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    //démarrage du chrono et des taches du decision maker
    logger().info() << "Start Chronometer" << logs::end;
    robot.chrono().start();

    //TODO check ARU and adversary here ?
    while (robot.chrono().getElapsedTimeInSec() <= 100) //todo mettre en parametre
    {
/*
        usleep(1000000);
        long time = robot.chrono().getElapsedTimeInSec();
        this->logger().info() << "execute chrono " << time << logs::end;

        robot.actions().ledBar().flash(time, LED_GREEN);
*/
        //test ARU
                            if (robot.actions().tirette().pressed())
                            {
                                      logger().error() << "ARU pressed !!!!!!" << logs::end;
                                      //stop all robot
                                      robot.stopAll();

                                      sleep(1);
                                      exit(0);
                            }
                            //robot.actions().ledBar().set(1, LED_OFF); //WARNING BUG DE LED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                            //robot.actions().ledBar().set(0, LED_OFF);
                            usleep(300000);

                            //robot.actions().ledBar().set(1, LED_RED);
                            //robot.actions().ledBar().set(0, LED_RED);

                            this->logger().debug() << "chrono " << robot.chrono().getElapsedTimeInSec() << logs::end;
    }

    /*
//ARU and adversary
          while (robot.chrono().getElapsedTimeInSec() <= 90)
          {
                    //test ARU
                    if (robot.actions().tirette().pressed())
                    {
                              logger().error() << "ARU pressed !!!!!!" << logs::end;
                              //stop all robot
                              robot.stop();

                              sharedData->end90s(true);
                              usleep(200000);
                              exit(0);
                    }
                    //robot.actions().ledBar().set(1, LED_OFF); //WARNING BUG DE LED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    //robot.actions().ledBar().set(0, LED_OFF);
                    usleep(300000);

                    //robot.actions().ledBar().set(1, LED_RED);
                    //robot.actions().ledBar().set(0, LED_RED);

                    this->logger().debug() << "chrono " << robot.chrono().getElapsedTimeInSec() << logs::end;
          }*/
    this->logger().info() << "execute end100s...stop... " << robot.chrono().getElapsedTimeInSec() << logs::end;
    robot.freeMotion();

    robot.end90s(true); //indique que l'action est effectuée au prog princ
    logger().info() << "cancel decisionmaker"  << logs::end;
    robot.decisionMaker_->cancel();
    //init robot for end
    robot.freeMotion(); //stop the robot

    robot.stopActions(); //stop specific actions, can take time for servos...

    robot.svgPrintEndOfFile();
/*

    logger().info() << "print lcd during sec" << logs::end;


    robot.actions().ledBar().flashAll(LED_GREEN);


    robot.actions().ledBar().k2mil(2, 50000, LED_GREEN);
    //robot.actions().ledBar().startK2mil(10, 50000, LED_GREEN, false); //TODO pb a corriger
    sleep(480);
    logger().info() << "executed " << robot.chrono().getElapsedTimeInSec() << " sec" << logs::end;
*/
    robot.stopAll(); //stop asserv and actionManagerTimer
    return NULL; //finish all state
}
