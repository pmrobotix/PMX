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
//! \file robot_odometry.h
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#ifndef _ROBOT_ODOMETRY_H_
#define _ROBOT_ODOMETRY_H_

#include "../types.h"
#include "../global.h"


//! \brief Compute robot position using odometry
//!
//! \param dTheta displacement (rotation) of the robot in radian 
//! \param dV displacement (forward) of the robot in vTops
EXPORTED_FUNCTION void odo_Integration(float dTheta, float dV);

//! \brief Set robot position used by odometry
//!
//! \param dL vTops elapsed on left encoder since last integration
//! \param dR vTops elapsed on right encoder since last integration
EXPORTED_FUNCTION void odo_SetPosition(float x, float y, float theta);

//! \brief Get current odometry robot position
//!
//! \param x [out] Robot position on x axis in meters
//! \param y [out] Robot position on y axis in meters
//! \param theta [out] Robot orientation in radians ]-Pi..Pi]
EXPORTED_FUNCTION void odo_GetPositionXYTheta(float *x, float *y, float *theta);

//! \brief Get current odometry robot position
//!
//! \return Robot position in meters and angle in radian
EXPORTED_FUNCTION RobotPosition odo_GetPosition(void);


#endif//_ROBOT_ODOMETRY_H_
