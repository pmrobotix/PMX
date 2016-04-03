#include "LegoEV3AsservExtended.hpp"

#include "../Common/Asserv.Insa/AsservInsa.hpp"

void LegoEV3AsservExtended::startMotionTimerAndOdo()
{

	pAsservInsa_->encoder_SetResolution(3640, 3640, 200);
	pAsservInsa_->motion_SetDefaultVmax(0.2);
	pAsservInsa_->motion_SetDefaultAccel(0.2);
	pAsservInsa_->motion_SetDefaultDecel(0.2);
	pAsservInsa_->motion_setMaxPwmValue(860); //max power ev3 using hardregulation
	pAsservInsa_->motion_Init();
	pAsservInsa_->motion_DisablePID();
	pAsservInsa_->motion_configureAlphaPID(0.0, 0.0, 0.0);
	pAsservInsa_->motion_configureDeltaPID(0.0, 0.0, 0.0);
	pAsservInsa_->motion_configureLeftPID(0.0, 0.0, 0.0);
	pAsservInsa_->motion_configureRightPID(0.0, 0.0, 0.0);

	//f=20 Hz =>every 50ms
	//f=100 Hz =>every 10ms
	pAsservInsa_->motion_SetSamplingFrequency(20);

}

