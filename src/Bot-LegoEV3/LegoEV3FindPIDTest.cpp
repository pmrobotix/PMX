#include "LegoEV3FindPIDTest.hpp"

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

void LegoEV3FindPIDTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	robot.getArgs().addArgument("sec", "Time (sec) for test");

	robot.getArgs().addArgument("mm", "Distance (mm) for test");

	robot.getArgs().addArgument("Dpp", "value(float) of p for Delta", "0.0");
	robot.getArgs().addArgument("Dd", "value(float) of d for Delta", "0.0");

	robot.getArgs().addArgument("angle", "Angle (degrees) for test", "0.0");

	robot.getArgs().addArgument("Ap", "value(float) of p for Alpha", "0.0");
	robot.getArgs().addArgument("Ad", "value(float) of d for Alpha", "0.0");

	//reparse arguments
	robot.parseConsoleArgs(argc, argv);
}

void LegoEV3FindPIDTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;
	configureConsoleArgs(argc, argv);

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	Arguments args = robot.getArgs();
	//PID values
	float Dp = 0.0;
	float Dd = 0.0;
	float Ap = 0.0;
	float Ad = 0.0;
	int mm = 0;
	int sec = 0;
	float angle = 0.0;

	if (args["sec"] != "0")
	{
		sec = atoi(args["sec"].c_str());
		logger().debug() << "Arg sec set " << args["sec"] << ", sec = " << sec << logs::end;
	}
	if (args["distmm"] != "0")
	{
		mm = atoi(args["distmm"].c_str());
		logger().debug() << "Arg Distmm set " << args["distmm"] << ", distmm = " << mm << logs::end;
	}
	if (args["Dp"] != "0")
	{
		Dp = atof(args["Dp"].c_str());
		logger().debug() << "Arg Dp set " << args["Dp"] << ", Dp = " << Dp << logs::end;
	}
	if (args["Dd"] != "0")
	{
		Dd = atof(args["Dd"].c_str());
		logger().debug() << "Arg Dd set " << args["Dd"] << ", Dd = " << Dd << logs::end;
	}
	if (args["angle"] != "0")
	{
		angle = atof(args["angle"].c_str());
		logger().debug() << "Arg angle set " << args["angle"] << ", angle = " << angle << logs::end;
	}
	if (args["Ap"] != "0")
	{
		Ap = atof(args["Ap"].c_str());
		logger().debug() << "Arg Ap set " << args["Ap"] << ", Ap = " << Ap << logs::end;
	}

	if (args["Ad"] != "0")
	{
		Ad = atof(args["Ad"].c_str());
		logger().debug() << "Arg Ad set " << args["Ad"] << ", Ad = " << Ad << logs::end;
	}

	utils::Chronometer chrono;
	long left;
	long right;


	robot.asserv().startMotionTimerAndOdo();
	robot.asserv().configureAlphaPID(Ap, 0.0, Ad); //surcharge
	robot.asserv().configureDeltaPID(Dp, 0.0, Dd);
	robot.asserv().setPosition(0.0, 300.0, 0.0);
	chrono.start();
	robot.asserv().findPidAD(angle, mm, sec);

	left = robot.asserv().base()->encoders().getLeftEncoder();
	right = robot.asserv().base()->encoders().getRightEncoder();
	logger().info() << "time= "
			<< chrono.getElapsedTimeInMilliSec()
			<< "ms ; left= "
			<< left
			<< " ; right= "
			<< right
			<< logs::end;

	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

