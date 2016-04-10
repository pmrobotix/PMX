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
//! \file motion.c
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#include "motion.h"

#include "motion_priv.h"
#include "motion_calibration.h"

#include "mouse_sensor.h"
#include "motor.h"
#include "motor_PID.h"

#include "robot_odometry.h"
#include "robot_trajectory.h"
#include "robot_slippage.h"

#include "path_manager.h"

#include "../protocol.h"
#include "Protocol/motion_protocol.h"

#include "../position.h"

#include "encoder.h"

#include "uart.h"

#include "../app_cfg.h"
#include "cpu_cfg.h"

#include "../log.h"

//tell the program if mouse sensor and/or motor encoder are available 
#define USE_MOUSE
//#define USE_ENCODER

MOTION_STATE RobotMotionState;

//nb of period since the beginning
static uint32 periodNb;

//semaphore for motion IT and Task
static OS_EVENT *semMotionIT;

//mutex protecting the currently executed command
static OS_EVENT *mtxMotionCommand;

//the currently executed command
static RobotCommand motionCommand;


//motion control task
void motion_ITTask(void *p_arg);

void motion_InitTimer(int frequency);
void initPWM(void);
void signalEndOfTraj(void);


//declaration of some internal function :
void path_Init(void);
void path_TriggerWaypoint(TRAJ_STATE state);


void motion_Init()
{
	int i,j;
	
	periodNb = 0;
	
	//init led pour mesure tempo asserv
	pd13_5 = 1;			// led 13_5 - > out
	
	initPWM();
	pid_Init();	
	//motor initialisation
	for(i=0; i<MAX_MOTION_CONTROL_TYPE_NUMBER; i++)
	{
		for(j=0; j<MOTOR_PER_TYPE; j++)	
		{
			initMotor(&motors[i][j]);
		}
	}
	
	motion_CalibrationInit();
	
	encoder_Init();
	mouse_Init();


	motion_FreeMotion();

	//timing semaphore : unlock the motion control task periodically
	semMotionIT = OSSemCreate(0);
	if(!semMotionIT)
	{
		LOG_ERROR("Unable to create semMotionIT");
	}

	//currently executed motion command protection mutex
	mtxMotionCommand = OSMutexCreate(MUTEX_MOTION_COMMAND_PRIO, &OSLastError);
	LOG_TEST_OS_ERROR(OSLastError);
	
	//create motion control task
	OSLastError = OSTaskCreate(motion_ITTask, NULL, getMotionTopOfStack(), MOTION_PRIO);
	LOG_TEST_OS_ERROR(OSLastError);
	
	motion_InitTimer(DEFAULT_SAMPLING_FREQ);
	traj_Init();
	slippage_Init();
	
	//path manager initialisation
	path_Init();
}


void signalEndOfTraj()
{
	if(motionCommand.cmdType == POSITION_COMMAND)
	{
		motion_FreeMotion();
	}
	
	if(nextWaypoint.type == WP_END_OF_TRAJ)
	{
		path_TriggerWaypoint(TRAJ_OK);
	}	
}


void resetAllPIDErrors(void)
{
	pid_ResetError(motors[LEFT_RIGHT][LEFT_MOTOR].PIDSys);
	pid_ResetError(motors[LEFT_RIGHT][RIGHT_MOTOR].PIDSys);
	pid_ResetError(motors[ALPHA_DELTA][ALPHA_MOTOR].PIDSys);
	pid_ResetError(motors[ALPHA_DELTA][DELTA_MOTOR].PIDSys);
}
	
void motion_FreeMotion()
{
	resetAllPIDErrors();
	RobotMotionState = FREE_MOTION;
	setPWM(0,0);
	OSTimeDly(1);
	RobotMotionState = FREE_MOTION;
	setPWM(0,0);
	OSTimeDly(1);
	RobotMotionState = FREE_MOTION;
	setPWM(0,0);
	resetAllPIDErrors();
}

void motion_AssistedHandling()
{
	motion_FreeMotion();
}

