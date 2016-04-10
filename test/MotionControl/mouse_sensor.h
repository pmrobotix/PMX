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
//! \file mouse_sensor.h
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#ifndef _MOUSE_SENSOR_H_
#define _MOUSE_SENSOR_H_

#include "../types.h"

//! Initialisation of I/O port for communication with mouse sensor
void mouse_Init(void);

//! Read both sensor and compute dLeft, dRight, dAlpha, dDelta
//! return whether the reading have been successfull or not
BOOL mouse_ReadSensor(int32 *dLeft, int32 *dRight, int32 *dAlpha, int32 *dDelta);

//! Test
BOOL mouse_ReadSensor_debug(int32 *dLeft, int32 *dRight, int32 *dAlpha, int32 *dDelta);

//! Update sum of motion on mice for calibration purpose
//! Don't use this function and mouse_ReadSensor in the same time
void mouse_UpdateCalibrationValues(void);

//! Run the mouse calibration procedure
void mouse_Calibration(int lineNb, float lineDist, int turnNb);
//! New calibration test
void mouse_Calibration2(int turnNb);

//! set calibration step i (synchronisation with user)
void mouseCalibSetStep(int i);


#endif//_MOUSE_SENSOR_H_
