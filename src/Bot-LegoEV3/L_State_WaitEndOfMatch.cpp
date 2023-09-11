#include "L_State_WaitEndOfMatch.hpp"

#include <sys/types.h>
#include <chrono>
#include <thread>

#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/LcdShield.hpp"
#include "../Common/Action/Sensors.hpp"
#include "../Common/Action/Tirette.hpp"
#include "../Common/Interface.Driver/AButtonDriver.hpp"
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

    while (robot.chrono().getElapsedTimeInSec() <= 98 || stop == true) {

        //ARU (attention 250ms!)
        if (robot.actions().tirette().pressed()) {
            //printf("===== ARU pressed !!!!!!\n");
            logger().error() << "ARU pressed !!!!!!" << logs::end;
            //stop all robot
            robot.asserv().setEmergencyStop();
            robot.asserv().stopMotors();

            stop = true;
            break;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(200000));
        std::this_thread::yield();
        if (c % 4 == 0) {
            robot.displayPoints();
            this->logger().info() << "chrono " << robot.chrono().getElapsedTimeInSec() << logs::end;
        }
        c++;
    }

    //pas de funny action si ARU
    if (!stop) {

        robot.actions().fork_front_right_deploy_half(0);
        robot.actions().fork_front_left_init(0);
        robot.actions().fork_front_right_init(0);
        robot.actions().fork_front_left_deploy_half(0);

        robot.actions().fork_front_right_deploy_half(0);
        robot.actions().fork_front_left_init(0);
        robot.actions().fork_front_right_init(0);
        robot.actions().fork_front_left_deploy_half(0);

        robot.actions().fork_front_right_deploy_half(0);
        robot.actions().fork_front_left_init(0);
        robot.actions().fork_front_right_init(0);
        robot.actions().fork_front_left_deploy_half(0);

        robot.actions().fork_front_right_deploy_half(0);
        robot.actions().fork_front_left_init(0);
        robot.actions().fork_front_right_init(0);
        robot.actions().fork_front_left_deploy_half(0);

        robot.actions().fork_front_right_deploy_half(0);
        robot.actions().fork_front_left_init(0);
        robot.actions().fork_front_right_init(0);


        this->logger().info() << "on recule pour la funny " << robot.chrono().getElapsedTimeInSec() << logs::end;
        //on recule pour la funny action
        robot.svgPrintPosition();
        robot.asserv().doLineAbs(-30);
        std::this_thread::sleep_for(std::chrono::microseconds(200000));
        robot.asserv().stopMotors();
        robot.svgPrintPosition();

        this->logger().info() << "funny ACTION" << robot.chrono().getElapsedTimeInSec() << logs::end;
        robot.actions().funny_action_full();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        robot.points += 5;
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    this->logger().debug() << "execute end100s...stop... " << robot.chrono().getElapsedTimeInSec() << logs::end;
    robot.asserv().stopMotors();

    robot.end90s(true); //indique que l'action est effectuée au prog princ

    if (robot.decisionMaker_ != NULL) {
        logger().info() << "cancel decisionmaker" << logs::end;
        //if (robot.decisionMaker_->state() == utils::CREATED) //non necessaire?
        robot.decisionMaker_->cancel();
    }
    robot.asserv().stopMotors();
    robot.actions().clearAll();
    robot.freeMotion(); //stop the robot
    robot.actions().sensors().stopTimerSensors();
    robot.stopExtraActions(); //stop sensors timer, stop specific actions, can take time for servos...
    robot.svgPrintEndOfFile();

    logger().info() << "Display Points after 100sec" << logs::end;
    robot.actions().lcd().clear();
    robot.actions().lcd().display_content_integer(robot.points, 4);
    robot.actions().lcd().display_content_string("points ?", 5);

    robot.actions().sensors().display(robot.points);
    robot.actions().sensors().display(robot.points);

//    if (!stop) {
    ButtonTouch b = BUTTON_NONE;

//cas de ARU pressed
    if (robot.actions().tirette().pressed()) {
        while (1) {
            logger().debug() << "cas de ARU pressed" << logs::end;

            //affichage des points
            robot.actions().lcd().clear();
            robot.actions().lcd().display_content_integer(robot.points, 4);
            robot.actions().lcd().display_content_string("points ?", 5);

            b = robot.actions().buttonBar().checkOneOfAllPressed();
            if (b == BUTTON_BACK_KEY) {
                stop = true;
                break;
            }
            if (b == BUTTON_ENTER_KEY) {
                stop = true;
                break;
            }
            if (!robot.actions().tirette().pressed()) {
                stop = true;
                break;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(20000));
            std::this_thread::yield();
        }
    } else {

        while (1) {
            logger().debug() << "Cas de ARU unpressed" << logs::end;

            //affichage des points
            robot.actions().lcd().clear();
            robot.actions().lcd().display_content_integer(robot.points, 4);
            robot.actions().lcd().display_content_string("points ?", 5);

            b = robot.actions().buttonBar().checkOneOfAllPressed();
            if (b == BUTTON_BACK_KEY) {
                stop = true;
                break;
            }
            if (b == BUTTON_ENTER_KEY) {
                stop = true;
                break;
            }
            if (robot.actions().tirette().pressed()) {
                stop = true;
                break;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(20000));
            std::this_thread::yield();
        }
    }

//   }

    return NULL; //finish all state
}
