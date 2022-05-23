#include "O_SensorsTest.hpp"

#include <chrono>
#include <string>
#include <thread>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Action.Driver/ASensorsDriver.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Common/Utils/PointerList.hpp"
#include "../Log/Logger.hpp"
#include "../Thread/Thread.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_SensorsTest::run(int argc, char** argv) {
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.setMyColor(PMXYELLOW);
    //robot.asserv().startMotionTimerAndOdo(false); //assistedHandling is enabled with "true" !
    utils::Chronometer chrono("O_SensorsTest");
    chrono.start();
    int front, back, left, right, mright, mleft;
    ASensorsDriver::bot_positions vadv;

/*
    //TEST en direct
    while (chrono.getElapsedTimeInSec() < 200) {
        int err = robot.actions().sensors().sync("beacon_sync");

        vadv = robot.actions().sensors().getPositionsAdv();

        for (ASensorsDriver::bot_positions::size_type i = 0; i < vadv.size(); i++) {
            logger().info() << "TEST en direct : vadv nb=" << vadv.size()
        << " detected=" << vadv[i].nbDetectedBots
        << " x=" << vadv[i].x
        << " y=" << vadv[i].y
        << " a=" << vadv[i].theta
        << " d=" << vadv[i].d
                            << logs::end;
        }


        //std::this_thread::sleep_for(chrono::nanoseconds(100000000));
        utils::sleep_for_micros(200000);
        //usleep(100000);
    }
*/
    //il faut mettre une position pour le filtre table
    //robot.asserv().setPositionAndColor(800, 200, 0,(robot.getMyColor() != PMXYELLOW));

    robot.actions().sensors().setIgnoreAllFrontNearObstacle(false);
    robot.actions().sensors().setIgnoreAllBackNearObstacle(false);

    robot.actions().start();
    robot.actions().sensors().addTimerSensors(200);

    while (chrono.getElapsedTimeInSec() < 200) {
        vadv = robot.actions().sensors().getPositionsAdv();

        for (ASensorsDriver::bot_positions::size_type i = 0; i < vadv.size(); i++) {
            logger().info() << " vadv nb="
                    << vadv.size()
                    << " detected="
                    << vadv[i].nbDetectedBots
                    << " x="
                    << vadv[i].x
                    << " y="
                    << vadv[i].y
                    << " a="
                    << vadv[i].theta
                    << " d="
                    << vadv[i].d
                    << logs::end;
        }

        front = robot.actions().sensors().front(1);
        front = robot.actions().sensors().back(1);
        logger().info() << " f=" << front << " b=" << back << logs::end;

        utils::sleep_for_micros(200000);
    }

    //TEST par front recu




//    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
//    robot.setMyColor(PMXVIOLET);//YELLOW
//    robot.asserv().startMotionTimerAndOdo(false); //assistedHandling is enabled with "true" !
//    robot.asserv().setPositionAndColor(400.0, 400.0, 0.0, (robot.getMyColor() != PMXVIOLET));//YELLOW
//    RobotPosition p = robot.asserv().pos_getPosition();
//    logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° "
//            << p.asservStatus << logs::end;
//
//    //execution sans le actionTimer
//    int front, back, left, right, mright, mleft;
//    utils::Chronometer chrono("O_SensorsTest");
//    chrono.start();
//    robot.actions().sensors().setIgnoreAllFrontNearObstacle(false);
//    robot.actions().sensors().setIgnoreAllBackNearObstacle(false);
//    /*
//     while (chrono.getElapsedTimeInSec() < 100) {
//     right = robot.actions().sensors().rightSide();
//     left = robot.actions().sensors().leftSide();
//     front = robot.actions().sensors().front(true);
//     back = robot.actions().sensors().back(true);
//
//     mright = robot.actions().sensors().multipleRightSide(10);
//     mleft = robot.actions().sensors().multipleLeftSide(10);
//
//     usleep(100000);
//     logger().info() << " front=" << front << "   back=" << back << "                       left=" << left
//     << "   right=" << right << logs::end;
//     logger().info() << "                                         " << "                       mleft=" << mleft << "   mright=" << mright<< logs::end;
//     logger().info() << logs::end;
//     }*/
//
//    //detection adverse
//    robot.actions().start();
//    robot.actions().sensors().addTimerSensors(50);
//
//    while (chrono.getElapsedTimeInSec() < 200) {
//        front = robot.actions().sensors().front(true);
//        back = robot.actions().sensors().back(true);
//
//        mright = robot.actions().sensors().multipleRightSide(10);
//        mleft = robot.actions().sensors().multipleLeftSide(10);
//
//        usleep(1000000);
//        logger().info() << " front=" << front << " back=" << back << "                       mleft=" << mleft << "   mright=" << mright
//                << logs::end;
//    }

    logger().info() << robot.getID() << " " << this->name() << " Happy End" << " N° " << this->position() << logs::end;
}

