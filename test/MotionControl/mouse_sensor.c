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
//! \file mouse_sensor.c
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#include "mouse_sensor.h"

#include "../global.h"
#include "../log.h"
#include "../exception_handler.h"

#include "uart.h"
#include "protocol/mouse_protocol.h"
#include "motion_calibration.h"
#include "robot_unitConversion.h"
#include "robot_trajectory.h"
#include "path_manager.h"
#include "motion.h"

#define CHANNEL_UART_MOUSE_1		UART_CH4
#define CHANNEL_UART_MOUSE_2		UART_CH0

#define MAX_ERROR_NUMBER			16

#define CALIBRATION_SPEED			0.2f
#define CALIBRATION_ACCELERATION	0.1f

#define MOUSE_NUMBER				2


#define SEND_CODE(x) code = (x); uart_SendAll(mice[mouseNb].uart, &code, sizeof(code));
#define SEND_ALL(x) uart_SendAll(mice[mouseNb].uart, (uint8*)&(x), sizeof(x))

/*
#define SEND_CODE(x) code = (x); uart_SendAll(mice[mouseNb].uart, &code, sizeof(code)); LOG_DEBUG1("code = %02X", x)
#define SEND_ALL(x) uart_SendAll(mice[mouseNb].uart, (uint8*)&(x), sizeof(x)); LOG_DEBUG1("data = %g", (float)x)
*/

typedef union
{
	float f;
	uint8 b[4];
} float_bytes;

typedef union
{
	int16 i;
	uint8 b[2];
} int_bytes;

//m32c compiler doesn't pack, pad nor rearrange structure without explicit option
typedef struct
{
	uint8 code;
	int_bytes deltaX;
	int_bytes deltaY;
	uint8 checksum;
} CALIB_MOTION_MOUSE;

//m32c compiler doesn't pack, pad nor rearrange structure without explicit option
typedef struct
{
	uint8 code;
	float_bytes X;
	float_bytes Y;
	uint8 checksum;
} MOTION_MOUSE;

typedef struct
{
	uint8 uart;		//uart channel number
	int error;		//number of error on the sensor so far
	
	float RatioKU;
	float RatioKV;
	
	float theta;
	float cosTheta;
	float sinTheta;
	
	float RX0;
	float RXA;
	
	float RY0;
	float RYA;
	
	float Delta0;
	float DeltaA;
	
	int32 calibU; 	//variables used for sensor calibration
	int32 calibV;
	
	float calibAlpha1;
	float calibAlpha2;
	float calibDelta;
	
	int32 nbGoodMessage;
	int32 nbCksmError;
	int32 nbUnkError;
} MOUSE;

static MOUSE mice[MOUSE_NUMBER];
static OS_FLAG_GRP *flagCalibStep; //flag for mouse calibration procedure steps

static float corrLR;
static float corrRR;

//! wait calibration step i
static void mouseCalibWaitNextStep(int i);

//! reset calibration procedure
static void mouseCalibInitStep(void);

static void sendCalibParameters(int mouseNb);

//! compute and return the checksum of the motion mouse structure
uint8 computeCalibChecksum(CALIB_MOTION_MOUSE *motion)
{
	uint8 check = motion->code;
	check += motion->deltaX.b[0];
	check += motion->deltaX.b[1];
	
	check += motion->deltaY.b[0];
	check += motion->deltaY.b[1];
	check = 0xFF - check;

	return check;
}

//! compute and return the checksum of the motion mouse structure
uint8 computeChecksum(MOTION_MOUSE *motion)
{
	uint8 check = motion->code;
	check += motion->X.b[0];
	check += motion->X.b[1];
	check += motion->X.b[2];
	check += motion->X.b[3];
	
	check += motion->Y.b[0];
	check += motion->Y.b[1];
	check += motion->Y.b[2];
	check += motion->Y.b[3];
	check = 0xFF - check;

	return check;
}

static HANDLED mouseFailureHandler(void)
{
	motion_DisablePID();
	LOG_ERROR("Mouse Failure : PID Disabled");
	
	return IT_HANDLED;
}

