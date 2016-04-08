#include "LegoEV3AsservExtended.hpp"

#include "../Common/Asserv.Insa/AsservInsa.hpp"

void LegoEV3AsservExtended::startMotionTimerAndOdo()
{

#ifdef SIMU
	//SIMU
	pAsservInsa_->encoder_SetResolution(3640, 3640, 145);
	//pAsservInsa_->encoder_SetResolution(3000, 3000, 145); //Changer aussi le parametre dans les drivers SIMU
	pAsservInsa_->motion_SetDefaultAccel(0.1);
	pAsservInsa_->motion_SetDefaultVmax(0.2);
	pAsservInsa_->motion_SetDefaultDecel(0.1);
	pAsservInsa_->motion_setMaxPwmValue(860); //max power ev3 using hardregulation
	pAsservInsa_->motion_Init();
	//RCVA PID
	pAsservInsa_->motion_configureAlphaPID(6700.0, 0.0, 1.0);
	pAsservInsa_->motion_configureDeltaPID(2700.0, 0.0, 1.0);
//	//NORMAL PID
//	pAsservInsa_->motion_configureAlphaPID(0.06, 0.001, 0.0005); //0.06 0.001 0.0005
//	pAsservInsa_->motion_configureDeltaPID(0.07, 0.001, 0.0);

	pAsservInsa_->motion_configureLeftPID(0.0, 0.0, 0.0);
	pAsservInsa_->motion_configureRightPID(0.0, 0.0, 0.0);

#else
	//EV3
	pAsservInsa_->encoder_SetResolution(3640, 3640, 145);
	pAsservInsa_->motion_SetDefaultAccel(0.1);
	pAsservInsa_->motion_SetDefaultVmax(0.2);
	pAsservInsa_->motion_SetDefaultDecel(0.1);
	pAsservInsa_->motion_setMaxPwmValue(860);//max power ev3 using hardregulation
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
	pAsservInsa_->motion_SetSamplingFrequency(40); //25ms pour EV3 pour avoir plus de step sur la vitesse

}

