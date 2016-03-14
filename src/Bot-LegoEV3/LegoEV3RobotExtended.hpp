#ifndef LegoEV3_ROBOTEXTENDED_HPP_
#define LegoEV3_ROBOTEXTENDED_HPP_

#include "../Common/Robot.hpp"
#include "../Log/LoggerFactory.hpp"
#include "LegoEV3ActionsExtended.hpp"

class Arguments;

class LegoEV3RobotExtended: public Robot
{
public:

	static LegoEV3RobotExtended & instance()
	{
		static LegoEV3RobotExtended instance;
		return instance;
	}

	~LegoEV3RobotExtended()
	{
		logger().debug() << "~LegoEV3RobotExtended()" << logs::end;
	}

	inline LegoEV3ActionsExtended& actions()
	{
		LegoEV3ActionsExtended& r_actions = *actions_;
		return r_actions;
	}

	void stop();

	void begin();

private:

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LegoEV3RobotExtended");
		return instance;
	}

	LegoEV3ActionsExtended* actions_;

	LegoEV3RobotExtended();
};

#endif

