#include "Asserv.hpp"

#include <unistd.h>
#include <cmath>

#include "../../Asserv.Insa/AsservInsa.hpp"
#include "../Utils/Chronometer.hpp"
#include "MovingBase.hpp"

Asserv::Asserv(std::string botId, Robot * robot)
{
	pMovingBase_ = new MovingBase(botId);
	asservdriver = AAsservDriver::create(botId);
	probot_ = robot;

	useInternalAsserv_ = true; //configuration par defaut

	if (useInternalAsserv_)
		pAsservInsa_ = new AsservInsa(robot);
	else
		pAsservInsa_ = NULL;

	ignoreRearCollision_ = true;
	ignoreFrontCollision_ = true;
	matchColorPosition_ = false;
}

MovingBase * Asserv::base()
{
	return pMovingBase_;
}

void Asserv::startMotionTimerAndOdo(bool assistedHandlingEnabled)
{
	if (useInternalAsserv_)
	{
		//f=20 Hz =>every 50ms
		//f=40 Hz => every 25ms
		//f=50 Hz => every 20ms
		//f=100 Hz =>every 10ms
		//f=200 Hz =>every 5ms
		//pAsservInsa_->motion_SetSamplingFrequency(50); //20ms pour APF, 50 pour EV3, pour avoir plus de step sur la vitesse
		pAsservInsa_->motion_Init(30);
	}
	else
	{

		asservdriver->motion_ActivateManager(true); //on active la carte d'asserv externe
		if (assistedHandlingEnabled)
			asservdriver->motion_AssistedHandling();
		else
			asservdriver->motion_FreeMotion();

	}
}
void Asserv::setLowSpeed(bool enable)
{
	asservdriver->motion_setLowSpeed(enable);
}
void Asserv::disablePID() //deprecated TODO a remplacer par freemotion
{
}

void Asserv::freeMotion()
{
	if (useInternalAsserv_)
		pAsservInsa_->motion_FreeMotion();
	else
		asservdriver->motion_FreeMotion();
}

void Asserv::assistedHandling()
{
	if (useInternalAsserv_)
		pAsservInsa_->motion_AssistedHandling();
	else
		asservdriver->motion_AssistedHandling();
}

void Asserv::stopMotionTimerAndOdo()
{
	if (useInternalAsserv_)
		pAsservInsa_->motion_StopTimer();
	else
	{
		asservdriver->path_InterruptTrajectory();
		asservdriver->motion_ActivateManager(false);
		//this->waitForEnd(); //TODO ?
	}
}

//matchColor = 0 => en bas à gauche
void Asserv::setPositionAndColor(float x_mm, float y_mm, float thetaInDegrees, bool matchColor = 0)
{
	printf("matchcolor%d\n", matchColor);
	setMatchColorPosition(matchColor);

	x_mm = getRelativeX(x_mm);
	thetaInDegrees = getRelativeAngle(thetaInDegrees);

	if (useInternalAsserv_)
		pAsservInsa_->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, thetaInDegrees * M_PI / 180.0);
	else
		asservdriver->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, thetaInDegrees * M_PI / 180.0);
}

void Asserv::ignoreFrontCollision(bool ignore) //TODO rename setIgnoreFrontCollision
{
	ignoreFrontCollision_ = ignore;
}

void Asserv::ignoreRearCollision(bool ignore) //TODO rename setIgnoreRearCollision
{
	ignoreRearCollision_ = ignore;
}

RobotPosition Asserv::pos_getPosition()
{
	RobotPosition p;
	if (useInternalAsserv_)
		p = pAsservInsa_->odo_GetPosition();
	else
		p = asservdriver->odo_GetPosition();
	return p;
}
float Asserv::pos_getX_mm()
{
	RobotPosition p;
	if (useInternalAsserv_)
		p = pAsservInsa_->odo_GetPosition();
	else
		p = asservdriver->odo_GetPosition();
	return p.x * 1000.0f;
}
float Asserv::pos_getY_mm()
{
	RobotPosition p;
	if (useInternalAsserv_)
		p = pAsservInsa_->odo_GetPosition();
	else
		p = asservdriver->odo_GetPosition();
	return p.y * 1000.0f;
}
// angle in radian
float Asserv::pos_getTheta()
{
	RobotPosition p;
	if (useInternalAsserv_)
		p = pAsservInsa_->odo_GetPosition();
	else
		p = asservdriver->odo_GetPosition();
	return p.theta;
}

// angle in degrees
float Asserv::pos_getThetaInDegree()
{
	return (pos_getTheta() * 180.0f) / M_PI;
}

void Asserv::setFrontCollision()
{
	logger().error() << "setFrontCollision ignoreFrontCollision_=" << ignoreFrontCollision_
			<< logs::end;

	if (!ignoreFrontCollision_)
	{
		if (useInternalAsserv_)
			pAsservInsa_->path_CollisionOnTrajectory();
		else
			asservdriver->path_CollisionOnTrajectory();
	}
}

void Asserv::setRearCollision()
{
	logger().error() << "setRearCollision ignoreRearCollision_=" << ignoreRearCollision_
			<< logs::end;
	if (!ignoreRearCollision_)
	{
		if (useInternalAsserv_)
			pAsservInsa_->path_CollisionRearOnTrajectory();
		else
			asservdriver->path_CollisionRearOnTrajectory();
	}
}

