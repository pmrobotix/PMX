#include "L_State_WaitEndOfMatch.hpp"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Action/Tirette.hpp"
#include "../Common/Asserv/MotorControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
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
    logger().debug() << "Start Chronometer" << logs::end;
    robot.chrono().start();

    uint c = 0;
    bool stop = false;
    while (robot.chrono().getElapsedTimeInSec() <= 100) {


        //test ARU
        if (robot.actions().tirette().pressed()) {
            printf("===== ARU pressed !!!!!!\n");
            logger().error() << "ARU pressed !!!!!!" << logs::end;
            //stop all robot
            stop = true;
            break;
        }

        usleep(100000);
        if (c % 2 == 0)
        {
            robot.displayPoints();
            this->logger().error() << "chrono " << robot.chrono().getElapsedTimeInSec()
                    << logs::end;
        }
        c++;
    }

    this->logger().info() << "execute end100s...stop... " << robot.chrono().getElapsedTimeInSec() << logs::end;
    robot.freeMotion();

    robot.end90s(true); //indique que l'action est effectuée au prog princ
    logger().info() << "cancel decisionmaker" << logs::end;

    if (robot.decisionMaker_ != NULL)
        //if (robot.decisionMaker_->state() == utils::CREATED)
        robot.decisionMaker_->cancel();


    //init robot for end
    robot.freeMotion(); //stop the robot
    robot.actions().sensors().stopTimerSensors();

    robot.stopExtraActions(); //stop specific actions, can take time for servos...
    robot.svgPrintEndOfFile();

    logger().info() << "Display Points after 100sec" << logs::end;
    robot.actions().lcd().clear();
    robot.actions().lcd().display_content_integer(robot.points, 4);
    robot.actions().lcd().display_content_string("points ?", 5);
    if (!stop) {
        ButtonTouch b = BUTTON_NONE;
        while (1) {
            b = robot.actions().buttonBar().checkOneOfAllPressed();
            if (b == BUTTON_BACK_KEY) {
                break;
            }
            if (b == BUTTON_ENTER_KEY) {
                break;
            }
            usleep(1000);
        }
    }

    return NULL; //finish all state
}
