#ifndef L_STATE_DECISIONMAKER_HPP
#define	L_STATE_DECISIONMAKER_HPP

#include <string>

#include "../Common/State/AAutomateState.hpp"
#include "../Log/LoggerFactory.hpp"

class Data;

class L_State_DecisionMaker: public AAutomateState
{
private:

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("L_State_DecisionMaker");
		return instance;
	}



public:

	L_State_DecisionMaker()
	{

	}

	~L_State_DecisionMaker()
	{
	}

	IAutomateState* execute(Robot & robot, void * data);

	std::string name()
	{
		return "L_State_DecisionMaker";
	}

	void IASetupDemo();
	void IASetupHomologation();
};

#endif
