/*!
 * \file
 * \brief Implémentation de la classe Automate.
 */

#include "Automate.hpp"

#include "../../Log/Logger.hpp"
#include "IAutomateState.hpp"

Automate::Automate()
		: current_(NULL)
{
}

void Automate::run(Robot& robot, IAutomateState* first, void* data)
{
	if (first == NULL)
	{
		logger().error() << "ERROR: Automate::run called with a NULL state" << logs::end;
	}
	IAutomateState* state = first;

	while (state != NULL)
	{
		this->current(state);

		if (this->current() != NULL)
		{
			logger().debug() << "1.Current state : name=" << state->name() << logs::end;

			state = this->current()->execute(robot, data);
			if (state != NULL)
			{
				logger().debug() << "2.Current state : name=" << state->name() << " done." << logs::end;
			}
		}
	}
}
