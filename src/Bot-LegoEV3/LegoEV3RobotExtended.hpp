#ifndef LegoEV3_ROBOTEXTENDED_HPP_
#define LegoEV3_ROBOTEXTENDED_HPP_

#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "../Log/LoggerFactory.hpp"
#include "LegoEV3ActionsExtended.hpp"
#include "LegoEV3AsservExtended.hpp"
#include "LegoEV3SvgWriterExtended.hpp"

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

	inline LegoEV3AsservExtended& asserv()
	{
		LegoEV3AsservExtended& r_asserv = *asserv_;
		return r_asserv;
	}

	inline LegoEV3SvgWriterExtended& svg()
	{
		LegoEV3SvgWriterExtended& r_svg = *psvg_;
		return r_svg;
	}

	void stop();

	void begin(int argc, char** argv);



	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LegoEV3RobotExtended");
		return instance;
	}
private:
	LegoEV3ActionsExtended * actions_;

	LegoEV3AsservExtended * asserv_;

	LegoEV3SvgWriterExtended * psvg_;

	LegoEV3RobotExtended();

};

#endif

