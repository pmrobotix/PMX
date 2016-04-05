#include <sys/types.h>
#include <cmath>

#include "../../Log/Logger.hpp"
#include "AsservInsa.hpp"

void AsservInsa::encoder_SetResolution(uint32 leftTicksPerM, uint32 rightTicksPerM, float entraxe_mm)
{
	leftTicksPerM_ = leftTicksPerM;
	rightTicksPerM_ = rightTicksPerM;

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

	//recompute values involving vTops
	encoder_SetDist(entraxe_mm / 1000.0);

	logger().debug() << "encoder_SetResolution  leftTicksPerM="
			<< leftTicksPerM
			<< " rightTicksPerM="
			<< rightTicksPerM
			<< " leftEncoderRatio="
			<< leftEncoderRatio
			<< " rightEncoderRatio="
			<< rightEncoderRatio

			<< " valueVTops="
			<< valueVTops

			<< logs::end;
}

uint32 AsservInsa::encoder_GetLeftResolution()
{
	return leftTicksPerM_;
}

uint32 AsservInsa::encoder_GetRightResolution()
{
	return rightTicksPerM_;
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

void AsservInsa::odo_SetPosition(float x_m, float y_m, float theta_rad)
{
	//printf("odo_SetPosition %f,%f %f\n", x, y, theta);
	//convert position from meters to vtops
	xTops = x_m / valueVTops;
	yTops = y_m / valueVTops;
	Theta = theta_rad;
	cosTheta = cosf(Theta);
	sinTheta = sinf(Theta);
}

RobotPosition AsservInsa::odo_GetPosition()
{
	RobotPosition pos;

	//convert position from ticks to meter
	pos.x = xTops * (float) valueVTops;
	pos.y = yTops * (float) valueVTops;
	pos.theta = Theta;

	return pos;
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
float AsservInsa::motion_GetDefaultVmax()
{
	return defaultVmax;
}
float AsservInsa::motion_GetDefaultAccel()
{
	return defaultAcc;
}
float AsservInsa::motion_GetDefaultDecel()
{
	return defaultDec;
}
void AsservInsa::motion_SetDefaultVmax(float speed_m_sec)
{
	defaultVmax = speed_m_sec;
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

