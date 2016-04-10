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
//! \file encoder.c
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/


#include "encoder.h"

#include "motion_calibration.h"
#include "../lm629.h"

//some values goes in pair :
//the first one is the measured/real one
//the second one is an integer approximation using funny units (vTops and sample)


//ratio vTops/ticks for left encoder
uint32 leftEncoderRatio;
//ratio vTops/ticks for right encoder
uint32 rightEncoderRatio;

//distance between both encoder wheels in vTops
uint32 distEncoder;
//distance between both encoder wheels in meters
float distEncoderMeter;

void encoder_Init()
{
	lm629_Load();
}


void encoder_SetDist(float dist)
{
	distEncoderMeter = dist;
	distEncoder = dist / valueVTops;
}


void encoder_SetResolution(uint32 leftTicksPerM, uint32 rightTicksPerM)
{
	if(leftTicksPerM > rightTicksPerM)
	{
		leftEncoderRatio = VTOPS_PER_TICKS;
		rightEncoderRatio = VTOPS_PER_TICKS * rightTicksPerM/(float)leftTicksPerM;
		valueVTops = 1/(float)(VTOPS_PER_TICKS * leftTicksPerM);
	}
	else
	{
		leftEncoderRatio = VTOPS_PER_TICKS * leftTicksPerM/(float)rightTicksPerM;
		rightEncoderRatio = VTOPS_PER_TICKS;
		valueVTops = 1/(float)(VTOPS_PER_TICKS * rightTicksPerM);
	}
	
	//recompute values involving vTops
	encoder_SetDist(distEncoderMeter);
}

static int32 lastLeft = 0;
static int32 lastRight = 0;
void encoder_ReadSensor(int32 *dLeft, int32 *dRight, int32 *dAlpha, int32 *dDelta)
{
	static float dTheta;
	static float dV;
	static int32 left, right;

	//read encoder
	left = -lm629_ReadRealPosition( LWHEEL );
	right = lm629_ReadRealPosition( RWHEEL );

	//convert units and save position
	left *= leftEncoderRatio;
	right *= rightEncoderRatio;

	//compute delta for left wheel
	*dLeft = left - lastLeft;
	lastLeft = left;
	//verify left encoder overflow
	CORRECT_DELTA_OVERFLOW(*dLeft, MAX_ENCODER_ABS_VTOPS);

	//compute delta for right wheel
	*dRight = right - lastRight;
	lastRight = right;
	//verify right encoder overflow
	CORRECT_DELTA_OVERFLOW(*dRight, MAX_ENCODER_ABS_VTOPS);
		
	//compute alpha and delta displacement
	*dAlpha = (*dRight-*dLeft)/2;
	*dDelta = (*dRight+*dLeft)/2;
}

