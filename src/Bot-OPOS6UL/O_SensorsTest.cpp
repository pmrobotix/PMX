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
    logger().info() << "Executing - " << this->desc() << logs::end;

    OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

    //execution sans le actionTimer
    bool front;
    bool rear;
    utils::Chronometer chrono("O_SensorsTest");
    chrono.start();
    while (chrono.getElapsedTimeInSec() < 100) {
        front = robot.actions().sensors().front();

        rear = robot.actions().sensors().rear();

        usleep(400000);
        logger().info() << " front=" << front << " rear=" << rear << logs::end;
    }

    robot.stopExtraActions();
    robot.stopAll();
    logger().info() << "Happy End." << logs::end;
}

