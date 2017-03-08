#ifndef OPOS6UL_ROBOTEXTENDED_HPP_
#define OPOS6UL_ROBOTEXTENDED_HPP_

#include "../Common/Robot.hpp"
#include "../Log/LoggerFactory.hpp"
#include "OPOS6UL_ActionsExtended.hpp"
#include "OPOS6UL_AsservExtended.hpp"
#include "OPOS6UL_IAExtended.hpp"

class OPOS6UL_SvgWriterExtended;

class OPOS6UL_RobotExtended: public Robot
{
public:

	static OPOS6UL_RobotExtended & instance()
	{
		static OPOS6UL_RobotExtended instance;
		return instance;
	}

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("OPOS6UL_RobotExtended");
		return instance;
	}

	~OPOS6UL_RobotExtended()
	{
	}

	inline OPOS6UL_ActionsExtended& actions()
	{
		OPOS6UL_ActionsExtended& r_actions = *p_actions_;
		return r_actions;
	}

	inline OPOS6UL_AsservExtended& asserv()
	{
		OPOS6UL_AsservExtended& r_asserv = *p_asserv_;
		return r_asserv;
	}

	inline OPOS6UL_IAExtended& ia()
	{
		OPOS6UL_IAExtended& r_ia = *p_ia_;
		return r_ia;
	}

	Data* sharedData;

	void begin(int argc, char** argv);

	void stop();

private:

	OPOS6UL_ActionsExtended* p_actions_;

	OPOS6UL_AsservExtended * p_asserv_;

	OPOS6UL_IAExtended * p_ia_;

	OPOS6UL_SvgWriterExtended * p_svg_;

	OPOS6UL_RobotExtended();
};

#endif

