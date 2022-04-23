#include "LegoEV3AsservExtended.hpp"

#include <stdio.h>
#include <cmath>
#include <string>

#include "../Asserv.Esial/AsservEsialR.hpp"
#include "../Asserv.Esial/config/config.h"
#include "../Asserv.Insa/AsservInsa.hpp"
#include "../Log/Logger.hpp"
#include "LegoEV3RobotExtended.hpp"

LegoEV3AsservExtended::LegoEV3AsservExtended(std::string botId, LegoEV3RobotExtended * robot) :
       Asserv(botId, robot) //on appelle le constructeur pere
{
    botId_ = botId;
    useAsservType_ = ASSERV_INT_ESIALR;
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
        pAsservEsialR_->startAsserv(1.0f/periodSec); //f=20 Hz => every 50ms

        pAsservEsialR_->motion_ActivateManager(true); //init and start
        if(assistedHandlingEnabled)
        {
            pAsservEsialR_->motion_AssistedHandling();
        }else
        {
            pAsservEsialR_->motion_FreeMotion();
        }


    }else if (useAsservType_ == ASSERV_INT_INSA) {//Real EV3 for internal ASSERVINSA config


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

bool LegoEV3AsservExtended::filtre_IsInsideTable(int dist_detect_mm, int lateral_pos_sensor_mm, std::string desc)
{
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
    logger().debug() << "filtreInsideTable" << " p.x=" << p.x << " p.y=" << p.y << " p.T=" << p.theta << " x=" << x
            << " y=" << y << " result = " << result << logs::end;

    if (result) {
        return true; //si ok
    } else
        return false; //si en dehors de la table*/
}

void LegoEV3AsservExtended::setLowSpeedForward(bool enable, int)
{
    //logger().error() << "LegoEV3AsservExtended::setLowSpeedForward 40!" << logs::end;
    pAsservEsialR_->motion_setLowSpeedForward(enable, 50);

}
void LegoEV3AsservExtended::setLowSpeedBackward(bool enable, int)
{
    //logger().error() << "LegoEV3AsservExtended::setLowSpeedBackward 40!" << logs::end;
    pAsservEsialR_->motion_setLowSpeedBackward(enable, 50);
}
