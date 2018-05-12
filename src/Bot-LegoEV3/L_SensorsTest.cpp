#include "L_SensorsTest.hpp"

#include <unistd.h>
#include <string>

#include "../Common/Action/Sensors.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_SensorsTest::run(int argc, char** argv)
{
    logger().info() << "Executing - " << this->desc() << logs::end;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    bool front;
    bool rear;
    utils::Chronometer chrono;
    chrono.start();
    while (chrono.getElapsedTimeInSec() < 100) {
        front = robot.actions().sensors().front();

        rear = robot.actions().sensors().rear();

        usleep(400000);
        logger().info() << " front=" << front << " rear=" << rear << logs::end;
    }

    robot.stopAll();
    logger().info() << "Happy End." << logs::end;
}

