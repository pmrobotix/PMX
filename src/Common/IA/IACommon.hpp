/*
 * IACommon.hpp
 *
 *  Created on: 11 mai 2017
 *      Author: pmx
 */

#ifndef COMMON_IA_IACOMMON_HPP_
#define COMMON_IA_IACOMMON_HPP_



typedef bool (*RobotAction)(void);

typedef struct
{
	char name[400];
	// in mm
	float minX;
	float minY;
	float width;
	float height;
	float startX;
	float startY;

	float startAngle;// in degrees
} ZONE;

typedef struct
{
	char name[400];
	RobotAction action;
	bool completed;
} ACTIONS;


#endif /* COMMON_IA_IACOMMON_HPP_ */
