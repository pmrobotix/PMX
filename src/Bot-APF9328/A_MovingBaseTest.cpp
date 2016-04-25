#include "A_MovingBaseTest.hpp"

#include <bits/basic_string.h>
#include <unistd.h>

#include "../Bot-LegoEV3/LegoEV3SvgWriterExtended.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "../Log/SvgWriter.hpp"
#include "APF9328RobotExtended.hpp"

using namespace std;

void A_MovingBaseTest::run(int argc, char** argv)
{

	logger().info() << "Executing - " << this->desc() << logs::end;

	APF9328RobotExtended &robot = APF9328RobotExtended::instance();
	robot.asserv().startMotionTimerAndOdo(); //just to give odometry

	robot.asserv().setPositionAndColor(0.0, 300.0, 0.0, false);

	robot.svgPrintPosition();

	robot.chrono().start();
	long left = robot.asserv().base()->encoders().getLeftEncoder();
	long right = robot.asserv().base()->encoders().getRightEncoder();
	robot.asserv().base()->motors().runMotorLeft(50, 1000);
	robot.asserv().base()->motors().runMotorRight(50, 1000);
	while (robot.chrono().getElapsedTimeInMilliSec() < 1500.0)
	{
		left = robot.asserv().base()->encoders().getLeftEncoder();
		right = robot.asserv().base()->encoders().getRightEncoder();
		logger().info() << "time= "
				<< robot.chrono().getElapsedTimeInMilliSec()
				<< " Lticks= "
				<< left
				<< " Rticks= "
				<< right
				<< " x="
				<< robot.asserv().pos_getX_mm()
				<< " y="
				<< robot.asserv().pos_getY_mm()
				<< " a="
				<< robot.asserv().pos_getThetaInDegree()
				<< logs::end;
		usleep(200000);
	}

	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

