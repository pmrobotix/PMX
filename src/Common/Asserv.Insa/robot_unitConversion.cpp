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
//! \file robot_unitConversion.c
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
//
// Modified by PM-ROBOTIX in CPP
/******************************************************************************/

#include <cmath>

#include "AsservInsa.hpp"

int32 AsservInsa::convertDistTovTops(float meters)
{
	float vTops = meters / valueVTops;
	//= m * vtops/m = vtops
	return round(vTops);
}

int32 AsservInsa::convertSpeedTovTopsPerPeriod(float vMax)
{
	float vTops = (loopDelayInMillis / 1000.0) * vMax / valueVTops;
	//=sec/period * m/sec / m/vtops
	//=sec/period * m/sec * vtops/m = vtops/period
	return round(vTops);
}

int32 AsservInsa::convertAccelTovTopsPerPeriodSqd(float accel)
{
	int32 result;
	float vTops = (loopDelayInMillis / 1000.0) * (loopDelayInMillis / 1000.0) * accel / valueVTops;
	//=sec/period * sec/period * m/s^2 * vtops/m = vtops/period^2
	result = round(vTops);

	//acceleration must be non-null
	if (result == 0)
	{
		return 1;
	}
	else
	{
		return result;
	}
}
