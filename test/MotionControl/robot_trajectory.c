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
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#include "robot_trajectory.h"

#include "../protocol.h"
#include "../Protocol/traj_protocol.h"
#include "../Protocol/can_id.h"

#include "motion.h"
#include "motion_calibration.h"

#include "robot_unitConversion.h"

#include "clothoid.h"
#include "path_manager.h"

#include "../log.h"

typedef struct
{
	float V0;
	float A0;
	float D1;
	float D2;
	float Beta;
	float Epsilon;
} ClothoidParam;

static ClothoidParam clotho;

static RobotCommand nextCommand;

static void motion_commandTraj_handler(const CANMessage_uart *msg);

void traj_Init()
{
	REGISTER_LOG_CLIENT("MOTION");
	protocol_SetHandler(M32_COMMAND_TRAJ, motion_commandTraj_handler);
}

static void motion_commandTraj_handler(const CANMessage_uart *msg)
{
	switch(GET_APP_SPE_CODE(msg->title))
	{
	case TRAJ_LR_STEP:
		motion_StepOrderLR(	&nextCommand,
							(msg->data.longs[0]*VTOPS_PER_TICKS)+motors[LEFT_RIGHT][LEFT_MOTOR].lastPos,
							(msg->data.longs[1]*VTOPS_PER_TICKS)+motors[LEFT_RIGHT][RIGHT_MOTOR].lastPos);
							
		path_LaunchTrajectory(&nextCommand);
		break;
		
	case TRAJ_AD_STEP:
		motion_StepOrderAD(	&nextCommand,
							(msg->data.longs[0]*VTOPS_PER_TICKS)+motors[ALPHA_DELTA][ALPHA_MOTOR].lastPos,
							(msg->data.longs[1]*VTOPS_PER_TICKS)+motors[ALPHA_DELTA][DELTA_MOTOR].lastPos);
		
		path_LaunchTrajectory(&nextCommand);
		break;
		
	case TRAJ_SETPWM:
		motion_DisablePID();
		setPWM(msg->data.shorts[0], msg->data.shorts[1]);
		break;
	
	case TRAJ_FREEMOTION:
		motion_FreeMotion();
		path_CancelTrajectory();
		break;
		
	case TRAJ_ASSISTEDHANDLING:
		motion_AssistedHandling();
		break;
		
	case TRAJ_SETVMAX:
		motion_SetDefaultSpeed(msg->data.floats[0]);
		break;
		
	case TRAJ_SETACCEL:
		motion_SetDefaultAccel(msg->data.floats[0]);
		break;
		
	case TRAJ_SETDECEL:
		motion_SetDefaultDecel(msg->data.floats[0]);
		break;
		
	case TRAJ_LINE:
		motion_Line(&nextCommand, msg->data.floats[0]);
		path_LaunchTrajectory(&nextCommand);
		break;
		
	case TRAJ_ROTATION:
		motion_Rotate(&nextCommand, msg->data.floats[0]);
		path_LaunchTrajectory(&nextCommand);
		break;
		
	case TRAJ_ARCCIRCLE:
		motion_ArcRotate(&nextCommand, msg->data.floats[0], msg->data.floats[1]);
		path_LaunchTrajectory(&nextCommand);
		break;

	case TRAJ_SPEED_LR:
		motion_SpeedControlLR(&nextCommand, msg->data.floats[0], 1.0f, 0.2f, msg->data.floats[1], 1.0f, 0.2f);
		path_LaunchTrajectory(&nextCommand);
		break;
		
	case TRAJ_SPEED_AD:
		motion_SpeedControlAD(&nextCommand, msg->data.floats[0], 1.0f, 0.2f, msg->data.floats[1], 1.0f, 0.2f);
		path_LaunchTrajectory(&nextCommand);
		break;


/*
**		CLOTHOIDS
*/
	case CLOTHO_V0A0:
		clotho.V0 = msg->data.floats[0];
		clotho.A0 = msg->data.floats[1];
		break;

	case CLOTHO_D1D2:
		clotho.D1 = msg->data.floats[0];
		clotho.D2 = msg->data.floats[1];
		break;

	case CLOTHO_BETA_EPS:
		clotho.Beta = msg->data.floats[0];
		clotho.Epsilon = msg->data.floats[1];
		break;

	case CLOTHO_RUN:
		CreateTwoSegmentTraj(clotho.V0, clotho.D1, clotho.D2, clotho.A0, clotho.Beta, clotho.Epsilon);
		break;

	default:
		LOG_ERROR("Unknown msg");
		break;
	};
	
}


void motion_Line(RobotCommand *out_cmd, float dist)
{
	motion_LineSpeedAcc(out_cmd, dist, motion_GetDefaultSpeed(), motion_GetDefaultAccel(), motion_GetDefaultDecel());
}

