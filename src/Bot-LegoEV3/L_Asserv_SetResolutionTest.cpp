#include "L_Asserv_SetResolutionTest.hpp"

#include <unistd.h>
#include <cstdlib>
#include <string>

#include "../Asserv.Insa/AsservInsa.hpp"
#include "../Common/Arguments.hpp"
#include "../Common/Asserv/EncoderControl.hpp"
#include "../Common/Asserv/MotorControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "../Log/SvgWriter.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void L_Asserv_SetResolutionTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	robot.getArgs().addArgument("distTicks", "Distance (ticks) for test");
	robot.getArgs().addArgument("p", "power", "100");

	//reparse arguments
	robot.parseConsoleArgs(argc, argv);
}

void L_Asserv_SetResolutionTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;
	configureConsoleArgs(argc, argv);

	utils::Chronometer chrono;
	long left;
	long right;

	//PID values
	float Dp = 0.0;
	float Dd = 0.0;
	float Ap = 0.0;
	float Ad = 0.0;
	int distTicks = 0;
	int p = 0;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	Arguments args = robot.getArgs();
	if (args["distTicks"] != "0")
	{
		distTicks = atoi(args["distTicks"].c_str());
		logger().info() << "Arg distTicks set "
				<< args["distTicks"]
				<< ", distTicks = "
				<< distTicks
				<< logs::end;
	}
	if (args["p"] != "0")
	{
		p = atoi(args["p"].c_str());
		logger().info() << "Arg p set " << args["p"] << ", p = " << p << " power" << logs::end;
	}
	//TODO deprecated ? a utiliser que pour asservInsa
/*
	robot.asserv().startMotionTimerAndOdo();

	robot.asserv().configureAlphaPID(Ap, 0.0, Ad); //surcharge
	robot.asserv().configureDeltaPID(Dp, 0.0, Dd);
	robot.asserv().setPositionAndColor(0.0, 300.0, 0.0, false);

	robot.svgw().writePosition(robot.asserv().pos_getX_mm(),
			robot.asserv().pos_getY_mm(),
			robot.asserv().pos_getTheta(),
			"bot");

	chrono.start();

	robot.asserv().base()->motors().runMotorLeft(p, 0);
	robot.asserv().base()->motors().runMotorRight(p, 0);

	left = robot.asserv().base()->encoders().getLeftEncoder();
	right = robot.asserv().base()->encoders().getRightEncoder();
	while (left < distTicks && right < distTicks)
	{
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
		usleep(10000);
	}
	robot.asserv().base()->motors().stopMotors();

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

	float setR = 1000.0 * (float) distTicks;	// / robot.asserv().pos_getX_mm();

	logger().info() << "current setResolutionLEFT = "
			<< robot.asserv().insa()->encoder_GetLeftResolution()
			<< "  setResolutionRIGHT = "
			<< robot.asserv().insa()->encoder_GetRightResolution()

			<< logs::end;
	logger().info() << "RESULT: setResolution (nb ticks pour 1m) = 1000* distT / mesure"
			<< logs::end;
	logger().info() << "RESULT: setResolution = " << setR << " / mesure_mm" << logs::end;
*/
	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

