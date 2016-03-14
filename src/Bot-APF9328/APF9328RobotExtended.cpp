
#include "APF9328RobotExtended.hpp"

#include <string>

#include "../Common/State/Data.hpp"
#include "../Log/Logger.hpp"
#include "APF9328State1.hpp"

APF9328RobotExtended::APF9328RobotExtended()
{
	id_ = "APF9328Robot";

	myColor_ = PMXNOCOLOR;
	actions_ = new APF9328ActionsExtended(id_);

	//on ecrase les versions par default
	actions_default = actions_;

	cArgs_.setDescription("(c) PM-ROBOTIX APF9328RobotExtended");
}

void APF9328RobotExtended::stop()
{

	Robot::stop();
	this->actions().stop(); //extra devices
}

void APF9328RobotExtended::begin()
{
	Robot::begin();

	logger().info() << "APF9328RobotExtended::start" << logs::end;

	//specific match case and strategies
		if (cArgs_["type"] == "m")
		{
			data_.isEmpty(true);
			IAutomateState* state1 = new APF9328State1();

			// Start the automate and wait for its return
			automate_.run(*this, state1, &data_);
		}
	logger().info() << "PMX APF9328RobotExtended - Happy End" << logs::end;

}
