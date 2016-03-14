#ifndef STATE1_HPP
#define	STATE1_HPP

#include <string>

#include "../../Log/LoggerFactory.hpp"
#include "AAutomateState.hpp"


class State1: public AAutomateState
{
private:

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("State1");
		return instance;
	}

	virtual IAutomateState* execute(Robot& robot, void * data);

public:

	State1()
	{
	}

	virtual ~State1()
	{
	}

	virtual std::string name()
	{
		return "State1";
	}
};

#endif
