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
//! \file clothoid.c
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#include <math.h>

#include "AsservInsa.hpp"

//! Taylor decomposition of FresnelC( sqrt(x/Pi) ) / sqrt(FresnelS( sqrt(x/Pi) ))
//! at x = Pi/2 and 3*Pi/2
//! the function should only be used with abs(x) > 0.1
float AsservInsa::FresnelCSSqrt(float x)
{
	float t2;
	float t3;
	float t7;

	if (x < 0)
	{
		return -FresnelCSSqrt(-x);
	}
	else if (x < 0.8f)
	{
		//code generated with maple
		t2 = -0.5235988 + x;
		t3 = t2 * t2;
		t7 = t3 * t3;

		return (0.2710734E1 - 0.1064432E1 * x + 0.121025E1 * t3 - 0.1761708E1 * t3 * t2 + 0.273796E1 * t7
				- 0.4446194E1 * t7 * t2);
	}
	else
	{
		//code generated with maple
		t2 = -0.2356194E1 + x;
		t3 = t2 * t2;
		t7 = t3 * t3;

		return (0.1941357E1 - 0.2476891 * x + 0.3094215E-1 * t3 - 0.115775E-1 * t3 * t2 + 0.4600835E-2 * t7
				- 0.1644186E-2 * t7 * t2);
	}
}

//! Taylor decomposition of FresnelS( sqrt(x/Pi) )
//! at x = Pi
float AsservInsa::FresnelSSqrt(float x)
{
	float t2;
	float t3;
	float t7;

	//code generated with maple
	t2 = -0.3141593E1 + x;
	t3 = t2 * t2;
	t7 = t3 * t3;
	return (-0.6174085E-1 + 0.1591549 * x - 0.1266515E-1 * t3 - 0.4615735E-2 * t3 * t2 + 0.3905568E-3 * t7
			+ 0.2106704E-4 * t7 * t2);
}

void AsservInsa::CreateTwoSegmentTraj(float V0, float distD1, float distD2, float A0, float beta, float epsilon)
{
	RobotCommand cmdLine1, cmdLine2, cmdClot1, cmdClot2;
	float deltaX, accel, T;
	float D;

	if (beta > 0)
	{
		deltaX = epsilon * (1 / tan((M_PI - beta) / 2.0) + FresnelCSSqrt(beta));
		accel = distEncoderMeter * V0 * V0 / 2.0 * M_PI / (epsilon * epsilon) * FresnelSSqrt(beta);
		T = sqrtf((distEncoderMeter * beta) / (2 * accel));
	}
	else
	{
		beta = -beta;
		deltaX = epsilon * (1 / tan((M_PI - beta) / 2.0) + FresnelCSSqrt(beta));
		accel = -distEncoderMeter * V0 * V0 / 2.0 * M_PI / (epsilon * epsilon) * FresnelSSqrt(beta);
		T = sqrtf((distEncoderMeter * beta) / (-2 * accel));
	}

	D = distD1 - deltaX;
	if (D < 0.0f)
		D = 0.0f;
	motion_SpeedControlLR(&cmdLine1, V0, D, A0, V0, D, A0);

	motion_SpeedControlLRTime(&cmdClot1, V0, T, -accel, V0, T, accel);

	motion_SpeedControlLRTime(&cmdClot2, V0 - accel * T, T, accel, V0 + accel * T, T, -accel);
	D = distD2 - deltaX;
	if (D < 0.0f)
		D = 0.0f;
	motion_SpeedControlLRV0(&cmdLine2, V0, V0, D, A0, V0, V0, D, A0);

	path_LaunchTrajectory(&cmdLine1);
	path_WaitEndOfTrajectory();

	path_LaunchTrajectory(&cmdClot1);
	path_WaitEndOfTrajectory();

	path_LaunchTrajectory(&cmdClot2);
	path_WaitEndOfTrajectory();

	path_LaunchTrajectory(&cmdLine2);
	path_WaitEndOfTrajectory();
}
