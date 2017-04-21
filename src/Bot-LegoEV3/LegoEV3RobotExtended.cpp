#include "LegoEV3RobotExtended.hpp"

#include <string>

#include "../Common/Asserv/MotorControl.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/State/Data.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "L_State_DecisionMaker.hpp"
#include "L_State1.hpp"
#include "LegoEV3SvgWriterExtended.hpp"

LegoEV3RobotExtended::LegoEV3RobotExtended()
{
	id_ = "LegoEV3Robot";
	myColor_ = PMXNOCOLOR;
	cArgs_.setDescription("(c) PM-ROBOTIX LegoEV3Robot");

	p_svg_ = new LegoEV3SvgWriterExtended(id_);
	svg_ = p_svg_;

	p_asserv_ = new LegoEV3AsservExtended(id_, this);
	asserv_default = p_asserv_;

	//on ecrase les versions par default avec la version extended
	p_actions_ = new LegoEV3ActionsExtended(id_, this);
	actions_default = p_actions_;

	p_ia_ = new LegoEV3IAExtended(id_, this);

	svg_->beginHeader();
	sharedData = NULL;

}
void LegoEV3RobotExtended::baseStop()
{
	this->asserv().freeMotion();
	this->asserv().base()->motors().stopMotors();
printf ("\nsvg end header\n");
	svg_->endHeader();
}
void LegoEV3RobotExtended::stop()
{

	Robot::stop();
	this->actions().stop(); //extra devices

	baseStop();
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
		IAutomateState* decisionMaker = new L_State_DecisionMaker();

		state1->addState("decisionMaker", decisionMaker);

		// Start the automate and wait for its return
		automate_.run(*this, state1, &data_);
	}

	logger().debug() << "PMX LegoEV3Robot - Happy End - " << this->chrono().getElapsedTimeInSec()
			<< " sec" << logs::end;
}