void mouse_Init()
{
	int i;
	uint8 code;
	
	REGISTER_LOG_CLIENT("MOUSE SENSOR");

	flagCalibStep = OSFlagCreate(0x00, &OSLastError);		
	LOG_TEST_OS_ERROR(OSLastError);

	ex_PushITHandler(MOUSE1_FAILURE, mouseFailureHandler);
	ex_PushITHandler(MOUSE2_FAILURE, mouseFailureHandler);
	
	mice[0].uart = CHANNEL_UART_MOUSE_1;
	mice[0].RatioKU = 1.00f;
	mice[0].RatioKV = 1.00f;
	mice[0].theta = -0.785997f;
	
	mice[0].Delta0 = 1500.0f;
	mice[0].DeltaA = 0.0f;
	
	mice[0].RX0 = -7668.8f;
	mice[0].RXA = 0.0f;
	
	mice[0].RY0 = 10150;//10565.61;
	mice[0].RYA = 0;//-3044.32*valueVTops/valueSample;
	
	
	mice[1].uart = CHANNEL_UART_MOUSE_2;
	mice[1].RatioKU = 1.00f;
	mice[1].RatioKV = 1.00f;
	mice[1].theta = -0.780678f;
	
	mice[1].Delta0 = 1499.5f;
	mice[1].DeltaA = 0.0f;
	
	mice[1].RX0 = -7741.7f;
	mice[1].RXA = 0.0f;
	
	mice[1].RY0 = -10214;//-10819.70;
	mice[1].RYA = 0;//3127.76*valueVTops/valueSample;
	
	corrLR = -6308*valueVTops/valueSample;
	corrRR = -6301*valueVTops/valueSample;
	
	for(i=0; i<MOUSE_NUMBER; i++)
	{
		//init mouse uart
		uart_Init(mice[i].uart, MOUSE_UART_BAUDRATE);
	
		//reset mice values
		mice[i].error = 0;
		mice[i].calibU = 0;
		mice[i].calibV = 0;	
		mice[i].nbGoodMessage = 0;
		mice[i].nbCksmError = 0;
		mice[i].nbUnkError = 0;
		mice[i].calibAlpha1 = 0.0f;
		mice[i].calibAlpha2 = 0.0f;
		mice[i].calibDelta = 0.0f;
		mice[i].cosTheta = cosf(mice[i].theta);
		mice[i].sinTheta = sinf(mice[i].theta);
	
		sendCalibParameters(i);
	}
}

BOOL getCalibValue(int mouseNb, CALIB_MOTION_MOUSE *motion)
{	
	uint8 notReceived;
	
	//wait answer during max one tick
	if(uart_GetDataSize(mice[mouseNb].uart) >= sizeof(CALIB_MOTION_MOUSE))
	{
		notReceived = uart_Get(mice[mouseNb].uart, (uint8*)&(motion->code), sizeof(motion->code), 1);
		if(notReceived == 0)
		{
			if(motion->code != DELTA_MOTION)
			{
				if((mice[mouseNb].nbUnkError++ % 100)==0)
				{
					//LOG_ERROR2("Mouse %d : Unknown message (0x%02X) !", mouseNb, motion->code);
				}
				return FALSE;
			}
		}
		else
		{
			//LOG_ERROR1("Mouse %d : Time'd out !", mouseNb);
			return FALSE;
		}
	
		//wait for the rest of mouse message during max one tick
		notReceived = uart_Get(mice[mouseNb].uart, ((uint8*)motion)+1, sizeof(CALIB_MOTION_MOUSE)-1, 1);		
		if(notReceived == 0)
		{
			if(computeCalibChecksum(motion) != motion->checksum)
			{
				//LOG_ERROR1("Mouse %d : checksum error !", mouseNb);
				return FALSE;
			}
			else
			{
				/*if(motion->deltaX.i != 0 || motion->deltaY.i != 0)
				{
					LOG_DEBUG3("M%d: %d,%d", mouseNb, motion->deltaX.i, motion->deltaY.i);
				}*/
				
				mice[mouseNb].error = 0;
				return TRUE;
			}
		}
		else
		{
			//LOG_ERROR1("Mouse %d : Time'd out !", mouseNb);
			return FALSE;
		}
	}
	return FALSE;
}

