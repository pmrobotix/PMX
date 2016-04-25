#include "APF9328AsservExtended.hpp"

#include "../Common/Asserv.Insa/AsservInsa.hpp"

void APF9328AsservExtended::startMotionTimerAndOdo()
{

#ifdef SIMU
	//SIMU CONSOLE pour APF !!!!Changer aussi le parametre dans les drivers SIMU !!!!
	pAsservInsa_->encoder_SetResolution(1433, 1433, 210);
	pAsservInsa_->motion_SetDefaultAccel(0.2);
	pAsservInsa_->motion_SetDefaultVmax(0.5);
	pAsservInsa_->motion_SetDefaultDecel(0.2);
	pAsservInsa_->motion_setMaxPwmValue(127);//max power md25 using hard regulation
	pAsservInsa_->motion_Init();
	//RCVA PID
	pAsservInsa_->motion_configureAlphaPID(10.0, 0.0, 0.0);
	pAsservInsa_->motion_configureDeltaPID(10.0, 0.0, 0.0);

	pAsservInsa_->motion_configureLeftPID(0.0, 0.0, 0.0);
	pAsservInsa_->motion_configureRightPID(0.0, 0.0, 0.0);

#else
	//real APF
	pAsservInsa_->encoder_SetResolution(3640, 3640, 145);
	pAsservInsa_->motion_SetDefaultAccel(0.1);
	pAsservInsa_->motion_SetDefaultVmax(0.2);
	pAsservInsa_->motion_SetDefaultDecel(0.1);
	pAsservInsa_->motion_setMaxPwmValue(127); //max power ev3 using hardregulation
	pAsservInsa_->motion_Init();
	pAsservInsa_->motion_configureAlphaPID(1500.0, 0.0, 50.0);
	pAsservInsa_->motion_configureDeltaPID(1500.0, 0.0, 50.0);
	pAsservInsa_->motion_configureLeftPID(0.0, 0.0, 0.0);
	pAsservInsa_->motion_configureRightPID(0.0, 0.0, 0.0);

#endif


	pAsservInsa_->motion_DisablePID();

	//f=20 Hz =>every 50ms
	//f=40 Hz => every 25ms
	//f=50 Hz => every 20ms
	//f=100 Hz =>every 10ms
	pAsservInsa_->motion_SetSamplingFrequency(100); //10ms pour APF pour avoir plus de step sur la vitesse

}

