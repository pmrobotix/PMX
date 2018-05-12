/*!
 * \file
 * \brief Implémentation de la classe LegoEV3SoundBarTest.
 */

#include "L_ServoObjectTest.hpp"

#include <string>

#include "../Common/Action/ServoObjectsSystem.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_ServoObjectTest::run(int argc, char** argv)
{
    logger().info() << "Executing - " << this->desc() << logs::end;

    LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();



    logger().info() << "Happy End." << logs::end;
    robot.stopAll();

}

