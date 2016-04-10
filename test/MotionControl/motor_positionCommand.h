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
//! \file motor_positionOrder.h
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#ifndef _MOTOR_POSITION_ORDER_H_
#define _MOTOR_POSITION_ORDER_H_

#include "../types.h"

#include "motor.h"
#include "motion_priv.h"

//possible phase of a position trajectory :
typedef enum
{
	TR_PRE_PHASE,			//trajectory not yet started
	TR_ACCEL_PHASE,			//0 - acceleration phase to reach goal speed
	TR_CONSTANT_PHASE,		//1 - motor has reached final speed
	TR_DECEL_PHASE,			//2 - deceleration phase before reaching goal position
	TR_END_PHASE,			//3 - motor has reached desired position
} TRAJ_PHASE;

//precomputed values defining the complete trajectory
typedef struct
{
	TRAJ_PHASE phase;		//current phase
	
	int32 period0;     		//period number of the start of trajectory
	int32 order0;			//motor position at trajectory start

	int32 VMax;				//goal speed in vTops/sample
	int32 order3;			//goal position

	int32 T01;				//number of sample between accel and constant phase
	int32 T12;				//number of sample between contant and decel phase
	int32 T23;				//number of sample between decel and end phase

	int32 Acceleration;		//acceleration used in movement in vTops/sample/sample
	int32 Deceleration;		//deceleration used in movement in vTops/sample/sample

	int32 lastOrder;		//current ordered position of motor
} PositionCommand;


//! \brief Create a trajectory for one motor with a trapezoidal speed curve
//!
//! The trajectory start with a speed=0 then accelerate at a rate equal to
//! 'Accel' until it reaches speed='VMax' before the end of the path
//! the wheel decelerate with a rate of 'Decel' then stop.
//!
//! \param motor Id number of motor on wich to apply the trajectory
//! \param out_command Result of the computation
//! \param dist Distance to run in meters
//! \param VMax Maximum speed at the wheel in meters/second
//! \param Accel Acceleration in meters/second^2
//! \param Decel Deceleration in meters/second^2
EXPORTED_FUNCTION void ComputePositionCommand (	PositionCommand *out_command,
												int32 dist,
												int32 VMax,
												int32 Accel,
												int32 Decel);

//! Configure the command for the current configuration of the motor.
//! You must execute this function before you launch the command.
//! \param out_command Result of the computation
//! \param motor Id number of motor on wich to apply the trajectory
void LoadPositionCommand( PositionCommand *out_command, MOTOR *motor, int32 periodNb);

//! \brief Compute the current order to apply on motor
//!
//! \param motor Id number of motor
//! \param posCommand The command that will be interpreted
//! \param Tn Motion control period number
//! \param out_Order Result of the computation
//! \return Is the path completed ?
EXPORTED_FUNCTION BOOL GetPositionOrder(PositionCommand *posCommand,
										uint32 Tn,
										int32 *out_Order);

#endif//_MOTOR_POSITION_ORDER_H_
