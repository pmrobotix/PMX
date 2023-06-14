#include "OPOS6UL_RobotExtended.hpp"

#include <string>

#include "../Common/Action/LcdShield.hpp"
#include "../Common/Action/LedBar.hpp"
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
    OPOS6UL_SvgWriterExtended *p_svg = new OPOS6UL_SvgWriterExtended(id_);
    setSVG(p_svg);

    OPOS6UL_AsservExtended * p_asserv = new OPOS6UL_AsservExtended(id_, this);
    //asserv_default_ = p_asserv_;
    setAsserv(p_asserv);

    p_actions_ = new OPOS6UL_ActionsExtended(id_, this);
    actions_default_ = p_actions_;

    p_ia_ = new OPOS6UL_IAExtended(id_, this);

    decisionMaker_ = NULL;

    //init SVG log file
    p_svg->beginHeader();

    points = 0;
    //2023
    force_end_of_match = false;
}

OPOS6UL_RobotExtended::~OPOS6UL_RobotExtended()
{
    this->asserv().endWhatTodo(); //on termine le thread d'asserv qui lie la position
    this->actions().stopExtra(); //extra devices
}

void OPOS6UL_RobotExtended::displayPoints()
{
    this->actions().lcd2x16().clear();
    this->actions().lcd2x16().setCursor(0, 0);
    this->actions().lcd2x16().print("Points = ");
    this->actions().lcd2x16().print(this->points);
}

void OPOS6UL_RobotExtended::stopExtraActions() {
    this->actions().stopExtra(); //extra devices
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

        decisionMaker_->start("O_State_DecisionMakerIA", 3);

        // Start the automate and wait for its return
        automate_.run(*this, stateInit);

        //attente du thread decisionMaker
        decisionMaker_->waitForEnd();
    }

    logger().info() << "PMX " << this->getID() << " Happy End - " << this->chrono().getElapsedTimeInSec() << " sec" << logs::end;
}

void OPOS6UL_RobotExtended::resetDisplayTS()
{
    actions().ledBar().resetAll();
}

void OPOS6UL_RobotExtended::displayTS(TRAJ_STATE ts)
{
    svgPrintPosition(2);

    if (ts == TRAJ_NEAR_OBSTACLE) {

        actions().ledBar().setOn(4);
    }
    if (ts == TRAJ_COLLISION) {
        actions().ledBar().setOn(2);
        actions().ledBar().setOn(1);
    }
    if (ts == TRAJ_FINISHED) {
        actions().ledBar().setOn(0);

    }
}

void OPOS6UL_RobotExtended::resetDisplayObstacle()
{
    actions().ledBar().resetAll();

}

void OPOS6UL_RobotExtended::displayObstacle(int level)
{
    if (level == 1)

        actions().ledBar().setOn(5);
    if (level == 2)
    {
        actions().ledBar().setOn(6);
        actions().ledBar().setOn(7);
    }
}
