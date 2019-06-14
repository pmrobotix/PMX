#include "O_SensorsTest.hpp"

#include <unistd.h>
#include <string>
#include <cmath>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_SensorsTest::run(int argc, char** argv)
{
    logger().info() << "N° " << this->position() << " - Executing - " << this->desc() << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();
    robot.setMyColor(PMXVIOLET);
    robot.asserv().startMotionTimerAndOdo(false); //assistedHandling is enabled with "true" !
    robot.asserv().setPositionAndColor(400.0, 400.0, 0.0, (robot.getMyColor() != PMXVIOLET));
    RobotPosition p = robot.asserv().pos_getPosition();
    logger().info() << "p= " << p.x * 1000.0 << " " << p.y * 1000.0 << " mm " << p.theta * 180.0f / M_PI << "° "
            << p.asservStatus << logs::end;

    //execution sans le actionTimer
    int front, back, left, right, mright, mleft;
    utils::Chronometer chrono("O_SensorsTest");
    chrono.start();
    robot.actions().sensors().setIgnoreAllFrontNearObstacle(false);
    robot.actions().sensors().setIgnoreAllBackNearObstacle(false);
    /*
     while (chrono.getElapsedTimeInSec() < 100) {
     right = robot.actions().sensors().rightSide();
     left = robot.actions().sensors().leftSide();
     front = robot.actions().sensors().front(true);
     back = robot.actions().sensors().back(true);

     mright = robot.actions().sensors().multipleRightSide(10);
     mleft = robot.actions().sensors().multipleLeftSide(10);

     usleep(100000);
     logger().info() << " front=" << front << "   back=" << back << "                       left=" << left
     << "   right=" << right << logs::end;
     logger().info() << "                                         " << "                       mleft=" << mleft << "   mright=" << mright<< logs::end;
     logger().info() << logs::end;
     }*/

    //detection adverse
    robot.actions().start();
    robot.actions().sensors().addTimerSensors(50);

    while (chrono.getElapsedTimeInSec() < 200) {
        front = robot.actions().sensors().front(true);
        back = robot.actions().sensors().back(true);

        mright = robot.actions().sensors().multipleRightSide(10);
        mleft = robot.actions().sensors().multipleLeftSide(10);

        usleep(1000000);
        logger().info() << " front=" << front << " back=" << back << "                       mleft=" << mleft << "   mright=" << mright
                << logs::end;
    }

    logger().info() << "Happy End." << logs::end;
}

