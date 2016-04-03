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
//! \file robot_trajectory.c
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
//
// Modified by PM-ROBOTIX in CPP
/******************************************************************************/

#include <cmath>
#include <cstdio>

#include "AsservInsa.hpp"

void AsservInsa::motion_Line(RobotCommand *out_cmd, float dist)
{
	motion_LineSpeedAcc(out_cmd, dist, motion_GetDefaultVmax(), motion_GetDefaultAccel(), motion_GetDefaultDecel());
}

void AsservInsa::motion_LineSpeed(RobotCommand *out_cmd, float dist, float VMax)
{
	motion_LineSpeedAcc(out_cmd, dist, VMax, motion_GetDefaultAccel(), motion_GetDefaultDecel());
}

void AsservInsa::motion_LineSpeedAcc(RobotCommand *out_cmd, float dist, float VMax, float Accel, float Decel)
{
	out_cmd->cmdType = POSITION_COMMAND;
	out_cmd->mcType = ALPHA_DELTA;

	//ALPHA
	ComputePositionCommand(&(out_cmd->cmd.posCmd[ALPHA_MOTOR]), 0, 0, 0, 0); //no move on alpha

	//DELTA
	ComputePositionCommand(&(out_cmd->cmd.posCmd[DELTA_MOTOR]),
			convertDistTovTops(dist),
			convertSpeedTovTopsPerPeriod(VMax),
			convertAccelTovTopsPerPeriodSqd(Accel),
			convertAccelTovTopsPerPeriodSqd(Decel));
}

void AsservInsa::motion_Rotate(RobotCommand *out_cmd, float angle)
{
	motion_RotateSpeedAcc(out_cmd, angle, motion_GetDefaultVmax(), motion_GetDefaultAccel(), motion_GetDefaultDecel());
}

void AsservInsa::motion_RotateSpeed(RobotCommand *out_cmd, float angle, float VMax)
{
	motion_RotateSpeedAcc(out_cmd, angle, VMax, motion_GetDefaultAccel(), motion_GetDefaultDecel());
}

void AsservInsa::motion_RotateSpeedAcc(RobotCommand *out_cmd, float angle, float VMax, float Accel, float Decel)
{
	out_cmd->cmdType = POSITION_COMMAND;
	out_cmd->mcType = ALPHA_DELTA;

	//ALPHA
	ComputePositionCommand(&(out_cmd->cmd.posCmd[ALPHA_MOTOR]),
			convertDistTovTops(angle * distEncoderMeter / 2.0f),
			convertSpeedTovTopsPerPeriod(VMax),
			convertAccelTovTopsPerPeriodSqd(Accel),
			convertAccelTovTopsPerPeriodSqd(Decel));

	//DELTA
	ComputePositionCommand(&(out_cmd->cmd.posCmd[DELTA_MOTOR]), 0, 0, 0, 0); //no move on delta
}

void AsservInsa::motion_ArcRotate(RobotCommand *out_cmd, float angle, float radius)
{
	motion_ArcRotateSpeedAcc(out_cmd,
			angle,
			radius,
			motion_GetDefaultVmax(),
			motion_GetDefaultAccel(),
			motion_GetDefaultDecel());
}

void AsservInsa::motion_ArcRotateSpeed(RobotCommand *out_cmd, float angle, float radius, float VMax)
{
	motion_ArcRotateSpeedAcc(out_cmd, angle, radius, VMax, motion_GetDefaultAccel(), motion_GetDefaultDecel());
}

void AsservInsa::motion_ArcRotateSpeedAcc(RobotCommand *out_cmd,
		float angle,
		float radius,
		float VMax,
		float Accel,
		float Decel)
{
	float dL, dR;
	float ratio;

	out_cmd->cmdType = POSITION_COMMAND;
	out_cmd->mcType = LEFT_RIGHT;

	dL = angle * (radius - distEncoderMeter / 2.0f);
	dR = angle * (radius + distEncoderMeter / 2.0f);

	if (fabs(dL) > fabs(dR))
	{
		ratio = dR / dL;

		//LEFT
		ComputePositionCommand(&(out_cmd->cmd.posCmd[LEFT_MOTOR]),
				convertDistTovTops(dL),
				convertSpeedTovTopsPerPeriod(VMax),
				convertAccelTovTopsPerPeriodSqd(Accel),
				convertAccelTovTopsPerPeriodSqd(Decel));

		//RIGHT
		ComputePositionCommand(&(out_cmd->cmd.posCmd[RIGHT_MOTOR]),
				convertDistTovTops(dR),
				convertSpeedTovTopsPerPeriod(VMax * ratio),
				convertAccelTovTopsPerPeriodSqd(Accel * ratio),
				convertAccelTovTopsPerPeriodSqd(Decel * ratio));
	}
	else
	{
		ratio = dL / dR;

		//LEFT
		ComputePositionCommand(&(out_cmd->cmd.posCmd[LEFT_MOTOR]),
				convertDistTovTops(dL),
				convertSpeedTovTopsPerPeriod(VMax * ratio),
				convertAccelTovTopsPerPeriodSqd(Accel * ratio),
				convertAccelTovTopsPerPeriodSqd(Decel * ratio));

		//RIGHT
		ComputePositionCommand(&(out_cmd->cmd.posCmd[RIGHT_MOTOR]),
				convertDistTovTops(dR),
				convertSpeedTovTopsPerPeriod(VMax),
				convertAccelTovTopsPerPeriodSqd(Accel),
				convertAccelTovTopsPerPeriodSqd(Decel));
	}
}

