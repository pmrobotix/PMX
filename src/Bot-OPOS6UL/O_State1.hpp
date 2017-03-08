#ifndef O_STATE1_HPP
#define	O_STATE1_HPP

#include <string>

#include "../Common/State/State1.hpp"
#include "../Log/LoggerFactory.hpp"

class O_State1: public State1
{
private:

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("O_State1");
		return instance;
	}
	void setPos();

public:

	O_State1()
	{
	}

	~O_State1()
	{
	}

	IAutomateState* execute(Robot& robot, void * data);

	std::string name()
	{
		return "O_State1";
	}
};

#endif
