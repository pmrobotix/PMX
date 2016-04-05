#include "LegoEV3AsservSetResolutionTest.hpp"

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

void LegoEV3AsservSetResolutionTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	robot.getArgs().addArgument("distTicks", "Distance (ticks) for test");

	//reparse arguments
	robot.parseConsoleArgs(argc, argv);
}

void LegoEV3AsservSetResolutionTest::run(int argc, char** argv)
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

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	Arguments args = robot.getArgs();
	if (args["distTicks"] != "0")
	{
		distTicks = atoi(args["distTicks"].c_str());
		logger().debug() << "Arg distTicks set " << args["distTicks"] << ", distTicks = " << distTicks << logs::end;
	}

	robot.asserv().startMotionTimerAndOdo();

	robot.asserv().configureAlphaPID(Ap, 0.0, Ad); //surcharge
	robot.asserv().configureDeltaPID(Dp, 0.0, Dd);
	robot.asserv().setPositionAndColor(0.0, 300.0, 0.0, false);

	robot.svg().writePosition(robot.asserv().pos_getX_mm(),
			robot.asserv().pos_getY_mm(),
			robot.asserv().pos_getTheta(),
			LEGOEV3_SVG_POS_ROBOT);

	chrono.start();

	//avancer d'un nombre de tick
	//mesurer
	//distT ticks = odo_mm mm
	//donc setResolution (nb ticks pour 1m) = 1000* distT / odo_mm

	robot.asserv().base()->motors().runMotorLeft(200, 0);
	robot.asserv().base()->motors().runMotorRight(200, 0);

	left = robot.asserv().base()->encoders().getLeftEncoder();
	right = robot.asserv().base()->encoders().getRightEncoder();
	while (left < distTicks && right < distTicks)
	{
		left = robot.asserv().base()->encoders().getLeftEncoder();
		right = robot.asserv().base()->encoders().getRightEncoder();
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

	robot.svg().writePosition(robot.asserv().pos_getX_mm(),
			robot.asserv().pos_getY_mm(),
			robot.asserv().pos_getTheta(),
			LEGOEV3_SVG_POS_ROBOT);

	float setR = 1000.0 * (float) distTicks ;// / robot.asserv().pos_getX_mm();


	logger().info() << "current setResolutionLEFT = "
			<< robot.asserv().insa()->encoder_GetLeftResolution()
			<< "  setResolutionRIGHT = "
			<< robot.asserv().insa()->encoder_GetRightResolution()

			<< logs::end;
	logger().info() << "RESULT: setResolution (nb ticks pour 1m) = 1000* distT / mesure" << logs::end;
	logger().info() << "RESULT: setResolution = " << setR << " / mesure_mm" << logs::end;

	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

