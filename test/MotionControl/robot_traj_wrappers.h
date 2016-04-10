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
//! \file robot_traj_wrappers.h
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#ifndef _ROBOT_TRAJ_WRAPPERS_H_
#define _ROBOT_TRAJ_WRAPPERS_H_

#include "robot_trajectory.h"
#include "path_manager.h"

//! This module provide higher level control on the motion control module
//! AI and strategy software can use this primitives directly

//! Compute and launch trajectory
//! \sa motion_Line
TRAJ_STATE motion_DoLine(float dist);

//! Compute and launch trajectory
//! \sa motion_Rotate
TRAJ_STATE motion_DoRotate(float angle);

//! Compute and launch trajectory
//! \sa motion_ArcRotate
TRAJ_STATE motion_DoArcRotate(float angle, float radius);

//! Compute the displacement needed on alpha and delta to reach the point x2, y2
//! from the point x1, y1 starting with the orientation theta1
void computeAlphaDelta(float x1, float y1, float theta1, float x2, float y2, float *out_angle, float *out_dist);

//! macro that translate movement for arc circle trajectory
#define motion_DoArcLeftForward(angle, radius) motion_DoArcRotate(fabs(angle), fabs(radius))
#define motion_DoArcRightForward(angle, radius) motion_DoArcRotate(-fabs(angle), -fabs(radius))
#define motion_DoArcLeftBackward(angle, radius) motion_DoArcRotate(-fabs(angle), fabs(radius))
#define motion_DoArcRightBackward(angle, radius) motion_DoArcRotate(fabs(angle), -fabs(radius))

//! Goto (x2,y2) from (x1,y1,theta1) using Turn-Stop-Line motion
TRAJ_STATE motion_GoToTSLFrom(float x1, float y1, float theta1, float x2, float y2);

//! Goto (x,y) using Turn-Stop-Line motion
TRAJ_STATE motion_GoToTSL(float x, float y);

//! Goto (x,y) using speed motion
TRAJ_STATE motion_GoToSpeed(float x, float y);

//! Goto a list of (x,y) position using speed motion
TRAJ_STATE motion_GoToSpeedPath(const Position *pos, int nb);

//! Orient toward the given angle
TRAJ_STATE motion_OrientTo(float angle);

#endif//_ROBOT_TRAJ_WRAPPERS_H_
