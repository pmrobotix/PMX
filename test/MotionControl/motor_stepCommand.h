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
//! \file motor_stepOrder.c
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#ifndef _MOTOR_STEP_ORDER_H_
#define _MOTOR_STEP_ORDER_H_

#include "../types.h"

typedef int32 StepCommand;


//! Set a step movement for a given motor
//! (for PID tuning purpose)
//!
//! \param out_command Pointer to the StepOrder structure that will contain the order
//! \param steporder Order that will be applied on the PID system
void ComputeStepOrder(StepCommand *out_command, int32 steporder);

//! Return the current motor order for a step movement
//! (for PID tuning purpose)
//!
//! \param motor Id number of motor
//! \param order StepOrder that will be interpreted
BOOL GetStepOrder(StepCommand *order, int32 *out_order);


#endif//_MOTOR_STEP_ORDER_H_
