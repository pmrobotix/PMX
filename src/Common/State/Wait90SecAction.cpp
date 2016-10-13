/*!
 * \file
 * \brief Implémentation de la classe Wait90SecAction.
 */

#include "Wait90SecAction.hpp"

#include <unistd.h>
#include <cstdlib>

#include "../../Log/Logger.hpp"
#include "../Asserv/Asserv.hpp"
#include "../Robot.hpp"
#include "../Utils/Chronometer.hpp"
#include "Data.hpp"

class Robot;

Wait90SecAction::Wait90SecAction(Robot& robot, void *data)
		: robot_(robot), data_(data)
{
}

void Wait90SecAction::execute()
{
	this->logger().debug() << "start" << logs::end;

	Data* sharedData = (Data*) data_;

	//démarrage du chrono
	logger().info() << "Start Chronometer" << logs::end;
	robot_.chrono().start();

	//ARU and adversary
	while (robot_.chrono().getElapsedTimeInSec() <= 90)
	{

		usleep(300000);
		this->logger().debug() << "chrono " << robot_.chrono().getElapsedTimeInSec() << logs::end;
	}

	this->logger().info() << "end90s...stop... "
			<< robot_.chrono().getElapsedTimeInSec()
			<< logs::end;


	sharedData->end90s(true); //indique que l'action est effectuée au prog princ
	robot_.asserv_default->freeMotion();
	robot_.stop();
	usleep(500000);
	exit(0);
}
