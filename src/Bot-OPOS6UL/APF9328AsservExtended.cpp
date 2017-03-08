#include "APF9328AsservExtended.hpp"

#include "../Common/Asserv.Insa/AsservInsa.hpp"

void APF9328AsservExtended::startMotionTimerAndOdo()
{

#ifdef SIMU
	//SIMU CONSOLE pour APF !!!!Changer aussi le parametre dans les drivers SIMU !!!!
	pAsservInsa_->encoder_SetResolution(1470, 1470, 187.5);
	pAsservInsa_->motion_SetDefaultAccel(0.2);
	pAsservInsa_->motion_SetDefaultVmax(0.4);
	pAsservInsa_->motion_SetDefaultDecel(0.2);
	pAsservInsa_->motion_setMaxPwmValue(127); //max power md25 using hard regulation
	pAsservInsa_->motion_Init();
	//RCVA PID
//	pAsservInsa_->motion_configureAlphaPID(190.0, 0.0, 5.0);
//	pAsservInsa_->motion_configureDeltaPID(190.0, 0.0, 5.0);
//	pAsservInsa_->motion_configureLeftPID(0.0, 0.0, 0.0);
//	pAsservInsa_->motion_configureRightPID(0.0, 0.0, 0.0);

//PID STD
//	pAsservInsa_->motion_configureAlphaPID(0.0020, 0.001, 0.0000005);
//	pAsservInsa_->motion_configureDeltaPID(0.0010, 0.0002, 0.000005);
//	pAsservInsa_->motion_configureLeftPID(0.015, 0.0, 0.0);
//	pAsservInsa_->motion_configureRightPID(0.015, 0.0, 0.0);

	pAsservInsa_->motion_configureAlphaPID(0.0035, 0.0005, 0.00005);
	pAsservInsa_->motion_configureDeltaPID(0.002, 0.00007, 0.00005);

	pAsservInsa_->motion_configureLeftPID(0.0015, 0.0, 0.0);
	pAsservInsa_->motion_configureRightPID(0.0015, 0.0, 0.0);

#else
	//real APF
	pAsservInsa_->encoder_SetResolution(1470, 1470, 187.5);
	pAsservInsa_->motion_SetDefaultAccel(0.2);
	pAsservInsa_->motion_SetDefaultVmax(0.4);
	pAsservInsa_->motion_SetDefaultDecel(0.2);
	pAsservInsa_->motion_setMaxPwmValue(127);//max power ev3 using hardregulation
	pAsservInsa_->motion_Init();
	//RCVA PID
//	pAsservInsa_->motion_configureAlphaPID(150.0, 0.0, 1.0);
//	pAsservInsa_->motion_configureDeltaPID(110.0, 0.0, 0.0);
//	pAsservInsa_->motion_configureLeftPID(0.0, 0.0, 0.0);
//	pAsservInsa_->motion_configureRightPID(0.0, 0.0, 0.0);

	//PID STD
	//maison D 0.0015, 0.0005, 0.0005

	//0.0025, 0.001, 0.0000005
	//pAsservInsa_->motion_configureAlphaPID(0.0025, 0.0005, 0.0005);

	//0.0035, 0.0009, 0.00005
	//0.0015, 0.0002, 0.000005
	//pAsservInsa_->motion_configureDeltaPID(0.0015, 0.0005, 0.0005);
	pAsservInsa_->motion_configureAlphaPID(0.0035, 0.0005, 0.00005);
	pAsservInsa_->motion_configureDeltaPID(0.0015, 0.00006, 0.00005);


	pAsservInsa_->motion_configureLeftPID(0.0015, 0.0, 0.0);
	pAsservInsa_->motion_configureRightPID(0.0015, 0.0, 0.0);

#endif

	pAsservInsa_->motion_DisablePID();

	//f=20 Hz =>every 50ms
	//f=40 Hz => every 25ms
	//f=50 Hz => every 20ms
	//f=100 Hz =>every 10ms
	//f=200 Hz =>every 5ms
	pAsservInsa_->motion_SetSamplingFrequency(50); //20ms pour APF pour avoir plus de step sur la vitesse

}

