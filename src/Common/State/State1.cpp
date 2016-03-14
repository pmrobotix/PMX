
#include "State1.hpp"

#include "../../Log/Logger.hpp"


IAutomateState*
State1::execute(Robot& , void *)
{
	logger().info() << "Start 1" << logs::end;




	return NULL; //finish all state
}



