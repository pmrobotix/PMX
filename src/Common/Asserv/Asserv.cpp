#include "Asserv.hpp"

#include "../../Log/Logger.hpp"
#include "MovingBase.hpp"

Asserv::Asserv(std::string botId, Robot * robot)
{
	pMovingBase_ = new MovingBase(botId);

	probot_ = robot;

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

void Asserv::ignoreFrontCollision(bool ignore)
{
	ignoreFrontCollision_ = ignore;
}

void Asserv::ignoreRearCollision(bool ignore)
{
	ignoreRearCollision_ = ignore;
}

// position x,y in mm
float Asserv::pos_getX_mm()
{
	return 0;
}
float Asserv::pos_getY_mm()
{
	return 0;
}
// angle in radian
float Asserv::pos_getTheta()
{
	return 0;
}
// angle in degrees
float Asserv::pos_getThetaInDegree()
{
	return 0;
}

void Asserv::setFrontCollision()
{
	logger().error() << "setFrontCollision ignoreFrontCollision_=" << ignoreFrontCollision_
			<< logs::end;

	if (!ignoreFrontCollision_)
	{
		//pAsservInsa_->path_CollisionOnTrajectory();
	}
}

void Asserv::setRearCollision()
{
	logger().error() << "setRearCollision ignoreRearCollision_=" << ignoreRearCollision_
			<< logs::end;
	if (!ignoreRearCollision_)
	{
		//pAsservInsa_->path_CollisionRearOnTrajectory();
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

	ignoreFrontCollision_ = f;
	ignoreRearCollision_ = r;
	//return ts;
}

TRAJ_STATE Asserv::doRotateAbs(float degrees)
{

}
TRAJ_STATE Asserv::doRotateLeft(float degrees)
{

}
TRAJ_STATE Asserv::doRotateRight(float degrees)
{

}

//relative motion (depends on current position of the robot)
TRAJ_STATE Asserv::doRotateTo(float thetaInDegree)
{

}
TRAJ_STATE Asserv::doMoveForwardTo(float xMM, float yMM)
{

}
TRAJ_STATE Asserv::doMoveForwardAndRotateTo(float xMM, float yMM, float thetaInDegree)
{

}
TRAJ_STATE Asserv::doMoveBackwardTo(float xMM, float yMM)
{

}
TRAJ_STATE Asserv::doMoveBackwardAndRotateTo(float xMM, float yMM, float thetaInDegree)
{

}
TRAJ_STATE Asserv::doMoveArcRotate(int degrees, float radiusMM)
{

}

