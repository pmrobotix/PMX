#include "Actions.hpp"

#include <unistd.h>

#include "../../Log/Logger.hpp"

void Actions::start()
{
	actionManager_.start("ActionManager");
	logger().debug("Actions is started");
}

void Actions::stop()
{
	this->stopDevices();
	this->waitAndStopManagers(); // wait end of tasks in managers
	logger().debug("Actions is stopped");
}

void Actions::stopDevices()
{
//default devices
}

void Actions::waitAndStopManagers()
{
	int time = 0;
	while (actionManager_.count() > 0)
	{
		logger().debug() << actionManager_.count() << logs::end;
		usleep(50000);
		time++;
		if (time >= 20) break; //10 secondes
	}
	actionManager_.stop();

	usleep(5000); //TODO  créer une Attente avec timeout de la fin de l'actionManager à la place du usleep
}
