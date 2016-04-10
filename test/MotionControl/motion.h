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
//! \file motion.h
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#ifndef _MOTION_H_
#define _MOTION_H_

#include "../types.h"
#include "robot_trajectory.h"

typedef enum
{
	TRAJECTORY_RUNNING,
	
	ASSISTED_HANDLING,
	
	FREE_MOTION,
	DISABLE_PID,
} MOTION_STATE;

extern MOTION_STATE RobotMotionState;

//! Load motion control module
void motion_Init(void);

//! Stop motion control for a moment
void motion_FreeMotion(void);

//! Set a robot command and execute it
void motion_SetCurrentCommand(RobotCommand *cmd);

//! Stop motion control and disable PID
void motion_DisablePID(void);

//! Assisted movement mode =)
void motion_AssistedHandling(void);

//! Stop motion control timer, used to shutdown motion control
void motion_StopTimer(void);

//! Directly set the pwm for the motor (used both internally and for test)
void setPWM(int16 pwmLeft, int16 pwmRight);

#endif//_MOTION_H_
