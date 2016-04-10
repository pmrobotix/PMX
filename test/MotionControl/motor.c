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
//! \file motor.c
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#include "motor.h"
#include "motor_PID.h"
#include "motion_calibration.h"

void initMotor(MOTOR *motor)
{
	int i;
	motor->PIDSys = pid_Create();
	motor->lastPos = 0;
	motor->posIndex = 0;
	
	for(i=0; i<MOTOR_SPEED_PERIOD_NB; i++)
	{
		motor->prevPos[i] = 0;
	}
}


void motors_ConfigAllIMax(int32 imax)
{
	int i,j;
	for(i=0; i<MAX_MOTION_CONTROL_TYPE_NUMBER; i++)
		for(j=0; j<MOTOR_PER_TYPE; j++)
			pid_ConfigIMax(motors[i][j].PIDSys, imax);
}

void updateMotor(MOTOR *motor, int32 delta)
{
	//update position
	motor->lastPos += delta;
	
	//store new value
	motor->prevPos[motor->posIndex] = motor->lastPos;
	
	//increment position table index
	motor->posIndex++;
	if(motor->posIndex >= MOTOR_SPEED_PERIOD_NB)
	{
		motor->posIndex = 0;
	}
}


int32 getMotorSpeed(MOTOR *motor)
{
	//motor->posIndex is always the index of the oldest position
	//so speed is the difference of current position and oldest
	int32 speed = (motor->lastPos - motor->prevPos[motor->posIndex]) / (MOTOR_SPEED_PERIOD_NB-1);
	CORRECT_DELTA_OVERFLOW(speed, MAX_ENCODER_ABS_VTOPS);
	
	return speed;
}
