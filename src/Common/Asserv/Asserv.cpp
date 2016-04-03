#include "Asserv.hpp"

#include "../../Log/Logger.hpp"
#include "MovingBase.hpp"

Asserv::Asserv(std::string botId)
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

	float meters = distance_mm / 1000.0f;

	logger().debug() << "motion_GetDefaultVmax=" << pAsservInsa_->motion_GetDefaultVmax() << logs::end;

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

void Asserv::setPosition(float x_mm, float y_mm, float degrees)
{

//	if (matchColor != 0) { //TODO set color
//	 //yMM = -yMM;
//	 xMM = 3000 - xMM;
//	 thetaDegrees = 180.0 - thetaDegrees;
//	 }
	pAsservInsa_->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, degrees * M_PI / 180.0);
}

/*
 void cc_setPosition(float xMM, float yMM, float thetaDegrees, int matchColor) {
 if (matchColor != 0) {
 //yMM = -yMM;
 xMM = 3000 - xMM;
 thetaDegrees = 180.0 - thetaDegrees;
 }

 odo_SetPosition(xMM / 1000.0, yMM / 1000.0, thetaDegrees * M_PI / 180.0);
 }
 */

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

void Asserv::setVmax(float vmax)
{
	pAsservInsa_->motion_SetDefaultVmax(vmax);
}

void Asserv::setAccel(float acc)
{
	pAsservInsa_->motion_SetDefaultAccel(acc);
}

void Asserv::setDecel(float dec)
{
	pAsservInsa_->motion_SetDefaultDecel(dec);
}

