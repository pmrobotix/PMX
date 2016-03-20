#include "LegoEV3MovingBaseTest.hpp"

#include <string>

#include "../Log/Logger.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void LegoEV3MovingBaseTest::run(Arguments *)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	robot.asserv().base().moveForward(520, 800);


	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

