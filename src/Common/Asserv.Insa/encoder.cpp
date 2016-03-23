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

#include "../Asserv/EncoderControl.hpp"
#include "../Asserv/MovingBase.hpp"
#include "AsservInsa.hpp"

int32 lastLeft = 0;
int32 lastRight = 0;
int useExternalEncoders = 0;

void AsservInsa::encoder_Init()
{
	base_->extEncoders().reset();
	base_->encoders().reset();
}

void AsservInsa::encoder_SetDist(float dist)
{
	distEncoderMeter = dist;
	distEncoder = dist / valueVTops;
}

void AsservInsa::encoder_SetResolution(uint32 leftTicksPerM, uint32 rightTicksPerM)
{

	if (leftTicksPerM > rightTicksPerM)
	{
		leftEncoderRatio = vtopsPerTicks;
		rightEncoderRatio = vtopsPerTicks * rightTicksPerM / (float) leftTicksPerM;
		valueVTops = 1 / (float) (vtopsPerTicks * leftTicksPerM);
	}
	else
	{
		leftEncoderRatio = vtopsPerTicks * leftTicksPerM / (float) rightTicksPerM;
		rightEncoderRatio = vtopsPerTicks;
		valueVTops = 1 / (float) (vtopsPerTicks * rightTicksPerM);
	}
#ifdef DEBUG_MOTION
	printf("encoder_SetResolution   %d , %d \n", leftTicksPerM, rightTicksPerM);
	printf("valueVTops %f = 1 / %f = 1 / %d x %d \n", valueVTops,
			(float) (VTOPS_PER_TICKS * rightTicksPerM), VTOPS_PER_TICKS,
			rightTicksPerM);
#endif

	//recompute values involving vTops
	encoder_SetDist(distEncoderMeter);
}

void AsservInsa::encoder_ReadSensor( int32 *dLeft, int32 *dRight, int32 *dAlpha, int32 *dDelta)
{
	int32 left, right;

	//read encoder
	if (useExternalEncoders)
	{
		left = base_->extEncoders().getLeftEncoder();
	}
	else
	{
		left = base_->encoders().getLeftEncoder();
	}

	if (useExternalEncoders)
	{
		right = base_->extEncoders().getRightEncoder();
	}
	else
	{
		right = base_->encoders().getRightEncoder();
	}

#ifdef DEBUG_ENCODER
	printf("encoder.c encoder_ReadSensor l:%d r:%d\n", left, right);
#endif

//convert units and save position
	left *= leftEncoderRatio;
	right *= rightEncoderRatio;

//compute delta for left wheel
	//if (abs(left - lastLeft) < 1000000) //ajout de chaff
	//{
	*dLeft = left - lastLeft;
	lastLeft = left;
	//}
	//else
	//{
	//*dLeft = 0;
	//}
//verify left encoder overflow
//CORRECT_DELTA_OVERFLOW(*dLeft, MAX_ENCODER_ABS_VTOPS);

//compute delta for right wheel
	//if (abs(right - lastRight) < 1000000)
	//{
	//*dRight = right - lastRight;
	//lastRight = right;
	//}
	//else
	//{
	//*dRight = 0;
	//}
//verify right encoder overflow
//CORRECT_DELTA_OVERFLOW(*dRight, MAX_ENCODER_ABS_VTOPS);

//compute alpha and delta displacement
	*dAlpha = (*dRight - *dLeft) / 2;
	*dDelta = (*dRight + *dLeft) / 2;
#ifdef DEBUG_ENCODER
	printf("encoder.c encoder_ReadSensor l:%d r:%d alpha:%d delta:%d\n", *dLeft, *dRight, *dAlpha, *dDelta);
	printf("%f %f\n", cc_getX(), cc_getY());
#endif
}

