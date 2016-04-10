#include "APF9328RobotExtended.hpp"

#include <string>

#include "../Common/State/Data.hpp"
#include "../Log/Logger.hpp"
#include "APF9328AsservExtended.hpp"
#include "APF9328State1.hpp"

APF9328RobotExtended::APF9328RobotExtended()
{
	id_ = "APF9328Robot";
	myColor_ = PMXNOCOLOR;
	cArgs_.setDescription("(c) PM-ROBOTIX APF9328Robot");

	//on ecrase les versions par default avec la version extended
	actions_ = new APF9328ActionsExtended(id_);
	delete (actions_default);
	actions_default = actions_;

	asserv_ = new APF9328AsservExtended(id_);
	delete (asserv_default);
	asserv_default = asserv_;

	psvg_ = new APF9328SvgWriterExtended(id_);
	psvg_->beginHeader();
}

void APF9328RobotExtended::stop()
{
	Robot::stop();
	this->actions().stop(); //extra devices
	this->asserv().freeMotion();
	psvg_->endHeader();
}

void APF9328RobotExtended::begin(int argc, char** argv)
{
	Robot::begin(argc, argv);

	logger().info() << "APF9328RobotExtended::start" << logs::end;

	//specific match case and strategies
	if (cArgs_["type"] == "m" or cArgs_["type"] == "M")
	{
		data_.isEmpty(true);
		IAutomateState* state1 = new APF9328State1();

		// Start the automate and wait for its return
		automate_.run(*this, state1, &data_);
	}

	logger().info() << "PMX APF9328RobotExtended - Happy End" << logs::end;
}
