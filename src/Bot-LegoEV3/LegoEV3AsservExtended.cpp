#include "LegoEV3AsservExtended.hpp"

#include <cmath>
#include <cstdio>

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Log/Logger.hpp"

AsservInsa * LegoEV3AsservExtended::insa()
{
	return pAsservInsa_;
}

LegoEV3AsservExtended::LegoEV3AsservExtended(std::string botId, Robot * robot) :
		Asserv(botId, robot) //on appelle le constructeur pere
{

	last_sens_left_ = 0;
	last_sens_right_ = 0;
	motorwheel_patch_m = 0.0; //metres
	motorwheel_patch_rad = 0.0; //radians
}

void LegoEV3AsservExtended::startMotionTimerAndOdo()
{

#ifdef SIMU
	//SIMU EV3
	printf("---LegoEV3AsservExtended > SIMU EV3\n");
	pAsservInsa_->encoder_SetResolution(1398, 1398, 129);
	pAsservInsa_->motion_SetDefaultAccel(0.4);
	pAsservInsa_->motion_SetDefaultVmax(0.6);
	pAsservInsa_->motion_SetDefaultDecel(0.3);
	pAsservInsa_->motion_setMaxPwmValue(860); //max power ev3 using hardregulation
	pAsservInsa_->motion_Init();
	//RCVA PI
//	pAsservInsa_->motion_configureAlphaPID(1200.0, 0.0, 0.0);
//	pAsservInsa_->motion_configureDeltaPID(900.0, 0.0, 0.0);
//	//NORMAL PID
	pAsservInsa_->motion_configureAlphaPID(0.011, 0.0, 0.00005); //(2017) 0.25, 0.001, 0.00005 //(2016)0.06 0.001 0.0005
	pAsservInsa_->motion_configureDeltaPID(0.011, 0.0, 0.0000); //(2017) 0.3, 0.001, 0.00005

	pAsservInsa_->motion_configureLeftPID(0.0, 0.0, 0.0);
	pAsservInsa_->motion_configureRightPID(0.0, 0.0, 0.0);
	motorwheel_patch_m = 0.0; //metres
	motorwheel_patch_rad = 0.0; //radians



#else
	//Real EV3

	printf("---LegoEV3AsservExtended > Real EV3\n");

	pAsservInsa_->encoder_SetResolution(1398, 1398, 129);//1395, 1395, 129
	pAsservInsa_->motion_SetDefaultAccel(1.0);
	pAsservInsa_->motion_SetDefaultVmax(2.0);
	pAsservInsa_->motion_SetDefaultDecel(1.0);
	pAsservInsa_->motion_setMaxPwmValue(860);//max power ev3 using hardregulation
	pAsservInsa_->motion_Init();

	//RCVA PID
//	pAsservInsa_->motion_configureAlphaPID(1200.0.0, 0.0, 0.0);
//	pAsservInsa_->motion_configureDeltaPID(l900.0.0, 0.0, 0.0);
//	pAsservInsa_->motion_configureLeftPID(0.0, 0.0, 0.0);
//	pAsservInsa_->motion_configureRightPID(0.0, 0.0, 0.0);
	//	//NORMAL PID
	pAsservInsa_->motion_configureLeftPID(0.0, 0.0, 0.0);
	pAsservInsa_->motion_configureRightPID(0.0, 0.0, 0.0);
	//pAsservInsa_->motion_configureAlphaPID(0.06, 0.002, 0.0005);//0.05 0.002 0.0005 (2016)
	//pAsservInsa_->motion_configureDeltaPID(0.05, 0.002, 0.0005);//0.05 0.002 0.0005 (2016)
	pAsservInsa_->motion_configureAlphaPID(0.07, 0.002, 0.0005);//(2017) 0.25, 0.001, 0.00005
	pAsservInsa_->motion_configureDeltaPID(0.07, 0.002, 0.0005);//(2017) 0.3, 0.001, 0.00005

	motorwheel_patch_m = 0.0;//metres
	motorwheel_patch_rad = -0.01;//radians
#endif

	pAsservInsa_->motion_DisablePID();

	//f=20 Hz =>every 50ms
	//f=40 Hz => every 25ms
	//f=50 Hz => every 20ms
	//f=100 Hz =>every 10ms
	pAsservInsa_->motion_SetSamplingFrequency(100);

}

