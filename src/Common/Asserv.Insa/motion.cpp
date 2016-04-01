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
//
// Modified by PM-ROBOTIX in CPP
/******************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstdlib>

#include "../Asserv/MovingBase.hpp"
#include "AsservInsa.hpp"

class MovingBase;

AsservInsa::AsservInsa()
{
	activate_ = false;
	stop_motion_ITTask = false;
	//SampleTime = 1; //... millisec //TODO

	leftEncoderRatio = 0;
	//ratio vTops/ticks for right encoder
	rightEncoderRatio = 0;
	//distance between both encoder wheels in vTops
	distEncoder = 0;

	//distance between both encoder wheels in meters
	distEncoderMeter = 0.0;

	//vTops is a virtual measure distance to avoid floating point computation
	valueVTops = 0.0;

	loopDelayInMillis = 0;

	defaultSamplingFreq = 10; //1 fois par second pour eviter la division par zero.

	vtopsPerTicks = 100; //1000 ?

	maxPwmValue_ = 0;

	defaultSpeed = 0.0;
	defaultAcc = 0.0;
	defaultDec = 0.0;

	timeOffset = 0;

	waitingSemaphore = false;

	Theta = 0.0; //en radian
	cosTheta = 0.0; //no unit //static
	sinTheta = 0.0;
	xTops = 0.0; //encoder vTops //static
	yTops = 0.0;

	pos_x = 0.0;
	pos_y = 0.0;
	pos_theta = 0.0;

	odoPeriodNb = 0;
	//nb of period since the beginning
	periodNb = 0;

	trajState = TRAJ_OK;

	pid_Nb = 0;
	stopAt = 0;

	RobotMotionState = DISABLE_PID;

	base_ = NULL;

}
AsservInsa::~AsservInsa()
{
}

void AsservInsa::setMovingBase(MovingBase *base)
{
	base_ = base;
}

long AsservInsa::currentTimeInMillis()
{
	struct timeval te;
	gettimeofday(&te, NULL); // get current time
	long long milliseconds = (te.tv_sec * 1000LL + te.tv_usec / 1000); // calculate milliseconds
	return (long) (milliseconds - timeOffset);
}

void AsservInsa::motion_Init()
{
	logger().debug() << "motion_Init started" << logs::end;

	int i, j;
	RobotMotionState = DISABLE_PID;
	periodNb = 0;

	initPWM();

	pid_Init();
	//motor initialisation
	for (i = 0; i < MAX_MOTION_CONTROL_TYPE_NUMBER; i++)
	{
		for (j = 0; j < MOTOR_PER_TYPE; j++)
		{
			initMotor(&motors[i][j]);
		}
	}

	encoder_Init();

	motion_FreeMotion();

	//currently executed motion command protection mutex
	pthread_mutex_init(&mtxMotionCommand, NULL);

	motion_SetSamplingFrequency(defaultSamplingFreq); //Hz

	//create motion control task thread
	start("AsservInsa");

	//slippage init
	slippage_Init();

	//path manager initialisation
	path_Init();
}

void AsservInsa::signalEndOfTraj()
{
#ifdef DEBUG_MOTION
	printf("motions.c : signalEndOfTraj %d\n", nextWaypoint.type);
#endif
	if (motionCommand.cmdType == POSITION_COMMAND)
	{
		motion_FreeMotion();
	}

	if (nextWaypoint.type == WP_END_OF_TRAJ)
	{
#ifdef DEBUG_MOTION
		printf("signalEndOfTraj path_TriggerWaypoint\n");
#endif
		path_TriggerWaypoint(TRAJ_OK);
	}

}

void AsservInsa::motion_FreeMotion()
{
	RobotMotionState = FREE_MOTION;
	setPWM(0, 0);
}

void AsservInsa::motion_AssistedHandling()
{
	motion_FreeMotion();
	RobotMotionState = ASSISTED_HANDLING;
}

void AsservInsa::motion_DisablePID()
{
	motion_FreeMotion();
	RobotMotionState = DISABLE_PID;
	setPWM(0, 0);

}
void AsservInsa::checkRobotCommand(RobotCommand *cmd)
{
	int cmdType = cmd->cmdType;
	if (cmdType != POSITION_COMMAND && cmdType != SPEED_COMMAND && cmdType != STEP_COMMAND)
	{
		printf("motion.c checkRobotCommand : ERROR: bad command type: %d\n", cmdType);
		exit(1);
	}
	int mcType = cmd->mcType;
	if (mcType != ALPHA_DELTA && mcType != LEFT_RIGHT)
	{
		printf("motion.c checkRobotCommand : ERROR: bad control type: %d\n", cmdType);
		exit(1);
	}
}
void AsservInsa::loadCommand(RobotCommand *cmd)
{
	this->checkRobotCommand(cmd);
	switch (cmd->cmdType)
	{
	case POSITION_COMMAND:
		LoadPositionCommand(&cmd->cmd.posCmd[0], &motors[cmd->mcType][0], periodNb);
		LoadPositionCommand(&cmd->cmd.posCmd[1], &motors[cmd->mcType][1], periodNb);
		break;

	case SPEED_COMMAND:
		//if we are in same command mode, we get the next order and apply it
		//as the starting point of new trajectory
		if (motionCommand.cmdType == SPEED_COMMAND && motionCommand.mcType == cmd->mcType)
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

	default:
		break;
	};
}

void AsservInsa::motion_SetCurrentCommand(RobotCommand *cmd)
{
	checkRobotCommand(cmd);
#ifdef DEBUG_MOTION
	printf("motion.c : motion_SetCurrentCommand  \n");
#endif
	RobotMotionState = DISABLE_PID;

	pthread_mutex_lock(&mtxMotionCommand);
#ifdef DEBUG_MOTION
	printf("motion.c : motion_SetCurrentCommand lock done commandType:%d controlType:%d\n",
			cmd->cmdType, cmd->mcType);
#endif
	loadCommand(cmd);
	motionCommand = *cmd;
	RobotMotionState = TRAJECTORY_RUNNING;
	pthread_mutex_unlock(&mtxMotionCommand);
#ifdef DEBUG_MOTION
	printf("motion.c : motion_SetCurrentCommand done\n");
#endif
}

//Motion control main loop
//implemented as an IT task, armed on a timer to provide
//a constant and precise period between computation
void AsservInsa::execute()
{

	/*
	 #ifdef LOG_PID_APPENDER
	 //pmx::Robot &logrobot = pmx::Robot::instance();
	 #endif
	 */
