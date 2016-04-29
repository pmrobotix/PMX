#ifndef APF9328_ROBOTEXTENDED_HPP_
#define APF9328_ROBOTEXTENDED_HPP_

#include "../Common/Robot.hpp"
#include "../Log/LoggerFactory.hpp"
#include "APF9328ActionsExtended.hpp"
#include "APF9328AsservExtended.hpp"
#include "APF9328IAExtended.hpp"

class APF9328SvgWriterExtended;

class APF9328RobotExtended: public Robot
{
public:

	static APF9328RobotExtended & instance()
	{
		static APF9328RobotExtended instance;
		return instance;
	}

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("APF9328RobotExtended");
		return instance;
	}

	~APF9328RobotExtended()
	{
	}

	inline APF9328ActionsExtended& actions()
	{
		APF9328ActionsExtended& r_actions = *p_actions_;
		return r_actions;
	}

	inline APF9328AsservExtended& asserv()
	{
		APF9328AsservExtended& r_asserv = *p_asserv_;
		return r_asserv;
	}

	inline APF9328IAExtended& ia()
	{
		APF9328IAExtended& r_ia = *p_ia_;
		return r_ia;
	}

	void begin(int argc, char** argv);

	void stop();

private:

	APF9328ActionsExtended* p_actions_;

	APF9328AsservExtended * p_asserv_;

	APF9328IAExtended * p_ia_;

	APF9328SvgWriterExtended * p_svg_;

	APF9328RobotExtended();
};

#endif