BOOL getValue(int mouseNb, MOTION_MOUSE *motion)
{
	uint8 notReceived;
		
	//verify nb of bytes in the uart stack	
	if(uart_GetDataSize(mice[mouseNb].uart) >= sizeof(MOTION_MOUSE))
	{
		//wait answer during max one tick
		notReceived = uart_Get(mice[mouseNb].uart, (uint8*)&motion->code, sizeof(motion->code), 1);	
		if(notReceived == 0)
		{
			if(motion->code != X_Y_MOTION)
			{
				if((mice[mouseNb].nbUnkError++ % 100)==0)
				{
					LOG_ERROR2("M%d : Unknown message = %ld", mouseNb, mice[mouseNb].nbUnkError);
				}
				mice[mouseNb].error++;
				return FALSE;
			}
		}
		else
		{
			LOG_ERROR1("Mouse %d : Time'd out ???", mouseNb);
			mice[mouseNb].error++;
			return FALSE;
		}
	
		//wait for the rest of mouse message during max one tick
		notReceived = uart_Get(mice[mouseNb].uart, ((uint8*)motion)+1, sizeof(MOTION_MOUSE)-1, 1);		
		if(notReceived == 0)
		{
			if(computeChecksum(motion) != motion->checksum)
			{
				if((mice[mouseNb].nbCksmError++ % 100)==0)
				{
					LOG_ERROR2("M%d: Checksum error= %ld", mouseNb, mice[mouseNb].nbCksmError);
				}
				mice[mouseNb].error++;
				return FALSE;
			}
			else
			{
				//quick integrity verification
				if((fabs(motion->X.f) >= 5000.0f) || (fabs(motion->Y.f) >= 5000.0f))
				{
					LOG_DEBUG3("M%d: %4.2g, %4.2g", mouseNb, motion->X.f, motion->Y.f);
					return FALSE;
				}
				
				mice[mouseNb].error = 0;
				return TRUE;
			}
		}
		else
		{
			LOG_ERROR1("Mouse %d : Time'd out ???", mouseNb);
			mice[mouseNb].error++;
			return FALSE;
		}
	}
	return FALSE;
}

/*
BOOL mouse_ReadSensor(int32 *dLeft, int32 *dRight, int32 *dAlpha, int32 *dDelta)
{
	int i;
	float alpha;
	
	COEF_MOUSE coef[MOUSE_NUMBER];
	
	float X[MOUSE_NUMBER];
	float Y[MOUSE_NUMBER];
	int32 speed[MOUSE_NUMBER];
	
	MOTION_MOUSE motion;
	
	int mouseResp = 0;
	
	speed[0] = getMotorSpeed(&motors[LEFT_RIGHT][LEFT_MOTOR]);
	speed[1] = getMotorSpeed(&motors[LEFT_RIGHT][RIGHT_MOTOR]);

	for(i=0; i<MOUSE_NUMBER; i++)
	{
		BOOL resp = FALSE;
		speed[i] = ABS(speed[i]);
	
		coef[i].RX = mice[i].RX0 + mice[i].RXA * speed[i];
		coef[i].RY = mice[i].RY0 + mice[i].RYA * speed[i];
		coef[i].Delta = mice[i].Delta0 + mice[i].DeltaA * speed[i];
			
		X[i] = 0.0f;
		Y[i] = 0.0f;
		
		while(getValue(i, &motion))
		{
			resp = TRUE;
			
			X[i] += motion.X.f;
			Y[i] += motion.Y.f;
		}
		
		mice[i].calibAlpha1 += -X[i]/coef[i].RY;
		mice[i].calibAlpha2 += Y[i]/coef[i].RX;
		mice[i].calibDelta += coef[i].Delta*X[i];
			
		if(resp) mouseResp++;
	}
	
	
	alpha = (Y[0]/coef[0].RX+Y[1]/coef[1].RX) / 2.0f;
	//alpha = -(X[0]/coef[0].RY+X[1]/coef[1].RY) / 2.0f;
	//alpha = (Y[0]/coef[0].RX - X[0]/coef[0].RY + Y[1]/coef[1].RX - X[1]/coef[1].RY) / 4.0f;
	*dDelta = (coef[0].Delta*(X[0]-alpha*coef[0].RY) + coef[1].Delta*(X[1]-alpha*coef[1].RY)) / 2.0f;
	*dAlpha = convertDistTovTops(alpha*distEncoderMeter/2.0f);
	
	if(mouseResp != 0)
	{
		*dLeft = (*dDelta)-(*dAlpha);
		*dRight = (*dDelta)+(*dAlpha);
		return TRUE;
	}
	else
	{
		*dDelta = 0;	*dAlpha = 0;
		*dLeft = 0;		*dRight = 0;
		return FALSE;
	}
}
*/