//static int32 left, right;
//static int32 alpha, delta;
	int32 dLeft, dRight; //static
	int32 dAlpha, dDelta; //static

//static int32 dLeft2, dRight2;
//static int32 dAlpha2, dDelta2;

	int32 ord0, ord1; //static
	bool fin0 = false, fin1 = false; //static
	int32 pwm0, pwm1; //static
	int32 pwm0b, pwm1b; //static

	logger().debug() << "AsservInsa execute started; stop_motion_ITTask=" << stop_motion_ITTask << logs::end;

	while (!stop_motion_ITTask)
	{

		long startTime = currentTimeInMillis();
		//logger().debug() << "AsservInsa execute startTime=" << startTime << logs::end;

		periodNb++;

		encoder_ReadSensor(&dLeft, &dRight, &dAlpha, &dDelta);
		odo_Integration(2 * dAlpha / (float) distEncoder, (float) dDelta);

		//odo_GetPosition();
		//update all motors data
		updateMotor(&motors[LEFT_RIGHT][LEFT_MOTOR], dLeft);
		updateMotor(&motors[LEFT_RIGHT][RIGHT_MOTOR], dRight);
		updateMotor(&motors[ALPHA_DELTA][ALPHA_MOTOR], dAlpha);
		updateMotor(&motors[ALPHA_DELTA][DELTA_MOTOR], dDelta);

		float dSpeed0 = 0;
		float dSpeed1 = 0;
		//order and pwm computation
		switch (RobotMotionState)
		{
		case TRAJECTORY_RUNNING:
			//logger().debug() << "AsservInsa TRAJECTORY_RUNNING" << logs::end;

			//lock motionCommand
			pthread_mutex_lock(&mtxMotionCommand);

			if (motionCommand.mcType == LEFT_RIGHT)
			{
				dSpeed0 = dLeft;
				dSpeed1 = dRight;
			}
			else if (motionCommand.mcType == ALPHA_DELTA)
			{
				dSpeed0 = dAlpha * 2.0;
				dSpeed1 = dDelta;
			}
			else
			{
				printf("motion.c : motion_ITTask : motionCommand error: %d (%d %d %d)\n",
						motionCommand.mcType,
						LEFT_RIGHT,
						ALPHA_DELTA,

						MAX_MOTION_CONTROL_TYPE_NUMBER);
				exit(1);
			}

			//choose the right function to compute new order value
			switch (motionCommand.cmdType)
			{
			case POSITION_COMMAND:
#ifdef DEBUG_MOTION
				printf("motion.c :  POSITION_COMMAND\n");
#endif
				fin0 = GetPositionOrder(&motionCommand.cmd.posCmd[0], periodNb, &ord0);
				fin1 = GetPositionOrder(&motionCommand.cmd.posCmd[1], periodNb, &ord1);

				break;

			case SPEED_COMMAND:
#ifdef DEBUG_MOTION
				printf("motion.c :  SPEED_COMMAND\n");
#endif
				fin0 = GetSpeedOrder(&motionCommand.cmd.speedCmd[0], periodNb, &ord0);
				fin1 = GetSpeedOrder(&motionCommand.cmd.speedCmd[1], periodNb, &ord1);
				break;

			case STEP_COMMAND:
#ifdef DEBUG_MOTION
				printf("motion.c :  STEP_COMMAND\n");
#endif
				fin0 = GetStepOrder(&motionCommand.cmd.stepCmd[0], &ord0);
				fin1 = GetStepOrder(&motionCommand.cmd.stepCmd[1], &ord1);
				break;
			}
			/*
			 #ifdef LOG_PID

			 log_status(currentTimeInMillis(), encoder_getLeftCounter(),
			 encoder_getRightCounter(), robot_getLeftPower(),
			 robot_getRightPower(), ord0, ord1,
			 motors[motionCommand.mcType][0].lastPos,
			 motors[motionCommand.mcType][1].lastPos, p.x, p.y, p.theta);
			 #endif
			 */
			/*
			 #ifdef LOG_SVG_APPENDER
			 apf_svg_writePosition(p.x, p.y, p.theta);
			 #endif
			 */
			//compute pwm for first motor
			//	printf("motion.c : pid_Compute : ORDER 0 : %d current:%d\n", ord0,
			//			motors[motionCommand.mcType][0].lastPos);
			pwm0 = pid_ComputeRcva(motors[motionCommand.mcType][0].PIDSys, ord0, dSpeed0);
//			pwm0 = pid_Compute(motors[motionCommand.mcType][0].PIDSys,
//					ord0,
//					motors[motionCommand.mcType][0].lastPos,
//					dSpeed0);
			//	printf("motion.c : pid_Compute : ORDER 1 : %d current:%d\n", ord1,
			//			motors[motionCommand.mcType][1].lastPos);
			//compute pwm for second motor
			pwm1 = pid_ComputeRcva(motors[motionCommand.mcType][1].PIDSys, ord1, dSpeed1);

//			pwm1 = pid_Compute(motors[motionCommand.mcType][1].PIDSys,
//					ord1,
//					motors[motionCommand.mcType][1].lastPos,
//					dSpeed1);

			//output pwm to motors
			if (motionCommand.mcType == LEFT_RIGHT)
			{
				//	printf("motion.c : LEFT_RIGHT mode, pid result : %d %d \n",	pwm0, pwm1);
				BOUND_INT(pwm0, maxPwmValue_);
				BOUND_INT(pwm1, maxPwmValue_);
				pwm0b = pwm0;
				pwm1b = pwm1;
				setPWM(pwm0b, pwm1b);

			}
			else if (motionCommand.mcType == ALPHA_DELTA)
			{
				// printf("motion.c : ALPHA_DELTA mode, pid result : %d %d \n", pwm0, pwm1);
				pwm0b = pwm1 - pwm0;
				BOUND_INT(pwm0b, maxPwmValue_);

				pwm1b = pwm1 + pwm0;
				BOUND_INT(pwm1b, maxPwmValue_);

				setPWM(pwm0b, pwm1b);

			}
			/*
			 #ifdef LOG_PID_APPENDER

			 apf_log(currentTimeInMillis(), dSpeed0, dSpeed1, pwm0b, pwm1b, ord0, ord1,
			 motors[motionCommand.mcType][0].lastPos, motors[motionCommand.mcType][1].lastPos, p.x, p.y,
			 p.theta);

			 //logrobot.base().mlogger().debug() << "Motion.c p=" << periodNb << "\tdSpeed=" << dSpeed1 << "\tpwm=" << pwm1b << "\tord1=" << ord1 << utils::end;
			 #endif
			 */

			logger().debug() << "execute dLeft="
					<< dLeft
					<< " dRight="
					<< dRight
					<< " dAlpha="
					<< dAlpha
					<< " dDelta="
					<< dDelta
					<< " pwm0b="
					<< pwm0b
					<< " pwm1b="
					<< pwm1b
					<< logs::end;

			//test end of traj
			if (fin0 && fin1)
			{
				/*
				 #ifdef LOG_PID
				 closeLog();
				 #endif
				 */
				signalEndOfTraj();
			}

			//unlock motionCommand
			pthread_mutex_unlock(&mtxMotionCommand);

			break;

		case ASSISTED_HANDLING:
		{
			//logger().debug() << "AsservInsa ASSISTED_HANDLING" << logs::end;
			//printf("motion_ITTask ASSISTED_HANDLING  \n");
			dSpeed0 = dLeft;
			dSpeed1 = dRight;
			//compute pwm for first motor

			pwm0 = pid_ComputeRcva(motors[motionCommand.mcType][0].PIDSys, dLeft, dSpeed0);
			/*pwm0 = pid_Compute(motors[motionCommand.mcType][0].PIDSys,
			 0,
			 motors[motionCommand.mcType][0].lastPos,
			 dSpeed0);*/

			//compute pwm for second motor
			pwm1 = pid_ComputeRcva(motors[motionCommand.mcType][1].PIDSys, dRight, dSpeed1);
			/*pwm1 = pid_Compute(motors[motionCommand.mcType][1].PIDSys,
			 0,
			 motors[motionCommand.mcType][1].lastPos,
			 dSpeed1);*/
			printf("motion_ITTask ASSISTED_HANDLING  pwm0=%d pwm1=%d \n", pwm0, pwm1);
			//write pwm in registers
			setPWM(pwm0, pwm1);
			break;
		}

		case DISABLE_PID:
		{
			//logger().debug() << "AsservInsa DISABLE_PID" << logs::end;
			//printf("motion_ITTask DISABLE_PID  \n");
			break;
		}
		case FREE_MOTION:
		{
			//logger().debug() << "AsservInsa FREE_MOTION" << logs::end;
			//printf("motion_ITTask FREE_MOTION  \n");
			break;
		}
		default:
			break;
		};

		long stopTime = currentTimeInMillis();
		long duration = stopTime - startTime;

//		logger().debug() << "loopDelayInMillis= "
//				<< loopDelayInMillis
//				<< " stopTime="
//				<< stopTime
//				<< " startTime="
//				<< startTime
//				<< " task duration="
//				<< duration
//				<< logs::end;

		if (duration < loopDelayInMillis && duration >= 0)
		{
			int t = 1000 * (loopDelayInMillis - duration);
			usleep(t);
		}
	}

	logger().debug() << "motion_ITTask execute exit()" << logs::end;
	/*
	 #ifdef LOG_PID
	 printf("closelog");
	 sleep(1);
	 closeLog();
	 #endif*/
//exit(2);
	//return 0;
}

void AsservInsa::activate(bool a)
{
	activate_ = a;
	if (!a)
		setPWM(0, 0);

}

void AsservInsa::motion_StopTimer()
{
	stop_motion_ITTask = true;
	setPWM(0, 0);
	setPWM(0, 0);
}

void AsservInsa::initPWM()
{

}

void AsservInsa::setPWM(int16 pwmLeft, int16 pwmRight)
{
#ifdef DEBUG_MOTION
	printf("motion.c setPWM : left %d  right %d\n", pwmLeft, pwmRight);
#endif

	if (base_ != NULL)
	{
		base_->motors().runMotorLeft(pwmLeft, 0);
		base_->motors().runMotorRight(pwmRight, 0);
	}
	else
	{
		logger().error() << "setPWM : Base is NULL !" << logs::end;
	}

}

