#include "APF9328State1.hpp"

#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Robot.hpp"
#include "../Common/State/AAutomateState.hpp"
#include "../Log/Logger.hpp"
#include "APF9328RobotExtended.hpp"

IAutomateState*
APF9328State1::execute(Robot&, void *)
{
	logger().info() << "APF9328Start 1" << logs::end;

	APF9328RobotExtended &robot = APF9328RobotExtended::instance();

	robot.asserv().startMotionTimerAndOdo();
	robot.asserv().setPositionAndColor(150, 1250, 90.0, (robot.getMyColor() == PMXGREEN));
	robot.svgPrintPosition();

	return this->getState("decisionMaker");
	//return NULL; //finish all state
}

