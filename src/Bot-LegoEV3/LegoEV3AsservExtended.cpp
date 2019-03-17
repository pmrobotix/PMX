#include "LegoEV3AsservExtended.hpp"

#include <cstdio>

#include "../Asserv.Esial/AsservEsialR.hpp"
#include "../Asserv.Esial/config/config.h"
#include "../Asserv.Insa/AsservInsa.hpp"
#include "../Log/Logger.hpp"

LegoEV3AsservExtended::LegoEV3AsservExtended(std::string botId, Robot * robot) :
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
        logger().info() << "Version configuration : " << Config::configVersion << logs::end;
        //logger().debug() << Config::dumpConfig() << logs::end;

        double periodSec = Config::asservPeriod;

        //start asserv thread
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


        printf("---LegoEV3AsservExtended > Real EV3\n");

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

