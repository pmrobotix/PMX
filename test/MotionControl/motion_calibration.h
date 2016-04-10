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
//! \file motion_calibration.h
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#ifndef _MOTION_CALIBRATION_H_
#define _MOTION_CALIBRATION_H_

#include "../types.h"

#include "motor.h"
#include "motor_PID.h"
#include "motion_priv.h"

//! max ratio between vTops and ticks (it corresponds to the ratio of the most accurate encoder)
#define VTOPS_PER_TICKS					0x0800

#define MAX_ENCODER_ABS_VTOPS			(VTOPS_PER_TICKS*0x000FFFFF)

#define DEFAULT_SAMPLING_FREQ			300	//Hz

#define DEFAULT_ENCODER_DIST			0.2356f //meter
#define DEFAULT_LEFT_TICKS_PER_M		86000
#define DEFAULT_RIGHT_TICKS_PER_M		85900

#define DEFAULT_SPEED					0.2f
#define DEFAULT_ACCEL					0.3f
#define DEFAULT_DECEL					0.3f

#define DEFAULT_SLIPPAGE				600000


//some values goes in pair :
//the first one is the measured/real one
//the second one is an integer approximation using funny units (vTops and sample)

//! value of a vTops in meter (ratio for 'major wheel')
extern float valueVTops; //	[meter/vTops]

//! value of a sample in second
extern float valueSample; // [sec/sample]

//! distance between both encoder wheels in vTops
extern uint32 distEncoder;
//! distance between both encoder wheels in meters
extern float distEncoderMeter;

typedef enum
{
	MOTOR_ENCODER,
	MOUSE_SENSOR,
	MOUSE_CALIBRATION
}ODOMETRY_SENSOR;

//! what kind of sensor do we use for odometry
extern ODOMETRY_SENSOR odometrySensorUsed;

//! should we send error on the UART (for debug and calibration purpose)
extern BOOL isSendErrorsEnabled;

//! motors used for motion (left / right and virtual alpha / delta)
extern MOTOR motors[MAX_MOTION_CONTROL_TYPE_NUMBER][MOTOR_PER_TYPE];

//! how much slippage should trigger an alarm
extern int32 slippageThreshold;

//! Initialization of the motion calibration module
void motion_CalibrationInit(void);

//! Set motion control sampling frequency and recompute value of sample time
void motion_SetSamplingFrequency(uint16 freq);

//! speed used for all robot motion by default 
void motion_SetDefaultSpeed(float speed);
float motion_GetDefaultSpeed(void);

//! acceleration used for all robot motion by default
void motion_SetDefaultAccel(float accel);
float motion_GetDefaultAccel(void);

//! deceleration used for all robot motion by default
void motion_SetDefaultDecel(float decel);
float motion_GetDefaultDecel(void);


#endif//_MOTION_CALIBRATION_H_
