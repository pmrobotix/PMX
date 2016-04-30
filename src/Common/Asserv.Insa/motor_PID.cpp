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
//! \file motor_PID.c
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
//
// Modified by PM-ROBOTIX in CPP
/******************************************************************************/

#include <stdio.h>

#include "AsservInsa.hpp"

void AsservInsa::pid_Init()
{
	pid_Nb = 0;
}

PID_SYSTEM AsservInsa::pid_Create()
{
	//no more available system
	if (pid_Nb >= MAX_PID_SYSTEM_NUMBER)
		return -1;

	//init configuration variables by default
	systemValues[pid_Nb].conf.kP = 0.0;
	systemValues[pid_Nb].conf.kI = 0.0;
	systemValues[pid_Nb].conf.kD = 0.0;

//	logger().debug() << "pid_Create pid_Nb=" << (int) pid_Nb
//			<< " systemValues[pid_Nb].conf.kP="
//			<< systemValues[pid_Nb].conf.kP
//			<< " systemValues[pid_Nb].conf.kI="
//			<< systemValues[pid_Nb].conf.kI
//			<< " systemValues[pid_Nb].conf.kD="
//			<< systemValues[pid_Nb].conf.kD
//			<< logs::end;

//init current state variables

	pid_Nb++;
	return pid_Nb - 1;
}

void AsservInsa::pid_Config(PID_SYSTEM system, float kp, float ki, float kd)
{

	//double SampleTimeInSec = ((double) SampleTime) / 1000;
	systemValues[system].lastTime = 0;
	systemValues[system].lastOutput = 0.0;
	systemValues[system].lastInput = 0.0;
	systemValues[system].conf.kP = kp;
	systemValues[system].conf.kI = ki * loopDelayInMillis / 1000.0;
	systemValues[system].conf.kD = kd / (loopDelayInMillis / 1000.0);

	logger().debug() << "pid_Config system="
			<< (int) system
			<< " systemValues[system].conf.kP="
			<< systemValues[system].conf.kP
			<< " systemValues[system].conf.kI="
			<< systemValues[system].conf.kI
			<< " systemValues[system].conf.kD="
			<< systemValues[system].conf.kD
			<< logs::end;

}

//void SetOutputLimits(PID_SYSTEM system,double Min, double Max) {
//	if (Min > Max)
//		return;
//	outMin = Min;
//	outMax = Max;
//
//	if (Output > outMax)
//		Output = outMax;
//	else if (Output < outMin)
//		Output = outMin;
//
//	if (ITerm > outMax)
//		ITerm = outMax;
//	else if (ITerm < outMin)
//		ITerm = outMin;
//}

/*working variables*/
//unsigned long lastTime;
//double Input, Output, Setpoint;
//double ITerm, lastInput;
/**
 * SetPoint : value to reach
 * Input : current value
 *
 * */
float AsservInsa::pid_Compute(PID_SYSTEM system, float setpoint, float input, float speed)
{
	float outMin = (-1.0 * maxPwmValue_);
	float outMax = (1.0 * maxPwmValue_);

	pidSystemValues* val = &(systemValues[system]);
	pidConfig conf = val->conf;

	float outPut = val->lastOutput;
	float lastInput = val->lastInput;
	float ITerm = val->ITerm;

	unsigned long now = currentTimeInMillis();
	unsigned long lastTime = val->lastTime;

	long timeChange = (now - lastTime);
	//printf("go timeChange=%ld loopDelayInMillis=%ld\n", timeChange, loopDelayInMillis);
	if (timeChange >= loopDelayInMillis)
	{
		float kp = conf.kP;
		float ki = conf.kI;
		float kd = conf.kD;

		/*Compute all the working error variables*/
		float error = setpoint - input;
		ITerm += (ki * error);
		if (ITerm > outMax)
			ITerm = outMax;
		else if (ITerm < outMin)
			ITerm = outMin;
		float dInput = (input - lastInput);

		/*Compute PID Output*/
		outPut = kp * error + ITerm - kd * dInput;
		if (outPut > outMax)
			outPut = outMax;
		else if (outPut < outMin)
			outPut = outMin;

		/*Remember some variables for next time*/
		val->lastInput = input;
		val->lastTime = now;
		val->ITerm = ITerm;

	}
	else
	{
		logger().debug() << "pid_Compute - P="
				<< conf.kP
				<< " D="
				<< conf.kD
				<< " input="
				<< input
				<< " setpoint="
				<< setpoint
				<< " speed="
				<< speed
				<< " outPut="
				<< outPut
				<< logs::end;
		/*
		 #ifdef DEBUG_PID
		 printf("conf.kP %f %f %f \n", conf.kP, conf.kI, conf.kD);
		 //printf("No pid calculation: %ld - %ld = %d ms\n", now, lastTime, timeChange);
		 #endif*/
	}
	val->lastOutput = outPut;
	return outPut;
}

