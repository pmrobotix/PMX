#include "L_TiretteTest.hpp"

#include <string>

#include "../Common/Action/Tirette.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_TiretteTest::run(int argc, char **argv)
{
    logger().info() << this->position() << " - Executing - " << this->desc() << logs::end;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

    logger().info() << "TIRETTE status" << logs::end;

    //robot.actions().tirette().monitor(3);
    int tirette = 0;
    while (1) {
        tirette = robot.actions().tirette().pressed(); //250ms
        logger().info() << "tirette = " << tirette << logs::end;
        utils::sleep_for_micros(10000);
    }
    logger().info() << "Happy End." << logs::end;
}

