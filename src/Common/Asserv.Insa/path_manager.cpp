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

#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#include "AsservInsa.hpp"

//collision handler
//static HANDLED ex_defaultCollisionCallback(void) {
//	motion_FreeMotion();
//
//	if (waitingSemaphore) {
//		path_TriggerWaypoint(TRAJ_COLLISION);
//	}
//
//	LOG_DEBUG("Collision Handler...");
//	return IT_HANDLED;
//}

void AsservInsa::path_Init(void)
{
	trajState = TRAJ_OK;
	waitingSemaphore = FALSE;
	sem_init(&semWaypoint, 0, 0);

}

void AsservInsa::path_SetNextWaypoint(WAYPOINT *wp)
{
	nextWaypoint = *wp;
}

//! Trigger the current waypoint
//! motion control internal function
void AsservInsa::path_TriggerWaypoint(TRAJ_STATE state)
{
	if (nextWaypoint.type != WP_NOT_SET)
	{
		trajState = state;
		sem_post(&semWaypoint);

	}
}

TRAJ_STATE AsservInsa::path_WaitWaypoint()
{
	if (RobotMotionState == TRAJECTORY_RUNNING)
	{
		//! Wait for waypoint semaphore
		waitingSemaphore = TRUE;

		sem_wait(&semWaypoint);

		waitingSemaphore = FALSE;

		return trajState;
	}
	else
	{
		return TRAJ_ERROR;
	}
}

TRAJ_STATE AsservInsa::path_WaitEndOfTrajectory()
{
	WAYPOINT *eot = (WAYPOINT *) calloc(1, sizeof(WAYPOINT));
	eot->type = WP_END_OF_TRAJ;
	eot->threshold = 0;
	path_SetNextWaypoint(eot);

	return path_WaitWaypoint();
}

void AsservInsa::path_LaunchTrajectory(RobotCommand *traj)
{
	sem_init(&semWaypoint, 0, 0);

	motion_SetCurrentCommand(traj);
}

void AsservInsa::path_CancelTrajectory()
{
	if (waitingSemaphore)
	{
		motion_FreeMotion();
		path_TriggerWaypoint(TRAJ_CANCELLED);
	}
}

void AsservInsa::path_InterruptTrajectory()
{
	if (waitingSemaphore)
	{
		motion_FreeMotion();
		path_TriggerWaypoint(TRAJ_INTERRUPTED);
	}
}

void AsservInsa::path_CollisionOnTrajectory()
{
	if (waitingSemaphore)
	{
		motion_FreeMotion();
		usleep(1000000);
		path_TriggerWaypoint(TRAJ_COLLISION);
	}
}

void AsservInsa::path_CollisionRearOnTrajectory()
{
	if (waitingSemaphore)
	{
		motion_FreeMotion();
		usleep(500000);
		path_TriggerWaypoint(TRAJ_COLLISION_REAR);
	}
}

