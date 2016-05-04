#ifndef L_STATE1_HPP
#define	L_STATE1_HPP

#include <string>

#include "../Common/State/State1.hpp"
#include "../Log/LoggerFactory.hpp"

class L_State1: public State1
{
private:

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("L_State1");
		return instance;
	}

	void setPos();
public:

	L_State1()
	{
	}

	~L_State1()
	{
	}

	IAutomateState* execute(Robot& robot, void * data);

	std::string name()
	{
		return "L_State1";
	}
};

#endif
