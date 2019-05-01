#include "L_State_WaitEndOfMatch.hpp"

#include <unistd.h>
#include <cstdlib>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Action/Tirette.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "../Thread/Thread.hpp"
#include "L_State_DecisionMakerIA.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

IAutomateState* L_State_WaitEndOfMatch::execute(Robot&)
{

    logger().info() << "executing..." << logs::end;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    //démarrage du chrono et des taches du decision maker
    logger().info() << "Start Chronometer" << logs::end;
    robot.chrono().start();

    bool front = false;
    bool rear = false;

    while (robot.chrono().getElapsedTimeInSec() <= 100)
    {
        /*
         usleep(1000000);
         long time = robot.chrono().getElapsedTimeInSec();
         this->logger().info() << "execute chrono " << time << logs::end;
        */

        //test ARU
        if (robot.actions().tirette().pressed()) {
            logger().error() << "ARU pressed !!!!!!" << logs::end;
            //stop all robot
            robot.stopAll();

            sleep(1);
            exit(0);
        }

        //test sensors
        front = false;
        rear = false;

        front = robot.actions().sensors().front();
        if (front) {
            robot.asserv().setFrontCollision();
        }

        rear = robot.actions().sensors().rear();
        if (rear) {
            robot.asserv().setRearCollision();
        }

        usleep(300000);

        this->logger().info() << "chrono " << robot.chrono().getElapsedTimeInSec() << " front=" << front << " rear=" << rear<< logs::end;
    }

    this->logger().info() << "execute end100s...stop... " << robot.chrono().getElapsedTimeInSec() << logs::end;
    robot.freeMotion();

    robot.end90s(true); //indique que l'action est effectuée au prog princ
    logger().info() << "cancel decisionmaker" << logs::end;
    robot.decisionMaker_->cancel();

    //init robot for end
    robot.freeMotion(); //stop the robot

    robot.stopActions(); //stop specific actions, can take time for servos...

    robot.svgPrintEndOfFile();

    robot.stopAll(); //stop asserv and actionManagerTimer
    return NULL; //finish all state
}
