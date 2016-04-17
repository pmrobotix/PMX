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
#include <sys/time.h>
#include <unistd.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "../../Log/Logger.hpp"
#include "../Asserv/Asserv.hpp"
#include "../Asserv/MotorControl.hpp"
#include "../Asserv/MovingBase.hpp"
#include "../Robot.hpp"
#include "../Position.hpp"
#include "AsservInsa.hpp"


class Robot;

class MovingBase;

AsservInsa::AsservInsa(Robot * robot)
{
	//Reference vers le robot
	robot_ = robot;

	//activate_ = false;
	stop_motion_ITTask = false;

	leftEncoderRatio = 0;
	rightEncoderRatio = 0; //ratio [vTops/ticks] for right encoder
	rightTicksPerM_ = 0;
	leftTicksPerM_ = 0;

	useExternalEncoders_ = 0;

	lastLeft_ = 0;
	lastRight_ = 0;

	distEncoder = 0; //distance between both encoder wheels in [vTops]
	distEncoderMeter = 0.0; //distance between both encoder wheels in [meters]

	valueVTops = 0.0; //vTops in [meter/vtops] is a virtual measure distance to avoid floating point computation

	loopDelayInMillis = 0;

	defaultSamplingFreq = 20; //5 fois par second par defaut (pour eviter la division par zero).

	//Ajoute plus de précision dans les calculs de trajectoire (en vtops par ticks)
	vtopsPerTicks = 100; //100 semble le meilleur compromis

	maxPwmValue_ = 0;

	defaultVmax = 0.0;
	defaultAcc = 0.0;
	defaultDec = 0.0;

	timeOffset = 0;

	waitingSemaphore = false;

	Theta = 0.0; //en [radian]
	cosTheta = 0.0; //no unit
	sinTheta = 0.0;
	xTops = 0.0; //encoder [vTops]
	yTops = 0.0;

	odoPeriodNb = 0;

	periodNb = 0; //nb of period since the beginning

	trajState = TRAJ_OK;

	pid_Nb = 0;
	stopAt = 0;

	RobotMotionState = DISABLE_PID;

	//base_ = NULL;

}
AsservInsa::~AsservInsa()
{
}

//void AsservInsa::setMovingBase(MovingBase *base)
//{
//	base_ = base;
//}

long AsservInsa::currentTimeInMillis()
{
	struct timeval te;
	gettimeofday(&te, NULL); // get current time
	long long milliseconds = (te.tv_sec * 1000LL + te.tv_usec / 1000); // calculate milliseconds
	return (long) (milliseconds - timeOffset);
}