void motion_LineSpeed(RobotCommand *out_cmd, float dist, float VMax)
{
	motion_LineSpeedAcc(out_cmd, dist, VMax, motion_GetDefaultAccel(), motion_GetDefaultDecel());
}

void motion_LineSpeedAcc(RobotCommand *out_cmd, float dist, float VMax, float Accel, float Decel)
{
	out_cmd->cmdType = POSITION_COMMAND;
	out_cmd->mcType = ALPHA_DELTA;
	
	//ALPHA
	ComputePositionCommand(	&(out_cmd->cmd.posCmd[ALPHA_MOTOR]),
							0,0,0,0);	//no move on alpha

	//DELTA
	ComputePositionCommand(	&(out_cmd->cmd.posCmd[DELTA_MOTOR]),
							convertDistTovTops(dist),
							convertSpeedTovTopsPerPeriod(VMax),
							convertAccelTovTopsPerPeriodSqd(Accel),
							convertAccelTovTopsPerPeriodSqd(Decel) );
}


void motion_Rotate(RobotCommand *out_cmd, float angle)
{
	motion_RotateSpeedAcc(out_cmd, angle, motion_GetDefaultSpeed(), motion_GetDefaultAccel(), motion_GetDefaultDecel());
}

void motion_RotateSpeed(RobotCommand *out_cmd, float angle, float VMax)
{
	motion_RotateSpeedAcc(out_cmd, angle, VMax, motion_GetDefaultAccel(), motion_GetDefaultDecel());
}

void motion_RotateSpeedAcc(RobotCommand *out_cmd, float angle, float VMax, float Accel, float Decel)
{
	out_cmd->cmdType = POSITION_COMMAND;
	out_cmd->mcType = ALPHA_DELTA;

	//ALPHA
	ComputePositionCommand(&(out_cmd->cmd.posCmd[ALPHA_MOTOR]),
								convertDistTovTops(angle*distEncoderMeter/2.0f),
								convertSpeedTovTopsPerPeriod(VMax),
								convertAccelTovTopsPerPeriodSqd(Accel),
								convertAccelTovTopsPerPeriodSqd(Decel));

	//DELTA
	ComputePositionCommand(&(out_cmd->cmd.posCmd[DELTA_MOTOR]),
								0, 0, 0, 0);	//no move on delta
}


void motion_ArcRotate(RobotCommand *out_cmd, float angle, float radius)
{
	motion_ArcRotateSpeedAcc(out_cmd, angle, radius, motion_GetDefaultSpeed(), motion_GetDefaultAccel(), motion_GetDefaultDecel());
}

void motion_ArcRotateSpeed(RobotCommand *out_cmd, float angle, float radius, float VMax)
{
	motion_ArcRotateSpeedAcc(out_cmd, angle, radius, VMax, motion_GetDefaultAccel(), motion_GetDefaultDecel());
}

void motion_ArcRotateSpeedAcc(RobotCommand *out_cmd, float angle, float radius, float VMax, float Accel, float Decel)
{
	float dL,dR;
	float ratio;
	
	out_cmd->cmdType = POSITION_COMMAND;
	out_cmd->mcType = LEFT_RIGHT;
	
	dL = angle*(radius-distEncoderMeter/2.0f);
	dR = angle*(radius+distEncoderMeter/2.0f);

	if(fabs(dL) > fabs(dR))
	{
		ratio = dR/dL;
		
		//LEFT
		ComputePositionCommand(&(out_cmd->cmd.posCmd[LEFT_MOTOR]),
									convertDistTovTops(dL),
									convertSpeedTovTopsPerPeriod(VMax),
									convertAccelTovTopsPerPeriodSqd(Accel),
									convertAccelTovTopsPerPeriodSqd(Decel));

		//RIGHT
		ComputePositionCommand(&(out_cmd->cmd.posCmd[RIGHT_MOTOR]),
									convertDistTovTops(dR),
									convertSpeedTovTopsPerPeriod(VMax*ratio),
									convertAccelTovTopsPerPeriodSqd(Accel*ratio),
									convertAccelTovTopsPerPeriodSqd(Decel*ratio));
	}
	else
	{
		ratio = dL/dR;
		
		//LEFT
		ComputePositionCommand(&(out_cmd->cmd.posCmd[LEFT_MOTOR]),
									convertDistTovTops(dL),
									convertSpeedTovTopsPerPeriod(VMax*ratio),
									convertAccelTovTopsPerPeriodSqd(Accel*ratio),
									convertAccelTovTopsPerPeriodSqd(Decel*ratio));

		//RIGHT
		ComputePositionCommand(&(out_cmd->cmd.posCmd[RIGHT_MOTOR]),
									convertDistTovTops(dR),
									convertSpeedTovTopsPerPeriod(VMax),
									convertAccelTovTopsPerPeriodSqd(Accel),
									convertAccelTovTopsPerPeriodSqd(Decel));
	}
}