void motion_DisablePID()
{
	motion_FreeMotion();
	RobotMotionState = DISABLE_PID;
	setPWM(0,0);
	OSTimeDly(1);
	RobotMotionState = DISABLE_PID;
	setPWM(0,0);
	resetAllPIDErrors();
}

void loadCommand(RobotCommand *cmd)
{	
	switch(cmd->cmdType)
	{
	case POSITION_COMMAND:
		LoadPositionCommand(&cmd->cmd.posCmd[0], &motors[cmd->mcType][0], periodNb);
		LoadPositionCommand(&cmd->cmd.posCmd[1], &motors[cmd->mcType][1], periodNb);
		break;
		
	case SPEED_COMMAND:
		//if we are in same command mode, we get the next order and apply it
		//as the starting point of new trajectory
		if(motionCommand.cmdType == SPEED_COMMAND && motionCommand.mcType == cmd->mcType)
		{
			int32 pos0, pos1;
			GetSpeedOrder(&motionCommand.cmd.speedCmd[0], periodNb, &pos0);
			GetSpeedOrder(&motionCommand.cmd.speedCmd[1], periodNb, &pos1);
			
			LoadSpeedCommand(&cmd->cmd.speedCmd[0], pos0, periodNb);
			LoadSpeedCommand(&cmd->cmd.speedCmd[1], pos1, periodNb);
		}
		else
		{
			LoadSpeedCommand(&cmd->cmd.speedCmd[0], motors[cmd->mcType][0].lastPos, periodNb);
			LoadSpeedCommand(&cmd->cmd.speedCmd[1], motors[cmd->mcType][1].lastPos, periodNb);
		}		
		break;
	};
}

void motion_SetCurrentCommand(RobotCommand *cmd)
{
	RobotMotionState = DISABLE_PID;
		
	OSMutexPend(mtxMotionCommand, 0, &OSLastError);
	LOG_TEST_OS_ERROR(OSLastError);
	
		loadCommand(cmd);
		motionCommand = *cmd;
		RobotMotionState = TRAJECTORY_RUNNING;
		
	OSLastError = OSMutexPost(mtxMotionCommand);
	LOG_TEST_OS_ERROR(OSLastError);
}


void SendMotorErrors(int32 error0, int32 error1)
{
	/*ParamMessage msg;

	msg.dest = MSG_STEP_TUNING_RESULT;
	msg.length = 8;
	msg.data.ulongs[0] = periodNb;
	
	if(periodNb % 2 ==0)
	{
		msg.code = STEP_RESULT_DATA_LEFT;
		msg.data.longs[1] = error0;
	}
	else
	{
		msg.code = STEP_RESULT_DATA_RIGHT;
		msg.data.longs[1] = error1;
	}
	
	protocol_ComputeChecksum(&msg);
	protocol_SendMessage(&msg);*/
}


#pragma INTERRUPT Motion_IT
void Motion_IT(void)
{
	OSIntEnter();
	
	if(RobotMotionState!=DISABLE_PID)
	{
		p13_5 = 1;	//period measurement
		OSSemPost(semMotionIT);
	}
	
	OSIntExit();
}


