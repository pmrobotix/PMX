#include "LegoEV3AsservExtended.hpp"

#include <stdio.h>
#include <cmath>
#include <string>

#include "../Asserv.Esial/AsservEsialR.hpp"
#include "../Asserv.Esial/config/config.h"
#include "../Asserv.Insa/AsservInsa.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3RobotExtended.hpp"

LegoEV3AsservExtended::LegoEV3AsservExtended(std::string botId, LegoEV3RobotExtended *robot) :
        Asserv(botId, robot) //on appelle le constructeur pere
{
    botId_ = botId;
    useAsservType_ = ASSERV_INT_ESIALR;

    pRobotExt_ = robot;

    //set the value setLowSpeedForward for asserv
    setLowSpeedvalue(32);
}

void LegoEV3AsservExtended::startMotionTimerAndOdo(bool assistedHandlingEnabled)
{

    //Real EV3 for ESIAL internal asserv config
    if (useAsservType_ == ASSERV_INT_ESIALR) {

        std::string filename = "config_";
        filename.append(botId_);
        filename.append(".txt");
        Config::loadFile(filename.c_str());
        logger().debug() << "Version configuration : " << Config::configVersion << logs::end;
        //logger().debug() << Config::dumpConfig() << logs::end;

        float periodSec = Config::asservPeriod;

        //on active le thread de check de position et les drivers //TODO a faire dans esialR et non dans asservdriver
        //asservdriver_->motion_ActivateManager(true);

        //start asserv thread et timer
        pAsservEsialR_->startAsserv(1.0f / periodSec); //f=20 Hz => every 50ms

        pAsservEsialR_->motion_ActivateManager(true); //init and start
        if (assistedHandlingEnabled) {
            pAsservEsialR_->motion_AssistedHandling();
        } else {
            pAsservEsialR_->motion_FreeMotion();
        }

    } else if (useAsservType_ == ASSERV_INT_INSA) { //Real EV3 for internal ASSERVINSA config

        printf("---LegoEV3AsservExtended > ASSERV_INT_INSA\n");

        pAsservInsa_->encoder_SetResolution(1398, 1398, 114); //1398, 1398, 134
        pAsservInsa_->motion_SetDefaultAccel(0.2);
        pAsservInsa_->motion_SetDefaultVmax(0.3);
        pAsservInsa_->motion_SetDefaultDecel(0.3);
        pAsservInsa_->motion_setMaxPwmValue(860); //max power ev3 using hardregulation
        //f=20 Hz => every 50ms
        //f=25 Hz => every 40ms
        //f=30 Hz => every 33ms
        //f=40 Hz => every 25ms
        //f=50 Hz => every 20ms
        //f=100Hz => every 10ms
        pAsservInsa_->motion_Init(25);

        //RCVA PID
        //       pAsservInsa_->motion_configureAlphaPID(1200.0.0, 0.0, 0.0);
        //       pAsservInsa_->motion_configureDeltaPID(l900.0.0, 0.0, 0.0);
        //       pAsservInsa_->motion_configureLeftPID(0.0, 0.0, 0.0);
        //       pAsservInsa_->motion_configureRightPID(0.0, 0.0, 0.0);

        //       //NORMAL PID
        pAsservInsa_->motion_configureLeftPID(0.0, 0.0, 0.0);
        pAsservInsa_->motion_configureRightPID(0.0, 0.0, 0.0);

        //belgique param
        //pAsservInsa_->motion_configureAlphaPID(0.005, 0.0005, 0.000005);
        //pAsservInsa_->motion_configureDeltaPID(0.003, 0.0005, 0.000005);

        //france param
        //pAsservInsa_->motion_configureAlphaPID(0.005, 0.0005, 0.00005);
        //pAsservInsa_->motion_configureDeltaPID(0.002, 0.0, 0.0);
        pAsservInsa_->motion_configureAlphaPID(1300.0, 0.0, 1.0);
        pAsservInsa_->motion_configureDeltaPID(700.0, 0.0, 1.0);

    }

}

