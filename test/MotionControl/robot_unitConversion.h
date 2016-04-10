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
//! \file robot_unitConversion.h
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#ifndef _ROBOT_UNIT_CONVERSION_H_
#define _ROBOT_UNIT_CONVERSION_H_

#include "../types.h"

// The following functions are helpers for convertion between units used
// in the motion control program :
//vTops is a virtual measure distance to avoid floating point computation
//sample is a time unit corresponding to one motion control period

//! \brief Compute the distance in vTops
//!
//! \param dist The distance in meter to convert
int32 convertDistTovTops(float dist);

//! \brief Compute the speed in vTops/sample
//!
//! \param speed The speed in meter/second to convert
int32 convertSpeedTovTopsPerPeriod(float speed);

//! \brief Compute the acceleration in vTops/sample^2
//!
//! \param accel The acceleration in meter/second^2 to convert
int32 convertAccelTovTopsPerPeriodSqd(float accel);

#endif//_ROBOT_UNIT_CONVERSION_H_
