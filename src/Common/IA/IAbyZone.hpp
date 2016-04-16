/*
 * IAbyZone.hpp
 *
 *  Created on: 14 avr. 2016
 *      Author: pmx
 */

#ifndef COMMON_IA_IABYZONE_HPP_
#define COMMON_IA_IABYZONE_HPP_

#include "../../Log/LoggerFactory.hpp"

class Asserv;

struct RobotPosition;


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
	// in degrees
	float startAngle;
} ZONE;

typedef struct
{
	ZONE *z1;
	ZONE *z2;
	float x;
	float y;
} ZONE_PATH;

typedef struct
{
	char name[400];
	RobotAction action;
	bool completed;
} ACTIONS;

class IAbyZone
{
private:
	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref IAbyZone.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("IAbyZone");
		return instance;
	}

	Asserv *asserv_;

	int _zones_count;
	ZONE* _zones[100];

	int _zones_path_count;
	ZONE_PATH* _zones_path[100];

	int _actions_count;
	ACTIONS* _actions[200];

public:
	IAbyZone();
	~IAbyZone()
	{

	}

	void ia_setAsserv(Asserv * asserv);

	void ia_start();
	void ia_clear();
	void ia_createZone(const char* name,
			float minX,
			float minY,
			float width,
			float height,
			float startX,
			float startY,
			float startAngleDeg);
	ZONE* ia_getZone(const char* zoneName);
	ZONE* ia_getZoneAt(float x, float y);
	ZONE* ia_getNearestZoneFrom(float x, float y);

	void ia_setPath(const char* zone1Name, const char* zone2Name, float x, float y);
	ZONE_PATH* ia_getZonePath(ZONE *z1, ZONE *z2);

	void ia_addAction(const char* name, RobotAction action);

	void ia_printZone(ZONE *z);
	void ia_checkZones();

	RobotPosition goToZone(const char *zoneName);

};
#endif /* COMMON_IA_IABYZONE_HPP_ */