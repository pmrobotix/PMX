#include "L_State1.hpp"

#include <unistd.h>

#include "../Common/Robot.hpp"
#include "../Common/State/Data.hpp"
#include "../Common/State/Wait90SecAction.hpp"
#include "../Log/Logger.hpp"

IAutomateState*
L_State1::execute(Robot& robot, void *data)
{
	logger().info() << "L_State1" << logs::end;
	Data* sharedData = (Data*) data;

	Wait90SecAction* action = new Wait90SecAction(robot, (void *) sharedData);
	action->start("Wait90SecAction");

	sleep(11);
	return NULL; //finish all state
}

