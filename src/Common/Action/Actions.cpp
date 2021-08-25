#include "Actions.hpp"

#include <unistd.h>

#include "../../Log/Logger.hpp"

Actions::Actions()
{
}

void Actions::start()
{
    actionManagerTimer_.start("ActionManagerTimer", 2); //TODO si superieur à 1, ca bloque le programme sur OPOSUL
}

void Actions::clearAll()
{
    actionManagerTimer_.clearActions();
    actionManagerTimer_.clearTimers();
}

void Actions::cancel()
{
    actionManagerTimer_.cancel();
}

void Actions::waitAndStopManagers()
{
    //Attente de a fin de tous les timers
    actionManagerTimer_.stop();
}

