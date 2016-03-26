#include "LegoEV3RobotExtended.hpp"

#include <string>

#include "../Common/State/Data.hpp"
#include "LegoEV3State1.hpp"

LegoEV3RobotExtended::LegoEV3RobotExtended()
{
	id_ = "LegoEV3Robot";

	myColor_ = PMXNOCOLOR;

	//on ecrase les versions par default avec la version extended
	actions_ = new LegoEV3ActionsExtended(id_);
	actions_default = actions_;

	asserv_ = new LegoEV3AsservExtended(id_);
	asserv_default = asserv_;

	cArgs_.setDescription("(c) PM-ROBOTIX LegoEV3Robot");

	psvg_ = new LegoEV3SvgWriterExtended(id_);
	psvg_->beginHeader();
}

void LegoEV3RobotExtended::stop()
{

	Robot::stop();
	this->actions().stop(); //extra devices
	this->asserv().stop();
	psvg_->endHeader();
}

void LegoEV3RobotExtended::begin()
{
	Robot::begin();
	logger().debug() << "LegoEV3RobotExtended::start" << logs::end;

	//specific match case and strategies
	if (cArgs_["type"] == "m" or cArgs_["type"] == "M")
	{
		data_.isEmpty(true);
		IAutomateState* state1 = new LegoEV3State1();

		// Start the automate and wait for its return
		automate_.run(*this, state1, &data_);
	}

	logger().debug() << "PMX LegoEV3RobotExtended - Happy End" << logs::end;
}

