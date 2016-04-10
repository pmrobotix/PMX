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
//! \file motor_speedOrder.h
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#ifndef _MOTOR_SPEED_ORDER_H_
#define _MOTOR_SPEED_ORDER_H_

#include "../types.h"

#include "motor.h"
#include "motion_priv.h"

//possible phase of a speed trajectory :
typedef enum
{
	SP_PRE_PHASE,			//trajectory not yet started
	SP_ACCEL_PHASE,			//acceleration (or deceleration) phase to reach goal speed
	SP_CONSTANT_PHASE,		//motor has reached final speed
	SP_END_PHASE,			//trajectory ended
} SPEED_PHASE;

//precomputed values defining the complete trajectory
typedef struct
{
	SPEED_PHASE phase;		//current phase

	int32 order0;			//motor position at trajectory start

	int32 period0;			//nb of period at the start of trajectory
	int32 period1;			//nb of period at end of acceleration (deceleration)
	int32 period2;			//nb of period at end of trajectory

	int32 Acceleration;		//acceleration used in movement in vTops/sample/sample
	
	int32 V0;				//start speed in vTops/sample
	int32 VGoal;			//goal speed in vTops/sample

	int32 lastOrder;		//current ordered position of motor

} SpeedCommand;


//! \brief Create a trajectory for one motor with a trapezoidal speed curve
//!
//! The trajectory start with a speed='V0' then accelerate with a rate of
//! 'Accel' until it reaches speed='VGoal', then when the total ran distance
//! is equal to distMax, the motor stops.
//!
//! \param out_command Result of the computation
//! \param V0 Speed at the start of the command
//! \param VGoal Speed to attain in vTops/sample
//! \param distMax Maximum distance to run in vTops
//! \param Accel Acceleration in vTops/sample^2
void ComputeSpeedCommand( SpeedCommand *out_command,
							int32 V0,
							int32 VGoal,
							int32 distMax,
							int32 Accel);

//! \brief Create a trajectory for one motor with a trapezoidal speed curve
//!
//! The trajectory start with a speed='V0' then accelerate with a rate of
//! 'Accel' during 'Time' periods
//!
//! \param out_command Result of the computation
//! \param V0 Speed at the start of the command
//! \param Time Number of period of the acceleration
//! \param Accel Acceleration in vTops/sample^2
void ComputeSpeedCommandTime( SpeedCommand *out_command,
								int32 V0,
								int32 Time,
								int32 Accel);
											
//! \brief Create a trajectory for one motor with a trapezoidal speed curve
//!
//! The trajectory start with a speed='V0' then accelerate with a rate of
//! 'Accel' but stop after 'Time' periods
//!
//! \param out_command Result of the computation
//! \param V0 Speed at the start of the command
//! \param Time Number of period of the acceleration
//! \param Accel Acceleration in vTops/sample^2
void ComputeSpeedCommandMaxTime( SpeedCommand *out_command,
									int32 V0,
									int32 VGoal,
									int32 Accel,
									int32 Time);
												
//! Configure the command for the current configuration of the motor.
//! You must execute this function before you launch the command.
//! \param out_cmd Result of the computation
//! \param lastPos Last position of motor
//! \param periodNb Current integration period number
void LoadSpeedCommand( SpeedCommand *out_cmd, int32 lastPos, int32 periodNb);


//! \brief Compute the current order to apply on motor
//!
//! \param motor Id number of motor
//! \param Tn Motion control period number
//! \param order Output of the computation
//! \return Is the path completed ?
EXPORTED_FUNCTION BOOL GetSpeedOrder(SpeedCommand *speedCommand, uint32 Tn, int32 *out_order);


#endif//_MOTOR_SPEED_ORDER_H_
