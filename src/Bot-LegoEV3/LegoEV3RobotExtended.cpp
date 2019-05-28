#include "LegoEV3RobotExtended.hpp"

#include <string>

#include "../Common/Action/LcdShield.hpp"
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
    asserv_default_ = p_asserv_;

    //on ecrase les versions par default avec la version extended
    p_actions_ = new LegoEV3ActionsExtended(id_, this);
    actions_default_ = p_actions_;

    p_ia_ = new LegoEV3IAExtended(id_, this);

    decisionMaker_ = NULL;

    svg_->beginHeader();

    points = 0;

}

void LegoEV3RobotExtended::displayPoints()
{
    this->actions().lcd().clear();
    this->actions().lcd().display_content_string("Points = ", 4);
    this->actions().lcd().display_content_integer(this->points, 5);

}

void LegoEV3RobotExtended::stopExtraActions()
{
    this->actions().stopExtra(); //extra devices
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

    logger().info() << "PMX LegoEV3Robot - Happy End - " << this->chrono().getElapsedTimeInSec() << " sec" << logs::end;
}

