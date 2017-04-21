#include "A_ServoStepTest.hpp"

#include <unistd.h>
#include <string>

#include "../Common/Action/ServoObjectsSystem.hpp"
#include "../Common/Action.Driver/AButtonDriver.hpp"
#include "../Log/Logger.hpp"
#include "APF9328ActionsExtended.hpp"
#include "APF9328RobotExtended.hpp"

using namespace std;

void A_ServoStepTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	APF9328RobotExtended &robot = APF9328RobotExtended::instance();

	int pos = 0;
	int step = 2;
/*
	robot.actions().servoObjects().leftDeploy(0, true);
	robot.actions().servoObjects().rightDeploy(0, true);
	robot.actions().servoObjects().centreDeploy(0, true);
*/
	ButtonTouch touch = BUTTON_NONE;

	while (touch != BUTTON_BACK_KEY)
	{
		touch = robot.actions().buttonBar().waitOneOfAllPressed();
		//logger().info() << "touch = " << touch << logs::end;
		if (touch == BUTTON_UP_KEY)
		{
			pos += step;
			if (pos >= 100)
				pos = 100;
			logger().info() << "+" << step << " " << pos << logs::end;
			/*
			robot.actions().servoObjects().leftDeploy(pos, true);
			robot.actions().servoObjects().rightDeploy(pos, true);*/

			usleep(50000);
		}

		if (touch == BUTTON_DOWN_KEY)
		{
			pos -= step;
			if (pos <= -100)
				pos = -100;
			logger().info() << "-" << step << " " << pos << logs::end;
			/*robot.actions().servoObjects().leftDeploy(pos, true);
			robot.actions().servoObjects().rightDeploy(pos, true);*/
			usleep(50000);
		}
	}
/*
	robot.actions().servoObjects().leftRelease();
	robot.actions().servoObjects().rightRelease();*/

	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