TRAJ_STATE Asserv::doLineAbs(float distance_mm) // if distance <0, move backward
{
	int f = ignoreFrontCollision_;
	int r = ignoreRearCollision_;
	if (distance_mm > 0)
	{
		ignoreRearCollision_ = true;
	}
	else
	{
		ignoreFrontCollision_ = true;
	}

	float meters = distance_mm / 1000.0f;
	logger().debug() << "Asserv::doLineAbs meters=" << meters << " mm=" << distance_mm << logs::end;
	TRAJ_STATE ts;
	if (useInternalAsserv_)
		ts = pAsservInsa_->motion_DoLine(meters);
	else
		ts = asservdriver->motion_DoLine(meters);

	logger().error() << "Asserv::doLineAbs f=" << f << " r=" << r << logs::end;

	ignoreFrontCollision_ = f;
	ignoreRearCollision_ = r;
	return ts;
}

TRAJ_STATE Asserv::doRotateAbs(float degrees)
{
	logger().debug() << "doRotateAbs degrees=" << degrees << logs::end;

	int f = ignoreFrontCollision_;
	int r = ignoreRearCollision_;
	ignoreRearCollision_ = true;
	ignoreFrontCollision_ = true;

	TRAJ_STATE ts;
	float radians = (degrees * M_PI) / 180.0f;
	if (useInternalAsserv_)
	{

		ts = pAsservInsa_->motion_DoRotate(radians);
	}
	else
		ts = asservdriver->motion_DoRotate(radians);


	logger().error() << "Asserv::doRotateAbs f=" << f << " r=" << r << logs::end;ignoreFrontCollision_ = f;
	ignoreRearCollision_ = r;

	return ts;
}
TRAJ_STATE Asserv::doRotateLeft(float degrees)
{
	return doRotateAbs(degrees);
}
TRAJ_STATE Asserv::doRotateRight(float degrees)
{
	return doRotateAbs(-degrees);
}

TRAJ_STATE Asserv::doFaceTo(float xMM, float yMM)
{
	logger().debug() << "doFaceTo xMM=" << xMM << " yMM=" << yMM << logs::end;

	int f = ignoreFrontCollision_;
	int r = ignoreRearCollision_;
	ignoreRearCollision_ = true;
	ignoreFrontCollision_ = true;

	TRAJ_STATE ts;
	if (useInternalAsserv_)
	{
		//TODO
		logger().error() << "TODO A CODER !!!!!!!!!!!!" << logs::end;
	}
	else
		ts = asservdriver->motion_DoFace(xMM, yMM);

	ignoreFrontCollision_ = f;
	ignoreRearCollision_ = r;

	return ts;

}

//relative motion (depends on current position of the robot)
TRAJ_STATE Asserv::doRotateTo(float thetaInDegree)
{
	logger().info() << "====2 doRotateTo thetaInDegree=" << thetaInDegree << "degrees " << logs::end;


	float currentThetaInDegree = pos_getThetaInDegree();
	float degrees = getRelativeAngle(thetaInDegree) - currentThetaInDegree;

// force it to be the positive remainder, so that 0 <= angle < 360
//degrees = (degrees + 360) % 360;
	degrees = (((int) (degrees * 1000.0f) + 360000) % 360000) / 1000.0f;

// reduce the angle
//reduction sur une plage de [0 à 360]
	if (degrees >= 360.0)
	{
		degrees = ((int) (degrees * 1000.0f) % 360000) / 1000.0f;

	}
	if (degrees < -360.0)
	{
		int d = (int) -(degrees * 1000.0f);
		d = d % 360000;
		degrees = -d / 1000.0f;
	}

// force into the minimum absolute value residue class, so that -180 < angle <= 180
	if (degrees >= 180) degrees -= 360;

	logger().info() << "==== doRotateTo degrees=" << degrees << "degrees " << logs::end;
	TRAJ_STATE ts = doRotateAbs(degrees);

	return ts;
}
TRAJ_STATE Asserv::doMoveForwardTo(float xMM, float yMM)
{
	float dx = getRelativeX(xMM) - pos_getX_mm();
	float dy = yMM - pos_getY_mm();
	float aRadian = atan2(dy, dx);
	logger().debug() << "doMoveForwardTo doRotateTo degrees=" << (aRadian * 180.0f) / M_PI << " dx="
			<< dx << " dy=" << dy
			<< "  (aRadian * 180.0f) / M_PI)= " << (aRadian * 180.0f) / M_PI
		<< " get=" << getRelativeAngle((aRadian * 180.0f) / M_PI)
		<< " xMM=" << xMM
		<< " yMM=" << yMM
	<< " getX=" << pos_getX_mm()
	<< " getY=" << pos_getY_mm()
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
	if (ts != TRAJ_OK) return ts;

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
	if (ts != TRAJ_OK) return ts;
	ts = doRotateTo(thetaInDegree);
	return ts;
}
TRAJ_STATE Asserv::doMoveArcRotate(int degrees, float radiusMM)
{
	//TODO
	return TRAJ_ERROR;
}

