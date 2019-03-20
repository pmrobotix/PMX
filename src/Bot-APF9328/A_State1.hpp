#ifndef A_STATE1_HPP
#define	A_STATE1_HPP

#include <string>

#include "../Common/State/IAutomateState.hpp"
#include "../Log/LoggerFactory.hpp"

class A_State1: public IAutomateState
{
private:

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("A_State1");
		return instance;
	}
	void setPos();

public:

	A_State1()
	{
	}

	~A_State1()
	{
	}

	IAutomateState* execute(Robot& robot);//, void * data);

	std::string name()
	{
		return "A_State1";
	}
};

#endif
