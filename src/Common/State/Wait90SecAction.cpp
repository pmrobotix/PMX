/*!
 * \file
 * \brief Implémentation de la classe Wait90SecAction.
 */

#include "Wait90SecAction.hpp"

#include <unistd.h>

#include "../../Log/Logger.hpp"
#include "../Robot.hpp"
#include "../Utils/Chronometer.hpp"

class Robot;

Wait90SecAction::Wait90SecAction(Robot& robot, void *data) :
		robot_(robot), data_(data)
{
}

void Wait90SecAction::execute()
{
	this->logger().debug() << "start" << logs::end;



	//démarrage du chrono
	logger().info() << "Start Chronometer" << logs::end;
	robot_.chrono().start();

	//ARU and adversary
	while (robot_.chrono().getElapsedTimeInSec() <= 10)
	{



		usleep(300000);
		this->logger().info() << "chrono " << robot_.chrono().getElapsedTimeInSec() << logs::end;
	}


	this->logger().info() << "end90s...stop... " << robot_.chrono().getElapsedTimeInSec()
			<< logs::end;


}
