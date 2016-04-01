#include "Asserv.hpp"

void Asserv::startMotionTimerAndOdo() //todo à surcharger
{
	/*
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
	*/
}

void Asserv::freeMotion()
{
	asservinsa_.motion_FreeMotion();
}

void Asserv::stopMotionTimerAndOdo()
{
	asservinsa_.motion_StopTimer();
}

// if distance <0, move backward
TRAJ_STATE Asserv::cc_move(float distance_mm)
{

	if (distance_mm > 0)
	{
		ignoreRearCollision_ = true;
	}
	else
	{
		ignoreFrontCollision_ = true;
	}

	//RobotCommand* cmd = (RobotCommand*) malloc(sizeof(RobotCommand));
	float meters = distance_mm / 1000.0f;

	logger().debug() << "motion_GetDefaultSpeed=" << asservinsa_.motion_GetDefaultSpeed() << logs::end;

	return asservinsa_.motion_DoLine(meters);

}

// position x,x in mm
float Asserv::pos_getX_mm()
{
	return asservinsa_.pos_getX_mm();
}
float Asserv::pos_getY_mm()
{
	return asservinsa_.pos_getY_mm();
}
// angle in radian
float Asserv::pos_getTheta()
{
	return asservinsa_.pos_getTheta();
}
// angle in degrees
float Asserv::pos_getThetaInDegree()
{
	return asservinsa_.pos_getThetaInDegree();
}

void Asserv::stop()
{

}

