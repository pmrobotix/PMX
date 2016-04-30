#ifndef A_STATE_DECISIONMAKER_HPP
#define	A_STATE_DECISIONMAKER_HPP

#include <string>

#include "../Common/State/AAutomateState.hpp"
#include "../Log/LoggerFactory.hpp"


class A_State_DecisionMaker: public AAutomateState
{
private:

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("A_State_DecisionMaker");
		return instance;
	}

public:

	A_State_DecisionMaker()
	{
	}

	~A_State_DecisionMaker()
	{
	}

	IAutomateState* execute(Robot & robot, void * data);

	std::string name()
	{
		return "A_State_DecisionMaker";
	}

	void IASetupDemo();

	void IASetupHomologation();
};

#endif
