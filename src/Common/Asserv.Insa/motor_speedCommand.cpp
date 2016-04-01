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
//! \file motor_speedCommand.c
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
//
// Modified by PM-ROBOTIX in CPP
/******************************************************************************/

#include <cstdlib>

#include "AsservInsa.hpp"

void AsservInsa::ComputeSpeedCommand(SpeedCommand *out_cmd,
int32 V0,
int32 VGoal,
int32 distMax,
int32 Accel)
{
	int32 P1;
	int32 P2;

	out_cmd->period0 = 0;
	out_cmd->order0 = 0;
	out_cmd->lastOrder = 0;

	out_cmd->phase = SP_PRE_PHASE;

	if (Accel == 0)
	{
		out_cmd->period0 = 0;
		out_cmd->period1 = 0;
		out_cmd->period2 = 0;
		out_cmd->phase = SP_END_PHASE;
	}

	Accel = abs(Accel);
	distMax = abs(distMax);

	out_cmd->V0 = V0;
	out_cmd->VGoal = VGoal;

	//convert acceleration
	if (out_cmd->VGoal - V0 > 0)
	{
		out_cmd->Acceleration = Accel;
	}
	else
	{
		out_cmd->Acceleration = -Accel;
	}

	//convert max distance
	if (out_cmd->VGoal > 0)
	{
		P2 = +distMax;
	}
	else
	{
		P2 = -distMax;
	}

	//compute acceleration duration
	out_cmd->period1 = (out_cmd->VGoal - V0) / out_cmd->Acceleration;

	//compute constant phase duration
	P1 = out_cmd->Acceleration * out_cmd->period1 * out_cmd->period1 / 2 + out_cmd->V0 * out_cmd->period1;

	if (out_cmd->VGoal != 0)
		out_cmd->period2 = (P2 - P1) / out_cmd->VGoal + out_cmd->period1;
	else
		out_cmd->period2 = out_cmd->period1;
}

void AsservInsa::ComputeSpeedCommandTime(SpeedCommand *out_cmd,
int32 V0,
int32 Time,
int32 Accel)
{
	out_cmd->order0 = 0;
	out_cmd->lastOrder = 0;

	out_cmd->phase = SP_PRE_PHASE;

	out_cmd->period0 = 0;
	out_cmd->period1 = Time;
	out_cmd->period2 = Time;

	out_cmd->Acceleration = Accel;
	out_cmd->V0 = V0;
	out_cmd->VGoal = V0 + Time * Accel;
}

void AsservInsa::ComputeSpeedCommandMaxTime(SpeedCommand *out_cmd,
int32 V0,
int32 VGoal,
int32 Accel,
int32 Time)
{
	out_cmd->period0 = 0;
	out_cmd->order0 = 0;
	out_cmd->lastOrder = 0;

	out_cmd->phase = SP_PRE_PHASE;

	if (Accel == 0)
	{
		out_cmd->period0 = 0;
		out_cmd->period1 = 0;
		out_cmd->period2 = 0;
		out_cmd->phase = SP_END_PHASE;
	}

	Accel = abs(Accel);
	out_cmd->V0 = V0;
	out_cmd->VGoal = VGoal;

	//convert acceleration
	if (out_cmd->VGoal - V0 > 0)
	{
		out_cmd->Acceleration = Accel;
	}
	else
	{
		out_cmd->Acceleration = -Accel;
	}

	//compute acceleration duration
	out_cmd->period1 = (out_cmd->VGoal - V0) / out_cmd->Acceleration;
	out_cmd->period2 = Time;

	//can we reach VGoal during max time
	if (out_cmd->period1 < Time)
	{
		//all is fine
	}
	else
	{
		out_cmd->period1 = Time;

		//compute speed at the end of traj
		out_cmd->VGoal = Time * out_cmd->Acceleration + V0;
	}
}

void AsservInsa::LoadSpeedCommand(SpeedCommand *out_cmd, int32 lastPos, int32 periodNb)
{
	//movement begin at next period
	out_cmd->period0 = periodNb;
	out_cmd->period1 += periodNb;
	out_cmd->period2 += periodNb;

	out_cmd->order0 = lastPos;
	out_cmd->lastOrder = lastPos;
}

bool AsservInsa::GetSpeedOrder(SpeedCommand *speedCmd, int32 PeriodN,
int32 *out_order)
{
	BOOL finished = FALSE;

	switch (speedCmd->phase)
	{
	case SP_PRE_PHASE:
		if (PeriodN >= speedCmd->period0)
		{
			speedCmd->phase = SP_ACCEL_PHASE;
		}
		else
		{
			*out_order = speedCmd->lastOrder + speedCmd->V0;
			break;
		}
		//acceleration phase
	case SP_ACCEL_PHASE:
		if (PeriodN >= speedCmd->period1)
		{
			speedCmd->phase = SP_CONSTANT_PHASE;
		}
		else
		{
			int32 Tn = PeriodN - speedCmd->period0;

			//For a fixed integration period, DeltaT = 1 period
			//units for acceleration and speed are in ticks/sample� and ticks/sample
			//so we can simplify the formula :
			//Acceleration*DeltaT*(Tx+DeltaT/2.0f) +V0*DeltaT + codo0;
			*out_order = speedCmd->Acceleration * Tn + speedCmd->Acceleration / 2 + speedCmd->V0 + speedCmd->lastOrder;
			break;
		}

		//constant speed phase
	case SP_CONSTANT_PHASE:
		if (PeriodN >= speedCmd->period2)
		{
			speedCmd->phase = SP_END_PHASE;
		}
		else
		{
			//Same computation than first phase with acceleration = 0
			*out_order = speedCmd->VGoal + speedCmd->lastOrder;
			break;
		}

	case SP_END_PHASE:
		//We continue at the same speed
		*out_order = speedCmd->VGoal + speedCmd->lastOrder;
		finished = TRUE;
		break;
	};

	speedCmd->lastOrder = *out_order;

	return finished;
}
