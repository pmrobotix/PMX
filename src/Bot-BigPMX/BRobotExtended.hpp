#ifndef PMX_BOTBIGPMX_ROBOTEXTENDED_HPP_
#define PMX_BOTBIGPMX_ROBOTEXTENDED_HPP_

#include "../Common/Robot.hpp"
#include "../Log/LoggerFactory.hpp"
#include "BActionsExtended.hpp"



class BRobotExtended: public Robot
{
public:

	static BRobotExtended & instance()
	{
		static BRobotExtended instance;
		return instance;
	}

private:

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger(
				"BRobotExtended");
		return instance;
	}

	BActionsExtended* actions_;

	BRobotExtended();

public:

	~BRobotExtended()
	{
	}

	inline BActionsExtended& actions()
	{
		BActionsExtended& r_actions = *actions_;
		return r_actions;
	}

	void begin();

	void stop();
};

#endif

