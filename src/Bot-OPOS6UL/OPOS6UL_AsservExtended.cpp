#include "OPOS6UL_AsservExtended.hpp"

#include <cmath>
#include <string>

#include "../Log/Logger.hpp"
#include "OPOS6UL_IAExtended.hpp"
#include "OPOS6UL_RobotExtended.hpp"

OPOS6UL_AsservExtended::OPOS6UL_AsservExtended(std::string botId, OPOS6UL_RobotExtended * robot) :
        Asserv(botId, robot) //on appelle le constructeur pere
{
    //useInternalAsserv_ = false; //configuration pour utiliser une carte d'asserv externe (ici la MBED)
    botId_ = botId;
    useAsservType_ = ASSERV_EXT;
    robot_extended_ = robot;


    //set the value setLowSpeedForward for asserv
    setLowSpeedvalue(20);

    //TODO essayer de surcharger les asservdriver pour avoir accès que log SVG
    //TODO asservdriver_ = AAsservDriver::create(botId, robot->svgw());
}


bool OPOS6UL_AsservExtended::filtre_IsInsideTable(int dist_detect_mm, int lateral_pos_sensor_mm, std::string desc)
{

    //logger().error() << "==== filtreInsideTable" << logs::end;
    float distmm = dist_detect_mm;
    //On filtre si c'est pas à l'exterieur du terrain
    float x = 0.0;
    float y = 0.0;
    bool result = false;
    RobotPosition p = pos_getPosition();
    x = p.x + ((lateral_pos_sensor_mm * 140.0 ) * cos(p.theta - M_PI_2)) + (distmm * cos(p.theta));
    y = p.y + ((lateral_pos_sensor_mm * 140.0 ) * sin(p.theta - M_PI_2)) + (distmm * sin(p.theta));

    //TODO utiliser les zones de l'ia ???

    //filtre table
    if ((x > 150 && x < 2850) && (y > 150 && y < 1850)) //en mm
        result = true;
    else
        result = false;

    //2022 - filtre triangle yellow //todo violet !!!!!!!!!!!!!!!!!!!!!!!!
    if(y <= (700 - (x)))
        result = false;

    if(y <= (-2300 + (x)))
        result = false;

    logger().debug() << desc << " filtreInsideTable : dist=" << dist_detect_mm
            << " capteur:" << lateral_pos_sensor_mm
            << " p.x=" << p.x << " p.y=" << p.y << " p.T=" << p.theta << " x=" << x
            << " y=" << y << " result = " << result << logs::end;

    if (result) {
        logger().debug() << desc << " filtreInsideTable : dist=" << dist_detect_mm
                    << " capteur:" << lateral_pos_sensor_mm
                    << " p.x=" << p.x << " p.y=" << p.y << " p.T=" << p.theta << " x=" << x
                    << " y=" << y << " result = " << result << logs::end;
        return true; //si ok
    } else
        return false; //si en dehors de la table*/


}
/* old version 2021
bool OPOS6UL_AsservExtended::filtre_IsInsideTable(int dist_detect_mm, int lateral_pos_sensor_mm, std::string desc)
{

    //logger().error() << "==== filtreInsideTable" << logs::end;
    float distmm = dist_detect_mm;
    //On filtre si c'est pas à l'exterieur du terrain
    float x = 0.0;
    float y = 0.0;
    bool result = false;
    RobotPosition p = pos_getPosition();
    x = p.x + ((lateral_pos_sensor_mm * 140.0 ) * cos(p.theta - M_PI_2)) + (distmm * cos(p.theta));
    y = p.y + ((lateral_pos_sensor_mm * 140.0 ) * sin(p.theta - M_PI_2)) + (distmm * sin(p.theta));
    if ((x > 150 && x < 2850) && (y > 150 && y < 1850)) //en mètre
        result = true;
    else
        result = false;
    logger().debug() << desc << " filtreInsideTable : dist=" << dist_detect_mm
            << " capteur:" << lateral_pos_sensor_mm
            << " p.x=" << p.x << " p.y=" << p.y << " p.T=" << p.theta << " x=" << x
            << " y=" << y << " result = " << result << logs::end;

    if (result) {
        logger().debug() << desc << " filtreInsideTable : dist=" << dist_detect_mm
                    << " capteur:" << lateral_pos_sensor_mm
                    << " p.x=" << p.x << " p.y=" << p.y << " p.T=" << p.theta << " x=" << x
                    << " y=" << y << " result = " << result << logs::end;
        return true; //si ok
    } else
        return false; //si en dehors de la table
}*/

//void OPOS6UL_AsservExtended::setLowSpeedForward(bool enable, int percent)
//{
//    if (percent < 0) percent = 1;
//    if (percent > 100) percent = 100;
//
//    logger().debug() << "OPOS6UL_AsservExtended::setLowSpeedForward = " << enable << logs::end;
//    //Asserv::setLowSpeedForward(enable, 45);
//    asservdriver_->motion_setLowSpeedForward(enable, percent);
//}
//void OPOS6UL_AsservExtended::setLowSpeedBackward(bool enable, int)
//{
//    logger().debug() << "OPOS6UL_AsservExtended::setLowSpeedBackward =" << enable << logs::end;
//    //Asserv::setLowSpeedBackward(enable, 45);
//    asservdriver_->motion_setLowSpeedBackward(enable, 55);
//}


//TODO a deplacer dans les actions avec l'update des sensors
void OPOS6UL_AsservExtended::update_adv()
{

    robot_extended_->ia().move_adv(pos_getAdvPosition().x, pos_getAdvPosition().y);

}
