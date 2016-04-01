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

void LegoEV3MovingBaseTest::run(Arguments *)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	robot.svg().logger().info() << "test LegoEV3LedBarTest" << logs::end;
	robot.svg().writePosition(200, 300, 0, LEGOEV3_SVG_POS_ROBOT);

	utils::Chronometer chrono;

	logger().info() << "runMotorLeft(200, 1000) " << logs::end;
	robot.asserv().base().motors().runMotorLeft(128, 1000); //run (power 128) pendant 1s

	chrono.start();
	while (chrono.getElapsedTimeInMilliSec() < 1500.0)
	{
		long left = robot.asserv().base().encoders().getLeftEncoder();
		logger().debug() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left ticks= " << left << logs::end;
		usleep(500000);
	}

	logger().info() << "STOPPED time= "
			<< chrono.getElapsedTimeInMilliSec()
			<< "ms ; left ticks= "
			<< robot.asserv().base().encoders().getLeftEncoder()
			<< logs::end;
	usleep(500000);

	logger().info() << "runMotorLeft(-100, 1000) " << logs::end;
	robot.asserv().base().motors().runMotorLeft(-100, 1000); //run with power -100 pendant 1s
	chrono.start();
	while (chrono.getElapsedTimeInMilliSec() < 1500.0)
	{
		long left = robot.asserv().base().encoders().getLeftEncoder();
		logger().debug() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left ticks= " << left << logs::end;
		usleep(500000);
	}

	logger().info() << "setMotorLeftPosition(200, 100) " << logs::end;
	robot.asserv().base().motors().setMotorLeftPosition(128, 100); //run until 100 ticks
	chrono.start();
	while (chrono.getElapsedTimeInMilliSec() < 1000.0)
	{
		long left = robot.asserv().base().encoders().getLeftEncoder();
		logger().debug() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left ticks= " << left << logs::end;
		usleep(500000);
	}

	logger().info() << "STOPPED time= "
			<< chrono.getElapsedTimeInMilliSec()
			<< "ms ; left ticks= "
			<< robot.asserv().base().encoders().getLeftEncoder()
			<< logs::end;

	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

