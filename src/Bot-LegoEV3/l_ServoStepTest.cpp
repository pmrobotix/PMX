#include "l_ServoStepTest.hpp"

#include <unistd.h>
#include <string>

#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/ServoObjectsSystem.hpp"
#include "../Common/Action.Driver/AButtonDriver.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_ServoStepTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	robot.getArgs().addArgument("num", "Numero du servo");
	robot.getArgs().addArgument("step", "nombre à augmenter ou diminuer (en %)", "2");

	//reparse arguments
	robot.parseConsoleArgs(argc, argv);
}

void L_ServoStepTest::run(int argc, char** argv)
{

	configureConsoleArgs(argc, argv); //on appelle les parametres specifiques pour ce test

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	Arguments args = robot.getArgs();

	logger().info() << "Executing - " << this->desc() << logs::end;
	//args.usage();

	int pos = 0;
	int step = 2;

	int num = 0;

	if (args["num"] != "0")
	{
		num = atoi(args["num"].c_str());
		logger().info() << "Arg num set " << args["num"] << ", num = " << num << logs::end;
	}

	if (args["step"] != "0")
	{
		step = atoi(args["step"].c_str());
		logger().info() << "Arg step set " << args["step"] << ", step = " << step << logs::end;
	}
	//robot.actions().servoObjects().leftDeploy(0, true);

	robot.actions().servoObjects().deploy((ServoLabel) num, 0.0, 0);

	ButtonTouch touch = BUTTON_NONE;

	while (touch != BUTTON_BACK_KEY)
	{
		touch = robot.actions().buttonBar().waitOneOfAllPressed();
		//logger().info() << "touch = " << touch << logs::end;
		if (touch == BUTTON_UP_KEY)
		{
			pos += step;
			if (pos >= 100) pos = 100;
			logger().info() << "+" << step << " pos=" << pos << logs::end;
			robot.actions().servoObjects().deploy((ServoLabel) num, pos, 0);

		}

		if (touch == BUTTON_DOWN_KEY)
		{
			pos -= step;
			if (pos <= -100) pos = -100;
			logger().info() << "-" << step << " pos=" << pos << logs::end;
			robot.actions().servoObjects().deploy((ServoLabel) num, pos, 0);

		}

		if (touch == BUTTON_ENTER_KEY)
		{
			logger().info() << "-" << step << " pos=" << pos << logs::end;
			robot.actions().servoObjects().release((ServoLabel) num);

		}

		if (touch == BUTTON_RIGHT_KEY)
		{
			pos = 0;
			robot.actions().servoObjects().release((ServoLabel) num);
			num = num + 1;
			if (num >= SERVO_enumTypeEnd) num--;

			logger().info() << "SERVO " << num << " pos=" << pos << logs::end;

		}

		if (touch == BUTTON_LEFT_KEY)
		{
			pos = 0;
			robot.actions().servoObjects().release((ServoLabel) num);
			num = num - 1;
			if (num < 0) num++;

			logger().info() << "SERVO " << num << " pos=" << pos << logs::end;

		}
		usleep(30000);
	}

	robot.actions().servoObjects().releaseAll();

	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

