#include "O_AsservTest.hpp"

#include <unistd.h>
#include <string>

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

using namespace std;

void O_AsservTest::run(int argc, char** argv)
{
	logger().info() << "Executing - " << this->desc() << logs::end;

	OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

	logger().info() << "Start Asserv " << logs::end;
	robot.asserv().startMotionTimerAndOdo(true);


	robot.asserv().setPositionAndColor(100.0, 100.0, 0.0, (robot.getMyColor() == PMXBLUE));
	RobotPosition p = robot.asserv().pos_getPosition();
	logger().info() << "p= " << p.x << " " << p.y << " " << p.theta << " " << p.asservStatus
			<< logs::end;
	robot.svgPrintPosition();

	if (robot.asserv().doLineAbs(400.0) != TRAJ_OK)
	{
		logger().info() << "Interruption !!" << logs::end;
	}
	p = robot.asserv().pos_getPosition();
	logger().info() << "p= " << p.x << " " << p.y << " " << p.theta << " " << p.asservStatus
			<< logs::end;
	robot.svgPrintPosition();

	logger().info() << "Stopping Robot... " << logs::end;
	robot.asserv().stopMotionTimerAndOdo();
	robot.stop();
	logger().info() << "Happy End." << logs::end;
}

