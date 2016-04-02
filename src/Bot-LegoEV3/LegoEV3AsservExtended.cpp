#include "LegoEV3AsservExtended.hpp"

#include "../Common/Asserv.Insa/AsservInsa.hpp"

void LegoEV3AsservExtended::startMotionTimerAndOdo()
{

	pAsservInsa_->encoder_SetResolution(1000, 1000, 200);
	pAsservInsa_->motion_SetDefaultSpeed(0.1);
	pAsservInsa_->motion_SetDefaultAccel(0.1);
	pAsservInsa_->motion_SetDefaultDecel(0.1);
	pAsservInsa_->motion_setMaxPwmValue(128);
	pAsservInsa_->motion_Init();
	pAsservInsa_->motion_DisablePID();
	pAsservInsa_->motion_configureAlphaPID(0.0, 0.0, 0.0);
	pAsservInsa_->motion_configureDeltaPID(100.0, 0.0, 0.0);
	pAsservInsa_->motion_configureLeftPID(10.0, 0.0, 0.0);
	pAsservInsa_->motion_configureRightPID(10.0, 0.0, 0.0);
	pAsservInsa_->motion_SetSamplingFrequency(50); //every 10ms

}

