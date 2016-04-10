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
//! \file robot_odometry.c
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#include "robot_odometry.h"

#include "motion_calibration.h"

#include "../log.h"

#include <math.h>

//every N period, we compute precise values for
//cos and sin of the robot angle
#define PRECISE_COMPUTATION_NTH_PERIOD		64

#ifndef M_2PI
	#define M_2PI							(2*M_PI)
#endif

static float Theta;					//rad
static float cosTheta, sinTheta;	//no unit
static float xTops, yTops;		//encoder vTops

static int32 odoPeriodNb = 0;

	
EXPORTED_FUNCTION void odo_Integration(float dTheta, float dV)
{
	float newCosTheta;
	float newSinTheta;
	int tempo;
	
	Theta += dTheta;
	
	if ( Theta > M_PI )
		Theta -= M_2PI;
	else if ( Theta <= -M_PI )
		Theta += M_2PI;

	tempo = odoPeriodNb++ & 0x7F;

	//optimisation of the cos/sin computation :
	//every X integration period, we compute precise value for cosTheta and sinTheta
	if(tempo == 0)
	{		
		newSinTheta = sinTheta+cosTheta*(dTheta);
		sinTheta = newSinTheta;
		
		//compute precise values for cos theta
		cosTheta = cosf(Theta);
	}
	else if(tempo == 0x40)
	{
		newCosTheta = cosTheta-sinTheta*(dTheta);
		cosTheta = newCosTheta;
		
		//compute precise values for sin theta
		sinTheta = sinf(Theta);
	}
	else
	{
		//taylor decomposition of cos(x) and sin(x) at second order for x ~= a is :
		//cos(a)-sin(a)*(x-a)+O(x-a)^2
		//sin(a)-cos(a)*(x-a)+O(x-a)^2
		newCosTheta = cosTheta-sinTheta*(dTheta);
		newSinTheta = sinTheta+cosTheta*(dTheta);
		
		cosTheta = newCosTheta;
		sinTheta = newSinTheta;
	}
	
	xTops += dV*cosTheta;
	yTops += dV*sinTheta;
}

EXPORTED_FUNCTION void odo_SetPosition(float x, float y, float theta)
{
	//convert position from meters to ticks
	xTops = x/valueVTops;
	yTops = y/valueVTops;
	Theta = theta;		
	cosTheta = cosf(Theta);
	sinTheta = sinf(Theta);
}

EXPORTED_FUNCTION void odo_GetPositionXYTheta(float *x, float *y, float *theta)
{
	if(x)
	{
		//convert position from ticks to meter
		*x = xTops * (float)valueVTops;
	}

	if(y)
	{
		//convert position from ticks to meter
		*y = yTops * (float)valueVTops;
	}

	if(theta)
	{
		*theta = Theta;
	}
}

EXPORTED_FUNCTION RobotPosition odo_GetPosition()
{
	RobotPosition pos;

	//convert position from ticks to meter
	pos.x = xTops * (float)valueVTops;
	pos.y = yTops * (float)valueVTops;
	pos.theta = Theta;

	return pos;
}