//Motion control main loop
//implemented as an IT task, armed on a timer to provide
//a constant and precise period between computation
void motion_ITTask(void *p_arg)
{	
	static int32 left, right;
	static int32 alpha, delta;
	
	static int32 dLeft, dRight;
	static int32 dAlpha, dDelta;
	
	static int32 dLeft2, dRight2;
	static int32 dAlpha2, dDelta2;
	
	static int32 ord0, ord1;
	static BOOL fin0, fin1;
	static int32 pwm0, pwm1;
	static int32 pwm0b, pwm1b;

	for(;;)
	{
		OSSemPend(semMotionIT, 0, &OSLastError);
		LOG_TEST_OS_ERROR(OSLastError);

		periodNb++;
		
				
		#ifdef USE_MOUSE
		if(odometrySensorUsed == MOTOR_ENCODER)
		{
			#ifdef USE_ENCODER
				encoder_ReadSensor(&dLeft, &dRight, &dAlpha, &dDelta);
			#else//NO USE_ENCODER
				dLeft = 0; dRight = 0;
				dAlpha = 0; dDelta = 0;
			#endif//END USE_ENCODER
		}
		else if(odometrySensorUsed == MOUSE_CALIBRATION)
		{
			#ifdef USE_ENCODER
				encoder_ReadSensor(&dLeft, &dRight, &dAlpha, &dDelta);
			#else//NO USE_ENCODER
				dLeft = 0; dRight = 0;
				dAlpha = 0; dDelta = 0;
			#endif//END USE_ENCODER
			
			//if needed update calibration values
			mouse_UpdateCalibrationValues();
		}
		else
		{
			mouse_ReadSensor(&dLeft, &dRight, &dAlpha, &dDelta);
			
			#ifdef USE_ENCODER
			encoder_ReadSensor(&dLeft2, &dRight2, &dAlpha2, &dDelta2);
			
				#ifdef SLIPPAGE_DETECTION
				slippage_Update(LEFT_MOTOR, dLeft2, dLeft);
				slippage_Update(RIGHT_MOTOR, dRight2, dRight);
				#endif//SLIPPAGE_DETECTION
				
			#endif//END USE_ENCODER
		}
		
		#else//NOT USE_MOUSE
		
			#ifdef USE_ENCODER
				encoder_ReadSensor(&dLeft, &dRight, &dAlpha, &dDelta);
			#else//NOT USE_ENCODER
				dLeft = 0; dRight = 0;
				dAlpha = 0; dDelta = 0;
			#endif//END USE_ENCODER
			
		#endif//END USE_MOUSE

		odo_Integration(2*dAlpha/(float)distEncoder, (float)dDelta);
				
		//send position
		if((periodNb&0x3F) == 0)
		{
			pos_SendPosition();
		}
				
		//update all motors 
		updateMotor(&motors[LEFT_RIGHT][LEFT_MOTOR], dLeft);
		updateMotor(&motors[LEFT_RIGHT][RIGHT_MOTOR], dRight);
		updateMotor(&motors[ALPHA_DELTA][ALPHA_MOTOR], dAlpha);
		updateMotor(&motors[ALPHA_DELTA][DELTA_MOTOR], dDelta);

		//order and pwm computation
		switch(RobotMotionState)
		{
		case TRAJECTORY_RUNNING:
			//lock motionCommand
			OSMutexPend(mtxMotionCommand, 0, &OSLastError);
			LOG_TEST_OS_ERROR(OSLastError);

				//choose the right function to compute new order value
				switch(motionCommand.cmdType)
				{
					case POSITION_COMMAND:
						fin0 = GetPositionOrder(&motionCommand.cmd.posCmd[0], periodNb, &ord0);
						fin1 = GetPositionOrder(&motionCommand.cmd.posCmd[1], periodNb, &ord1);
						break;
						
					case SPEED_COMMAND:
						fin0 = GetSpeedOrder(&motionCommand.cmd.speedCmd[0], periodNb, &ord0);
						fin1 = GetSpeedOrder(&motionCommand.cmd.speedCmd[1], periodNb, &ord1);
						break;
						
					case STEP_COMMAND:
						fin0 = GetStepOrder(&motionCommand.cmd.stepCmd[0], &ord0);
						fin1 = GetStepOrder(&motionCommand.cmd.stepCmd[1], &ord1);
						break;
				}
				
				//compute pwm for first motor
				pwm0 = pid_Compute(motors[motionCommand.mcType][0].PIDSys,
								ord0-motors[motionCommand.mcType][0].lastPos);
				
				//compute pwm for second motor			
				pwm1 = pid_Compute(motors[motionCommand.mcType][1].PIDSys,
								ord1-motors[motionCommand.mcType][1].lastPos);
				
				
				//output pwm to motors
				if(motionCommand.mcType == LEFT_RIGHT)
				{
					setPWM(pwm0, pwm1);
				}
				else if(motionCommand.mcType == ALPHA_DELTA)
				{
					pwm0b = pwm1 - pwm0;
					BOUND_INT(pwm0b, 0x7EFF);
					
					pwm1b = pwm1 + pwm0;
					BOUND_INT(pwm1b, 0x7EFF);
					
					setPWM(pwm0b, pwm1b);
				}
				
				
				//if required send both errors
				if(isSendErrorsEnabled)
				{
					SendMotorErrors(ord0 - motors[motionCommand.mcType][0].lastPos,
									ord1 - motors[motionCommand.mcType][1].lastPos);
				}
				
				//test end of traj
				if(fin0 && fin1)
				{
					signalEndOfTraj();
				}
				
			//unlock motionCommand
			OSLastError = OSMutexPost(mtxMotionCommand);
			LOG_TEST_OS_ERROR(OSLastError);
			break;
			
		case ASSISTED_HANDLING:
			{							
				//compute pwm for first motor
				pwm0 = pid_Compute(motors[motionCommand.mcType][0].PIDSys,
								dLeft);

				//compute pwm for second motor			
				pwm1 = pid_Compute(motors[motionCommand.mcType][1].PIDSys,
								dRight);

				//write pwm in registers
				setPWM(pwm0, pwm1);
			}
			break;

		case DISABLE_PID:
		case FREE_MOTION:
		default:
			break;
		};
		
		p13_5 = 0;	//period measurement
	}
}

