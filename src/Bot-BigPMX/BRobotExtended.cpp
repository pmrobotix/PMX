
#include "BRobotExtended.hpp"

#include <string>

#include "../Common/State/Data.hpp"
#include "../Log/Logger.hpp"
#include "BState1.hpp"

BRobotExtended::BRobotExtended()
{
	id_ = "BRobot";

	myColor_ = PMXNOCOLOR;
	actions_ = new BActionsExtended(id_);

	//on ecrase les versions par default
	actions_default = actions_;

	cArgs_.setDescription("(c) PM-ROBOTIX BRobotExtended");
}

void BRobotExtended::stop()
{

	Robot::stop();
	this->actions().stop(); //extra devices
}

void BRobotExtended::begin()
{
	Robot::begin();

	logger().info() << "BRobotExtended::start" << logs::end;

	//specific match case and strategies
		if (cArgs_["type"] == "m")
		{
			data_.isEmpty(true);
			IAutomateState* state1 = new BState1();

			// Start the automate and wait for its return
			automate_.run(*this, state1, &data_);
		}
	logger().info() << "PMX BRobotExtended - Happy End" << logs::end;

}
