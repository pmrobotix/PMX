#include "LegoEV3RobotExtended.hpp"

#include <string>

#include "../Common/Utils/Chronometer.hpp"
#include "../Thread/Thread.hpp"
#include "L_State_DecisionMakerIA.hpp"
#include "L_State_Init.hpp"
#include "L_State_WaitEndOfMatch.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3IAExtended.hpp"
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

    decisionMaker_ = NULL;

    svg_->beginHeader();

}

void LegoEV3RobotExtended::stopActions()
{
    this->actions().stop(); //extra devices

}

void LegoEV3RobotExtended::begin(int argc, char** argv)
{
    Robot::begin(argc, argv);
    logger().debug() << "LegoEV3RobotExtended::begin" << logs::end;

    //specific match case ands strategies
    if (cArgs_["type"] == "m" or cArgs_["type"] == "M") {

        this->isEmpty(true);
        decisionMaker_ = new L_State_DecisionMakerIA(*this);

        IAutomateState* stateInit = new L_State_Init();
        IAutomateState* stateWaitEndOfMatch = new L_State_WaitEndOfMatch();
        stateInit->addState("WaitEndOfMatch", stateWaitEndOfMatch);

        decisionMaker_->start("L_State_DecisionMakerIA");

        // Start the automate and wait for its return
        automate_.run(*this, stateInit);

        //attente du thread decisionMaker
        decisionMaker_->waitForEnd();

    }

    logger().debug() << "PMX LegoEV3Robot - Happy End - " << this->chrono().getElapsedTimeInSec() << " sec"
            << logs::end;
}

