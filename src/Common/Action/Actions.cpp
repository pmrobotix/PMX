#include "Actions.hpp"

#include <unistd.h>

#include "../../Log/Logger.hpp"

void Actions::start()
{
	actionManagerTimer_.start("ActionManagerTimer");
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
	while (actionManagerTimer_.countActions() > 0)
	{
		logger().debug() << actionManagerTimer_.countActions() << logs::end;
		//TODO parcourir les taches et les mettre à false ?
		usleep(1000);
		time++;
		if (time >= 20) break; //0.2 secondes
	}

	//TODO parcourir toutes les timers ??

	actionManagerTimer_.stop();

	usleep(5000); //TODO  créer une Attente avec timeout de la fin de l'actionManager à la place du usleep
}

