#include "L_State_WaitEndOfMatch.hpp"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Action/Tirette.hpp"
#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Asserv/MotorControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
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

    bool front = false;
    bool rear = false;

    uint c = 0;
    uint lastdetect_front_nb_ =0;
    while (robot.chrono().getElapsedTimeInSec() <= 100) {

        //test ARU
        if (robot.actions().tirette().pressed()) {
            printf("===== ARU pressed !!!!!!\n");
            logger().error() << "ARU pressed !!!!!!" << logs::end;
            //stop all robot
            break;
        }

        //test sensors
//        front = false;
//        rear = false;
//
//        front = robot.actions().sensors().front();
//        if (front) {
//            robot.asserv().setFrontCollision();
//        }
//
//        rear = robot.actions().sensors().rear();
//        if (rear) {
//            robot.asserv().setRearCollision();
//        }
        //AVANT
        bool front = robot.actions().sensors().front();
        bool frontVeryclosed = robot.actions().sensors().frontVeryClosed();
        if (front) {
            //send collision to asserv
            if (lastdetect_front_nb_ == 0) {

                if (!robot.asserv_default->getIgnoreFrontCollision()) {
                    robot.asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
                    robot.asserv_default->setFrontCollision();
                    robot.asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche

                    robot.asserv_default->setLowSpeed(true);
                }
            }
            lastdetect_front_nb_++;
        } else {
            lastdetect_front_nb_ = 0;
            robot.asserv_default->setLowSpeed(false);
        }
        if (lastdetect_front_nb_ > 0) {
            if (frontVeryclosed) {

                if (!robot.asserv_default->getIgnoreFrontCollision()) {
                    robot.asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
                    robot.asserv_default->setFrontCollision();
                    robot.asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
                }
            }
        }
        //ARRIERE TODO







        usleep(100000);
        if (c % 10 == 0)
            this->logger().info() << "chrono " << robot.chrono().getElapsedTimeInSec() << " front=" << front << " rear="
                    << rear << logs::end;

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

    robot.stopActions(); //stop specific actions, can take time for servos...

    robot.svgPrintEndOfFile();

    robot.stopAll(); //stop asserv and actionManagerTimer
    printf("555\n");
    return NULL; //finish all state
}