void motion_SpeedControlLR(RobotCommand *out_cmd, float spLeft, float distLeft, float accLeft, float spRight, float distRight, float accRight)
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

void motion_SpeedControlADMaxTime(RobotCommand *out_cmd,
								float spAlpha, float accAlpha, 
								float spDelta, float accDelta,
								int duringMs)
{
	int32 time;
	
	out_cmd->cmdType = SPEED_COMMAND;
	out_cmd->mcType = ALPHA_DELTA;
	
	time = (DEFAULT_SAMPLING_FREQ*duringMs)/1000;
	
	ComputeSpeedCommandMaxTime( &(out_cmd->cmd.speedCmd[ALPHA_MOTOR]),
								getMotorSpeed(&motors[ALPHA_DELTA][ALPHA_MOTOR]),
								convertSpeedTovTopsPerPeriod(spAlpha),
								convertAccelTovTopsPerPeriodSqd(accAlpha),
								time);
									
	ComputeSpeedCommandMaxTime( &(out_cmd->cmd.speedCmd[DELTA_MOTOR]),
								getMotorSpeed(&motors[ALPHA_DELTA][DELTA_MOTOR]),
								convertSpeedTovTopsPerPeriod(spDelta),
								convertAccelTovTopsPerPeriodSqd(accDelta),
								time);
}

void motion_SpeedControlLRDecel(RobotCommand *out_cmd,
						float spLeft, float distLeft, float accLeft, float decLeft,
						float spRight, float distRight, float accRight, float decRight)
{
	int32 motorSpeed;
	int32 goalSpeed;
	
	out_cmd->cmdType = SPEED_COMMAND;
	out_cmd->mcType = LEFT_RIGHT;
	
	motorSpeed = getMotorSpeed(&motors[LEFT_RIGHT][LEFT_MOTOR]);
	goalSpeed = convertSpeedTovTopsPerPeriod(spLeft);
	
	if( ((motorSpeed < goalSpeed) && motorSpeed > 0)
		|| ((motorSpeed > goalSpeed) && motorSpeed < 0) )
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
	
	if( ((motorSpeed < goalSpeed) && motorSpeed > 0)
	|| ((motorSpeed > goalSpeed) && motorSpeed < 0) )
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

void motion_SpeedControlLRV0(RobotCommand *out_cmd,
							float V0Left, float spLeft, float distLeft, float accLeft,
							float V0Right, float spRight, float distRight, float accRight)
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

void motion_SpeedControlLRTime(RobotCommand *out_cmd,
							float V0Left, float TLeft, float accLeft,
							float V0Right, float TRight, float accRight)
{	
	out_cmd->cmdType = SPEED_COMMAND;
	out_cmd->mcType = LEFT_RIGHT;
	
	//LEFT
	ComputeSpeedCommandTime(&(out_cmd->cmd.speedCmd[LEFT_MOTOR]),
							convertSpeedTovTopsPerPeriod(V0Left),
							(int32)(TLeft / valueSample),
							convertAccelTovTopsPerPeriodSqd(accLeft));

	//RIGHT
	ComputeSpeedCommandTime(&(out_cmd->cmd.speedCmd[RIGHT_MOTOR]),
							convertSpeedTovTopsPerPeriod(V0Right),
							(int32)(TRight / valueSample),
							convertAccelTovTopsPerPeriodSqd(accRight));
}

void motion_SpeedControlAD(RobotCommand *out_cmd, float spAlpha, float distAlpha, float accAlpha, float spDelta, float distDelta, float accDelta)
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

void motion_StepOrderLR(RobotCommand *out_cmd, int32 posLeft, int32 posRight)
{
	out_cmd->cmdType = STEP_COMMAND;
	out_cmd->mcType = LEFT_RIGHT;
	
	//LEFT
	ComputeStepOrder(&(out_cmd->cmd.stepCmd[LEFT_MOTOR]), posLeft);
	
	//RIGHT
	ComputeStepOrder(&(out_cmd->cmd.stepCmd[RIGHT_MOTOR]), posRight);
}

void motion_StepOrderAD(RobotCommand *out_cmd, int32 posAlpha, int32 posDelta)
{
	out_cmd->cmdType = STEP_COMMAND;
	out_cmd->mcType = ALPHA_DELTA;
	
	//ALPHA
	ComputeStepOrder(&(out_cmd->cmd.stepCmd[ALPHA_MOTOR]), posAlpha);
	
	//DELTA
	ComputeStepOrder(&(out_cmd->cmd.stepCmd[DELTA_MOTOR]), posDelta);
}

