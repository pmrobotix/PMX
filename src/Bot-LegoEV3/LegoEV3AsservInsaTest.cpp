#include "LegoEV3AsservInsaTest.hpp"

#include <string>

#include "../Log/Logger.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void LegoEV3AsservInsaTest::run(Arguments *)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	robot.asserv().startAsservInsa();

	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