//ratio vTops/ticks for left encoder
extern uint32 leftEncoderRatio;
//ratio vTops/ticks for right encoder
extern uint32 rightEncoderRatio;

BOOL mouse_ReadSensor(int32 *dLeft, int32 *dRight, int32 *dAlpha, int32 *dDelta)
{
	int i;
	float alpha;
		
	float X[MOUSE_NUMBER];
	float Y[MOUSE_NUMBER];
	
	MOTION_MOUSE motion;
		
	for(i=0; i<MOUSE_NUMBER; i++)
	{			
		X[i] = 0.0f;
		Y[i] = 0.0f;
		
		while(getValue(i, &motion))
		{			
			X[i] += motion.X.f;
			Y[i] += motion.Y.f;
		}
	}
		
	//convert units and save position
	*dLeft = X[0] * leftEncoderRatio;
	*dRight = X[1] * rightEncoderRatio;

	//compute alpha and delta displacement
	*dAlpha = (*dRight-*dLeft)/2;
	*dDelta = (*dRight+*dLeft)/2;

	return TRUE;
}

void mouse_UpdateCalibrationValues()
{
	int i;
	CALIB_MOTION_MOUSE motion;
	
	for(i=0; i<MOUSE_NUMBER; i++)
	{
		while(getCalibValue(i, &motion))
		{
			mice[i].calibU += motion.deltaX.i;
			mice[i].calibV += motion.deltaY.i;
		}
	}
}

int32 getU(int mouseNb)
{
	return mice[mouseNb].calibU;
}

int32 getV(int mouseNb)
{
	return mice[mouseNb].calibV;
}

float getCalibAlpha1(int mouseNb)
{
	return mice[mouseNb].calibAlpha1;
}

float getCalibAlpha2(int mouseNb)
{
	return mice[mouseNb].calibAlpha2;
}
float getCalibDelta(int mouseNb)
{
	return mice[mouseNb].calibDelta;
}

void reset(int mouseNb)
{
	mice[mouseNb].calibU = 0;
	mice[mouseNb].calibV = 0;
	mice[mouseNb].calibAlpha1 = 0.0f;
	mice[mouseNb].calibAlpha2 = 0.0f;
	mice[mouseNb].calibDelta = 0.0f;
}

void mouse_Calibration2(int turnNb)
{	
	int i;
	static int mvt = 0;
	RobotCommand cmd;
	
	//reset values of calibration	
	for(i=0; i<MOUSE_NUMBER; i++)
	{
		reset(i);
	}
	
	switch(mvt++%4)
	{
	case 0:
		LOG_DEBUG("Calibration rotation");
		motion_Rotate(&cmd, 2*M_PI*turnNb);
		break;
	case 1:
		LOG_DEBUG("Calibration line");
		motion_Line(&cmd, 2.0f);
		break;
	case 2:
		LOG_DEBUG("Calibration -line");
		motion_Line(&cmd, -2.0f);
		break;
	case 3:
		LOG_DEBUG("Calibration -rotation");
		motion_Rotate(&cmd, -2*M_PI*turnNb);
		break;
	};
	
	path_LaunchTrajectory(&cmd);
	
	path_WaitEndOfTrajectory();
	
	for(i=0; i<MOUSE_NUMBER; i++)
	{
		LOG_DEBUG4("M%d a= %g,%g / d= %g", i, (float)getCalibAlpha1(i), (float)getCalibAlpha2(i), (float)getCalibDelta(i)*valueVTops);
	}
}

