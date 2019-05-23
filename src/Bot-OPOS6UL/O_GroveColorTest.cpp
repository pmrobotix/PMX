#include "O_GroveColorTest.hpp"

#include <string>

#include "../Log/Logger.hpp"
#include "OPOS6UL_RobotExtended.hpp"


using namespace std;

void O_GroveColorTest::run(int argc, char** argv)
{
    logger().info() << this->position() << " - Executing - " << this->desc() << logs::end;

	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();


	//TODO


	logger().info() << "Happy End." << logs::end;
}

