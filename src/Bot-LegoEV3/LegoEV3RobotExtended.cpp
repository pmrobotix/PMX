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

LegoEV3RobotExtended::LegoEV3RobotExtended() {
    id_ = "LegoEV3Robot";
    myColor_ = PMXNOCOLOR;
    cArgs_.setDescription("(c) PM-ROBOTIX LegoEV3Robot");

    p_svg_ = new LegoEV3SvgWriterExtended(id_); // TODO simplify
    svg_ = p_svg_;

    p_asserv_ = new LegoEV3AsservExtended(id_, this); // TODO simplify and use directly asserv_default_
    asserv_default_ = p_asserv_;

    //on ecrase les versions par default avec la version extended
    p_actions_ = new LegoEV3ActionsExtended(id_, this); // TODO simplify and use directly actions_default_
    actions_default_ = p_actions_;

    p_ia_ = new LegoEV3IAExtended(id_, this); //TODO create an IA on the default robot

    decisionMaker_ = NULL;

    svg_->beginHeader();
}

void LegoEV3RobotExtended::displayPoints() {
    this->actions().lcd().clear();
    std::string p = "Points=";
    this->actions().lcd().display_content_string(p, 4);
    this->actions().lcd().display_content_integer(this->points, 4, 9);
}

void LegoEV3RobotExtended::stopExtraActions() {
    this->actions().stopExtra(); //extra devices
}

void LegoEV3RobotExtended::begin(int argc, char** argv) {
    Robot::begin(argc, argv);

    logger().debug() << "LegoEV3RobotExtended::begin cArgs_.isOption('t')=" << cArgs_.isOption('t') << logs::end;

    //specific match case ands strategies
    if (cArgs_["type"] == "m" or cArgs_["type"] == "M") {    //if (cArgs_.isOption('t') == 0) {

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
    logger().info() << "PMX " << this->getID() << " Happy End - " << this->chrono().getElapsedTimeInSec() << " sec" << logs::end;
}

void LegoEV3RobotExtended::resetDisplayTS() {
    actions().ledBar().setOff(1); //TODO all ?

}

void LegoEV3RobotExtended::displayTS(TRAJ_STATE ts) {
    if (ts == TRAJ_NEAR_OBSTACLE) actions().ledBar().set(1, LED_AMBER);
    if (ts == TRAJ_COLLISION) actions().ledBar().set(1, LED_RED);
    if (ts == TRAJ_FINISHED) actions().ledBar().set(1, LED_GREEN);
}

void LegoEV3RobotExtended::resetDisplayObstacle() {
    actions().ledBar().setOff(0);

}

void LegoEV3RobotExtended::displayObstacle(int level) {
    if (level == 1) actions().ledBar().set(0, LED_YELLOW);
    if (level == 2) actions().ledBar().set(0, LED_RED);
}