//calibration function for mouse sensor
void mouse_Calibration(int lineNb, float lineDist, int turnNb)
{
	int i, k;
	RobotCommand cmd;
	int32 DeltaUPlus[MOUSE_NUMBER];
	int32 DeltaUMinus[MOUSE_NUMBER];
	int32 DeltaVPlus[MOUSE_NUMBER];
	int32 DeltaVMinus[MOUSE_NUMBER];
	
	int step = 0;
		
	mouseCalibInitStep();
	
	//reset values of calibration	
	for(i=0; i<MOUSE_NUMBER; i++)
	{
		uint8 code;
		
		DeltaUPlus[i] = 0;
		DeltaUMinus[i] = 0;
		
		DeltaVPlus[i] = 0;
		DeltaVMinus[i] = 0;
				
		reset(i);
		
		//set mouse in calibration mode
		code = MOUSE_CALIBRATION_MODE; 
		uart_SendAll(mice[i].uart, &code, sizeof(code));
	}
	
	odometrySensorUsed = MOUSE_CALIBRATION;
	
	LOG_DEBUG("Mouse Calibration");
	

	//make the robot run N times x meters forward and backward
	for(k=0; k<lineNb; k++)
	{
		LOG_DEBUG1("Calibration line %d", k);
		
		//run forward
		motion_LineSpeedAcc(&cmd, lineDist, CALIBRATION_SPEED, CALIBRATION_ACCELERATION, CALIBRATION_ACCELERATION);
		path_LaunchTrajectory(&cmd);
		/*if(path_WaitEndOfTrajectory() != TRAJ_OK)
		{
			LOG_ERROR("Unable to complete calibration");
			return;
		}*/
		
		//wait user input
		LOG_DEBUG1("Calib : %4.2f m forward, click next when done", lineDist);
		mouseCalibWaitNextStep(step++);

		//accumulate values of displacement
		for(i=0; i<MOUSE_NUMBER; i++)
		{
			DeltaUPlus[i] += getU(i);
			DeltaVMinus[i] += getV(i);
			reset(i);
		}

		//run backward
		motion_LineSpeedAcc(&cmd, -lineDist, CALIBRATION_SPEED, CALIBRATION_ACCELERATION, CALIBRATION_ACCELERATION);
		path_LaunchTrajectory(&cmd);
		/*if(path_WaitEndOfTrajectory() != TRAJ_OK)
		{
			LOG_ERROR("Unable to complete calibration");
			return;
		}*/
		
		//wait user input
		LOG_DEBUG1("Calib : %4.2f m backward, click next when done", lineDist);
		mouseCalibWaitNextStep(step++);
		
		//accumulate values of displacement
		for(i=0; i<MOUSE_NUMBER; i++)
		{
			DeltaUMinus[i] += getU(i);
			DeltaVPlus[i] += getV(i);
			reset(i);
		}
	}
	
	//compute ratio for each mouse
	for(i=0; i<MOUSE_NUMBER; i++)
	{
		float theta;
		
		LOG_DEBUG5("Mouse %d => Du+ = %ld | Du- = %ld | Dv+ = %ld | Dv- = %ld", i, DeltaUPlus[i], DeltaUMinus[i], DeltaVPlus[i], DeltaVMinus[i]);
		mice[i].RatioKU = -DeltaUMinus[i]/(float)DeltaUPlus[i];
		mice[i].RatioKV = -DeltaVPlus[i]/(float)DeltaVMinus[i];
		
		theta = atan2f(	DeltaVPlus[i]-DeltaVMinus[i],
						DeltaUPlus[i]-DeltaUMinus[i] );
							
		mice[i].theta = theta;
		mice[i].cosTheta = cosf(mice[i].theta);
		mice[i].sinTheta = sinf(mice[i].theta);
		
		mice[i].Delta0 = (lineNb*lineDist) / (valueVTops * ((float)DeltaUPlus[i]*cosf(theta) - (float)DeltaVMinus[i]*sinf(theta)));
								
		LOG_DEBUG4("Ratio : coef = %f | U = %f | V = %f | Theta = %f", mice[i].Delta0, mice[i].RatioKU, mice[i].RatioKV, theta);
	}
	
	//make N * 2*Pi rotation
	LOG_DEBUG("Calibration rotation");
	motion_RotateSpeedAcc(&cmd, 2*M_PI*turnNb, CALIBRATION_SPEED, CALIBRATION_ACCELERATION, CALIBRATION_ACCELERATION);
	path_LaunchTrajectory(&cmd);
	/*if(path_WaitEndOfTrajectory() != TRAJ_OK)
	{
		LOG_ERROR("Unable to complete calibration");
		return;
	}*/
	
	LOG_DEBUG1("Calib : %d turn anticlockwise, click next when done", turnNb);
	mouseCalibWaitNextStep(step++);
	
	//compute mouse positions
	for(i=0; i<MOUSE_NUMBER; i++)
	{
		int32 deltaU, deltaV;
		int32 deltaX, deltaY;
		
		deltaU = getU(i);
		deltaV = getV(i);
		reset(i);
		
		if(deltaU < 0)
		{
			deltaU *= mice[i].RatioKU;
		}
		if(deltaV < 0)
		{
			deltaV *= mice[i].RatioKV;
		}
		
		deltaX = deltaU*cosf(mice[i].theta) - deltaV*sinf(mice[i].theta);
		deltaY = deltaU*sinf(mice[i].theta) + deltaV*cosf(mice[i].theta);
		
		mice[i].RX0 = deltaY / (2*M_PI*turnNb);
		mice[i].RY0 = -deltaX / (2*M_PI*turnNb);
		
		LOG_DEBUG5("Mouse %d | dU = %ld | dV = %ld | dX = %ld | dY = %ld", i, deltaU, deltaV, deltaX, deltaY);
		LOG_DEBUG2("RX = %f | RY = %f", mice[i].RX0, mice[i].RY0);
		sendCalibParameters(i);
	}
	
	odometrySensorUsed = MOUSE_SENSOR;
}

