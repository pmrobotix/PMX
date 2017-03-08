#ifndef O_STATE_DECISIONMAKER_HPP
#define	O_STATE_DECISIONMAKER_HPP

#include <string>

#include "../Common/State/AAutomateState.hpp"
#include "../Log/LoggerFactory.hpp"


class O_State_DecisionMaker: public AAutomateState
{
private:

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("O_State_DecisionMaker");
		return instance;
	}

public:

	O_State_DecisionMaker()
	{
	}

	~O_State_DecisionMaker()
	{
	}

	IAutomateState* execute(Robot & robot, void * data);

	std::string name()
	{
		return "O_State_DecisionMaker";
	}

	void IASetupDemo();

	void IASetupHomologation();
};

#endif
