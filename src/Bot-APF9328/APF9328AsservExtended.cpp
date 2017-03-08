#include "APF9328AsservExtended.hpp"

#include <cmath>

#include "../Log/Logger.hpp"

AsservInsa * APF9328AsservExtended::insa()
{
	return pAsservInsa_;
}


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


void APF9328AsservExtended::freeMotion()
{
	pAsservInsa_->motion_FreeMotion();
}

void APF9328AsservExtended::assistedHandling()
{
	pAsservInsa_->motion_AssistedHandling();
}

void APF9328AsservExtended::stopMotionTimerAndOdo()
{
	pAsservInsa_->motion_StopTimer();
}

// if distance <0, move backward
TRAJ_STATE APF9328AsservExtended::doLineAbs(float distance_mm)
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
	TRAJ_STATE ts = pAsservInsa_->motion_DoLine(meters);

	ignoreFrontCollision_ = f;
	ignoreRearCollision_ = r;
	return ts;
}

//permet de tourner sur un angle défini (inclus 2 ou 3 tours sur soi-même)
TRAJ_STATE APF9328AsservExtended::doRotateAbs(float degrees)
{
	logger().debug() << "doRotateAbs degrees=" << degrees << logs::end;

	float radians = (degrees * M_PI) / 180.0f;
	int f = ignoreFrontCollision_;
	int r = ignoreRearCollision_;
	ignoreRearCollision_ = true;
	ignoreFrontCollision_ = true;

	TRAJ_STATE ts = pAsservInsa_->motion_DoRotate(radians);

	ignoreFrontCollision_ = f;
	ignoreRearCollision_ = r;

	return ts;
}

//rotate to an absolute angle
TRAJ_STATE APF9328AsservExtended::doRotateLeft(float degrees)
{
	return doRotateAbs(degrees);
}
TRAJ_STATE APF9328AsservExtended::doRotateRight(float degrees)
{
	return doRotateAbs(-degrees);
}

//rotate from current to an angle thetaInDegree (terrain landmark and matchcolor = 0), we think all in the x,y landmark.
TRAJ_STATE APF9328AsservExtended::doRotateTo(float thetaInDegree)
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

TRAJ_STATE APF9328AsservExtended::doMoveForwardTo(float xMM, float yMM)
{
	float dx = getRelativeX(xMM) - pos_getX_mm();
	float dy = yMM - pos_getY_mm();
	float aRadian = atan2(dy, dx);
	logger().debug() << "doMoveForwardTo doRotateTo degrees=" << (aRadian * 180.0f) / M_PI << " dx="
			<< dx << " dy=" << dy << " aRadian=" << ((aRadian * 180.0f) / M_PI) << " degrees. "
			<< logs::end;
	doRotateTo(getRelativeAngle((aRadian * 180.0f) / M_PI));
	float dist = sqrt(dx * dx + dy * dy);
	logger().debug() << "doMoveForwardTo dist sqrt(dx * dx + dy * dy)=" << dist << logs::end;
	return doLineAbs(dist);
}
TRAJ_STATE APF9328AsservExtended::doMoveForwardAndRotateTo(float xMM, float yMM, float thetaInDegree)
{
	TRAJ_STATE ts;
	ts = doMoveForwardTo(xMM, yMM);
	if (ts != TRAJ_OK)
		return ts;

	ts = doRotateTo(thetaInDegree);
	return ts;
}
TRAJ_STATE APF9328AsservExtended::doMoveBackwardTo(float xMM, float yMM)
{
	xMM = getRelativeX(xMM);

	float dx = xMM - pos_getX_mm();
	float dy = yMM - pos_getY_mm();
	float aRadian = atan2(dy, dx);

	doRotateTo(getRelativeAngle(((M_PI + aRadian) * 180.0f) / M_PI)); //TODO angle au plus court ?

	float dist = sqrt(dx * dx + dy * dy);
	return doLineAbs(-dist);
}
TRAJ_STATE APF9328AsservExtended::doMoveBackwardAndRotateTo(float xMM, float yMM, float thetaInDegree)
{
	TRAJ_STATE ts;
	ts = doMoveBackwardTo(xMM, yMM);
	if (ts != TRAJ_OK)
		return ts;
	ts = doRotateTo(thetaInDegree);
	return ts;
}

TRAJ_STATE APF9328AsservExtended::doMoveArcRotate(int degrees, float radiusMM)
{
	return pAsservInsa_->motion_DoArcRotate(degrees * M_PI / 180.0, radiusMM / 1000.0);
}

void APF9328AsservExtended::configureAlphaPID(float Ap, float Ai, float Ad)
{
	pAsservInsa_->motion_configureAlphaPID(Ap, Ai, Ad);
}

void APF9328AsservExtended::configureDeltaPID(float Dp, float Di, float Dd)
{
	pAsservInsa_->motion_configureDeltaPID(Dp, Di, Dd);
}

void APF9328AsservExtended::findPidAD(float degrees, int mm, int sec)
{
	float meters = mm / 1000.0f;
	float radians = (degrees * M_PI) / 180.0f;
	pAsservInsa_->motion_findPidAD(radians, meters, sec);
}

void APF9328AsservExtended::findPidLR(float posl_mm, int posr_mm, int sec)
{
	float posl = posl_mm / 1000.0f;
	float posr = posr_mm / 1000.0f;
	pAsservInsa_->motion_findPidLR(posl, posr, sec);
}



void APF9328AsservExtended::setPositionAndColor(float x_mm, float y_mm, float thetaInDegrees, bool matchColor = 0)
{
	setMatchColorPosition(matchColor);

	x_mm = getRelativeX(x_mm);
	thetaInDegrees = getRelativeAngle(thetaInDegrees);

	pAsservInsa_->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, thetaInDegrees * M_PI / 180.0);
}

void APF9328AsservExtended::setVmax(float vmax)
{
	pAsservInsa_->motion_SetDefaultVmax(vmax);
}

void APF9328AsservExtended::setAccel(float acc)
{
	pAsservInsa_->motion_SetDefaultAccel(acc);
}

void APF9328AsservExtended::setDecel(float dec)
{
	pAsservInsa_->motion_SetDefaultDecel(dec);
}

void APF9328AsservExtended::setFrontCollision()
{
	logger().error() << "setFrontCollision ignoreFrontCollision_="
			<< ignoreFrontCollision_
			<< logs::end;

	if (!ignoreFrontCollision_)
		pAsservInsa_->path_CollisionOnTrajectory();
}

void APF9328AsservExtended::setRearCollision()
{
	logger().error() << "setRearCollision ignoreRearCollision_="
			<< ignoreRearCollision_
			<< logs::end;
	if (!ignoreRearCollision_)
		pAsservInsa_->path_CollisionRearOnTrajectory();
}



// position x,x in mm
float APF9328AsservExtended::pos_getX_mm()
{
	return pAsservInsa_->pos_getX_mm();
}
float APF9328AsservExtended::pos_getY_mm()
{
	return pAsservInsa_->pos_getY_mm();
}
// angle in radian
float APF9328AsservExtended::pos_getTheta()
{
	return pAsservInsa_->pos_getTheta();
}
// angle in degrees
float APF9328AsservExtended::pos_getThetaInDegree()
{
	return pAsservInsa_->pos_getThetaInDegree();
}