//Motion control main loop
//implemented as an IT handler armed on a timer to provide
//a constant and precise period between computation
void motion_InitTimer(int frequency)
{
	ta0mr = 0x40;   // XX0X XX00
					// |||| ||||
					// |||| |||+- must always be 0 in timer mode
					// |||| ||+-- must always be 0 in timer mode
					// |||| |+--- 0: pulse is not output at pin TA1out
					// |||| |     1: pulse is output at pin TA1out
					// |||| |        TA0out is automatically  output
					// |||| +---- 0: gate function: timer counts only
					// ||||          when TA0in is held "L"
					// ||||       1: gate function: timer counts only
					// ||||          when TA0in is held "H"
					// |||+------ 0: gate function not available
					// |||        1: gate function available
					// ||+------- must always be 0 in timer mode
					// |+-------- count source select bits:
					// +--------- count source select bits:
					//            00:  f1
					//            01:  f8
					//            10:  f2n
					//            11:  fc32

	motion_SetSamplingFrequency(frequency);

	// level 6 priority: larger number ==> higher priority
	ta0ic = 0x06;
			
	asm("\tNOP");
	asm("\tNOP");
	
	tabsr |= 0x01;          //  start timer A0
}

void motion_StopTimer()
{
	tabsr &= 0xFE;          //  stop timer A0
	setPWM(0, 0);
}

void initPWM()
{
	//config pwm -> out on ta2out and ta3out
	ps1_4 = 1;
	psl1_4 = 0;
	psc_4 = 0;	//ta2out
	
	ps1_6 = 1;
	psl1_6 = 1;	//ta3out
	
	//0010.0011
	ta2mr = 0x23;			//pwm 8 bits sur f1
	ta3mr = 0x23;			//pwm 8 bits sur f1
	
	ta2 = 0x0000;			//init pwm to 0
	ta3 = 0x0000;			//init pwm to 0
	
	tabsr |= 0x04;          //start timer A2
	tabsr |= 0x08;          //start timer A3
	
	pd7_3 = 1;				//73 - > out
	pd7_5 = 1;				//75 - > out
}

void setPWM(int16 pwmLeft, int16 pwmRight)
{
	p7_5 = pwmLeft<0;
	ta3 = ((2*abs(pwmLeft))&0xFF00) | 0x06;		//~20kHz
	
	p7_3 = pwmRight>0;
	ta2 = ((2*abs(pwmRight))&0xFF00) | 0x06;	//~20kHz
}

