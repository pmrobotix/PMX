#include "L_Asserv_FindPIDTest.hpp"

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

void L_Asserv_FindPIDTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	robot.getArgs().addArgument("sec", "Time (sec) for test");

	robot.getArgs().addArgument("mm", "Distance (mm) for test");

	robot.getArgs().addArgument("Dp", "value(float) of p for Delta", "0.0");
	robot.getArgs().addArgument("Di", "value(float) of i for Delta", "0.0");
	robot.getArgs().addArgument("Dd", "value(float) of d for Delta", "0.0");

	robot.getArgs().addArgument("angle", "Angle (degrees) for test", "0.0");

	robot.getArgs().addArgument("Ap", "value(float) of p for Alpha", "0.0");
	robot.getArgs().addArgument("Ai", "value(float) of i for Alpha", "0.0");
	robot.getArgs().addArgument("Ad", "value(float) of d for Alpha", "0.0");

	//reparse arguments
	robot.parseConsoleArgs(argc, argv);
}

void L_Asserv_FindPIDTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;
	configureConsoleArgs(argc, argv);

	//PID values
	float Dp = 0.0;
	float Di = 0.0;
	float Dd = 0.0;
	float Ap = 0.0;
	float Ai = 0.0;
	float Ad = 0.0;
	int mm = 0;
	int sec = 0;
	float angle_degrees = 0.0;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	Arguments args = robot.getArgs();

	if (args["sec"] != "0")
	{
		sec = atoi(args["sec"].c_str());
		logger().debug() << "Arg sec set " << args["sec"] << ", sec = " << sec << logs::end;
	}
	if (args["mm"] != "0")
	{
		mm = atoi(args["mm"].c_str());
		logger().debug() << "Arg mm set " << args["mm"] << ", distmm = " << mm << logs::end;
	}
	if (args["Dp"] != "0")
	{
		Dp = atof(args["Dp"].c_str());
		logger().debug() << "Arg Dp set " << args["Dp"] << ", Dp = " << Dp << logs::end;
	}
	if (args["Di"] != "0")
	{
		Di = atof(args["Di"].c_str());
		logger().debug() << "Arg Di set " << args["Di"] << ", Di = " << Di << logs::end;
	}
	if (args["Dd"] != "0")
	{
		Dd = atof(args["Dd"].c_str());
		logger().debug() << "Arg Dd set " << args["Dd"] << ", Dd = " << Dd << logs::end;
	}
	if (args["angle"] != "0")
	{
		angle_degrees = atof(args["angle"].c_str());
		logger().debug() << "Arg angle set "
				<< args["angle"]
				<< ", angle = "
				<< angle_degrees
				<< logs::end;
	}
	if (args["Ap"] != "0")
	{
		Ap = atof(args["Ap"].c_str());
		logger().debug() << "Arg Ap set " << args["Ap"] << ", Ap = " << Ap << logs::end;
	}
	if (args["Ai"] != "0")
	{
		Ai = atof(args["Ai"].c_str());
		logger().debug() << "Arg Ai set " << args["Ai"] << ", Ai = " << Ai << logs::end;
	}
	if (args["Ad"] != "0")
	{
		Ad = atof(args["Ad"].c_str());
		logger().debug() << "Arg Ad set " << args["Ad"] << ", Ad = " << Ad << logs::end;
	}

	utils::Chronometer chrono;
	long left;
	long right;
	//utiliser que dans le cas asservInsa?
/*
	robot.asserv().startMotionTimerAndOdo();
	robot.asserv().configureAlphaPID(Ap, Ai, Ad); //surcharge
	robot.asserv().configureDeltaPID(Dp, Di, Dd);
	robot.asserv().setPositionAndColor(0.0, 300.0, 0.0, false);

	robot.svgPrintPosition();

	chrono.start();
	robot.asserv().findPidAD(angle_degrees, mm, sec);

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
*/
	robot.svgPrintPosition();

	robot.stopAll();
	logger().info() << "Happy End." << logs::end;
}