void AsservInsa::motion_SpeedControlLR(RobotCommand *out_cmd,
		float spLeft,
		float distLeft,
		float accLeft,
		float spRight,
		float distRight,
		float accRight)
{
	out_cmd->cmdType = SPEED_COMMAND;
	out_cmd->mcType = LEFT_RIGHT;

	//LEFT
	ComputeSpeedCommand(&(out_cmd->cmd.speedCmd[LEFT_MOTOR]),
			getMotorSpeed(&motors[LEFT_RIGHT][LEFT_MOTOR]),
			convertSpeedTovTopsPerPeriod(spLeft),
			convertDistTovTops(distLeft),
			convertAccelTovTopsPerPeriodSqd(accLeft));

	//RIGHT
	ComputeSpeedCommand(&(out_cmd->cmd.speedCmd[RIGHT_MOTOR]),
			getMotorSpeed(&motors[LEFT_RIGHT][RIGHT_MOTOR]),
			convertSpeedTovTopsPerPeriod(spRight),
			convertDistTovTops(distRight),
			convertAccelTovTopsPerPeriodSqd(accRight));
}

void AsservInsa::motion_SpeedControlADMaxTime(RobotCommand *out_cmd,
		float spAlpha,
		float accAlpha,
		float spDelta,
		float accDelta,
		int duringMs)
{
	int32 time;

	out_cmd->cmdType = SPEED_COMMAND;
	out_cmd->mcType = ALPHA_DELTA;

	//time = (DEFAULT_SAMPLING_FREQ * duringMs) / 1000;
	time = (defaultSamplingFreq * duringMs) / 1000;

	ComputeSpeedCommandMaxTime(&(out_cmd->cmd.speedCmd[ALPHA_MOTOR]),
			getMotorSpeed(&motors[ALPHA_DELTA][ALPHA_MOTOR]),
			convertSpeedTovTopsPerPeriod(spAlpha),
			convertAccelTovTopsPerPeriodSqd(accAlpha),
			time);

	ComputeSpeedCommandMaxTime(&(out_cmd->cmd.speedCmd[DELTA_MOTOR]),
			getMotorSpeed(&motors[ALPHA_DELTA][DELTA_MOTOR]),
			convertSpeedTovTopsPerPeriod(spDelta),
			convertAccelTovTopsPerPeriodSqd(accDelta),
			time);
}

void AsservInsa::motion_SpeedControlLRDecel(RobotCommand *out_cmd,
		float spLeft,
		float distLeft,
		float accLeft,
		float decLeft,
		float spRight,
		float distRight,
		float accRight,
		float decRight)
{
	int32 motorSpeed;
	int32 goalSpeed;

	out_cmd->cmdType = SPEED_COMMAND;
	out_cmd->mcType = LEFT_RIGHT;

	motorSpeed = getMotorSpeed(&motors[LEFT_RIGHT][LEFT_MOTOR]);
	goalSpeed = convertSpeedTovTopsPerPeriod(spLeft);

	if (((motorSpeed < goalSpeed) && motorSpeed > 0) || ((motorSpeed > goalSpeed) && motorSpeed < 0))
	{
		//LEFT
		ComputeSpeedCommand(&(out_cmd->cmd.speedCmd[LEFT_MOTOR]),
				motorSpeed,
				goalSpeed,
				convertDistTovTops(distLeft),
				convertAccelTovTopsPerPeriodSqd(accLeft));
	}
	else
	{
		//LEFT
		ComputeSpeedCommand(&(out_cmd->cmd.speedCmd[LEFT_MOTOR]),
				motorSpeed,
				goalSpeed,
				convertDistTovTops(distLeft),
				convertAccelTovTopsPerPeriodSqd(decLeft));
	}

	motorSpeed = getMotorSpeed(&motors[LEFT_RIGHT][RIGHT_MOTOR]);
	goalSpeed = convertSpeedTovTopsPerPeriod(spRight);

	if (((motorSpeed < goalSpeed) && motorSpeed > 0) || ((motorSpeed > goalSpeed) && motorSpeed < 0))
	{
		//RIGHT
		ComputeSpeedCommand(&(out_cmd->cmd.speedCmd[RIGHT_MOTOR]),
				motorSpeed,
				goalSpeed,
				convertDistTovTops(distRight),
				convertAccelTovTopsPerPeriodSqd(accRight));
	}
	else
	{
		//RIGHT
		ComputeSpeedCommand(&(out_cmd->cmd.speedCmd[RIGHT_MOTOR]),
				motorSpeed,
				goalSpeed,
				convertDistTovTops(distRight),
				convertAccelTovTopsPerPeriodSqd(decRight));
	}
}

