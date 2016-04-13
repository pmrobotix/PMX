
#include "State1.hpp"

#include <unistd.h>

#include "../../Log/Logger.hpp"


IAutomateState*
State1::execute(Robot& , void *)
{
	logger().info() << "State1" << logs::end;




	return NULL; //finish all state
}



