#include "Asserv.hpp"

#include "../../Log/Logger.hpp"
#include "MovingBase.hpp"

Asserv::Asserv(std::string botId)
//: movingbase_(botId, *this), asservinsa_()
{
	pMovingBase_ = new MovingBase(botId);

	pAsservInsa_ = new AsservInsa();
	pAsservInsa_->setMovingBase(pMovingBase_); //doit etre surchargé

	ignoreRearCollision_ = false;
	ignoreFrontCollision_ = false;
}

MovingBase * Asserv::base()
{
	return pMovingBase_;
}

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
	pAsservInsa_->motion_FreeMotion();
}

void Asserv::stopMotionTimerAndOdo()
{
	pAsservInsa_->motion_StopTimer();
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

	logger().debug() << "motion_GetDefaultSpeed=" << pAsservInsa_->motion_GetDefaultSpeed() << logs::end;

	return pAsservInsa_->motion_DoLine(meters);

}

void Asserv::configureAlphaPID(float Ap, float Ai, float Ad)
{
	pAsservInsa_->motion_configureAlphaPID(Ap, Ai, Ad);
}

void Asserv::configureDeltaPID(float Dp, float Di, float Dd)
{
	pAsservInsa_->motion_configureDeltaPID(Dp, Di, Dd);
}

void Asserv::findPidAD(float degrees, int mm, int sec)
{
	float meters = mm / 1000.0f;
	float radians = (degrees * M_PI) / 180.0f;
	pAsservInsa_->motion_findPidAD(radians, meters, sec);

}

// position x,x in mm
float Asserv::pos_getX_mm()
{
	return pAsservInsa_->pos_getX_mm();
}
float Asserv::pos_getY_mm()
{
	return pAsservInsa_->pos_getY_mm();
}
// angle in radian
float Asserv::pos_getTheta()
{
	return pAsservInsa_->pos_getTheta();
}
// angle in degrees
float Asserv::pos_getThetaInDegree()
{
	return pAsservInsa_->pos_getThetaInDegree();
}

