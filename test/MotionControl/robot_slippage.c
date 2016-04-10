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
//! \file robot_slippage.c
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#include "robot_slippage.h"
#include "motor.h"
#include "motion_calibration.h"
#include "../log.h"
#include "../collision.h"

#define MAX_PERIOD		128

static int32 slippage[MOTOR_PER_TYPE];				//current slippage
static int32 values[MOTOR_PER_TYPE][MAX_PERIOD];	//all the previous values of slippage
static int32 index[MOTOR_PER_TYPE];					//index in the values table

static void resetSlippage(int motor);

void slippage_Init()
{
	int i;
	
	for(i=0; i<MOTOR_PER_TYPE; i++)
		resetSlippage(i);
		
	REGISTER_LOG_CLIENT("SLIPPAGE");
}

static void resetSlippage(int motor)
{
	int i;
	for(i=0; i<MAX_PERIOD; i++)
		values[motor][i] = 0;
		
	slippage[motor] = 0;
	index[motor] = 0;
}

void slippage_Update(int motor, int32 dWheel, int32 dRobot)
{
	//substract oldest slippage value
	slippage[motor] -= values[motor][index[motor]];

	//compute new slippage value
	if(dWheel < 0)
		values[motor][index[motor]] = dRobot - dWheel;
	else
		values[motor][index[motor]] = dWheel - dRobot;

	slippage[motor] += values[motor][index[motor]];
	
	//update index value
	index[motor]++;
	if(index[motor] >= MAX_PERIOD)
	{
		index[motor] = 0; //restart at the beginning of the buffer
	}
	
	//test if we are above threshold
	if(slippage[motor] > slippageThreshold)
	{
		if(dWheel > 0)
		{
			LOG_DEBUG3("[%d] Front Collision %ld/%ld", motor, slippage[motor], slippageThreshold);
			collision_SignalShock(FRONT_SHOCK);
		}
		else
		{
			LOG_DEBUG3("[%d] Back Collision %ld/%ld", motor, slippage[motor], slippageThreshold);
			collision_SignalShock(BACK_SHOCK);
		}
		
		resetSlippage(motor);
	}
}
