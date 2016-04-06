#include "LegoEV3AsservRunTest.hpp"

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

void LegoEV3AsservRunTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	//match color
	robot.getArgs().addArgument("x", "x mm");
	robot.getArgs().addArgument("y", "y mm");
	robot.getArgs().addArgument("a", "theta degrees");

	//reparse arguments
	robot.parseConsoleArgs(argc, argv);
}

void LegoEV3AsservRunTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;
	configureConsoleArgs(argc, argv);

	utils::Chronometer chrono;
	long left;
	long right;

	float x = 0.0;
	float y = 0.0;
	float a = 0.0;

	logger().debug() << logs::end;
	logger().debug() << logs::end;
	logger().debug() << logs::end;
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	robot.setMyColor(PMXVIOLET);

	Arguments args = robot.getArgs();

	if (args["x"] != "0")
	{
		x = atof(args["x"].c_str());
	}
	if (args["y"] != "0")
	{
		y = atof(args["y"].c_str());
	}
	if (args["a"] != "0")
	{
		a = atof(args["a"].c_str());
	}

	robot.asserv().startMotionTimerAndOdo();

	robot.asserv().setPositionAndColor(0.0, 300.0, 0.0, (robot.getMyColor() == PMXGREEN));

	robot.svg().writePosition(robot.asserv().pos_getX_mm(),
			robot.asserv().pos_getY_mm(),
			robot.asserv().pos_getTheta(),
			LEGOEV3_SVG_POS_ROBOT);

	chrono.start();


	/*
	robot.asserv().doMoveForwardTo(400, 300);

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

	robot.svg().writePosition(robot.asserv().pos_getX_mm(),
			robot.asserv().pos_getY_mm(),
			robot.asserv().pos_getTheta(),
			LEGOEV3_SVG_POS_ROBOT);

	robot.asserv().doMoveForwardTo(400, 1000);

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

	robot.svg().writePosition(robot.asserv().pos_getX_mm(),
			robot.asserv().pos_getY_mm(),
			robot.asserv().pos_getTheta(),
			LEGOEV3_SVG_POS_ROBOT);
*/
	robot.asserv().doMoveForwardTo(0, 1000);

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

	robot.svg().writePosition(robot.asserv().pos_getX_mm(),
			robot.asserv().pos_getY_mm(),
			robot.asserv().pos_getTheta(),
			LEGOEV3_SVG_POS_ROBOT);

	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

