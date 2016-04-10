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
//! \file clothoid.h
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#ifndef _CLOTHOID_H_
#define _CLOTHOID_H_


//! Taylor decomposition of FresnelC( sqrt(x/Pi) ) / FresnelS( sqrt(x/Pi) )
//! at x = Pi/2 and 3*Pi/2
//! the function should only be used with abs(x) > 0.5
float FresnelCSSqrt(float x);

//! Taylor decomposition of FresnelS( sqrt(x/Pi) )
//! at x = Pi
float FresnelSSqrt(float x);

//! Create a clothoid trajectory made of two line segments, smoothly linked by
//! an arc of clothoid
//! This is a test function
void CreateTwoSegmentTraj(float V0, float distD1, float distD2, float A0, float beta, float epsilon);

#endif//_CLOTHOID_H_

