#include "L_AsservRunTest.hpp"

#include <unistd.h>
#include <cstdlib>
#include <string>

#include "../Common/Arguments.hpp"
#include "../Common/Asserv/EncoderControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3RobotExtended.hpp"
#include "LegoEV3SvgWriterExtended.hpp"

using namespace std;

void L_AsservRunTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	//match color
	robot.getArgs().addArgument("x", "x mm");
	//robot.getArgs().addArgument("y", "y mm");
	//robot.getArgs().addArgument("d", "distance");

	//reparse arguments
	robot.parseConsoleArgs(argc, argv);
}

void L_AsservRunTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;
	configureConsoleArgs(argc, argv);

	utils::Chronometer chrono;
	long left;
	long right;

	float x = 0.0;
	float y = 0.0;
	float a = 0.0;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	Arguments args = robot.getArgs();

	if (args["x"] != "0")
	{
		x = atof(args["x"].c_str());
		logger().debug() << "Arg x set " << args["x"] << ", x = " << x << logs::end;
	}
	/*if (args["y"] != "0")
	 {
	 y = atof(args["y"].c_str());
	 logger().debug() << "Arg y set " << args["y"] << ", y = " << y << logs::end;
	 }
	if (args["d"] != "0")
	{
		a = atof(args["d"].c_str());
		logger().debug() << "Arg a set " << args["d"] << ", a = " << a << logs::end;
	}*/

	robot.asserv().startMotionTimerAndOdo();

	robot.asserv().setPositionAndColor(0.0, 300.0, 0.0, (robot.getMyColor() != PMXYELLOW));

	robot.svgPrintPosition();

	chrono.start();
	//robot.actions().start();
	//robot.actions().sensors().startSensors();

	//robot.asserv().doMoveForwardAndRotateTo(x, 300, 0);

	robot.asserv().doLineAbs(x);

	left = robot.asserv().base()->encoders().getLeftEncoder();
	right = robot.asserv().base()->encoders().getRightEncoder();
	logger().info() << "time= "
			<< chrono.getElapsedTimeInMilliSec()
			<< "ms ; left= "
			<< left
			<< " ; right= "
			<< right
			<< " x="
			<< robot.asserv().pos_getX_mm()
			<< " y="
			<< robot.asserv().pos_getY_mm()
			<< " a="
			<< robot.asserv().pos_getThetaInDegree()
			<< logs::end;

	robot.svgPrintPosition();

	/*
	robot.asserv().doMoveBackwardTo(0, 300);

	left = robot.asserv().base()->encoders().getLeftEncoder();
	right = robot.asserv().base()->encoders().getRightEncoder();
	logger().info() << "time= "
			<< chrono.getElapsedTimeInMilliSec()
			<< "ms ; left= "
			<< left
			<< " ; right= "
			<< right
			<< " x="
			<< robot.asserv().pos_getX_mm()
			<< " y="
			<< robot.asserv().pos_getY_mm()
			<< " a="
			<< robot.asserv().pos_getThetaInDegree()
			<< logs::end;

	robot.svgPrintPosition();

	sleep(3);

	robot.asserv().doMoveForwardTo(x, 300);

	left = robot.asserv().base()->encoders().getLeftEncoder();
	right = robot.asserv().base()->encoders().getRightEncoder();
	logger().info() << "time= "
			<< chrono.getElapsedTimeInMilliSec()
			<< "ms ; left= "
			<< left
			<< " ; right= "
			<< right
			<< " x="
			<< robot.asserv().pos_getX_mm()
			<< " y="
			<< robot.asserv().pos_getY_mm()
			<< " a="
			<< robot.asserv().pos_getThetaInDegree()
			<< logs::end;

	robot.svgPrintPosition();

	robot.asserv().doMoveBackwardTo(0, 300);

	left = robot.asserv().base()->encoders().getLeftEncoder();
	right = robot.asserv().base()->encoders().getRightEncoder();
	logger().info() << "time= "
			<< chrono.getElapsedTimeInMilliSec()
			<< "ms ; left= "
			<< left
			<< " ; right= "
			<< right
			<< " x="
			<< robot.asserv().pos_getX_mm()
			<< " y="
			<< robot.asserv().pos_getY_mm()
			<< " a="
			<< robot.asserv().pos_getThetaInDegree()
			<< logs::end;

	robot.svgPrintPosition();
*/
	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

