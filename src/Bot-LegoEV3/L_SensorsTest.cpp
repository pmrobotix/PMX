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

    bool front, frontV;
    bool rear, rearV;
    utils::Chronometer chrono("L_SensorsTest");
    chrono.start();
    while (chrono.getElapsedTimeInSec() < 100) {
        front = robot.actions().sensors().front();
        frontV = robot.actions().sensors().frontVeryClosed();

        rear = robot.actions().sensors().rear();
        rearV = robot.actions().sensors().rearVeryClosed();

        usleep(400000);
        logger().info() << " front=" << front << " rear=" << rear << " fV=" << frontV << " rV=" << rearV << logs::end;
    }

    logger().info() << "Happy End." << logs::end;
}