bool LegoEV3AsservExtended::filtre_IsInsideTableXY(int d_mm, int x_mm, int y_mm, float theta_deg, int *x_botpos,
        int *y_botpos)
{
    //return true; //PATCH

    //table verticale
    int table_x = 2000;
    int table_y = 3000;
    RobotPosition p = pos_getPosition();

    //coordonnées de l'objet detecté sur la table// M_P/2
//    *x_botpos = p.x + (d_mm * cos(p.theta - M_PI_2 + (theta_deg * M_PI / 180.0f)));
//    *y_botpos = p.y + (d_mm * sin(p.theta - M_PI_2 + (theta_deg * M_PI / 180.0f)));
    float a = (p.theta - M_PI_2 + (theta_deg * M_PI / 180.0f));
    std::fmod(a, 2 * M_PI);
    if (a < -M_PI)
        a += M_PI;
    if (a > M_PI)
        a -= M_PI;

    //ADV coord
    *x_botpos = p.x + (d_mm * cos(a));
    *y_botpos = p.y + (d_mm * sin(a));

//    logger().error() << "DEBUG --xy_botpos= " << *x_botpos << " " << *y_botpos
//                    << "pos: " << p.x << " " << p.y << " p_rad:" << p.theta << " --balise: " << d_mm << " " << x_mm << " "
//                    << y_mm << " t_deg:" << theta_deg << logs::end;

    if (getRobot()->getMyColor() == PMXGREEN) {
        if (*x_botpos > 650 && *x_botpos < 850 && *y_botpos < 1250 && *y_botpos > 1000) {

            getRobot()->B3_is_taken = true;
        }

        if (*x_botpos > 650 && *x_botpos < 850 && *y_botpos < 2000 && *y_botpos > 1775) {

            getRobot()->B4_is_taken = true;
        }

        if (*x_botpos > 0 && *x_botpos < 650 && *y_botpos < 2800 && *y_botpos > 2100) {

            getRobot()->A5_is_taken = true;
        }

        if (*x_botpos > 2000 - 650 && *x_botpos < 2000 && *y_botpos < 2800 && *y_botpos > 2100) {

            getRobot()->D5_is_taken = true;
        }
    } else {
        if (*x_botpos > (2000 - 850) && *x_botpos < (2000 - 650) && *y_botpos < 1250 && *y_botpos > 1000) {

            getRobot()->B3_is_taken = true;
        }

        if (*x_botpos > (2000 - 850) && *x_botpos < (2000 - 650) && *y_botpos < 2000 && *y_botpos > 1775) {

            getRobot()->B4_is_taken = true;
        }

        if (*x_botpos > (2000 - 650) && *x_botpos < 2000 && *y_botpos < 2800 && *y_botpos > 2100) {

            getRobot()->A5_is_taken = true;
        }
        if (*x_botpos > 0 && *x_botpos < 650 && *y_botpos < 2800 && *y_botpos > 2100) {

            getRobot()->D5_is_taken = true;
        }

    }
    //getRobot()->B4_is_taken = true;

    //on filtre si c'est en dehors de la table verticale! avec 10cm de marge
    if ((*x_botpos > 100 && *x_botpos < table_x - 100) && (*y_botpos > 100 && *y_botpos < table_y - 100)) {
        logger().debug() << "INSIDE filtre_IsInsideTableXY xy_botpos=" << *x_botpos << " " << *y_botpos << "pos: "
                << p.x << " " << p.y << " p_rad:" << p.theta << " balise: " << d_mm << " " << x_mm << " " << y_mm
                << " t_deg:" << theta_deg << logs::end;
        return true;
    } else
        return false;

}

