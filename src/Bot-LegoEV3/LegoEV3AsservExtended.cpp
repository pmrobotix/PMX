#include "LegoEV3AsservExtended.hpp"

#include "../Common/Asserv.Insa/AsservInsa.hpp"

void LegoEV3AsservExtended::startMotionTimerAndOdo()
{

	pAsservInsa_->encoder_SetResolution(3640, 3640, 145);
	//pAsservInsa_->encoder_SetResolution(1000, 1000, 145);
	pAsservInsa_->motion_SetDefaultVmax(0.1);
	pAsservInsa_->motion_SetDefaultAccel(0.2);
	pAsservInsa_->motion_SetDefaultDecel(0.1);
	pAsservInsa_->motion_setMaxPwmValue(860); //max power ev3 using hardregulation
	pAsservInsa_->motion_Init();
	pAsservInsa_->motion_configureAlphaPID(0.006, 0.0, 0.0);
	pAsservInsa_->motion_configureDeltaPID(0.007, 0.0, 0.0);
	pAsservInsa_->motion_configureLeftPID(0.0, 0.0, 0.0);
	pAsservInsa_->motion_configureRightPID(0.0, 0.0, 0.0);
	pAsservInsa_->motion_DisablePID();

	//f=20 Hz =>every 50ms
	//f=50 Hz => every 20ms
	//f=100 Hz =>every 10ms
	pAsservInsa_->motion_SetSamplingFrequency(20);

}

