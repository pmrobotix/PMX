#ifndef SSTATE1_HPP
#define	SSTATE1_HPP

#include <string>

#include "../Common/State/State1.hpp"
#include "../Log/LoggerFactory.hpp"


class BState1: public State1
{
private:

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("BState1");
		return instance;
	}

public:

	BState1()
	{
	}

	~BState1()
	{
	}

	IAutomateState* execute(Robot& robot, void * data);

	std::string name()
	{
		return "BState1";
	}
};

#endif
