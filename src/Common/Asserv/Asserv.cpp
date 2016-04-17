#include "Asserv.hpp"

#include <cmath>

#include "MovingBase.hpp"

Asserv::Asserv(std::string botId, Robot * robot)
{
	pMovingBase_ = new MovingBase(botId);

	probot_ = robot;

	pAsservInsa_ = new AsservInsa(robot);
	//pAsservInsa_->setMovingBase(pMovingBase_);

	ignoreRearCollision_ = false;
	ignoreFrontCollision_ = false;
	matchColorPosition_ = false;
}

MovingBase * Asserv::base()
{
	return pMovingBase_;
}

AsservInsa * Asserv::insa()
{
	return pAsservInsa_;
}

void Asserv::startMotionTimerAndOdo() //todo à surcharger
{
}

void Asserv::freeMotion()
{
	pAsservInsa_->motion_FreeMotion();
}

void Asserv::assistedHandling()
{
	pAsservInsa_->motion_AssistedHandling();
}

void Asserv::stopMotionTimerAndOdo()
{
	pAsservInsa_->motion_StopTimer();
}

// if distance <0, move backward
TRAJ_STATE Asserv::doLineAbs(float distance_mm)
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
	return pAsservInsa_->motion_DoLine(meters);
}

//permet de tourner sur un angle défini (inclus 2 ou 3 tours sur soi-même)
TRAJ_STATE Asserv::doRotateAbs(float degrees)
{
	logger().debug() << "doRotateAbs degrees=" << degrees << logs::end;

	float radians = (degrees * M_PI) / 180.0f;
	int f = ignoreFrontCollision_;
	int r = ignoreRearCollision_;
	ignoreRearCollision_ = true;
	ignoreFrontCollision_ = true;

	TRAJ_STATE ts = pAsservInsa_->motion_DoRotate(radians);

	ignoreRearCollision_ = f;
	ignoreFrontCollision_ = r;
	return ts;
}

//rotate to an absolute angle
TRAJ_STATE Asserv::doRotateLeft(float degrees)
{
	return doRotateAbs(degrees);
}
TRAJ_STATE Asserv::doRotateRight(float degrees)
{
	return doRotateAbs(-degrees);
}

//rotate from current to an angle thetaInDegree (terrain landmark and matchcolor = 0), we think all in the x,y landmark.
TRAJ_STATE Asserv::doRotateTo(float thetaInDegree)
{
	float currentThetaInDegree = pos_getThetaInDegree();
	float degrees = getRelativeAngle(thetaInDegree) - currentThetaInDegree;

	// force it to be the positive remainder, so that 0 <= angle < 360
		//degrees = (degrees + 360) % 360;
		degrees = (((int) (degrees * 1000.0f) + 360000) % 360000) / 1000.0f;

	// reduce the angle
	//reduction sur une plage de [0 à 360]
	if (degrees > 360)
	{
		degrees = ((int) (degrees * 1000.0f) % 360000) / 1000.0f;

	}
	if (degrees < -360)
	{
		int d = (int) -(degrees * 1000.0f);
		d = d % 360000;
		degrees = -d / 1000.0f;
	}

	// force into the minimum absolute value residue class, so that -180 < angle <= 180
	if (degrees > 180)
		degrees -= 360;

	logger().debug() << "doRotateTo degrees=" << degrees << "degrees " << logs::end;
	TRAJ_STATE ts = doRotateAbs(degrees);

	return ts;
}

TRAJ_STATE Asserv::doMoveForwardTo(float xMM, float yMM)
{
	float dx = getRelativeX(xMM) - pos_getX_mm();
	float dy = yMM - pos_getY_mm();
	float aRadian = atan2(dy, dx);
	logger().debug() << "doMoveForwardTo doRotateTo degrees="
			<< (aRadian * 180.0f) / M_PI
			<< " dx="
			<< dx
			<< " dy="
			<< dy
			<< " aRadian="
			<< ((aRadian * 180.0f) / M_PI)
			<< " degrees. "
			<< logs::end;
	doRotateTo(getRelativeAngle((aRadian * 180.0f) / M_PI));
	float dist = sqrt(dx * dx + dy * dy);
	logger().debug() << "doMoveForwardTo dist sqrt(dx * dx + dy * dy)=" << dist << logs::end;
	return doLineAbs(dist);
}
TRAJ_STATE Asserv::doMoveForwardAndRotateTo(float xMM, float yMM, float thetaInDegree)
{
	TRAJ_STATE ts;
	ts = doMoveForwardTo(xMM, yMM);
	if (ts != TRAJ_OK)
		return ts;

	ts = doRotateTo(thetaInDegree);
	return ts;
}
TRAJ_STATE Asserv::doMoveBackwardTo(float xMM, float yMM)
{
	xMM = getRelativeX(xMM);

	float dx = xMM - pos_getX_mm();
	float dy = yMM - pos_getY_mm();
	float aRadian = atan2(dy, dx);

	doRotateTo(getRelativeAngle(((M_PI + aRadian) * 180.0f) / M_PI)); //TODO angle au plus court ?

	float dist = sqrt(dx * dx + dy * dy);
	return doLineAbs(-dist);
}
TRAJ_STATE Asserv::doMoveBackwardAndRotateTo(float xMM, float yMM, float thetaInDegree)
{
	TRAJ_STATE ts;
	ts = doMoveBackwardTo(xMM, yMM);
	if (ts != TRAJ_OK)
		return ts;
	ts = doRotateTo(thetaInDegree);
	return ts;
}

TRAJ_STATE Asserv::doMoveArcRotate(int degrees, float radiusMM)
{
	return pAsservInsa_->motion_DoArcRotate(degrees * M_PI / 180.0, radiusMM / 1000.0);
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

void Asserv::findPidLR(float posl_mm, int posr_mm, int sec)
{
	float posl = posl_mm / 1000.0f;
	float posr = posr_mm / 1000.0f;
	pAsservInsa_->motion_findPidLR(posl, posr, sec);
}

void Asserv::setPositionAndColor(float x_mm, float y_mm, float thetaInDegrees, bool matchColor = 0)
{
	setMatchColorPosition(matchColor);

	x_mm = getRelativeX(x_mm);
	thetaInDegrees = getRelativeAngle(thetaInDegrees);

	pAsservInsa_->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, thetaInDegrees * M_PI / 180.0);
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


