#include "LegoEV3TiretteTest.hpp"

#include <string>

#include "../Log/Logger.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void LegoEV3TiretteTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	logger().info() << "TIRETTE status" << logs::end;

	robot.actions().tirette().monitor(3);

	logger().info() << "Happy End." << logs::end;
}

