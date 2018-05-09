#include "Actions.hpp"

#include <unistd.h>

#include "../../Log/Logger.hpp"

void Actions::start()
{
    actionManagerTimer_.start("ActionManagerTimer");
    started_ = true;
    logger().debug("Actions is started");
}


void Actions::emergencyStop()
{
    if(!started_) return;
    actionManagerTimer_.cancel();
    //actionManagerTimer_.waitForEnd();
}

void Actions::stop()
{
    if(!started_) return;
    actionManagerTimer_.clearActions();
    actionManagerTimer_.clearTimers();


    logger().debug("No more Actions");
}

void Actions::cancel()
{
    if(!started_) return;
    actionManagerTimer_.cancel();
}

void Actions::waitAndStopManagers() //ne fct pas
{
    if(!started_) return;
    /*
    int time = 0;
    while (actionManagerTimer_.countActions() > 0) {
        logger().debug() << actionManagerTimer_.countActions() << logs::end;
        //TODO parcourir les taches et les mettre à false ? si parametre force == true
        usleep(1000);
        time++;
        if (time >= 20)
            break; //0.2 secondes
    }
*/
    //TODO parcourir toutes les timers ??si parametre force == true

    actionManagerTimer_.stop();

    usleep(5000); //TODO  créer une Attente avec timeout de la fin de l'actionManager à la place du usleep

    logger().error() << "END waitAndStopManagers() : NbActions= " << actionManagerTimer_.countActions()
        << "NbTimers= " << actionManagerTimer_.countTimers()
            << logs::end;
}


