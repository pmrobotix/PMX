/*******************************************************************************
* ClubElek Robot motion control software for Eurobot 2007
* Copyright (C) 2006-2007 ClubElek
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*******************************************************************************/
//! \file motion_calibration.c
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#include "motion_calibration.h"

#include "motion.h"
#include "motor.h"
#include "encoder.h"
#include "mouse_sensor.h"

#include "../log.h"

#include "../protocol.h"
#include "../Protocol/can_id.h"
#include "../Protocol/motion_protocol.h"

#include "../strategy/strategy.h"

#include "cpu_cfg.h"
#include "../global.h"
#include "../app_cfg.h"



//value of a vTops in meter (ratio for 'major wheel')
float valueVTops; 	// [meter/vTops]

//value of a sample in second
float valueSample; 	// [sec/sample]


//default values
static float defSpeed = DEFAULT_SPEED;	//robot speed
static float defAccel = DEFAULT_ACCEL; //robot acceleration
static float defDecel = DEFAULT_DECEL; //robot deceleration

//do we need to send the errors of both motors
BOOL isSendErrorsEnabled;

//what kind of sensor do we use for odometry
ODOMETRY_SENSOR odometrySensorUsed;

//how much slippage is tolerable before detecting a collision
int32 slippageThreshold;

//info on our motors
MOTOR motors[MAX_MOTION_CONTROL_TYPE_NUMBER][MOTOR_PER_TYPE];

static int calibStep;

//Message handlers
static void motion_wheelPIDTuning_handler(const CANMessage_uart *msg);
static void motion_alphaDeltaPIDTuning_handler(const CANMessage_uart *msg);
static void motion_odometryTuning_handler(const CANMessage_uart *msg);

static OS_EVENT *mtxDefaultSpeeds;

#define SLIPPAGE_RATIO			300000
#define CALIB_SPEED_NB			6

static float calibSpeed[CALIB_SPEED_NB] =
{ 0.1f, 0.2f, 0.3f, 0.4f, 0.50f, 0.60f };

static uint32 calibMouse[CALIB_SPEED_NB][2] =
{
	{86079,	84765},
	{85570,	84740},
	{83305,	83313},
	{84427,	84931},
	{85689,	84833},
	{81971,	82442}
};

/*
{86122,	84595},
{85699,	84570},
{85179,	85188},
{84976,	84506},
{85517,	85003},
{80742,	82030}
*/

void motion_CalibrationInit(void)
{
	REGISTER_LOG_CLIENT("MOTION");
	
	mtxDefaultSpeeds = OSMutexCreate(DEF_SPEED_MTX_PRIO, &OSLastError);
	LOG_TEST_OS_ERROR(OSLastError);
	
	encoder_SetDist(DEFAULT_ENCODER_DIST);
	encoder_SetResolution(DEFAULT_LEFT_TICKS_PER_M, DEFAULT_RIGHT_TICKS_PER_M);
		
	slippageThreshold = DEFAULT_SLIPPAGE;
	
	isSendErrorsEnabled = FALSE;
	
	odometrySensorUsed = MOUSE_SENSOR;
	
	pid_ConfigKP(motors[ALPHA_DELTA][ALPHA_MOTOR].PIDSys, 3000);
	pid_ConfigKI(motors[ALPHA_DELTA][ALPHA_MOTOR].PIDSys, 10);
	pid_ConfigKD(motors[ALPHA_DELTA][ALPHA_MOTOR].PIDSys, 10000);
	pid_ConfigDPeriod(motors[ALPHA_DELTA][ALPHA_MOTOR].PIDSys, 3);
	
	pid_ConfigKP(motors[ALPHA_DELTA][DELTA_MOTOR].PIDSys, 3000);
	pid_ConfigKI(motors[ALPHA_DELTA][DELTA_MOTOR].PIDSys, 10);
	pid_ConfigKD(motors[ALPHA_DELTA][DELTA_MOTOR].PIDSys, 5000);
	pid_ConfigDPeriod(motors[ALPHA_DELTA][DELTA_MOTOR].PIDSys, 3);
	
	
	pid_ConfigKP(motors[LEFT_RIGHT][LEFT_MOTOR].PIDSys, 2500);
	pid_ConfigKP(motors[LEFT_RIGHT][RIGHT_MOTOR].PIDSys, 2500);
	
	//register handlers
	protocol_SetHandler(M32_WHEEL_PID_TUNING, motion_wheelPIDTuning_handler);
	protocol_SetHandler(M32_ALPHADELTA_PID_TUNING, motion_alphaDeltaPIDTuning_handler);
	protocol_SetHandler(M32_ODOMETRY_TUNING, motion_odometryTuning_handler);
}