void LegoEV3AsservExtended::freeMotion()
{
	pAsservInsa_->motion_FreeMotion();
}

void LegoEV3AsservExtended::assistedHandling()
{
	pAsservInsa_->motion_AssistedHandling();
}

void LegoEV3AsservExtended::stopMotionTimerAndOdo()
{
	pAsservInsa_->motion_StopTimer();
}

// if distance <0, move backward
TRAJ_STATE LegoEV3AsservExtended::doLineAbs(float distance_mm)
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

	int sens_left = 0;
	int sens_right = 0;

	if (distance_mm >= 0)
	{
		sens_left = 1;
		if (sens_left != last_sens_left_)
		{
			meters = meters + (motorwheel_patch_m);
			logger().debug() << "change sens_left m=" << meters << logs::end;
		}
		sens_right = 1;
		if (sens_right != last_sens_right_)
		{
			meters = meters + (motorwheel_patch_m);
			logger().debug() << "change sens_right m=" << meters << logs::end;
		}
	}
	else
	{
		sens_left = 0;
		if (sens_left != last_sens_left_)
		{
			meters = meters - (motorwheel_patch_m);
		}
		sens_right = 0;
		if (sens_right != last_sens_right_)
		{
			meters = meters - (motorwheel_patch_m);
		}
	}

	TRAJ_STATE ts = pAsservInsa_->motion_DoLine(meters);

	//*********************************
	last_sens_left_ = sens_left;
	last_sens_right_ = sens_right;
	//*********************************

	ignoreFrontCollision_ = f;
	ignoreRearCollision_ = r;
	return ts;
}

//permet de tourner sur un angle défini (inclus 2 ou 3 tours sur soi-même)
TRAJ_STATE LegoEV3AsservExtended::doRotateAbs(float degrees)
{
	logger().debug() << "doRotateAbs degrees=" << degrees << logs::end;

	float radians = (degrees * M_PI) / 180.0f;
	int f = ignoreFrontCollision_;
	int r = ignoreRearCollision_;
	ignoreRearCollision_ = true;
	ignoreFrontCollision_ = true;

	int sens_left = 0;
	int sens_right = 0;

	if (degrees > 0)
	{
		sens_left = 0;
		if (sens_left != last_sens_left_)
		{
			radians = radians + motorwheel_patch_rad;
			logger().error() << "doRotateAbs > change sens_left + rad=" << radians << logs::end;
		}
		sens_right = 1;
		if (sens_right != last_sens_right_)
		{
			radians = radians + motorwheel_patch_rad;
			logger().error() << "doRotateAbs > change sens_right + rad=" << radians << logs::end;
		}

	}
	else if (degrees < 0)
	{
		sens_left = 1;
		if (sens_left != last_sens_left_)
		{
			radians = radians - motorwheel_patch_rad;
			logger().error() << "doRotateAbs > change sens_left - rad=" << radians << logs::end;
		}
		sens_right = 0;
		if (sens_right != last_sens_right_)
		{
			radians = radians - motorwheel_patch_rad;
			logger().error() << "doRotateAbs > change sens_right - rad=" << radians << logs::end;
		}
	}

	TRAJ_STATE ts = pAsservInsa_->motion_DoRotate(radians);

	//*********************************
	last_sens_left_ = sens_left;
	last_sens_right_ = sens_right;
	//*********************************

	ignoreFrontCollision_ = f;
	ignoreRearCollision_ = r;

	return ts;
}

//rotate to an absolute angle
TRAJ_STATE LegoEV3AsservExtended::doRotateLeft(float degrees)
{
	return doRotateAbs(degrees);
}
TRAJ_STATE LegoEV3AsservExtended::doRotateRight(float degrees)
{
	return doRotateAbs(-degrees);
}

//rotate from current to an angle thetaInDegree (terrain landmark and matchcolor = 0), we think all in the x,y landmark.
TRAJ_STATE LegoEV3AsservExtended::doRotateTo(float thetaInDegree)
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
	if (degrees > 180) degrees -= 360;

	logger().debug() << "doRotateTo degrees=" << degrees << "degrees " << logs::end;
	TRAJ_STATE ts = doRotateAbs(degrees);

	return ts;
}

