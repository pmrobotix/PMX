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
//! \file robot_slippage.h
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#ifndef _ROBOT_SLIPPAGE_H_
#define _ROBOT_SLIPPAGE_H_

#include "../types.h"
#include "../exception_handler.h"

//! Initialise module
void slippage_Init(void);

//! Update current slippage value for a motor
//! \param motor Motor that will be updated (should be LEFT_MOTOR or RIGHT_MOTOR)
//! \param dWheel displacement value of the wheel (motor encoder)
//! \param dRobot displacement value of the robot (wheel encoder / mouse sensor)
void slippage_Update(int motor, int32 dWheel, int32 dRobot);

#endif//_ROBOT_SLIPPAGE_H_