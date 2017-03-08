#include "OPOS6UL_RobotExtended.hpp"

#include <string>

#include "../Common/Asserv/MotorControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/State/Data.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "O_State_DecisionMaker.hpp"
#include "O_State1.hpp"
#include "OPOS6UL_SvgWriterExtended.hpp"

OPOS6UL_RobotExtended::OPOS6UL_RobotExtended()
{
	id_ = "OPOS6UL_Robot";
	myColor_ = PMXNOCOLOR;
	cArgs_.setDescription("(c) PM-ROBOTIX OPOS6UL_Robot");

	//on ecrase les versions par default avec la version extended
	p_svg_ = new OPOS6UL_SvgWriterExtended(id_);
	svg_ = p_svg_;

	p_asserv_ = new OPOS6UL_AsservExtended(id_, this);
	asserv_default = p_asserv_;

	p_actions_ = new OPOS6UL_ActionsExtended(id_, this);
	actions_default = p_actions_;

	p_ia_ = new OPOS6UL_IAExtended(id_, this);

	svg_->beginHeader();
	sharedData = NULL;
}

void OPOS6UL_RobotExtended::stop()
{
	Robot::stop();
	this->actions().stop(); //extra devices
	this->asserv().freeMotion();
	this->asserv().base()->motors().stopMotors();

	svg_->endHeader();
}

void OPOS6UL_RobotExtended::begin(int argc, char** argv)
{
	Robot::begin(argc, argv);

	logger().info() << "OPOS6UL_RobotExtended::start" << logs::end;

	//specific match case and strategies
	if (cArgs_["type"] == "m" or cArgs_["type"] == "M")
	{
		data_.isEmpty(true);
		IAutomateState* state1 = new O_State1();
		IAutomateState* decisionMaker = new O_State_DecisionMaker();

		state1->addState("decisionMaker", decisionMaker);

		// Start the automate and wait for its return
		automate_.run(*this, state1, &data_);
	}

	logger().info() << "PMX OPOS6UL_RobotExtended - Happy End - "
			<< this->chrono().getElapsedTimeInSec()
			<< " sec"
			<< logs::end;
}