void motion_SetDefaultSpeed(float speed)
{
	int i;
	BOOL found = FALSE;
	
	OSMutexPend(mtxDefaultSpeeds, 0, &OSLastError);
	LOG_TEST_OS_ERROR(OSLastError);
	
	defSpeed = speed;

	//find appropriate speed and set mouse resolution
	for(i=0; i<CALIB_SPEED_NB; i++)
	{
		if(speed == calibSpeed[i])
		{
			encoder_SetResolution(calibMouse[i][0], calibMouse[i][1]);
			found = TRUE;
			break;
		}
	}
	
	motors_ConfigAllIMax(SLIPPAGE_RATIO * MAX(speed, 0.3f));
	
	if(!found)
	{		
		encoder_SetResolution(DEFAULT_LEFT_TICKS_PER_M, DEFAULT_RIGHT_TICKS_PER_M);
	}
	
	OSMutexPost(mtxDefaultSpeeds);
}

float motion_GetDefaultSpeed()
{
	float speed;
	OSMutexPend(mtxDefaultSpeeds, 0, &OSLastError);
	LOG_TEST_OS_ERROR(OSLastError);
	
	speed = defSpeed;
	
	OSMutexPost(mtxDefaultSpeeds);
	
	return speed;
}

void motion_SetDefaultAccel(float accel)
{
	OSMutexPend(mtxDefaultSpeeds, 0, &OSLastError);
	LOG_TEST_OS_ERROR(OSLastError);
	
	defAccel = accel;
	
	OSMutexPost(mtxDefaultSpeeds);
}

float motion_GetDefaultAccel()
{
	float accel;
	OSMutexPend(mtxDefaultSpeeds, 0, &OSLastError);
	LOG_TEST_OS_ERROR(OSLastError);
	
	accel = defAccel;
	
	OSMutexPost(mtxDefaultSpeeds);
	
	return accel;
}

void motion_SetDefaultDecel(float decel)
{
	OSMutexPend(mtxDefaultSpeeds, 0, &OSLastError);
	LOG_TEST_OS_ERROR(OSLastError);
	
	defDecel = decel;
	
	OSMutexPost(mtxDefaultSpeeds);
}

float motion_GetDefaultDecel()
{
	float decel;
	OSMutexPend(mtxDefaultSpeeds, 0, &OSLastError);
	LOG_TEST_OS_ERROR(OSLastError);
	
	decel = defDecel;
	
	OSMutexPost(mtxDefaultSpeeds);
	
	return decel;
}

// handler for MSG_WHEEL_PID_TUNING
static void motion_wheelPIDTuning_handler(const CANMessage_uart *msg)
{	
	switch(GET_APP_SPE_CODE(msg->title))
	{
	case PID_LEFT_KP:
		pid_ConfigKP(motors[LEFT_RIGHT][LEFT_MOTOR].PIDSys, msg->data.ulongs[0]);
		break;
	case PID_LEFT_KI:
		pid_ConfigKI(motors[LEFT_RIGHT][LEFT_MOTOR].PIDSys, msg->data.ulongs[0]);
		break;
	case PID_LEFT_KD:
		pid_ConfigKD(motors[LEFT_RIGHT][LEFT_MOTOR].PIDSys, msg->data.ulongs[0]);
		break;

	case PID_LEFT_PERIOD_D:
		pid_ConfigDPeriod(motors[LEFT_RIGHT][LEFT_MOTOR].PIDSys, msg->data.uchars[0]);
		break;
	case PID_LEFT_IMAX:
		pid_ConfigIMax(motors[LEFT_RIGHT][LEFT_MOTOR].PIDSys, msg->data.ulongs[0]);
		break;


	case PID_RIGHT_KP:
		pid_ConfigKP(motors[LEFT_RIGHT][RIGHT_MOTOR].PIDSys, msg->data.ulongs[0]);
		break;
	case PID_RIGHT_KI:
		pid_ConfigKI(motors[LEFT_RIGHT][RIGHT_MOTOR].PIDSys, msg->data.ulongs[0]);
		break;
	case PID_RIGHT_KD:
		pid_ConfigKD(motors[LEFT_RIGHT][RIGHT_MOTOR].PIDSys, msg->data.ulongs[0]);
		break;

	case PID_RIGHT_PERIOD_D:
		pid_ConfigDPeriod(motors[LEFT_RIGHT][RIGHT_MOTOR].PIDSys, msg->data.uchars[0]);
		break;
	case PID_RIGHT_IMAX:
		pid_ConfigIMax(motors[LEFT_RIGHT][RIGHT_MOTOR].PIDSys, msg->data.ulongs[0]);
		break;

	default:
		LOG_ERROR("Unknown msg");
		break;
	};
}

