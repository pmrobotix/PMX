#ifndef LegoEV3_ROBOTEXTENDED_HPP_
#define LegoEV3_ROBOTEXTENDED_HPP_

#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "../Log/LoggerFactory.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3IAExtended.hpp"

class LegoEV3SvgWriterExtended;

class LegoEV3IAExtended;

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
		LegoEV3ActionsExtended& r_actions = *p_actions_;
		return r_actions;
	}

	inline LegoEV3AsservExtended& asserv()
	{
		LegoEV3AsservExtended& r_asserv = *p_asserv_;
		return r_asserv;
	}


	inline LegoEV3IAExtended& ia()
	{
		LegoEV3IAExtended& r_ia = *p_ia_;
		return r_ia;
	}

	Data* sharedData; //surcharge ?

	void stop();

	void begin(int argc, char** argv);

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LegoEV3RobotExtended");
		return instance;
	}
private:
	LegoEV3ActionsExtended * p_actions_;

	LegoEV3AsservExtended * p_asserv_;

	LegoEV3IAExtended * p_ia_;

	LegoEV3SvgWriterExtended * p_svg_;

	LegoEV3RobotExtended();

};

#endif

