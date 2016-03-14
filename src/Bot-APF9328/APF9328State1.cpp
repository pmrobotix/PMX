
#include "APF9328State1.hpp"

#include "../Log/Logger.hpp"


IAutomateState*
APF9328State1::execute(Robot& , void *)
{
	logger().info() << "APF9328Start 1" << logs::end;


	//TODO cligoter led


	return NULL; //finish all state
}



