/*!
 * \file
 * \brief Implémentation de la classe ActionManager.
 */

#include "ActionManager.hpp"

#include <stddef.h>
#include <unistd.h>

ActionManager::ActionManager()
		: stop_(false)
{
}

void ActionManager::execute()
{
	logger().info("ActionManager is started");

	int size = 0;
	while (!stop_)
	{
		lock();
		size = actions_.size();

		//logger().debug() << "size : " << size << logs::end;
		if (size > 0)
		{

			IAction * action = actions_.front();
			if (action == NULL)
			{
				logger().error("ActionManager NULL action");
			}
			actions_.pop_front();
			unlock();

			bool persist = action->execute();

			lock();
			if (persist)
			{
				actions_.push_back(action);
			}
			else
			{
				delete action;
			}
			unlock();
		}
		else
		{
			unlock();
			// Comme il n'y a pas d'action, le processus attend avant de
			// relancer la boucle d'execution
			usleep(1000);
		}
	}

	stop_ = false;
	logger().debug("ActionManager is stopped");
}

void ActionManager::debug()
{
	lock();
	//logger().info() << "Defined actions" << logs::end;
	utils::PointerList<IAction *>::iterator i = actions_.begin();
	while (i != actions_.end())
	{
		IAction * action = *i;
		logger().info() << " - " << action->info() << logs::end;
		i++;
	}
	unlock();
	//logger().info() << "End of defined actions" << logs::end;
}
