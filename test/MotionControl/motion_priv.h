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
//! \file motion_priv.h
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#ifndef _MOTION_PRIV_H_
#define _MOTION_PRIV_H_

#include "motor_PID.h"


//! Macro that correct integer overflow for a difference of two value
//! typically when you calculate an error using error = new-old, and new has overflowed
//! the macro substract or add the maximum value if needed to compensate the overflow of new
#define CORRECT_DELTA_OVERFLOW(value, max_value) {if((value)>(max_value/2))value-=max_value; else if((value)<(-(max_value/2)))value+=max_value;}


//! Macro that bound an integer to a maximum absolute value
//! for instance :
//! BOUND_INT(i, 100) with i==143 set i=100
//! BOUND_INT(i, 100) with i==-160 set i=-100
//! BOUND_INT(i, 100) with i==-16 set i=-16
#define BOUND_INT(value, max_abs) {if(value>max_abs)value=max_abs; else if(value<-max_abs)value=-max_abs;}
			
			
//different kind of command generation
typedef enum
{
	POSITION_COMMAND,
	SPEED_COMMAND,
	STEP_COMMAND,
} MOTOR_COMMAND_TYPE;

//different kind of command interpretation (virtual motors)
typedef enum
{
	LEFT_RIGHT,
	ALPHA_DELTA,
	
	MAX_MOTION_CONTROL_TYPE_NUMBER,
} MOTION_CONTROL_TYPE;


#endif//_MOTION_PRIV_H_
