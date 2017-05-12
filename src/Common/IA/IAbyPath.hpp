/*
 * IAbyPath.hpp
 *
 *  Created on: 4 mai 2017
 *      Author: pmx
 */

#ifndef COMMON_IA_IABYPATH_HPP_
#define COMMON_IA_IABYPATH_HPP_

#include <sstream>
#include <string>

#include "../../Log/LoggerFactory.hpp"
#include "../Robot.hpp"
#include "IACommon.hpp"

struct RobotPosition;

class Playground;

class Robot;

class IAbyPath
{
private:
	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref IAbyPath.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("IAbyPath");
		return instance;
	}

	inline const logs::Logger & loggerSvg()
	{
		std::ostringstream s;
		s << "IAbyPath4" << robot_->getID();
		const logs::Logger & svg_ = logs::LoggerFactory::logger(s.str());
		return svg_;
	}

	Robot * robot_;

	int _zones_count;
	ZONE* _zones[100];

	int _actions_count;
	ACTIONS* _actions[200];


public:
	IAbyPath(Robot *robot);

	~IAbyPath()
	{
	}

	void ia_start();
	void toSVG(Playground * p);

	void ia_clear();
	void ia_createZone(const char* name, float minX, float minY, float width, float height,
			float startX, float startY, float startAngleDeg);
	ZONE* ia_getZone(const char* zoneName);
	ZONE* ia_getZoneAt(float x, float y);
	ZONE* ia_getNearestZoneFrom(float x, float y);

	void ia_addAction(const char* name, RobotAction action);

	void ia_printZone(ZONE *z);
	void ia_checkZones();

	void goToZone(const char *zoneName, RobotPosition *zone_p);
};

#endif /* COMMON_IA_IABYPATH_HPP_ */
