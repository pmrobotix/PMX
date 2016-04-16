#include "LegoEV3RobotExtended.hpp"

#include <string>

#include "../Common/State/Data.hpp"
#include "L_State1.hpp"
#include "LegoEV3IAExtended.hpp"

LegoEV3RobotExtended::LegoEV3RobotExtended()
{
	id_ = "LegoEV3Robot";
	myColor_ = PMXNOCOLOR;
	cArgs_.setDescription("(c) PM-ROBOTIX LegoEV3Robot");

	//on ecrase les versions par default avec la version extended
	actions_ = new LegoEV3ActionsExtended(id_);
	delete (actions_default);
	actions_default = actions_;

	asserv_ = new LegoEV3AsservExtended(id_);
	delete (asserv_default);
	asserv_default = asserv_;

	ia_= new LegoEV3IAExtended(id_, asserv_);

	psvg_ = new LegoEV3SvgWriterExtended(id_);
	psvg_->beginHeader();

}

void LegoEV3RobotExtended::stop()
{
	Robot::stop();
	this->actions().stop(); //extra devices

	psvg_->endHeader();
}

void LegoEV3RobotExtended::begin(int argc, char** argv)
{
	Robot::begin(argc, argv);
	logger().debug() << "begin()" << logs::end;

	//specific match case and strategies
	if (cArgs_["type"] == "m" or cArgs_["type"] == "M")
	{
		data_.isEmpty(true);
		IAutomateState* state1 = new L_State1();

		// Start the automate and wait for its return
		automate_.run(*this, state1, &data_);
	}

	logger().debug() << "PMX LegoEV3Robot - Happy End" << logs::end;
}