void AsservInsa::motion_SpeedControlLRV0(RobotCommand *out_cmd,
		float V0Left,
		float spLeft,
		float distLeft,
		float accLeft,
		float V0Right,
		float spRight,
		float distRight,
		float accRight)
{
	out_cmd->cmdType = SPEED_COMMAND;
	out_cmd->mcType = LEFT_RIGHT;

	//LEFT
	ComputeSpeedCommand(&(out_cmd->cmd.speedCmd[LEFT_MOTOR]),
			convertSpeedTovTopsPerPeriod(V0Left),
			convertSpeedTovTopsPerPeriod(spLeft),
			convertDistTovTops(distLeft),
			convertAccelTovTopsPerPeriodSqd(accLeft));

	//RIGHT
	ComputeSpeedCommand(&(out_cmd->cmd.speedCmd[RIGHT_MOTOR]),
			convertSpeedTovTopsPerPeriod(V0Right),
			convertSpeedTovTopsPerPeriod(spRight),
			convertDistTovTops(distRight),
			convertAccelTovTopsPerPeriodSqd(accRight));
}

void AsservInsa::motion_SpeedControlLRTime(RobotCommand *out_cmd,
		float V0Left,
		float TLeft,
		float accLeft,
		float V0Right,
		float TRight,
		float accRight)
{
	out_cmd->cmdType = SPEED_COMMAND;
	out_cmd->mcType = LEFT_RIGHT;

	//LEFT
	ComputeSpeedCommandTime(&(out_cmd->cmd.speedCmd[LEFT_MOTOR]),
			convertSpeedTovTopsPerPeriod(V0Left),
			(int32) (TLeft / (loopDelayInMillis/1000.0)),
			convertAccelTovTopsPerPeriodSqd(accLeft));

	//RIGHT
	ComputeSpeedCommandTime(&(out_cmd->cmd.speedCmd[RIGHT_MOTOR]),
			convertSpeedTovTopsPerPeriod(V0Right),
			(int32) (TRight / (loopDelayInMillis/1000.0)),
			convertAccelTovTopsPerPeriodSqd(accRight));
}

void AsservInsa::motion_SpeedControlAD(RobotCommand *out_cmd,
		float spAlpha,
		float distAlpha,
		float accAlpha,
		float spDelta,
		float distDelta,
		float accDelta)
{
	out_cmd->cmdType = SPEED_COMMAND;
	out_cmd->mcType = ALPHA_DELTA;

	//ALPHA
	ComputeSpeedCommand(&(out_cmd->cmd.speedCmd[ALPHA_MOTOR]),
			getMotorSpeed(&motors[ALPHA_DELTA][ALPHA_MOTOR]),
			convertSpeedTovTopsPerPeriod(spAlpha),
			convertDistTovTops(distAlpha),
			convertAccelTovTopsPerPeriodSqd(accAlpha));

	//DELTA
	ComputeSpeedCommand(&(out_cmd->cmd.speedCmd[DELTA_MOTOR]),
			getMotorSpeed(&motors[ALPHA_DELTA][DELTA_MOTOR]),
			convertSpeedTovTopsPerPeriod(spDelta),
			convertDistTovTops(distDelta),
			convertAccelTovTopsPerPeriodSqd(accDelta));
}

void AsservInsa::motion_StepOrderLR(RobotCommand *out_cmd, int32 posLeft, int32 posRight, int seconds)
{
	out_cmd->cmdType = STEP_COMMAND;
	out_cmd->mcType = LEFT_RIGHT;

	//LEFT
	ComputeStepOrder(&(out_cmd->cmd.stepCmd[LEFT_MOTOR]), posLeft, seconds);

	//RIGHT
	ComputeStepOrder(&(out_cmd->cmd.stepCmd[RIGHT_MOTOR]), posRight, seconds);
}

void AsservInsa::motion_StepOrderAD(RobotCommand *out_cmd, int32 posAlpha, int32 posDelta, int seconds)
{
	out_cmd->cmdType = STEP_COMMAND;
	out_cmd->mcType = ALPHA_DELTA;
	printf("1: %d\n", out_cmd->cmdType);
	//ALPHA
	ComputeStepOrder(&(out_cmd->cmd.stepCmd[ALPHA_MOTOR]), posAlpha, seconds);
	printf("2: %d\n", out_cmd->cmdType);
	//DELTA
	ComputeStepOrder(&(out_cmd->cmd.stepCmd[DELTA_MOTOR]), posDelta, seconds);
	printf("3: %d\n", out_cmd->cmdType);
}

