#include "O_SensorsTest.hpp"

#include <unistd.h>
#include <string>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_SensorsTest::run(int argc, char** argv)
{
    logger().info() << this->position() << " - Executing - " << this->desc() << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    //execution sans le actionTimer
    int front, back, left, right;
    utils::Chronometer chrono("O_SensorsTest");
    chrono.start();
    while (chrono.getElapsedTimeInSec() < 100) {
        right = robot.actions().sensors().rightSide();
        left = robot.actions().sensors().leftSide();
        front = robot.actions().sensors().front();
        back = robot.actions().sensors().back();

        usleep(400000);
        logger().info() << " front=" << front << " back=" << back << " left=" << left<< " right=" << right << logs::end;
    }

    //TODO execution avec l'actionTimer




    logger().info() << "Happy End." << logs::end;
}

