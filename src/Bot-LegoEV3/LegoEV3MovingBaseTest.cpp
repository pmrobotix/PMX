#include "LegoEV3MovingBaseTest.hpp"

#include <unistd.h>
#include <string>

#include "../Common/Asserv/EncoderControl.hpp"
#include "../Common/Asserv/MotorControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "../Log/SvgWriter.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3RobotExtended.hpp"
#include "LegoEV3SvgWriterExtended.hpp"

using namespace std;

void LegoEV3MovingBaseTest::run(int argc, char** argv)
{

	logger().info() << "Executing - " << this->desc() << logs::end;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	robot.asserv().startMotionTimerAndOdo(); //just to give odometry

	robot.svg().logger().info() << "test LegoEV3LedBarTest" << logs::end;
	robot.svg().writePosition(robot.asserv().pos_getX_mm(),
			robot.asserv().pos_getY_mm(),
			robot.asserv().pos_getTheta(),
			LEGOEV3_SVG_POS_ROBOT);

	utils::Chronometer chrono;

	long left = 0;
	long right = 0;

	logger().info() << "runMotor(200, 1000) " << logs::end;
	robot.asserv().base()->motors().runMotorLeft(128, 1000); //run (power 128) pendant 1s
	robot.asserv().base()->motors().runMotorRight(128, 1000); //run (power 128) pendant 1s
	chrono.start();
	while (chrono.getElapsedTimeInMilliSec() < 1500.0)
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
	usleep(500000);

	logger().info() << "runMotor(-100, 1000) " << logs::end;
	robot.asserv().base()->motors().runMotorLeft(-100, 1000); //run with power -100 pendant 1s
	robot.asserv().base()->motors().runMotorRight(-100, 1000); //run with power -100 pendant 1s
	chrono.start();
	while (chrono.getElapsedTimeInMilliSec() < 1500.0)
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

	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

