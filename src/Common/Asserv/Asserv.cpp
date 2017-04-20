#include "Asserv.hpp"

#include "../../Asserv.Insa/AsservInsa.hpp"
#include "../../Log/Logger.hpp"
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

	ignoreRearCollision_ = false;
	ignoreFrontCollision_ = false;
	matchColorPosition_ = false;
}

MovingBase * Asserv::base()
{
	return pMovingBase_;
}

void Asserv::startMotionTimerAndOdo() //doit etre surchargé
{
}

void Asserv::freeMotion()
{
}

void Asserv::assistedHandling()
{
}

void Asserv::stopMotionTimerAndOdo()
{
}

void Asserv::setPositionAndColor(float x_mm, float y_mm, float thetaInDegrees, bool matchColor = 0)
{
	setMatchColorPosition(matchColor);

	x_mm = getRelativeX(x_mm);
	thetaInDegrees = getRelativeAngle(thetaInDegrees);
}

void Asserv::ignoreFrontCollision(bool ignore) //TODO rename setIgnoreFrontCollision
{
	ignoreFrontCollision_ = ignore;
}

void Asserv::ignoreRearCollision(bool ignore) //TODO rename setIgnoreRearCollision
{
	ignoreRearCollision_ = ignore;
}

// position x,y in mm
float Asserv::pos_getX_mm()
{
	RobotPosition p = asservdriver->odo_GetPosition();
	return p.x;
}
float Asserv::pos_getY_mm()
{
	RobotPosition p = asservdriver->odo_GetPosition();
	return p.y;
}
// angle in radian
float Asserv::pos_getTheta()
{
	RobotPosition p = asservdriver->odo_GetPosition();
	return p.theta;
}
// angle in degrees
float Asserv::pos_getThetaInDegree()
{
	RobotPosition p = asservdriver->odo_GetPosition();
	return (p.theta * 180.0f) / M_PI;
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
	//TRAJ_STATE ts = pAsservInsa_->motion_DoLine(meters);
	TRAJ_STATE ts = asservdriver->motion_DoLine(meters);

	ignoreFrontCollision_ = f;
	ignoreRearCollision_ = r;
	return ts;
}

TRAJ_STATE Asserv::doRotateAbs(float degrees)
{
	return TRAJ_ERROR;
}
TRAJ_STATE Asserv::doRotateLeft(float degrees)
{
	return TRAJ_ERROR;
}
TRAJ_STATE Asserv::doRotateRight(float degrees)
{
	return TRAJ_ERROR;
}

//relative motion (depends on current position of the robot)
TRAJ_STATE Asserv::doRotateTo(float thetaInDegree)
{
	return TRAJ_ERROR;
}
TRAJ_STATE Asserv::doMoveForwardTo(float xMM, float yMM)
{
	return TRAJ_ERROR;
}
TRAJ_STATE Asserv::doMoveForwardAndRotateTo(float xMM, float yMM, float thetaInDegree)
{
	return TRAJ_ERROR;
}
TRAJ_STATE Asserv::doMoveBackwardTo(float xMM, float yMM)
{
	return TRAJ_ERROR;
}
TRAJ_STATE Asserv::doMoveBackwardAndRotateTo(float xMM, float yMM, float thetaInDegree)
{
	return TRAJ_ERROR;
}
TRAJ_STATE Asserv::doMoveArcRotate(int degrees, float radiusMM)
{
	return TRAJ_ERROR;
}

