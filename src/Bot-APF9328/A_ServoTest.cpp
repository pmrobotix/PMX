#include "A_ServoTest.hpp"

#include <unistd.h>
#include <string>

#include "../Common/Action/ServoObjectsSystem.hpp"
#include "../Log/Logger.hpp"
#include "APF9328ActionsExtended.hpp"
#include "APF9328RobotExtended.hpp"

using namespace std;

void A_ServoTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	APF9328RobotExtended &robot = APF9328RobotExtended::instance();

	robot.actions().servoObjects().leftDeploy();
	sleep(1);
	robot.actions().servoObjects().leftRelease();

	robot.actions().servoObjects().rightDeploy();
	sleep(1);
	robot.actions().servoObjects().rightRelease();


	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

