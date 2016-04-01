#include "LegoEV3AsservInsaTest.hpp"

#include <string>

#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Asserv/EncoderControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void LegoEV3AsservInsaTest::run(Arguments *)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	utils::Chronometer chrono;
	long left;
	long right;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	robot.asserv().startMotionTimerAndOdo();
	chrono.start();
	robot.asserv().cc_move(100);

	left = robot.asserv().base().encoders().getLeftEncoder();
	right = robot.asserv().base().encoders().getRightEncoder();
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

	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

