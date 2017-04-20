/*!
 * \file
 * \brief Implémentation de la classe L_ServoUsingMotorTest.
 */

#include "L_ServoUsingMotorTest.hpp"

#include <unistd.h>
#include <cstdlib>
#include <string>

#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/FunnyAction.hpp"
#include "../Common/Action.Driver/AButtonDriver.hpp"
#include "../Common/Arguments.hpp"
#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_ServoUsingMotorTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();


	robot.getArgs().addArgument("step", "number for steps [1]", "1");

	//reparse arguments
	robot.parseConsoleArgs(argc, argv);
}

void L_ServoUsingMotorTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;
	configureConsoleArgs(argc, argv);

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	int step = 0;
	Arguments args = robot.getArgs();

	step = atoi(args["step"].c_str());
	logger().debug() << "Arg step set " << args["step"] << ", step = " << step
			<< logs::end;
/*
	int pos = 0;
	robot.actions().funnyAction().reset();
	//sleep(1);
	robot.actions().funnyAction().activate(-20);
	sleep(1);
	robot.actions().funnyAction().activate(300);
	sleep(6);
	robot.actions().funnyAction().activate(-150);
	sleep(1);*/
	/*
	ButtonTouch touch = BUTTON_NONE;

	while (touch != BUTTON_BACK_KEY)
	{
		touch = robot.actions().buttonBar().waitOneOfAllPressed();
		//logger().info() << "touch = " << touch << logs::end;
		if (touch == BUTTON_UP_KEY)
		{
			pos += step;
			logger().info() << "+" << step << " " << pos << logs::end;
			robot.actions().parasol().activate(step);
			usleep(50000);
		}

		if (touch == BUTTON_DOWN_KEY)
		{
			pos -= step;
			logger().info() << "-" << step << " " << pos << logs::end;
			robot.actions().parasol().activate(-step);
			usleep(50000);
		}
		//robot.actions().parasol().activate(pos);
		//usleep(10000);
	}
*/
	//robot.actions().funnyAction().release();

	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

