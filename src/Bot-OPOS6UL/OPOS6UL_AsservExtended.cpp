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


bool OPOS6UL_AsservExtended::filtre_IsInsideTable(int dist_detect_mm, int lateral_pos_sensor_mm)
{
    //logger().error() << "==== filtreInsideTable" << logs::end;
    float distmetre = dist_detect_mm / 1000.0;
    //On filtre si c'est pas à l'exterieur du terrain
    float x = 0.0;
    float y = 0.0;
    bool result = false;
    RobotPosition p = pos_getPosition();
    x = p.x + ((lateral_pos_sensor_mm * 140.0 / 1000.0) * cos(p.theta - M_PI_2)) + (distmetre * cos(p.theta));
    y = p.y + ((lateral_pos_sensor_mm * 140.0 / 1000.0) * sin(p.theta - M_PI_2)) + (distmetre * sin(p.theta));
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

void OPOS6UL_AsservExtended::setLowSpeedForward(bool enable, int)
{
    logger().debug() << "LegoEV3AsservExtended::setLowSpeedForward = " << enable << logs::end;
    //Asserv::setLowSpeedForward(enable, 45);
    asservdriver_->motion_setLowSpeedForward(enable, 55);
}
void OPOS6UL_AsservExtended::setLowSpeedBackward(bool enable, int)
{
    logger().debug() << "LegoEV3AsservExtended::setLowSpeedBackward =" << enable << logs::end;
    //Asserv::setLowSpeedBackward(enable, 45);
    asservdriver_->motion_setLowSpeedBackward(enable, 55);
}