// RCVA
int32 AsservInsa::pid_Compute_rcva_chaff(PID_SYSTEM system, int32 error, double vitesse)
{
	pidSystemValues * val;
	double P, I, D;
	double pwm;

	error /= vtopsPerTicks;

	//printf("motor_PID.c pid_Compute %d, err:%d ", system, error);
	val = &(systemValues[system]);

	printf("motor_PID.c pid_Compute pid error:%d kP:%f (vitesse:%f)\n", error, val->conf.kP, vitesse);

	P = error * val->conf.kP;
	I = 0;
	float kd = (float) val->conf.kD;

	D = -vitesse * kd;
	D = 0; //TODO a verifier
	pwm = P + I + D;

	pwm /= 256.0f;
	printf("motor_PID.c pid_Compute pid P:%f I:%f D:%f -> pwm:%f\n", P, I, D, pwm);
	//bound the resulting pwm
	if (pwm > maxPwmValue_)
	{
		pwm = maxPwmValue_;
	}
	else if (pwm < -maxPwmValue_)
	{
		pwm = -maxPwmValue_;
	}

	return (int32) pwm;
}

//! \brief Compute the PID sum of a system
//!
//! \param system The system on which we compute the PID
//! \param error The difference between measured and reference values in ticks/sample
//! \param vitesse The vitesse in vTops/sample
//! \return The new command to apply on the system (pwm)
int32 AsservInsa::pid_ComputeRcva(PID_SYSTEM system, float error, float vitesse)
{
	//http://www.rcva.fr/index.php?option=com_content&view=article&id=27&Itemid=42&limitstart=8
	/*
	 commande = ecart * GAIN_PROPORTIONNEL_DISTANCE
	 commande_distance = commande � GAIN_DERIVE_DISTANCE*vitesse
	 commande_roue_D= commande_distance
	 commande_roue_G= commande_distance
	 */
	pidSystemValues *val;
	val = &(systemValues[system]);

// 	#if(LEVEL_PID <= DEBUG)
// 		writeDebugStreamLine("cpid.c : before error=%d vitesse=%d", error, vitesse);
// 	#endif

	float fferror = ((float) (error)) / (float) vtopsPerTicks; //=[Ticks/sample]
	vitesse /= (float) vtopsPerTicks; //=[Ticks/sample]

	float cmd = fferror * val->conf.kP;
	//float cmd = error * val->conf.kP;
	float pwm = cmd - (val->conf.kD * vitesse);
	pwm /= 128.0f; //donne la pente de décélération du PID !!
	logger().error() << "pid_ComputeRcva error="
			<< error
			<< " ferror="
			<< ferror
			<< " cmd="
			<< cmd
			<< " vitesse="
			<< vitesse
			<< " pwm="
			<< pwm
			<< " vtopsPerTicks="
			<< vtopsPerTicks
			<< " val->conf.kP="
			<< val->conf.kP
			<< " val->conf.kD="
			<< val->conf.kD

			<< logs::end;

	//bound the resulting pwm
	if (pwm > maxPwmValue_)
	{
		pwm = maxPwmValue_;
	}
	else if (pwm < -maxPwmValue_)
	{
		pwm = -maxPwmValue_;
	}
	return (int32) pwm;
}

/*
 EXPORTED_FUNCTION int32 pid_Compute(PID_SYSTEM system, int32 error)
 {
 pidSystemValues *val;
 int32 P,I,D;
 int32 pwm;
 int32 errDif;
 int last;

 error /= VTOPS_PER_TICKS;

 val = &(systemValues[system]);

 //stock last values of the error, so we can
 //differentiate over a custom period
 val->curDiff++;
 if(val->curDiff >= MAX_D_PERIOD)
 {
 val->curDiff = 0; //restart at the beginning of the buffer
 }

 //modif test :
 //sum of all errors of the MAX_D_PERIOD previous period
 val->errSum -= val->dValues[val->curDiff];
 val->errSum += error;

 //bound integration values
 if(val->errSum > val->conf.iMax)
 {
 val->errSum = val->conf.iMax;
 signalErrorOverflow(system);
 }
 else if(val->errSum < -val->conf.iMax)
 {
 val->errSum = -val->conf.iMax;
 signalErrorOverflow(system);
 }

 //fill the rotating buffer
 val->dValues[val->curDiff] = error;

 //if we are at the begining of the rotating buffer
 //we have to take a value at the end of it
 if(val->conf.dPeriod > val->curDiff)
 {
 last = MAX_D_PERIOD + val->curDiff - val->conf.dPeriod;
 }
 else
 {
 last = val->curDiff - val->conf.dPeriod;
 }

 //differential of the error over the period
 errDif = error - val->dValues[last];

 P = error * val->conf.kP;
 I = val->errSum * val->conf.kI;
 D = errDif * val->conf.kD;

 pwm = P + I + D;

 pwm /= 256;

 //bound the resulting pwm
 if(pwm > MAX_PWM_VALUE)
 {
 pwm = MAX_PWM_VALUE;
 }
 else if(pwm < -MAX_PWM_VALUE)
 {
 pwm = -MAX_PWM_VALUE;
 }

 return pwm;
 }
 */
