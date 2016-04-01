#include "LegoEV3FindPIDTest.hpp"

#include <string>

#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Asserv/EncoderControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void LegoEV3FindPIDTest::run(Arguments *)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	utils::Chronometer chrono;
	long left;
	long right;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	robot.asserv().startMotionTimerAndOdo();
	chrono.start();
	robot.asserv().cc_move(300);

	left = robot.asserv().base().encoders().getLeftEncoder();
	right = robot.asserv().base().encoders().getRightEncoder();
	logger().info() << "time= " << chrono.getElapsedTimeInMilliSec() << "ms ; left= " << left << " ; right= " << right
			<< logs::end;

	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

