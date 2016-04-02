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
//
// Modified by PM-ROBOTIX in CPP
/******************************************************************************/

#include <stddef.h>

#include "../../Log/Logger.hpp"
#include "../Asserv/EncoderControl.hpp"
#include "../Asserv/MovingBase.hpp"
#include "AsservInsa.hpp"


void AsservInsa::encoder_Init()
{
	if (base_ != NULL)
	{
		base_->extEncoders().reset();
		base_->encoders().reset();
		lastRight_ = 0;
		lastLeft_ = 0;
	}
	else
	{
		logger().error() << "encoder_Init : Base is NULL !" << logs::end;
	}
}

void AsservInsa::encoder_SetDist(float dist)
{
	distEncoderMeter = dist;
	distEncoder = dist / valueVTops;
}

void AsservInsa::encoder_ReadSensor(int32 *dLeft, int32 *dRight, int32 *dAlpha, int32 *dDelta, int32 *left, int32 *right)
{
	//int32 left = 0;
	//int32 right = 0;
	if (base_ != NULL)
	{
		//read encoder
		if (useExternalEncoders_)
		{
			*left = base_->extEncoders().getLeftEncoder();
		}
		else
		{
			*left = base_->encoders().getLeftEncoder();
		}

		if (useExternalEncoders_)
		{
			*right = base_->extEncoders().getRightEncoder();
		}
		else
		{
			*right = base_->encoders().getRightEncoder();
		}

	}
	else
	{
		logger().error() << "encoder_ReadSensor : Base is NULL !" << logs::end;
	}

#ifdef DEBUG_ENCODER
	printf("encoder.c encoder_ReadSensor l:%d r:%d\n", *left, *right);
#endif

//convert units and save position
	*left *= leftEncoderRatio;
	*right *= rightEncoderRatio;

//compute delta for left wheel
	//if (abs(left - lastLeft) < 1000000) //ajout de chaff
	//{
	*dLeft = *left - lastLeft_;
	lastLeft_ = *left;
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
	*dRight = *right - lastRight_;
	lastRight_ = *right;
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

}


