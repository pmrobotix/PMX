#ifndef SSTATE1_HPP
#define	SSTATE1_HPP

#include <string>

#include "../Common/State/State1.hpp"
#include "../Log/LoggerFactory.hpp"


class SState1: public State1
{
private:

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("SState1");
		return instance;
	}

	IAutomateState* execute(Robot& robot, void * data);

	std::string name()
	{
		return "SState1";
	}
};

#endif
