#include "LegoEV3AsservInsaTest.hpp"

#include <string>

#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Asserv/EncoderControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3RobotExtended.hpp"

using namespace std;

void LegoEV3AsservInsaTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	utils::Chronometer chrono;
	long left;
	long right;

	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();
	robot.asserv().startMotionTimerAndOdo();

	robot.svg().writePosition(robot.asserv().pos_getX_mm(),
				robot.asserv().pos_getY_mm(),
				robot.asserv().pos_getTheta(),
				LEGOEV3_SVG_POS_ROBOT);

	chrono.start();
	robot.asserv().cc_move(1000);

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

	robot.svg().writePosition(robot.asserv().pos_getX_mm(),
				robot.asserv().pos_getY_mm(),
				robot.asserv().pos_getTheta(),
				LEGOEV3_SVG_POS_ROBOT);
	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

