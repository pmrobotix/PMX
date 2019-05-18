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
    logger().error() << "Start Chronometer getIgnoreFrontCollision=" << robot.asserv().getIgnoreFrontCollision()
            << logs::end;
    robot.chrono().start();

    bool front = false;
    bool rear = false;
    uint c = 0;
    uint lastdetect_front_nb_ = 0;
    uint lastdetect_rear_nb_ = 0;
    bool stop = false;
    while (robot.chrono().getElapsedTimeInSec() <= 98) {

        robot.displayPoints();
        //test ARU
        if (robot.actions().tirette().pressed()) {
            printf("===== ARU pressed !!!!!!\n");
            logger().error() << "ARU pressed !!!!!!" << logs::end;
            //stop all robot
            stop = true;
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
            if (!robot.asserv_default->getIgnoreFrontCollision()) {
                robot.asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
                robot.asserv_default->setFrontCollision();
                robot.asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche

                //robot.asserv_default->setLowSpeed(true);
            }

            //exit(0);

        }
        /*
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
         this->logger().error() << "====> setFrontCollision frontVeryclosed! nb="<< lastdetect_front_nb_
         << " ignorefrontdefault=" << robot.asserv_default->getIgnoreFrontCollision()
         << " ignorefront=" << robot.asserv().getIgnoreFrontCollision()
         << logs::end;
         //if (!robot.asserv_default->getIgnoreFrontCollision()) {
         this->logger().error() << "====> non ignored setFrontCollision frontVeryclosed! nb="<< lastdetect_front_nb_<< logs::end;
         robot.asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
         robot.asserv_default->setFrontCollision();
         robot.asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
         // }
         }
         }
         */
        //ARRIERE
        bool rear = robot.actions().sensors().rear();
        bool rearVeryclosed = robot.actions().sensors().rearVeryClosed();
        /*        if (rear) {
         //send collision to asserv
         if (lastdetect_rear_nb_ == 0) {

         if (!robot.asserv_default->getIgnoreRearCollision()) {
         robot.asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
         robot.asserv_default->setRearCollision();
         robot.asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche

         //robot.asserv_default->setLowSpeed(true);//TODO a faire en arrière parceque ca interfere avec la partie au dessus
         }
         }
         lastdetect_rear_nb_++;
         } else {
         lastdetect_rear_nb_ = 0;
         //robot.asserv_default->setLowSpeed(false);//TODO a faire en arrière parceque ca interfere avec la partie au dessus
         }
         if (lastdetect_rear_nb_ > 0) {
         if (rearVeryclosed) {

         if (!robot.asserv_default->getIgnoreRearCollision()) {
         robot.asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
         robot.asserv_default->setRearCollision();
         robot.asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
         }
         }
         }
         */
        usleep(100000);
        if (c % 10 == 0)
            this->logger().error() << "chrono " << robot.chrono().getElapsedTimeInSec() << " nb="
                    << lastdetect_front_nb_ << " front=" << front << " frontV=" << frontVeryclosed << "    nb="
                    << lastdetect_rear_nb_ << " rear=" << rear << " rearV=" << rearVeryclosed << "    ignorefrontd="
                    << robot.asserv_default->getIgnoreFrontCollision() << "    ignorefront="
                    << robot.asserv().getIgnoreFrontCollision() << logs::end;

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
