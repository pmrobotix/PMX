#include "L_AsservLineRotateTest.hpp"

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

using namespace std;

void L_AsservLineRotateTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	//match color
	robot.getArgs().addArgument("d", "distance mm");
	//robot.getArgs().addArgument("y", "y mm");
	robot.getArgs().addArgument("a", "angle degre", "0");

	//reparse arguments
	robot.parseConsoleArgs(argc, argv);
}

void L_AsservLineRotateTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;
	configureConsoleArgs(argc, argv);

	utils::Chronometer chrono;
	long left;
	long right;

	float d = 0.0;
	//float y = 0.0;
	float a = 0.0;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	Arguments args = robot.getArgs();

	if (args["d"] != "0")
	{
		d = atof(args["d"].c_str());
		logger().debug() << "Arg d set " << args["d"] << ", d = " << d << logs::end;
	}
	/*if (args["y"] != "0")
	 {
	 y = atof(args["y"].c_str());
	 logger().debug() << "Arg y set " << args["y"] << ", y = " << y << logs::end;
	 }*/
	if (args["a"] != "0")
	{
		a = atof(args["a"].c_str());
		logger().debug() << "Arg a set " << args["a"] << ", a = " << a << logs::end;
	}

	robot.asserv().startMotionTimerAndOdo();
	logger().error() << "robot.getMyColor()  = "<< robot.getMyColor()  << logs::end;
	logger().error() << "(robot.getMyColor() != PMXYELLOW) = "<< (robot.getMyColor() != PMXYELLOW) << logs::end;
	robot.asserv().setPositionAndColor(0.0, 0.0, 0.0, (robot.getMyColor() != PMXYELLOW));

	robot.svgPrintPosition();

	chrono.start();
	//robot.actions().start();
	//robot.actions().sensors().startSensors();

	robot.asserv().doMoveForwardTo(d, 300);

	//robot.asserv().doLineAbs(d);

	left = robot.asserv().base()->encoders().getLeftEncoder();
	right = robot.asserv().base()->encoders().getRightEncoder();
	logger().info() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left
			<< " ; right= " << right << " x=" << robot.asserv().pos_getX_mm() << " y="
			<< robot.asserv().pos_getY_mm() << " a=" << robot.asserv().pos_getThetaInDegree()
			<< logs::end;

	robot.svgPrintPosition();

	if (a != 0)
	{
		robot.asserv().doRotateAbs(a);

		left = robot.asserv().base()->encoders().getLeftEncoder();
		right = robot.asserv().base()->encoders().getRightEncoder();
		logger().info() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left
				<< " ; right= " << right << " x=" << robot.asserv().pos_getX_mm() << " y="
				<< robot.asserv().pos_getY_mm() << " a=" << robot.asserv().pos_getThetaInDegree()
				<< logs::end;

		robot.svgPrintPosition();

	}
	/*
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

