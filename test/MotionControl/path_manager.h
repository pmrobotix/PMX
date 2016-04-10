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
//! \file path_manager.h
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#ifndef _PATH_MANAGER_H_
#define _PATH_MANAGER_H_

#include "../global.h"
#include "robot_trajectory.h"

//! possible return state of a trajectory
typedef enum
{
	TRAJ_OK,				//trajectory successfully completed
	TRAJ_ERROR,				//unknown error (not implemented !)
	TRAJ_COLLISION,			//trajectory interrupted because of a collision
	TRAJ_NEAR_OBSTACLE,		//trajectory interrupted because of a near collision
	TRAJ_CANCELLED,			//trajectory cancelled by remote user (for debug only)
	TRAJ_INTERRUPTED,		//trajectory interrupted by software
} TRAJ_STATE;

typedef enum
{
	WP_NOT_SET,				//no waypoint set for the moment

	WP_POS_ALPHA_SUPERIOR,	//waypoint triggered when alpha value is above threshold value
	WP_POS_ALPHA_INFERIOR,	//waypoint triggered when alpha value is below threshold value
	WP_POS_DELTA_SUPERIOR,	//waypoint triggered when delta value is above threshold value
	WP_POS_DELTA_INFERIOR,	//waypoint triggered when delta value is below threshold value

	WP_END_OF_TRAJ,			//waypoint when both motor command are finished
} WAYPOINT_TYPE;

//! structure used to represent trajectory waypoint
typedef struct
{
	WAYPOINT_TYPE type;
	int32 threshold;
} WAYPOINT;

extern WAYPOINT nextWaypoint;


//! Set the next waypoint
void path_SetNextWaypoint(WAYPOINT *wp);

//! \brief Wait for the current waypoint
//!
//! You must have called path_SetNextWaypoint before
TRAJ_STATE path_WaitWaypoint(void);

//! \brief Wait for the end of the current trajectory
//!
//! This is an utility function that call
//! path_SetNextWaypoint and path_WaitWaypoint()
TRAJ_STATE path_WaitEndOfTrajectory(void);

//! Execute trajectory, this is an asynchronous call
void path_LaunchTrajectory(RobotCommand *traj);

//! Cancel the current trajectory and stop the robot
//!
//! Mostly needed for remote control, use collision function
//! when a collision is detected.
void path_CancelTrajectory(void);

//! Interrupt current trajectory and stop robot
//!
//! Used to stop on a particular event (for instance actuator event)
void path_InterruptTrajectory(void);

#endif//_PATH_MANAGER_H_
