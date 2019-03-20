#include "A_MovingBaseTest.hpp"

#include <unistd.h>
#include <cstdlib>
#include <string>
#include <iomanip>

#include "../Common/Arguments.hpp"
#include "../Common/Asserv/EncoderControl.hpp"
#include "../Common/Asserv/ExtEncoderControl.hpp"
#include "../Common/Asserv/MotorControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Robot.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "APF9328AsservExtended.hpp"
#include "APF9328RobotExtended.hpp"

using namespace std;

void A_MovingBaseTest::configureConsoleArgs(int argc, char** argv) //surcharge
{
	APF9328RobotExtended &robot = APF9328RobotExtended::instance();

	robot.getArgs().addArgument("LR", "L(0) or R(1) or LR(2)", "2");
	robot.getArgs().addArgument("t", "time (ms)", "3000");
	robot.getArgs().addArgument("p", "power", "10");
	robot.getArgs().addArgument("d", "distance ticks", "500");

	//reparse arguments
	robot.parseConsoleArgs(argc, argv);
}

void A_MovingBaseTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;
	configureConsoleArgs(argc, argv); //on appelle les parametres specifiques pour ce test

	APF9328RobotExtended &robot = APF9328RobotExtended::instance();

	Arguments args = robot.getArgs();

	int LR = 0;
	int d = 0;
	int p = 0;
	int t = 0;
	if (args["LR"] != "0")
	{
		LR = atoi(args["LR"].c_str());
		logger().info() << "Arg LR set " << args["LR"] << ", LR = " << LR << logs::end;
	}
	if (args["t"] != "0")
	{
		t = atoi(args["t"].c_str());
		logger().info() << "Arg t set " << args["t"] << ", t = " << t << " ms" << logs::end;
	}
	if (args["p"] != "0")
	{
		p = atoi(args["p"].c_str());
		logger().info() << "Arg p set " << args["p"] << ", p = " << p << " power" << logs::end;
	}
	if (args["d"] != "0")
	{
		d = atoi(args["d"].c_str());
		logger().info() << "Arg d set " << args["d"] << ", d = " << d << " ticks" << logs::end;
	}

	robot.asserv().startMotionTimerAndOdo(); //just to give odometry

	robot.asserv().setPositionAndColor(0.0, 300.0, 0.0, (robot.getMyColor() == PMXVIOLET));

	robot.svgPrintPosition();

	utils::Chronometer chrono;

	long left = 0;
	long right = 0;

	long extleft = 0;
	long extright = 0;

	logger().info() << "runMotor(" << p << ", " << t << ") " << logs::end;
	if (LR == 0 || LR == 2)
		robot.asserv().base()->motors().runMotorLeft(p, t);
	if (LR == 1 || LR == 2)
		robot.asserv().base()->motors().runMotorRight(p, t);
	chrono.start();
	while (chrono.getElapsedTimeInMilliSec() < t + 100)
	{
		left = robot.asserv().base()->encoders().getLeftEncoder();
		right = robot.asserv().base()->encoders().getRightEncoder();

		extleft = robot.asserv().base()->extEncoders().getLeftEncoder();
		extright = robot.asserv().base()->extEncoders().getRightEncoder();
		logger().info() << "t="
				<< std::setw(10) << chrono.getElapsedTimeInMilliSec()
				<< " Lticks="
				<< std::setw(8) << left
				<< " Rticks="
				<< std::setw(8) << right
				<< " extL="
				<< std::setw(8) << extleft
				<< " extR="
				<< std::setw(8) << extright
				<< " x="
				<< std::setw(8) << robot.asserv().pos_getX_mm()
				<< " y="
				<< std::setw(8) << robot.asserv().pos_getY_mm()
				<< " a="
				<< std::setw(8) << robot.asserv().pos_getThetaInDegree()
				<< logs::end;
		usleep(300000);
	}

	logger().info() << "STOPPED t= "
			<< chrono.getElapsedTimeInMilliSec()
			<< " Lticks= "
			<< robot.asserv().base()->encoders().getLeftEncoder()
			<< " Rticks= "
			<< robot.asserv().base()->encoders().getRightEncoder()
			<< " extL= "
			<< extleft
			<< " extR= "
			<< extright
			<< " x="
			<< robot.asserv().pos_getX_mm()
			<< " y="
			<< robot.asserv().pos_getY_mm()
			<< " a="
			<< robot.asserv().pos_getThetaInDegree()
			<< logs::end;
	robot.svgPrintPosition();
	/*
	 usleep(1000000);

	 logger().info() << "runMotor(-" << p << ", 1000) " << logs::end;
	 if (LR == 0 || LR == 2)
	 robot.asserv().base()->motors().runMotorLeft(-p, 2000);
	 if (LR == 1 || LR == 2)
	 robot.asserv().base()->motors().runMotorRight(-p, 2000);
	 chrono.start();
	 while (chrono.getElapsedTimeInMilliSec() < 2500.0)
	 {
	 left = robot.asserv().base()->encoders().getLeftEncoder();
	 right = robot.asserv().base()->encoders().getRightEncoder();

	 extleft = robot.asserv().base()->extEncoders().getLeftEncoder();
	 extright = robot.asserv().base()->extEncoders().getRightEncoder();
	 logger().info() << "time= "
	 << chrono.getElapsedTimeInMilliSec()
	 << " Lticks= "
	 << left
	 << " Rticks= "
	 << right
	 << " extLticks= "
	 << extleft
	 << " extRticks= "
	 << extright
	 << " x="
	 << robot.asserv().pos_getX_mm()
	 << " y="
	 << robot.asserv().pos_getY_mm()
	 << " a="
	 << robot.asserv().pos_getThetaInDegree()
	 << logs::end;
	 usleep(200000);
	 }

	 logger().info() << "STOPPED time= "
	 << chrono.getElapsedTimeInMilliSec()
	 << " Lticks= "
	 << robot.asserv().base()->encoders().getLeftEncoder()
	 << " Rticks= "
	 << robot.asserv().base()->encoders().getRightEncoder()
	 << " extLticks= "
	 << extleft
	 << " extRticks= "
	 << extright
	 << " x="
	 << robot.asserv().pos_getX_mm()
	 << " y="
	 << robot.asserv().pos_getY_mm()
	 << " a="
	 << robot.asserv().pos_getThetaInDegree()
	 << logs::end;
	 */
	robot.svgPrintPosition();

	robot.stop();

	logger().info() << "Happy End." << logs::end;
}

