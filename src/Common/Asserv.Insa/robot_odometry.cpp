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
//
// Modified by PM-ROBOTIX in CPP
/******************************************************************************/

#include <cmath>
#include <cstdio>

#include "AsservInsa.hpp"

void AsservInsa::odo_Integration(float dTheta, float dV)
{
	float newCosTheta;
	float newSinTheta;
	int tempo;

	Theta += dTheta;

	if (Theta > M_PI)
		Theta -= M_2PI;
	else if (Theta <= -M_PI)
		Theta += M_2PI;

	tempo = odoPeriodNb++ & 0x7F;

	//optimisation of the cos/sin computation :
	//every X integration period, we compute precise value for cosTheta and sinTheta
	if (tempo == 0)
	{
		newSinTheta = sinTheta + cosTheta * (dTheta);
		sinTheta = newSinTheta;

		//compute precise values for cos theta
		cosTheta = cosf(Theta);
	}
	else if (tempo == PRECISE_COMPUTATION_NTH_PERIOD)
	{
		newCosTheta = cosTheta - sinTheta * (dTheta);
		cosTheta = newCosTheta;

		//compute precise values for sin theta
		sinTheta = sinf(Theta);
	}
	else
	{
		//taylor decomposition of cos(x) and sin(x) at second order for x ~= a is :
		//cos(a)-sin(a)*(x-a)+O(x-a)^2
		//sin(a)-cos(a)*(x-a)+O(x-a)^2
		newCosTheta = cosTheta - sinTheta * (dTheta);
		newSinTheta = sinTheta + cosTheta * (dTheta);

		cosTheta = newCosTheta;
		sinTheta = newSinTheta;
	}
	//printf("odo_Integration %f,%f %f\n",xTops,yTops,Theta);
	xTops += dV * cosTheta;
	yTops += dV * sinTheta;

}

void AsservInsa::odo_SetPosition(float x, float y, float theta)
{
	printf("odo_SetPosition %f,%f %f\n", x, y, theta);
	//convert position from meters to ticks
	xTops = x / valueVTops;
	yTops = y / valueVTops;
	Theta = theta;
	cosTheta = cosf(Theta);
	sinTheta = sinf(Theta);
}

void AsservInsa::odo_GetPositionXYTheta(float *x, float *y, float *theta)
{
	if (x)
	{
		//convert position from ticks to meter
		*x = xTops * (float) valueVTops;
	}

	if (y)
	{
		//convert position from ticks to meter
		*y = yTops * (float) valueVTops;
	}

	if (theta)
	{
		*theta = Theta;
	}
}

RobotPosition AsservInsa::odo_GetPosition()
{
	RobotPosition pos;

	//convert position from ticks to meter
	pos.x = xTops * (float) valueVTops;
	pos.y = yTops * (float) valueVTops;
	pos.theta = Theta;

	return pos;
}
