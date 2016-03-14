#ifndef PMX_S_ROBOTEXTENDED_HPP_
#define PMX_S_ROBOTEXTENDED_HPP_

#include "../Common/Robot.hpp"
#include "../Log/LoggerFactory.hpp"
#include "SActionsExtended.hpp"

class SRobotExtended: public Robot
{
public:

	static SRobotExtended & instance()
	{
		static SRobotExtended instance;
		return instance;
	}

private:

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("SRobotExtended");
		return instance;
	}

	SActionsExtended* actions_;

	SRobotExtended();

public:

	~SRobotExtended()
	{
	}

	inline SActionsExtended& actions()
	{
		SActionsExtended& r_actions = *actions_;
		return r_actions;
	}

	void stop();

	void begin();
};

#endif

