#include "SRobotExtended.hpp"

#include <string>

#include "../Common/State/Data.hpp"
#include "../Log/Logger.hpp"
#include "SState1.hpp"

SRobotExtended::SRobotExtended()
{
	id_ = "SRobot";

	myColor_ = PMXNOCOLOR;

	//on ecrase les versions par default
	actions_ = new SActionsExtended(id_);
	actions_default = actions_;

	cArgs_.setDescription("(c) PM-ROBOTIX SRobotExtended");
}

void SRobotExtended::stop()
{
	Robot::stop();
	this->actions().stop(); //extra devices
}

void SRobotExtended::begin()
{
	Robot::begin();
	logger().info() << "SRobotExtended::start" << logs::end;


	//specific match case and strategies
	if (cArgs_["type"] == "m")
	{
		data_.isEmpty(true);
		IAutomateState* state1 = new SState1();

		// Start the automate and wait for its return
		automate_.run(*this, state1, &data_);
	}

	logger().info() << "PMX SRobotExtended - Happy End" << logs::end;

}
