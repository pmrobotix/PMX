#include "APF9328RobotExtended.hpp"

#include <string>

#include "../Common/Asserv/MotorControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/State/Data.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "A_State_DecisionMaker.hpp"
#include "A_State1.hpp"
#include "APF9328SvgWriterExtended.hpp"

APF9328RobotExtended::APF9328RobotExtended()
{
	id_ = "APF9328Robot";
	myColor_ = PMXNOCOLOR;
	cArgs_.setDescription("(c) PM-ROBOTIX APF9328Robot");

	//on ecrase les versions par default avec la version extended
	p_svg_ = new APF9328SvgWriterExtended(id_);
	svg_ = p_svg_;

	p_asserv_ = new APF9328AsservExtended(id_, this);
	asserv_default = p_asserv_;

	p_actions_ = new APF9328ActionsExtended(id_, this);
	actions_default = p_actions_;

	p_ia_ = new APF9328IAExtended(id_, this);

	svg_->beginHeader();
}

void APF9328RobotExtended::stop()
{
	Robot::stop();
	this->actions().stop(); //extra devices
	this->asserv().freeMotion();
	this->asserv().base()->motors().stopMotors();

	svg_->endHeader();
}

void APF9328RobotExtended::begin(int argc, char** argv)
{
	Robot::begin(argc, argv);

	logger().info() << "APF9328RobotExtended::start" << logs::end;

	//specific match case and strategies
	if (cArgs_["type"] == "m" or cArgs_["type"] == "M")
	{
		data_.isEmpty(true);
		IAutomateState* state1 = new A_State1();
		IAutomateState* decisionMaker = new A_State_DecisionMaker();

		state1->addState("decisionMaker", decisionMaker);

		// Start the automate and wait for its return
		automate_.run(*this, state1, &data_);
	}

	logger().info() << "PMX APF9328RobotExtended - Happy End - "
			<< this->chrono().getElapsedTimeInSec()
			<< " sec"
			<< logs::end;
}
