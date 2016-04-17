#include "APF9328MovingBaseTest.hpp"

#include <bits/basic_string.h>
#include <unistd.h>

#include "../Bot-LegoEV3/LegoEV3SvgWriterExtended.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "../Log/SvgWriter.hpp"
#include "APF9328RobotExtended.hpp"

using namespace std;

void APF9328MovingBaseTest::run(int argc, char** argv)
{

	logger().info() << "Executing - " << this->desc() << logs::end;

	APF9328RobotExtended &robot = APF9328RobotExtended::instance();
	robot.asserv().startMotionTimerAndOdo(); //just to give odometry

	robot.svgw().logger().info() << "test LegoEV3LedBarTest" << logs::end;
	robot.svgw().writePosition(robot.asserv().pos_getX_mm(),
			robot.asserv().pos_getY_mm(),
			robot.asserv().pos_getTheta(),
			"bot");

	utils::Chronometer chrono;
	chrono.start();
	long left = robot.asserv().base()->encoders().getLeftEncoder();
	long right = robot.asserv().base()->encoders().getRightEncoder();
	robot.asserv().base()->motors().runMotorLeft(10, 0); //run (power 127) pendant 1s
	robot.asserv().base()->motors().runMotorRight(10, 0);
	while (chrono.getElapsedTimeInMilliSec() < 4500.0)
	{
		left = robot.asserv().base()->encoders().getLeftEncoder();
		right = robot.asserv().base()->encoders().getRightEncoder();
		logger().info() << "time= "
				<< chrono.getElapsedTimeInMilliSec()
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
	/*
	 logger().info() << "runMotor(127, 1000) " << logs::end;
	 robot.asserv().base()->motors().runMotorLeft(127, 1000); //run (power 127) pendant 1s
	 robot.asserv().base()->motors().runMotorRight(127, 1000); //run (power 127) pendant 1s

	 while (chrono.getElapsedTimeInMilliSec() < 4500.0)
	 {
	 left = robot.asserv().base()->encoders().getLeftEncoder();
	 right = robot.asserv().base()->encoders().getRightEncoder();
	 logger().debug() << "time= "
	 << chrono.getElapsedTimeInMilliSec()
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

	 logger().info() << "STOPPED time= "
	 << chrono.getElapsedTimeInMilliSec()
	 << " Lticks= "
	 << robot.asserv().base()->encoders().getLeftEncoder()
	 << " Rticks= "
	 << robot.asserv().base()->encoders().getRightEncoder()
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
	 APF9328_SVG_POS_ROBOT);
	 */
	/*
	 usleep(500000);

	 logger().info() << "runMotor(-100, 1000) " << logs::end;
	 robot.asserv().base()->motors().runMotorLeft(-100, 1000); //run with power -100 pendant 1s
	 robot.asserv().base()->motors().runMotorRight(-100, 1000); //run with power -100 pendant 1s
	 chrono.start();
	 while (chrono.getElapsedTimeInMilliSec() < 1500.0)
	 {
	 long left = robot.asserv().base()->encoders().getLeftEncoder();
	 long right = robot.asserv().base()->encoders().getRightEncoder();
	 logger().debug() << "time= "
	 << chrono.getElapsedTimeInMilliSec()
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

	 logger().info() << "STOPPED time= "
	 << chrono.getElapsedTimeInMilliSec()
	 << " Lticks= "
	 << robot.asserv().base()->encoders().getLeftEncoder()
	 << " Rticks= "
	 << robot.asserv().base()->encoders().getRightEncoder()
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


	 usleep(500000);

	 int dist_ticks = 1000;
	 logger().info() << "setMotorPosition(120, " << dist_ticks << ") " << logs::end;
	 robot.asserv().base()->motors().setMotorLeftPosition(120, dist_ticks); //run until 300 ticks
	 robot.asserv().base()->motors().setMotorRightPosition(120, dist_ticks); //run until 300 ticks
	 chrono.start();
	 left = robot.asserv().base()->encoders().getLeftEncoder();
	 right = robot.asserv().base()->encoders().getRightEncoder();
	 while (left < dist_ticks)
	 {
	 left = robot.asserv().base()->encoders().getLeftEncoder();
	 right = robot.asserv().base()->encoders().getRightEncoder();
	 logger().debug() << "time= "
	 << chrono.getElapsedTimeInMilliSec()
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

	 logger().info() << "STOPPED time= "
	 << chrono.getElapsedTimeInMilliSec()
	 << " Lticks= "
	 << robot.asserv().base()->encoders().getLeftEncoder()
	 << " Rticks= "
	 << robot.asserv().base()->encoders().getRightEncoder()
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

	 usleep(1000000);

	 int dist_ticks2 = 100;
	 logger().info() << "setMotorPosition(-50, " << dist_ticks2 << ") " << logs::end;
	 robot.asserv().base()->motors().setMotorLeftPosition(-50, dist_ticks2); //run until 100 ticks
	 robot.asserv().base()->motors().setMotorRightPosition(-50, dist_ticks2); //run until 100 ticks
	 chrono.start();
	 while (left > dist_ticks - dist_ticks2)
	 {
	 left = robot.asserv().base()->encoders().getLeftEncoder();
	 right = robot.asserv().base()->encoders().getRightEncoder();
	 logger().debug() << "time= "
	 << chrono.getElapsedTimeInMilliSec()
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

	 logger().info() << "STOPPED time= "
	 << chrono.getElapsedTimeInMilliSec()
	 << " Lticks= "
	 << robot.asserv().base()->encoders().getLeftEncoder()
	 << " Rticks= "
	 << robot.asserv().base()->encoders().getRightEncoder()
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
	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

