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
//! \file encoder.h
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/


#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "../types.h"

//! Initialisation
void encoder_Init(void);

//! Set distance between encoder
//! \param dist Encoder distance in meters
void encoder_SetDist(float dist);

//! Set encoder resolution and recompute value of vTops etc...
//! \param leftTicksPerM left encoder resolution in ticks per meter
//! \param rightTicksPerM right encoder resolution in ticks per meter
void encoder_SetResolution(uint32 leftTicksPerM, uint32 rightTicksPerM);

//! Get new encoders values
//! 
//! \param dLeft left motor motion
//! \param dRight right motor motion
//! \param dAlpha alpha motor motion
//! \param dDelta delta motor motion
void encoder_ReadSensor(int32 *dLeft, int32 *dRight, int32 *dAlpha, int32 *dDelta);

#endif//_ENCODER_H_
