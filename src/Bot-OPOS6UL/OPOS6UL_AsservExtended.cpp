#include "OPOS6UL_AsservExtended.hpp"

#include <cmath>
#include <string>

#include "../Common/Action/LedBar.hpp"
#include "../Log/Logger.hpp"
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

bool OPOS6UL_AsservExtended::filtre_IsInsideTable(int dist_detect_mm, int lateral_pos_sensor_mm)
{
    //logger().error() << "==== filtreInsideTable" << logs::end;
    float distmetre = dist_detect_mm / 1000.0;
    //On filtre si c'est pas à l'exterieur du terrain
    float x = 0.0;
    float y = 0.0;
    bool result = false;
    RobotPosition p = pos_getPosition();
    x = p.x + ((lateral_pos_sensor_mm / 1000.0) * cos(p.theta - M_PI_2)) + (distmetre * cos(p.theta));
    y = p.y + ((lateral_pos_sensor_mm / 1000.0) * sin(p.theta - M_PI_2)) + (distmetre * sin(p.theta));
    if ((x > 0.150 && x < 2.850) && (y > 0.150 && y < 1.500)) //en mètre
        result = true;
    else
        result = false;
    logger().debug() << "filtreInsideTable" << " p.x=" << p.x << " p.y=" << p.y << " p.T=" << p.theta << " x=" << x
            << " y=" << y << " result = " << result << logs::end;

    if (result) {
        return true; //si ok
    } else
        return false; //si en dehors de la table*/
}
