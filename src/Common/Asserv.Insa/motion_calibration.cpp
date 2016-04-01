#include "AsservInsa.hpp"




void AsservInsa::encoder_SetResolution(uint32 leftTicksPerM, uint32 rightTicksPerM, float entraxe_mm)
{

	if (leftTicksPerM > rightTicksPerM)
	{
		leftEncoderRatio = vtopsPerTicks;
		rightEncoderRatio = vtopsPerTicks * rightTicksPerM / (float) leftTicksPerM;
		valueVTops = 1 / (float) (vtopsPerTicks * leftTicksPerM);
		// 1 / ( vtops/tick * tick/meter) = meter/vtops
	}
	else
	{
		leftEncoderRatio = vtopsPerTicks * leftTicksPerM / (float) rightTicksPerM;
		rightEncoderRatio = vtopsPerTicks;
		valueVTops = 1 / (float) (vtopsPerTicks * rightTicksPerM);
	}

/*
	printf("encoder_SetResolution   %d , %d \n", leftTicksPerM, rightTicksPerM);
	printf("valueVTops %f = 1 / %f = 1 / %d x %d \n",
			valueVTops,
			(float) (vtopsPerTicks * rightTicksPerM),
			vtopsPerTicks,
			rightTicksPerM);*/

	//recompute values involving vTops
	encoder_SetDist(entraxe_mm / 1000.0);

	logger().debug() << "encoder_SetResolution  leftTicksPerM=" << leftTicksPerM
			<< " rightTicksPerM=" << rightTicksPerM
			<< " valueVTops=" << valueVTops

			<< logs::end;
}

void AsservInsa::motion_SetSamplingFrequency(uint frequency)
{
	if (frequency == 0)
	{
		logger().error() << "motion_InitTimer frequency= " << frequency << logs::end;
		frequency = 1;
	}

	loopDelayInMillis = 1000.0 / (float) frequency;
	logger().debug() << "loopDelayInMillis= " << loopDelayInMillis << logs::end;

	//need a recomputation of all values involving sampling time
	// kP/kI/kD/dPeriod
	// speed/acceleration

}

void AsservInsa::pos_SetPosition(float x, float y, float theta)	//TODO add match color
{
	//TODO Odometry... odo_SetPosition(float x, float y, float theta)
	pos_x = x;
	pos_y = y;
	pos_theta = theta;
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

void AsservInsa::pos_GetPositionXYTheta(float *x, float *y, float *theta)
{
	//TODO Odometry... odo_GetPositionXYTheta(float *x, float *y, float *theta)
	*x = pos_x;
	*y = pos_y;
	*theta = pos_theta;
}

// position x,x in mm
float AsservInsa::pos_getX_mm()
{
	RobotPosition p = odo_GetPosition();
	return p.x * 1000.0f;
}
float AsservInsa::pos_getY_mm()
{
	RobotPosition p = odo_GetPosition();
	return p.y * 1000.0f;
}
// angle in radian
float AsservInsa::pos_getTheta()
{
	RobotPosition p = odo_GetPosition();
	return p.theta;
}
// angle in degrees
float AsservInsa::pos_getThetaInDegree()
{
	RobotPosition p = odo_GetPosition();
	return (p.theta * 180.0f) / M_PI;
}

// en m/s
float AsservInsa::motion_GetDefaultSpeed()
{
	return defaultSpeed; //TODO replace by speed
}
float AsservInsa::motion_GetDefaultAccel()
{
	return defaultAcc;
}
float AsservInsa::motion_GetDefaultDecel()
{
	return defaultDec;
}
void AsservInsa::motion_SetDefaultSpeed(float speed_m_sec)
{
	defaultSpeed = speed_m_sec;
}
void AsservInsa::motion_SetDefaultAccel(float accel)
{
	defaultAcc = accel;
}
void AsservInsa::motion_SetDefaultDecel(float decel)
{
	defaultDec = decel;
}

void AsservInsa::motion_configureAlphaPID(float p, float i, float d)
{
	pid_Config(motors[ALPHA_DELTA][ALPHA_MOTOR].PIDSys, p, i, d);
}
void AsservInsa::motion_configureDeltaPID(float p, float i, float d)
{
	pid_Config(motors[ALPHA_DELTA][DELTA_MOTOR].PIDSys, p, i, d);
}
void AsservInsa::motion_configureLeftPID(float p, float i, float d)
{
	pid_Config(motors[LEFT_RIGHT][LEFT_MOTOR].PIDSys, p, i, d);
}
void AsservInsa::motion_configureRightPID(float p, float i, float d)
{
	pid_Config(motors[LEFT_RIGHT][RIGHT_MOTOR].PIDSys, p, i, d);
}

void AsservInsa::motion_setMaxPwmValue(int maxPwmValue)
{
	maxPwmValue_ = maxPwmValue;
}

