#include "OPOS6UL_RobotExtended.hpp"

#include <string>

#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "../Thread/Thread.hpp"
#include "O_State_DecisionMakerIA.hpp"
#include "O_State_Init.hpp"
#include "O_State_WaitEndOfMatch.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_IAExtended.hpp"
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

    decisionMaker_ = NULL;

    //init SVG log file
    svg_->beginHeader();

    //init domotique + abeille placés
    points = 10;
}

void OPOS6UL_RobotExtended::stopActions()
{
    this->actions().stop(); //extra devices

}

void OPOS6UL_RobotExtended::begin(int argc, char** argv)
{
    Robot::begin(argc, argv);

    logger().debug() << "OPOS6UL_RobotExtended::begin" << logs::end;

    //specific match cases and strategies
    if (cArgs_["type"] == "m" or cArgs_["type"] == "M") {
        this->isEmpty(true);

        decisionMaker_ = new O_State_DecisionMakerIA(*this);

        IAutomateState* stateInit = new O_State_Init();
        IAutomateState* stateWaitEndOfMatch = new O_State_WaitEndOfMatch();
        stateInit->addState("WaitEndOfMatch", stateWaitEndOfMatch);

        decisionMaker_->start("O_State_DecisionMakerIA");

        // Start the automate and wait for its return
        automate_.run(*this, stateInit);

        //attente du thread decisionMaker
        decisionMaker_->waitForEnd();
    }

    logger().info() << "PMX OPOS6UL_RobotExtended - Happy End - " << this->chrono().getElapsedTimeInSec() << " sec" << logs::end;
}