TRAJ_STATE LegoEV3AsservExtended::doMoveForwardTo(float xMM, float yMM)
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
TRAJ_STATE LegoEV3AsservExtended::doMoveForwardAndRotateTo(float xMM, float yMM,
		float thetaInDegree)
{
	TRAJ_STATE ts;
	ts = doMoveForwardTo(xMM, yMM);
	if (ts != TRAJ_OK) return ts;

	ts = doRotateTo(thetaInDegree);
	return ts;
}
TRAJ_STATE LegoEV3AsservExtended::doMoveBackwardTo(float xMM, float yMM)
{
	xMM = getRelativeX(xMM);

	float dx = xMM - pos_getX_mm();
	float dy = yMM - pos_getY_mm();
	float aRadian = atan2(dy, dx);

	doRotateTo(getRelativeAngle(((M_PI + aRadian) * 180.0f) / M_PI)); //TODO angle au plus court ?

	float dist = sqrt(dx * dx + dy * dy);
	return doLineAbs(-dist);
}
TRAJ_STATE LegoEV3AsservExtended::doMoveBackwardAndRotateTo(float xMM, float yMM,
		float thetaInDegree)
{
	TRAJ_STATE ts;
	ts = doMoveBackwardTo(xMM, yMM);
	if (ts != TRAJ_OK) return ts;
	ts = doRotateTo(thetaInDegree);
	return ts;
}

TRAJ_STATE LegoEV3AsservExtended::doMoveArcRotate(int degrees, float radiusMM)
{
	return pAsservInsa_->motion_DoArcRotate(degrees * M_PI / 180.0, radiusMM / 1000.0);
}

void LegoEV3AsservExtended::configureAlphaPID(float Ap, float Ai, float Ad)
{
	pAsservInsa_->motion_configureAlphaPID(Ap, Ai, Ad);
}

void LegoEV3AsservExtended::configureDeltaPID(float Dp, float Di, float Dd)
{
	pAsservInsa_->motion_configureDeltaPID(Dp, Di, Dd);
}

void LegoEV3AsservExtended::findPidAD(float degrees, int mm, int sec)
{
	float meters = mm / 1000.0f;
	float radians = (degrees * M_PI) / 180.0f;
	pAsservInsa_->motion_findPidAD(radians, meters, sec);
}

void LegoEV3AsservExtended::findPidLR(float posl_mm, int posr_mm, int sec)
{
	float posl = posl_mm / 1000.0f;
	float posr = posr_mm / 1000.0f;
	pAsservInsa_->motion_findPidLR(posl, posr, sec);
}


void LegoEV3AsservExtended::setPositionAndColor(float x_mm, float y_mm, float thetaInDegrees,
		bool matchColor = 0)
{
	setMatchColorPosition(matchColor);

	x_mm = getRelativeX(x_mm);
	thetaInDegrees = getRelativeAngle(thetaInDegrees);

	pAsservInsa_->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, thetaInDegrees * M_PI / 180.0);
}

void LegoEV3AsservExtended::setVmax(float vmax)
{
	pAsservInsa_->motion_SetDefaultVmax(vmax);
}

void LegoEV3AsservExtended::setAccel(float acc)
{
	pAsservInsa_->motion_SetDefaultAccel(acc);
}

void LegoEV3AsservExtended::setDecel(float dec)
{
	pAsservInsa_->motion_SetDefaultDecel(dec);
}

void LegoEV3AsservExtended::setFrontCollision()
{
	logger().error() << "setFrontCollision ignoreFrontCollision_=" << ignoreFrontCollision_
			<< logs::end;

	if (!ignoreFrontCollision_) pAsservInsa_->path_CollisionOnTrajectory();
}

void LegoEV3AsservExtended::setRearCollision()
{
	logger().error() << "setRearCollision ignoreRearCollision_=" << ignoreRearCollision_
			<< logs::end;
	if (!ignoreRearCollision_) pAsservInsa_->path_CollisionRearOnTrajectory();
}

// position x,x in mm
float LegoEV3AsservExtended::pos_getX_mm()
{
	return pAsservInsa_->pos_getX_mm();
}
float LegoEV3AsservExtended::pos_getY_mm()
{
	return pAsservInsa_->pos_getY_mm();
}
// angle in radian
float LegoEV3AsservExtended::pos_getTheta()
{
	return pAsservInsa_->pos_getTheta();
}
// angle in degrees
float LegoEV3AsservExtended::pos_getThetaInDegree()
{
	return pAsservInsa_->pos_getThetaInDegree();
}

