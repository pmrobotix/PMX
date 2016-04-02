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
//! \file robot_traj_wrappers.c
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
//
// Modified by PM-ROBOTIX in CPP
/******************************************************************************/

#include <stddef.h>
#include <cmath>

#include "AsservInsa.hpp"


TRAJ_STATE AsservInsa::motion_findPidAD(float angle_rad, float meters, int sec)
{
	checkRobotCommand(&cmd);
	motion_StepOrderAD(&cmd, convertDistTovTops(angle_rad * distEncoderMeter / 2.0f), (int32)(meters / valueVTops), sec);
	path_LaunchTrajectory(&cmd);
	return path_WaitEndOfTrajectory();
}

TRAJ_STATE AsservInsa::motion_DoLine(float dist_meters)
{
	checkRobotCommand(&cmd);
	motion_Line(&cmd, dist_meters);
	path_LaunchTrajectory(&cmd);
	return path_WaitEndOfTrajectory();
}

TRAJ_STATE AsservInsa::motion_DoRotate(float angle)
{
	checkRobotCommand(&cmd);
	motion_Rotate(&cmd, angle);
	path_LaunchTrajectory(&cmd);
	return path_WaitEndOfTrajectory();
}

TRAJ_STATE AsservInsa::motion_DoArcRotate(float angle, float radius)
{
	checkRobotCommand(&cmd);
	motion_ArcRotate(&cmd, angle, radius);
	path_LaunchTrajectory(&cmd);
	return path_WaitEndOfTrajectory();
}

void AsservInsa::computeAlphaDelta(float x1,
		float y1,
		float theta1,
		float x2,
		float y2,
		float *out_angle,
		float *out_dist)
{
	float dx, dy;
	float dTheta;

	dx = x2 - x1;
	dy = y2 - y1;

	//s'il n'y a pas de deplacement a faire,
	//et bien on en fait pas...
	if (dx == 0.0f && dy == 0.0f)
	{
		*out_angle = 0.0f;
		*out_dist = 0.0f;
		return;
	}

	//angle to run to reach position
	dTheta = atan2f(dy, dx) - theta1;

	//bound angle value to [-Pi ; Pi]
	if (dTheta < -M_PI)
		*out_angle = dTheta + M_2PI;
	else if (dTheta > M_PI)
		*out_angle = dTheta - M_2PI;
	else
		*out_angle = dTheta;

	//distance to run
	*out_dist = sqrtf(dx * dx + dy * dy);
}

TRAJ_STATE AsservInsa::motion_GoToSpeed(float x, float y)
{
	Position pos;
	pos.x = x;
	pos.y = y;

	return motion_GoToSpeedPath(&pos, 1);
}

TRAJ_STATE AsservInsa::motion_GoToSpeedPath(const Position *pos, int nb)
{
	TRAJ_STATE state;
	float spAlpha, spDelta;
	float x1, y1, theta1;
	float dist;
	float angle;
	int i = 0;

	if (nb == 0)
		return TRAJ_OK;

	//compute displacement needed from current position
	pos_GetPositionXYTheta(&x1, &y1, &theta1);
	computeAlphaDelta(x1, y1, theta1, pos[i].x, pos[i].y, &angle, &dist);

	while (i < nb)
	{
		if ((fabs(angle) >= 0.5f) || (dist >= 0.10f))
		{
			spAlpha = angle / 4.0f;

			if (fabs(angle) < 0.5f)
				spDelta = 0.30f;
			else if (dist <= 0.1f)
				spDelta = 0.1f;
			else
				spDelta = 0.0f;

			if (spAlpha > 0.3f)
				spAlpha = 0.3f;
			else if (spAlpha < -0.3f)
				spAlpha = -0.3f;

			checkRobotCommand(&cmd);
			motion_SpeedControlADMaxTime(&cmd, spAlpha, 0.5f, spDelta, 0.5f, 50);
			path_LaunchTrajectory(&cmd);

			//compute displacement needed from current position during waiting time
			pos_GetPositionXYTheta(&x1, &y1, &theta1);
			computeAlphaDelta(x1, y1, theta1, pos[i].x, pos[i].y, &angle, &dist);

			state = path_WaitEndOfTrajectory();
			if (state != TRAJ_OK)
			{
				motion_FreeMotion();
				return state;
			}
		}
		else
		{
			i++;

			if (i < nb)
			{
				//compute displacement
				pos_GetPositionXYTheta(&x1, &y1, &theta1);
				computeAlphaDelta(x1, y1, theta1, pos[i].x, pos[i].y, &angle, &dist);
			}
		}
	}

	motion_FreeMotion();
	return TRAJ_OK;
}

//TSL = Turn - Stop - Line
TRAJ_STATE AsservInsa::motion_GoToTSLFrom(float x1, float y1, float theta1, float x2, float y2)
{
	TRAJ_STATE state;

	float dist;
	float angle;

	computeAlphaDelta(x1, y1, theta1, x2, y2, &angle, &dist);

	//rotation	
	state = motion_DoRotate(angle);
	if (state != TRAJ_OK)
		return state;

	//line
	return motion_DoLine(dist);
}

TRAJ_STATE AsservInsa::motion_GoToTSL(float x, float y)
{
	float x1;
	float y1;
	float theta1;

	pos_GetPositionXYTheta(&x1, &y1, &theta1);

	return motion_GoToTSLFrom(x1, y1, theta1, x, y);
}

TRAJ_STATE AsservInsa::motion_OrientTo(float angle)
{
	float theta, dTheta;

	pos_GetPositionXYTheta(NULL, NULL, &theta);

	dTheta = angle - theta;

	//bound angle value to [-Pi ; Pi]
	if (dTheta < -M_PI)
		dTheta += M_2PI;
	else if (dTheta > M_PI)
		dTheta -= M_2PI;

	return motion_DoRotate(dTheta);
}
