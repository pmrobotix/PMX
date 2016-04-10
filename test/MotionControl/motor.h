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
//! \file motor.h
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "motion_priv.h"

//LEFT - RIGHT
#define LEFT_MOTOR		0
#define RIGHT_MOTOR		1

//ALPHA - DELTA (virtual motors)
#define ALPHA_MOTOR		0
#define DELTA_MOTOR		1

//Two motors per set
#define MOTOR_PER_TYPE	2

//Number of period used to compute motor speed
#define MOTOR_SPEED_PERIOD_NB		9

//! structure used internally for stocking current value for each motors
typedef struct
{
  PID_SYSTEM PIDSys;
  int32 lastPos;
  
  int8 posIndex;
  int32 prevPos[MOTOR_SPEED_PERIOD_NB];	//previous value of position for speed computation
} MOTOR;


//! initialisation of the motor structure
void initMotor(MOTOR *motor);

//config iMax for all motors
void motors_ConfigAllIMax(int32 imax);

//! update motor position and store value for speed computation
void updateMotor(MOTOR *motor, int32 delta);

//! Compute current motor speed
int32 getMotorSpeed(MOTOR *motor);


#endif//_MOTOR_H_