void AsservInsa::motion_Init()
{
	//logger().debug() << "motion_Init started" << logs::end;

	int i, j;
	RobotMotionState = DISABLE_PID;

	periodNb = 0;

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
	int32 dLeft, dRight; //static
	int32 dAlpha, dDelta; //static
	int32 left, right;

	int32 ord0, ord1; //static
	bool fin0 = false, fin1 = false; //static
	int32 pwm0, pwm1; //static
	int32 pwm0b, pwm1b; //static

	float delta_y = 0.0;
	float delta_x = 0.0;

	float x_mm = 0.0;
	float y_mm = 0.0;
	float angle_rad = 0.0;

	long startTime = currentTimeInMillis();
	long currentTime = startTime;
	logger().debug() << "execute started; loopDelayInMillis="
			<< loopDelayInMillis
			<< " ms"
			<< logs::end;

	loggerFile().debug() << "NbPeriod, currentTime(ms), Lencoder(vtops), Rencoder(vtops), Lencoder(ticks), Rencoder(ticks), speed0, speed1, speed0(mm/s), speed1(mm/s), pwm0, pwm1, lastpos0(ticks), lastpos1(ticks), ord0(ticks), ord1(ticks), x(mm), y(mm), angle(degre)"
			<< logs::end;

	while (!stop_motion_ITTask)
	{
		periodNb++;

		currentTime = currentTimeInMillis();
		//logger().debug() << "AsservInsa execute currentTime=" << currentTime << logs::end;

		//TODO use external or internl encoders !!
		encoder_ReadSensor(&dLeft, &dRight, &dAlpha, &dDelta, &left, &right);
		float temp = (2.0 * (float) dAlpha) / (float) distEncoder;
		odo_Integration(temp, (float) dDelta);
		RobotPosition p = odo_GetPosition();

		//update all motors data
		updateMotor(&motors[LEFT_RIGHT][LEFT_MOTOR], dLeft);
		updateMotor(&motors[LEFT_RIGHT][RIGHT_MOTOR], dRight);
		updateMotor(&motors[ALPHA_DELTA][ALPHA_MOTOR], dAlpha);
		updateMotor(&motors[ALPHA_DELTA][DELTA_MOTOR], dDelta);


		x_mm = p.x * 1000.0;
		y_mm = p.y * 1000.0;
		angle_rad = p.theta;
		robot_->svgw().writePosition(x_mm,
				y_mm,
				angle_rad,
				"bot-pos");

		/*

		loggerSvg().info() << "<circle cx=\""
				<< x_mm
				<< "\" cy=\""
				<< -y_mm
				<< "\" r=\"1\" fill=\"blue\" />"
				<< logs::end;
		delta_y = 50.0 * sin(angle_rad);
		delta_x = 50.0 * cos(angle_rad);
		loggerSvg().info() << "<line x1=\""
				<< x_mm
				<< "\" y1=\""
				<< -y_mm
				<< "\" x2=\""
				<< x_mm + delta_x
				<< "\" y2=\""
				<< -y_mm - delta_y
				<< "\" stroke-width=\"0.1\" stroke=\"grey\"  />"
				<< logs::end;
*/


		float dSpeed0 = 0;
		float dSpeed1 = 0;
		//order and pwm computation
		switch (RobotMotionState)
		{
		case TRAJECTORY_RUNNING:
			//logger().debug() << "execute TRAJECTORY_RUNNING" << logs::end;

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

			//compute pwm for first motor
//			pwm0 = pid_Compute(motors[motionCommand.mcType][0].PIDSys,
//					(float) ord0,
//					(float) motors[motionCommand.mcType][0].lastPos,
//					dSpeed0);
//			//compute pwm for second motor
//			pwm1 = pid_Compute(motors[motionCommand.mcType][1].PIDSys,
//					(float) ord1,
//					(float) motors[motionCommand.mcType][1].lastPos,
//					dSpeed1);

			//RCVA only for speed ? not position ?
			pwm0 = pid_ComputeRcva(motors[motionCommand.mcType][0].PIDSys,
					(float) (ord0 - motors[motionCommand.mcType][0].lastPos),
					dSpeed0);

			pwm1 = pid_ComputeRcva(motors[motionCommand.mcType][1].PIDSys,
					(float) (ord1 - motors[motionCommand.mcType][1].lastPos),
					dSpeed1);

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

//			logger().debug() << "execute dLeft="
//					<< dLeft
//					<< " dRight="
//					<< dRight
//					<< " dAlpha="
//					<< dAlpha
//					<< " dDelta="
//					<< dDelta
//					<< " pwm0b="
//					<< pwm0b
//					<< " pwm1b="
//					<< pwm1b
//					<< logs::end;

			loggerFile().debug() << periodNb
					<< ", "
					<< currentTime - startTime
					<< ", "
					<< left
					<< ", "
					<< right
					<< ", "
					<< left / leftEncoderRatio
					<< ", "
					<< right / rightEncoderRatio
					<< ", "
					<< dSpeed0
					<< ", "
					<< dSpeed1
					<< ", "
					<< dSpeed0 * valueVTops / ((float) loopDelayInMillis / 1000.0) * 1000.0 //= mm/s
					<< ", "
					<< dSpeed1 * valueVTops / ((float) loopDelayInMillis / 1000.0) * 1000.0 //= mm/s
					<< ", "
					<< pwm0
					<< ", "
					<< pwm1

					<< ", "
					<< motors[motionCommand.mcType][0].lastPos / leftEncoderRatio
					<< ", "
					<< motors[motionCommand.mcType][1].lastPos / leftEncoderRatio

					<< ", "
					<< (float) (ord0 / leftEncoderRatio)
					<< ", "
					<< (float) (ord1 / rightEncoderRatio)
					<< ", "
					<< x_mm
					<< ", "
					<< y_mm
					<< ", "
					<< angle_rad

					<< logs::end;

//			logger().info() << periodNb
//					<< ", "
//					<< currentTime - startTime
//					<< ", "
//					<< left
//					<< ", "
//					<< right
//					<< ", "
//					<< left / leftEncoderRatio
//					<< ", "
//					<< right / rightEncoderRatio
//
//					<< ", "
//					<< dSpeed0
//					<< ", "
//					<< dSpeed1
//					<< ", "
//					<< dSpeed0 * valueVTops / ((float) loopDelayInMillis / 1000.0) * 1000.0 //= mm/s
//					<< ", "
//					<< dSpeed1 * valueVTops / ((float) loopDelayInMillis / 1000.0) * 1000.0 //= mm/s
//					<< ", "
//					<< pwm0
//					<< ", "
//					<< pwm1
//
//					<< ", "
//					<< motors[motionCommand.mcType][0].lastPos / leftEncoderRatio
//					<< ", "
//					<< motors[motionCommand.mcType][1].lastPos / leftEncoderRatio
//
//					<< ", "
//					<< (float) (ord0 / leftEncoderRatio)
//					<< ", "
//					<< (float) (ord1 / rightEncoderRatio)
//					<< ", "
//					<< x_mm
//					<< ", "
//					<< y_mm
//					<< ", "
//					<< angle_rad
//
//					<< logs::end;

			//test end of traj
			if (fin0 && fin1)
			{
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
		long duration = stopTime - currentTime;

//		logger().debug() << "loopDelayInMillis= "
//				<< loopDelayInMillis
//				<< " stopTime="
//				<< stopTime
//				<< " currentTime="
//				<< currentTime
//				<< " task duration="
//				<< duration
//				<< logs::end;

		if (duration < loopDelayInMillis && duration >= 0)
		{
			int t = 1000 * (loopDelayInMillis - duration);
			usleep(t);
		}
	}

	//logger().debug() << "motion_ITTask execute exit()" << logs::end;
	/*
	 #ifdef LOG_PID
	 printf("closelog");
	 sleep(1);
	 closeLog();
	 #endif*/
//exit(2);
	//return 0;
}
//
//void AsservInsa::activate(bool a)
//{
//	activate_ = a;
//	if (!a)
//		setPWM(0, 0);
//
//}

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
	//logger().debug() << "setPWM left=" << pwmLeft << " right=" << pwmRight << logs::end;

	if (robot_ != NULL)
	{
		robot_->asserv_default->base()->motors().runMotorLeft(pwmLeft, 0);
		robot_->asserv_default->base()->motors().runMotorRight(pwmRight, 0);
//		base_->motors().runMotorLeft(pwmLeft, 0);
//		base_->motors().runMotorRight(pwmRight, 0);
	}
	else
	{
		logger().error() << "setPWM : Base is NULL !" << logs::end;
	}

}

