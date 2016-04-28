#include "A_SensorsTest.hpp"

#include <unistd.h>
#include <string>

#include "../Common/Action/Sensors.hpp"
#include "../Log/Logger.hpp"
#include "APF9328ActionsExtended.hpp"
#include "APF9328RobotExtended.hpp"

using namespace std;

void A_SensorsTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	APF9328RobotExtended &robot = APF9328RobotExtended::instance();

	bool front;
	bool rear;
	while (1)
	{
		front = robot.actions().sensors().front();

		rear = robot.actions().sensors().rear();
		usleep(300000);
		logger().info() << " front=" << front << " rear=" << rear << logs::end;
	}

	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

