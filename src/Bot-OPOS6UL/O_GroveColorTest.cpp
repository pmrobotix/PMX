#include "O_GroveColorTest.hpp"

#include <string>

#include "../Log/Logger.hpp"
#include "OPOS6UL_RobotExtended.hpp"


using namespace std;

void O_GroveColorTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();


	while (1)
	{

	}

	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

