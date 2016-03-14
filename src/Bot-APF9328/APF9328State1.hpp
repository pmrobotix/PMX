#ifndef SSTATE1_HPP
#define	SSTATE1_HPP

#include <string>

#include "../Common/State/State1.hpp"
#include "../Log/LoggerFactory.hpp"


class APF9328State1: public State1
{
private:

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("APF9328State1");
		return instance;
	}

public:

	APF9328State1()
	{
	}

	~APF9328State1()
	{
	}

	IAutomateState* execute(Robot& robot, void * data);

	std::string name()
	{
		return "APF9328State1";
	}
};

#endif
