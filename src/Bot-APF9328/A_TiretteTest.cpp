#include "A_TiretteTest.hpp"

#include <string>

#include "../Log/Logger.hpp"
#include "APF9328RobotExtended.hpp"


using namespace std;

void A_TiretteTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	APF9328RobotExtended &robot = APF9328RobotExtended::instance();

	logger().info() << "TIRETTE status" << logs::end;

	robot.actions().tirette().monitor(3);



	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

