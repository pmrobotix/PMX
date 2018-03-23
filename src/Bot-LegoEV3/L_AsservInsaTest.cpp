#include <string>

#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Asserv/EncoderControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "L_AsservInsaTest.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

//set max ACC VMAX DEC
void L_AsservInsaTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	robot.getArgs().addArgument("Acc", "Acceleration(float)");
	robot.getArgs().addArgument("Vmax", "MaxSpeed(float)");
	robot.getArgs().addArgument("Dec", "Deceleration(float)");

	robot.getArgs().addArgument("mm", "Distance (mm) for test", "0.0");

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

void L_AsservInsaTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;
	configureConsoleArgs(argc, argv);

	utils::Chronometer chrono;
	long left;
	long right;

	//PID values
	float Dp = 0.0;
	float Di = 0.0;
	float Dd = 0.0;
	float Ap = 0.0;
	float Ai = 0.0;
	float Ad = 0.0;
	int distmm = 0;
	float angle = 0.0;

	float Acc = 0.0;
	float Vmax = 0.0;
	float Dec = 0.0;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	Arguments args = robot.getArgs();
	if (args["mm"] != "0")
	{
		distmm = atoi(args["mm"].c_str());
		logger().debug() << "Arg mm set " << args["mm"] << ", distmm = " << distmm << logs::end;
	}
	if (args["Acc"] != "0")
	{
		Acc = atof(args["Acc"].c_str());
		logger().debug() << "Arg Acc set " << args["Acc"] << ", Acc = " << Acc << logs::end;
	}
	if (args["Vmax"] != "0")
	{
		Vmax = atof(args["Vmax"].c_str());
		logger().debug() << "Arg Vmax set " << args["Vmax"] << ", Vmax = " << Vmax << logs::end;
	}
	if (args["Dec"] != "0")
	{
		Dec = atof(args["Dec"].c_str());
		logger().debug() << "Arg Dec set " << args["Dec"] << ", Dec = " << Dec << logs::end;
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
		angle = atof(args["angle"].c_str());
		logger().debug() << "Arg angle set " << args["angle"] << ", angle = " << angle << logs::end;
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
	//TODO DEPRECATED ?? utiliser que dans le cas asservInsa
/*
	robot.asserv().startMotionTimerAndOdo(true);
	//surcharge des valeurs
	robot.asserv().configureAlphaPID(Ap, Ai, Ad);
	robot.asserv().configureDeltaPID(Dp, Di, Dd);
	robot.asserv().setPositionAndColor(0.0, 300.0, 0.0, (robot.getMyColor() != PMXYELLOW));
	robot.asserv().setVmax(Vmax);
	robot.asserv().setAccel(Acc);
	robot.asserv().setDecel(Dec);

	robot.svgPrintPosition();

	if (angle != 0)
		robot.asserv().doRotateAbs(angle);

	chrono.start();
	if (distmm != 0)
		robot.asserv().doLineAbs(distmm);

	left = robot.asserv().base()->encoders().getLeftEncoder();
	right = robot.asserv().base()->encoders().getRightEncoder();
	logger().info() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left=" << left
			<< " ; right= " << right << " x=" << robot.asserv().pos_getX_mm() << " y="
			<< robot.asserv().pos_getY_mm() << " degrees=" << robot.asserv().pos_getThetaInDegree()
			<< logs::end;
*/
	robot.svgPrintPosition();

	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

