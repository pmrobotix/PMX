#include "OPOS6UL_AsservExtended.hpp"

#include <string>

#include "../Common/Action/LcdShield.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

OPOS6UL_AsservExtended::OPOS6UL_AsservExtended(std::string botId, OPOS6UL_RobotExtended * robot) :
        Asserv(botId, robot) //on appelle le constructeur pere
{
    //useInternalAsserv_ = false; //configuration pour utiliser une carte d'asserv externe (ici la MBED)
    botId_ = botId;
    useAsservType_ = ASSERV_EXT;
    robot_extended_ = robot;
}

void OPOS6UL_AsservExtended::resetDisplayTS()
{
    robot_extended_->actions().ledBar().resetAll();
    //robot_extended_->actions().lcd2x16().clear();

}

void OPOS6UL_AsservExtended::displayTS(TRAJ_STATE ts)
{
    robot_extended_->actions().ledBar().setOn(ts);
/*
    robot_extended_->actions().lcd2x16().home();
    if (ts == TRAJ_OK) {
        robot_extended_->actions().lcd2x16().println("TRAJ_OK");
    }
    if (ts == TRAJ_ERROR) {
        robot_extended_->actions().lcd2x16().println("TRAJ_ERROR");
    }
    if (ts == TRAJ_COLLISION) {
        robot_extended_->actions().lcd2x16().println("TRAJ_COLLISION");
    }
    if (ts == TRAJ_CANCELLED) {
        robot_extended_->actions().lcd2x16().println("TRAJ_CANCELLED");
    }
    if (ts == TRAJ_INTERRUPTED) {
        robot_extended_->actions().lcd2x16().println("TRAJ_INTERRUPTED");
    }
    if (ts == TRAJ_NEAR_OBSTACLE) {
        robot_extended_->actions().lcd2x16().println("TRAJ_NEAR_OBSTACLE");
    }*/

    //sleep(1);
}