//handler for MSG_ALPHADELTA_PID_TUNING
static void motion_alphaDeltaPIDTuning_handler(const CANMessage_uart *msg)
{
	switch(GET_APP_SPE_CODE(msg->title))
	{
	case PID_ALPHA_KP:
		pid_ConfigKP(motors[ALPHA_DELTA][ALPHA_MOTOR].PIDSys, msg->data.ulongs[0]);
		break;	
	case PID_ALPHA_KI:
		pid_ConfigKI(motors[ALPHA_DELTA][ALPHA_MOTOR].PIDSys, msg->data.ulongs[0]);
		break;
	case PID_ALPHA_KD:
		pid_ConfigKD(motors[ALPHA_DELTA][ALPHA_MOTOR].PIDSys, msg->data.ulongs[0]);
		break;

	case PID_ALPHA_PERIOD_D:
		pid_ConfigDPeriod(motors[ALPHA_DELTA][ALPHA_MOTOR].PIDSys, msg->data.uchars[0]);
		break;
	case PID_ALPHA_IMAX:
		pid_ConfigIMax(motors[ALPHA_DELTA][ALPHA_MOTOR].PIDSys, msg->data.ulongs[0]);
		break;


	case PID_DELTA_KP:
		pid_ConfigKP(motors[ALPHA_DELTA][DELTA_MOTOR].PIDSys, msg->data.ulongs[0]);
		break;	
	case PID_DELTA_KI:
		pid_ConfigKI(motors[ALPHA_DELTA][DELTA_MOTOR].PIDSys, msg->data.ulongs[0]);
		break;
	case PID_DELTA_KD:
		pid_ConfigKD(motors[ALPHA_DELTA][DELTA_MOTOR].PIDSys, msg->data.ulongs[0]);
		break;

	case PID_DELTA_PERIOD_D:
		pid_ConfigDPeriod(motors[ALPHA_DELTA][DELTA_MOTOR].PIDSys, msg->data.uchars[0]);
		break;
	case PID_DELTA_IMAX:
		pid_ConfigIMax(motors[ALPHA_DELTA][DELTA_MOTOR].PIDSys, msg->data.ulongs[0]);
		break;

	default:
		LOG_ERROR("Unknown msg");
		break;
	};
}

void motion_SetSamplingFrequency(uint16 freq)
{
	valueSample = 1 / (float)freq;

	//need a recomputation of all values involving sampling time
	// kP/kI/kD/dPeriod
	// speed/acceleration

	// Set up Timer A0 Reload Register for x msec interval
	ta0 = CPU_CLK_FREQ/(freq*8);
}

//handler for MSG_ODOMETRY_TUNING
static void motion_odometryTuning_handler(const CANMessage_uart *msg)
{
	switch(GET_APP_SPE_CODE(msg->title))
	{
	case SET_DIST_BETWEEN_ENCODER:
		encoder_SetDist(msg->data.floats[0]);
		break;

	case SET_ENCODER_RESOLUTION:
		encoder_SetResolution(msg->data.ulongs[0], msg->data.ulongs[1]);
		break;

	case SET_SAMPLING_FREQUENCY:
		motion_SetSamplingFrequency(msg->data.ushorts[0]);
		break;
		
		
	case SET_SENSOR_MOTOR:
		odometrySensorUsed = MOTOR_ENCODER;
		break;
		
	case SET_SENSOR_MOUSE:
		odometrySensorUsed = MOUSE_SENSOR;
		break;
				
	case RUN_MOUSE_CALIBRATION:
		calibStep = 0;
		strat_setStrategy(STRAT_CALIB_MOUSE);
		break;
		
	case RUN_MOUSE_CALIBRATION2:
		calibStep = 0;
		strat_setStrategy(STRAT_CALIB_MOUSE2);
		break;
		
	case CALIB_NEXT_STEP: 
		mouseCalibSetStep(calibStep++);
		break;
		
	case SET_SLIPPAGE_THRESHOLD:
		slippageThreshold = msg->data.longs[0];
		break;
		
	case ENABLE_ERROR_SENDING:
		isSendErrorsEnabled = TRUE;
		break;
		
	case DISABLE_ERROR_SENDING:
		isSendErrorsEnabled = FALSE;
		break;
		
	default:
		LOG_ERROR("Unknown msg");
		break;
	};
}