//TODO fonction pour les capteurs de proximité ...a rendre generique pour tous les robots
//dist_detect_mm : distance detecté de l'objet
//lateral_pos_sensor_mm : position du capteur : gauche -1 ; droite +1
bool LegoEV3AsservExtended::filtre_IsInsideTable(int dist_detect_mm, int lateral_pos_sensor_mm, std::string desc)
{

    float dist_sensor_from_axis_robot_mm = 110.0;
    //logger().error() << "==== filtreInsideTable" << logs::end;
    float distmm = dist_detect_mm;
    //On filtre si c'est pas à l'exterieur du terrain
    float x = 0.0;
    float y = 0.0;
    bool result = false;
    RobotPosition p = pos_getPosition();
    x = p.x + ((lateral_pos_sensor_mm * dist_sensor_from_axis_robot_mm) * cos(p.theta - M_PI_2))
            + (distmm * cos(p.theta));
    y = p.y + ((lateral_pos_sensor_mm * dist_sensor_from_axis_robot_mm) * sin(p.theta - M_PI_2))
            + (distmm * sin(p.theta));

    //TODO utiliser les zones de l'ia ???

    //filtre table Horizontale
//    if ((x > 150 && x < 2850) && (y > 150 && y < 1850)) //en mm
//        result = true;
//    else
//        result = false;

    //filtre table verticale
    if ((x > 150 && x < 1850) && (y > 150 && y < 2850)) //en mm
        result = true;
    else
        result = false;

//    //2022 filtre triangle yellow //todo violet !!!!!!!!!!!!!!!!!!!!!!!!
//    if(y <= (700 - (x)))
//        result = false;
//
//    if(y <= (-2300 + (x)))
//        result = false;

//    logger().debug() << desc << " filtreInsideTable : dist=" << dist_detect_mm
//            << " capteur:" << lateral_pos_sensor_mm
//            << " p.x=" << p.x << " p.y=" << p.y << " p.T=" << p.theta << " x=" << x
//            << " y=" << y << " result = " << result << logs::end;

    if (result) {
        logger().debug() << desc << " filtreInsideTable : dist=" << dist_detect_mm << " capteur:"
                << lateral_pos_sensor_mm << " p.x=" << p.x << " p.y=" << p.y << " p.T=" << p.theta << " x=" << x
                << " y=" << y << " result = " << result << logs::end;
        return true; //si ok
    } else
        return false; //si en dehors de la table*/

    /*
     //logger().error() << "==== filtreInsideTable" << logs::end;
     float distmetre = dist_detect_mm / 1000.0;
     //On filtre si c'est pas à l'exterieur du terrain
     float x = 0.0;
     float y = 0.0;
     bool result = false;
     RobotPosition p = pos_getPosition();
     x = p.x + ((lateral_pos_sensor_mm * 100.0 / 1000.0) * cos(p.theta - M_PI_2)) + (distmetre * cos(p.theta));
     y = p.y + ((lateral_pos_sensor_mm * 100.0 / 1000.0) * sin(p.theta - M_PI_2)) + (distmetre * sin(p.theta));
     if ((x > 0.150 && x < 2.850) && (y > 0.150 && y < 1.500)) //en mètre
     result = true;
     else
     result = false;
     logger().error() << "filtreInsideTable" << " p.x=" << p.x << " p.y=" << p.y << " p.T=" << p.theta << " x=" << x
     << " y=" << y << " result = " << result << logs::end;

     if (result) {
     return true; //si ok
     } else
     return false; //si en dehors de la table*/
}

//void LegoEV3AsservExtended::setLowSpeedForward(bool enable, int percent)
//{
//    //logger().error() << "LegoEV3AsservExtended::setLowSpeedForward 40!" << logs::end;
//    pAsservEsialR_->motion_setLowSpeedForward(enable, percent);
//
//}
//void LegoEV3AsservExtended::setLowSpeedBackward(bool enable, int percent)
//{
//    //logger().error() << "LegoEV3AsservExtended::setLowSpeedBackward 40!" << logs::end;
//    pAsservEsialR_->motion_setLowSpeedBackward(enable, percent);
//}
