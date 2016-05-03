#include "L_State1.hpp"

#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Robot.hpp"
#include "../Common/State/AAutomateState.hpp"
#include "../Common/State/Data.hpp"
#include "../Common/State/Wait90SecAction.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3RobotExtended.hpp"

IAutomateState*
L_State1::execute(Robot& r, void *data)
{
	logger().info() << "L_State1" << logs::end;
	Data* sharedData = (Data*) data;
	LegoEV3RobotExtended &robot = LegoEV3RobotExtended::instance();

	Wait90SecAction* action = new Wait90SecAction(robot, (void *) sharedData);
	action->start("Wait90SecAction");

	robot.asserv().startMotionTimerAndOdo();
	robot.asserv().setPositionAndColor(85, 1000, 0.0, (robot.getMyColor() == PMXGREEN));
	robot.svgPrintPosition();

	return this->getState("decisionMaker");
	return NULL; //finish all state
}

