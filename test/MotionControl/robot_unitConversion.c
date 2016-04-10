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
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#include "robot_unitConversion.h"
#include "../global.h"
#include "motion_calibration.h"


int32 convertDistTovTops(float dist)
{
    float tmp = dist/valueVTops;
    return round(tmp);
}

int32 convertSpeedTovTopsPerPeriod(float speed)
{
    float tmp = valueSample*speed/valueVTops;
    return round(tmp);
}

int32 convertAccelTovTopsPerPeriodSqd(float accel)
{
	int32 result;
    float tmp = valueSample*valueSample*accel/valueVTops;
	result = round(tmp);

	//acceleration must be non-null
    if(result == 0)
    {
        return 1;
    }
	else
	{
		return result;
	}
}
