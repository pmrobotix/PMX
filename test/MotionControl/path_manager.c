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
//! \file path_manager.c
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#include "path_manager.h"
#include "motion.h"
#include "../exception_handler.h"
#include "../log.h"

WAYPOINT nextWaypoint;
static BOOL waitingSemaphore;
static OS_EVENT *semWaypoint;
static TRAJ_STATE trajState;

void path_TriggerWaypoint(TRAJ_STATE state);

//collision handler
static HANDLED ex_defaultCollisionCallback(void)
{
	motion_FreeMotion();
	
	if(waitingSemaphore)
	{
		path_TriggerWaypoint(TRAJ_COLLISION);
	}

	LOG_DEBUG("Collision Handler...");
	return IT_HANDLED;
}


void path_Init(void)
{
	REGISTER_LOG_CLIENT("PATH");
	
	waitingSemaphore = FALSE;
	semWaypoint = OSSemCreate(0);
	if(semWaypoint == NULL)
	{
		LOG_ERROR("Unable to create waypoint semaphore");
	}
		
	ex_PushITHandler(COLLISION, ex_defaultCollisionCallback);
	ex_PushITHandler(APPROACHING_COLLISION, ex_defaultCollisionCallback);
}

void path_SetNextWaypoint(WAYPOINT *wp)
{
	nextWaypoint = *wp;
}

//! Trigger the current waypoint
//! motion control internal function
void path_TriggerWaypoint(TRAJ_STATE state)
{
	if(nextWaypoint.type != WP_NOT_SET)
	{
		trajState = state;
		OSLastError = OSSemPost(semWaypoint);
		LOG_TEST_OS_ERROR(OSLastError);
	}
}

TRAJ_STATE path_WaitWaypoint()
{
	if(RobotMotionState == TRAJECTORY_RUNNING)
	{		
		//! Wait for waypoint semaphore
		waitingSemaphore = TRUE;

		OSSemPend(semWaypoint, 0, &OSLastError);
		LOG_TEST_OS_ERROR(OSLastError);
		
		waitingSemaphore = FALSE;

		if(OSLastError == 0)
			return trajState;
		else
			return TRAJ_ERROR;
	}
	else
	{
		return TRAJ_ERROR;
	}
}

TRAJ_STATE path_WaitEndOfTrajectory()
{
	WAYPOINT eot;
	eot.type = WP_END_OF_TRAJ;
	path_SetNextWaypoint(&eot);

	return path_WaitWaypoint();
}

void path_LaunchTrajectory(RobotCommand *traj)
{
	OSSemSet(semWaypoint, 0, &OSLastError);
	LOG_TEST_OS_ERROR(OSLastError);
	
	motion_SetCurrentCommand(traj);	
}

void path_CancelTrajectory()
{
	if(waitingSemaphore)
	{
		motion_FreeMotion();
		path_TriggerWaypoint(TRAJ_CANCELLED);
	}
}

void path_InterruptTrajectory()
{
	if(waitingSemaphore)
	{
		motion_FreeMotion();
		path_TriggerWaypoint(TRAJ_INTERRUPTED);
	}	
}

