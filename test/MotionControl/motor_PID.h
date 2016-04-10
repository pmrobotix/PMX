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
//! \file motor_PID.h
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#ifndef _MOTOR_PID_H_
#define _MOTOR_PID_H_

#include "../types.h"

//PID system are identified by a number
typedef uint8 PID_SYSTEM;

//structure used to stock configuration values of the PID system
typedef struct
{
	uint32 kP;			//proportionnal coefficient (unit = 1/vTops)
	uint32 kI;			//integral coefficient (unit = 1/vTops/Sample)
	uint32 kD;			//derivation coefficient (unit = Sample/vTops)

	int16 dPeriod;		//derivation interval, must be smaller than MAX_D_PERIOD (unit = Sample)
	int32 iMax;			//absolute value of the bound of sum of errors (unit = vTops)
} pidConfig;


//! \brief Initialisation of the PID system
EXPORTED_FUNCTION void pid_Init(void);

//! \brief Create a new PID system
//! \return The new system ID
EXPORTED_FUNCTION PID_SYSTEM pid_Create(void);

//! \brief Configure the PID system
//!
//! \param system The system that is configured
//! \param config The new configuration
EXPORTED_FUNCTION void pid_Configure(PID_SYSTEM system, pidConfig config);

//! \brief Configure kP value of the PID system
EXPORTED_FUNCTION void pid_ConfigKP(PID_SYSTEM system, uint32 kp);
//! \brief Configure kI value of the PID system
EXPORTED_FUNCTION void pid_ConfigKI(PID_SYSTEM system, uint32 ki);
//! \brief Configure kD value of the PID system
EXPORTED_FUNCTION void pid_ConfigKD(PID_SYSTEM system, uint32 kd);
//! \brief Configure dPeriod value of the PID system
EXPORTED_FUNCTION void pid_ConfigDPeriod(PID_SYSTEM system, uint8 dperiod);
//! \brief Configure iMax value of the PID system
EXPORTED_FUNCTION void pid_ConfigIMax(PID_SYSTEM system, int32 imax);

//! \brief Get the current configuration of the PID system
//!
//! \param system The system
//! \return The current configuration of the system
EXPORTED_FUNCTION pidConfig pid_GetConfig(PID_SYSTEM system);


//! \brief Compute the PID sum of a system
//!
//! \param system The system on which we compute the PID
//! \param error The difference between measured and reference values
//! \return The new command to apply on the system (pwm)
EXPORTED_FUNCTION int32 pid_Compute(PID_SYSTEM system, int32 error);

//! \brief Reset the integral error
//!
//! When for example the power supply of the motors is shut down,
//! the pid accumulate error (integral term), if the power is
//! set up again after a while, the motor will try to correct
//! abruptly this error. To avoid this problem, we provide the
//! following function to reset the integral of the error and
//! all previous error for differential.
//! \param system The system that will be reseted
EXPORTED_FUNCTION void pid_ResetError(PID_SYSTEM system);


#endif//_MOTOR_PID_H_
