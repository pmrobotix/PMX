#ifndef LegoEV3_ASSERVEXTENDED_HPP_
#define LegoEV3_ASSERVEXTENDED_HPP_

#include <string>

#include "../Common/Asserv/Asserv.hpp"

class LegoEV3AsservExtended: public Asserv
{
private:

public:
	LegoEV3AsservExtended(std::string botId)
			: Asserv(botId) //on appelle le constructeur pere
	{
		asservinsa_.setMovingBase(&movingBase_); //surcharge
	}

	~LegoEV3AsservExtended()
	{
	}

	void startMotionTimerAndOdo()
	{

		asservinsa_.encoder_SetResolution(1000, 1000, 200);
		asservinsa_.motion_SetDefaultSpeed(0.1);
		asservinsa_.motion_SetDefaultAccel(0.1);
		asservinsa_.motion_SetDefaultDecel(0.1);

		asservinsa_.motion_setMaxPwmValue(128);
		asservinsa_.motion_Init();
		asservinsa_.motion_FreeMotion();
		asservinsa_.motion_configureAlphaPID(300.0, 0.0, 0.0);
		asservinsa_.motion_configureDeltaPID(300.0, 0.0, 0.0);
		asservinsa_.motion_configureLeftPID(10.0, 0.0, 0.0);
		asservinsa_.motion_configureRightPID(10.0, 0.0, 0.0);
		asservinsa_.motion_SetSamplingFrequency(100);

	}

};

#endif