void mouseCalibWaitNextStep(int i)
{
	OSFlagPend(flagCalibStep,
				(0x01 << i),
				OS_FLAG_WAIT_SET_ALL,
				0,
				&OSLastError);
				
	LOG_TEST_OS_ERROR(OSLastError);
}

void mouseCalibSetStep(int i)
{
	OSFlagPost(flagCalibStep,
				(0x01 << i),
				OS_FLAG_SET,
				&OSLastError);
				
	LOG_TEST_OS_ERROR(OSLastError);
}

void mouseCalibInitStep()
{
	OSFlagPost(flagCalibStep,
				0xFF,
				OS_FLAG_CLR,
				&OSLastError);
				
	LOG_TEST_OS_ERROR(OSLastError);
}

	
void sendCalibParameters(int mouseNb)
{
	uint8 code;
	uint16 freq = DEFAULT_SAMPLING_FREQ;
	float distEncoderMeter2 = distEncoderMeter/2.0f;
	
	
	OSTimeDly(10);
	SEND_CODE(MOUSE_SEND_RATIOKU);
	SEND_ALL(mice[mouseNb].RatioKU);
	
	SEND_CODE(MOUSE_SEND_RATIOKV);
	SEND_ALL(mice[mouseNb].RatioKV);
	
	SEND_CODE(MOUSE_SEND_THETA);
	SEND_ALL(mice[mouseNb].theta);
	
	SEND_CODE(MOUSE_SEND_RX);
	SEND_ALL(mice[mouseNb].RX0);
	
	SEND_CODE(MOUSE_SEND_RY);
	SEND_ALL(mice[mouseNb].RY0);
	
	SEND_CODE(MOUSE_SEND_COEFDELTA);
	SEND_ALL(mice[mouseNb].Delta0);
	
	SEND_CODE(MOUSE_SEND_VALUEVTOPS);
	SEND_ALL(valueVTops);
	
	SEND_CODE(MOUSE_SEND_DISTENCODER2);
	SEND_ALL(distEncoderMeter2);
	
	SEND_CODE(MOUSE_SEND_FREQ);
	SEND_ALL(freq);
	
	LOG_DEBUG1("code = %02X", MOUSE_SEND_FREQ);
	LOG_DEBUG1("freq = %d", freq);
	
	OSTimeDly(10);
	SEND_CODE(MOUSE_RESET);
	
	OSTimeDly(10);
	SEND_CODE(MOUSE_ENABLE);
}
