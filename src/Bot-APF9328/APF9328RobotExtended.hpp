#ifndef PMX_BOTBIGPMX_ROBOTEXTENDED_HPP_
#define PMX_BOTBIGPMX_ROBOTEXTENDED_HPP_

#include "../Common/Robot.hpp"
#include "../Log/LoggerFactory.hpp"
#include "APF9328ActionsExtended.hpp"
#include "APF9328SvgWriterExtended.hpp"

class APF9328RobotExtended: public Robot
{
public:

	static APF9328RobotExtended & instance()
	{
		static APF9328RobotExtended instance;
		return instance;
	}

	~APF9328RobotExtended()
	{
	}

	inline APF9328ActionsExtended& actions()
	{
		APF9328ActionsExtended& r_actions = *actions_;
		return r_actions;
	}

	inline APF9328SvgWriterExtended& svg()
	{
		APF9328SvgWriterExtended& r_svg = *psvg_;
		return r_svg;
	}

	void begin();

	void stop();

private:

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("APF9328RobotExtended");
		return instance;
	}

	APF9328ActionsExtended* actions_;

	APF9328SvgWriterExtended * psvg_;

	APF9328RobotExtended();
};

#endif
