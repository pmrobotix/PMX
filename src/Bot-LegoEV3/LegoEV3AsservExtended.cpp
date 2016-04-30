#include "LegoEV3AsservExtended.hpp"

#include "../Common/Asserv.Insa/AsservInsa.hpp"

void LegoEV3AsservExtended::startMotionTimerAndOdo()
{

#ifdef SIMU
	//SIMU EV3
	printf("---LegoEV3AsservExtended > SIMU EV3\n");
	pAsservInsa_->encoder_SetResolution(1395, 1395, 135);
	pAsservInsa_->motion_SetDefaultAccel(0.2);
	pAsservInsa_->motion_SetDefaultVmax(0.4);
	pAsservInsa_->motion_SetDefaultDecel(0.2);
	pAsservInsa_->motion_setMaxPwmValue(860);//max power ev3 using hardregulation
	pAsservInsa_->motion_Init();
	//RCVA PID
//	pAsservInsa_->motion_configureAlphaPID(1200.0, 0.0, 0.0);
//	pAsservInsa_->motion_configureDeltaPID(900.0, 0.0, 0.0);
//	//NORMAL PID
	pAsservInsa_->motion_configureAlphaPID(0.06, 0.002, 0.0005);//0.06 0.001 0.0005
	pAsservInsa_->motion_configureDeltaPID(0.05, 0.002, 0.0005);

	pAsservInsa_->motion_configureLeftPID(0.0, 0.0, 0.0);
	pAsservInsa_->motion_configureRightPID(0.0, 0.0, 0.0);

#else
	//Real EV3
	printf("---LegoEV3AsservExtended > Real EV3\n");
	pAsservInsa_->encoder_SetResolution(1395, 1395, 135);
	pAsservInsa_->motion_SetDefaultAccel(0.2);
	pAsservInsa_->motion_SetDefaultVmax(0.4);
	pAsservInsa_->motion_SetDefaultDecel(0.2);
	pAsservInsa_->motion_setMaxPwmValue(860); //max power ev3 using hardregulation
	pAsservInsa_->motion_Init();

	//RCVA PID
//	pAsservInsa_->motion_configureAlphaPID(1200.0.0, 0.0, 0.0);
//	pAsservInsa_->motion_configureDeltaPID(900.0.0, 0.0, 0.0);
//	pAsservInsa_->motion_configureLeftPID(0.0, 0.0, 0.0);
//	pAsservInsa_->motion_configureRightPID(0.0, 0.0, 0.0);
	//	//NORMAL PID
	pAsservInsa_->motion_configureAlphaPID(0.06, 0.002, 0.0005); //0.05 0.002 0.0005
	pAsservInsa_->motion_configureDeltaPID(0.05, 0.002, 0.0005); //0.05 0.002 0.0005
	pAsservInsa_->motion_configureLeftPID(0.0, 0.0, 0.0);
	pAsservInsa_->motion_configureRightPID(0.0, 0.0, 0.0);

#endif

	pAsservInsa_->motion_DisablePID();

	//f=20 Hz =>every 50ms
	//f=40 Hz => every 25ms
	//f=50 Hz => every 20ms
	//f=100 Hz =>every 10ms
	pAsservInsa_->motion_SetSamplingFrequency(50); //20ms pour EV3 pour avoir plus de step sur la vitesse

}

