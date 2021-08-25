/*!
 * \file
 * \brief Implémentation de la classe ActionManagerTimer.
 */

#include "ActionManagerTimer.hpp"
#include <semaphore.h>

ActionManagerTimer::ActionManagerTimer() :
        stopActionsAndTimers_(false), pause_(false), chronoTimer_("ActionManagerTimer"), val(0)
{
    //init du semaphore valeur 0 pour une pause par defaut.
    sem_init(&AMT, 0, 0);
    //post increment
    //wait decremente
    //1 = running
    //0 = pause
}

void ActionManagerTimer::execute() {
    //logger().debug() << "ActionManagerTimer is started" << logs::end;

    int sizeT = 0;
    int sizePT = 0;
    int sizeA = 0;
    long tps = 0;
    long starttime = 0;

    chronoTimer_.start();

    while (!stopActionsAndTimers_) {
        if (pause_) {
            logger().debug("paused...");
            sem_wait(&AMT);
        }

        if (!stopActionsAndTimers_ && (sizeA == 0) && (sizeT == 0) && (sizePT == 0)) {
            //on laisse le temps de faire autre chose s'il n'y a rien à faire
            logger().debug("nothing to do... wait semaphore...");
            sem_wait(&AMT);
        }
        else {
            this->yield();
            utils::Thread::sleep_for_micros(1);
        }

        //on traite les timers (deprecated)
        //___________________________________________________________________
        mtimer_.lock();
        // On teste s'il y a un timer à executer
        sizeT = timers_.size();

        if (sizeT > 0 && !stopActionsAndTimers_) {
            starttime = chronoTimer_.getElapsedTimeInMicroSec();
            utils::PointerList<ITimerListener *>::iterator i = timers_.begin();
            while (i != timers_.end() && !stopActionsAndTimers_) {
                ITimerListener * timer = *i;
                tps = starttime - timer->getLastTime();

                if (tps >= (timer->timeSpan() * 1000)) {
                    timer->onTimer(chronoTimer_);
                    timer->setLastTime(starttime);
                }
                i++;
            }
            utils::Thread::sleep_for_micros(1); //permet de ne pas avoir 100% du processeur
        }
        mtimer_.unlock();
        //____________________________________________________________________

        //on laisse le temps de faire autre chose si besoin
        this->yield();

        //on parcours la liste de timer
        //logger().debug() << "PTIMERS sizePT=" << ptimers_.size() << logs::end;
        mtimer_.lock();
        sizePT = ptimers_tobestarted_.size();

        if (sizePT > 0 && !stopActionsAndTimers_) {
            while (sizePT != 0) {
                ITimerPosixListener * ptimer = ptimers_tobestarted_.front();
                if (ptimer == NULL) {
                    logger().error("ptimers_notrunning_ is NULL");
                }
                if (!ptimer->getRunning()) {
                    ptimer->startTimer();
                }
                else {
                    logger().error() << "ptimers already started, name=" << ptimer->name() << logs::end;
                }
                ptimers_.push_back(ptimer);
                ptimers_tobestarted_.pop_front();
                sizePT = ptimers_tobestarted_.size();
            }
        }
        mtimer_.unlock();

        this->yield();

        //on traite les actions
        maction_.lock();
        // On teste s'il y a une tache à faire
        sizeA = actions_.size();
        if (sizeA > 0 && !stopActionsAndTimers_) {
            while (sizeA != 0) {
                //logger().debug() << "ACTIONS sizeA = " << sizeA << logs::end;

                IAction * action = actions_.front();
                if (action == NULL) {
                    logger().error("action is NULL");
                }
                else {
                    actions_.pop_front();
                    maction_.unlock();

                    bool persistaction = action->execute();

                    maction_.lock();
                    if (persistaction == true) {
                        actions_.push_back(action);
                        utils::Thread::sleep_for_micros(100);        //permet de ne pas avoir 100% du processeur
                    }
                }
                sizeA = actions_.size(); //Maj
            }
        }
        maction_.unlock();
    }

    logger().debug("ActionManagerTimer is stopped and finished");
    stopActionsAndTimers_ = false; //on reinitialise le stop.
}

void ActionManagerTimer::stopTimer(std::string timerNameToDelete) {
    bool found = false;
    utils::PointerList<ITimerListener *>::iterator save;
    utils::PointerList<ITimerListener *>::iterator i = timers_.begin();
    mtimer_.lock();
    while (i != timers_.end()) {
        ITimerListener * timer = *i;
        if (timer->name() == timerNameToDelete) {
            save = i;
            found = true;
            timer->onTimerEnd(chronoTimer_);
        }
        i++;
    }
    if (found) timers_.erase(save);
    else logger().debug() << "Timer [" << timerNameToDelete << "] not found or already deleted." << logs::end;

    mtimer_.unlock();
}

void ActionManagerTimer::stopPTimer(std::string timerNameToDelete) {

    bool found = false;
    utils::PointerList<ITimerPosixListener *>::iterator save;
    utils::PointerList<ITimerPosixListener *>::iterator i = ptimers_.begin();
    mtimer_.lock();
    while (i != ptimers_.end()) {
        ITimerPosixListener * ptimer = *i;
        //logger().debug() << "PTimer [" << ptimer->name() << "] found" << logs::end;
        if (ptimer->name() == timerNameToDelete) {
            save = i;
            found = true;
            ptimer->onTimerEnd(chronoTimer_);
            ptimer->remove();
        }
        i++;
    }
    if (found) ptimers_.erase(save);
    else logger().debug() << "PTimer [" << timerNameToDelete << "] not found or already deleted." << logs::end;

    mtimer_.unlock();
    unblock("stopPTimer");

}
void ActionManagerTimer::stopAllPTimers() {
    mtimer_.lock();
    ptimers_tobestarted_.clear();
    while (ptimers_.size() != 0) {
        ITimerPosixListener * ptimer = ptimers_.front();
        if (ptimer == NULL) {
            logger().error("ptimers_ is NULL");
        }
        else {
            //appel de la fonction de fin
            ptimer->onTimerEnd(chronoTimer_);
            //suppression du timer en cours d'execution
            ptimer->remove();
            //suppression de la liste
            ptimers_.pop_front();
        }
    }
    ptimers_.clear();
    mtimer_.unlock();

    unblock("stopAllPTimers");

}

void ActionManagerTimer::stop() {
    logger().debug() << "stop all ActionManagerTimer..." << logs::end;
    this->stopActionsAndTimers_ = true;
    stopAllPTimers();

    this->waitForEnd();
}

void ActionManagerTimer::pause(bool value) {
    this->pause_ = value;
    //on parcours et on mets en pause tous les timers courants
    utils::PointerList<ITimerPosixListener *>::iterator i = ptimers_.begin();
    mtimer_.lock();
    while (i != ptimers_.end()) {
        ITimerPosixListener * ptimer = *i;
        ptimer->setPause(value);
        i++;
    }
    mtimer_.unlock();
    sem_getvalue(&AMT, &val);
    if (val == 0) sem_post(&AMT);
}

void ActionManagerTimer::debugActions() {

    std::ostringstream temp;
    temp << "Print current actions";
    maction_.lock();
    utils::PointerList<IAction *>::iterator i = actions_.begin();
    while (i != actions_.end()) {
        IAction * action = *i;
        temp << " - " << action->info();
        i++;
    }
    maction_.unlock();
    logger().debug() << temp.str() << logs::end;
}

void ActionManagerTimer::debugPTimers() {
    std::ostringstream temp;
    temp << "Print posixtimers to be started :";
    mtimer_.lock();
    if (ptimers_tobestarted_.size() != 0) {

        utils::PointerList<ITimerPosixListener *>::iterator i = ptimers_tobestarted_.begin();
        while (i != ptimers_tobestarted_.end()) {
            ITimerPosixListener * ptimer = *i;
            temp << " - " << ptimer->name();
            i++;
        }
    }
    mtimer_.unlock();
    logger().debug() << temp.str() << logs::end;

    std::ostringstream temp2;
    temp2 << "Print current posixtimers :";
    mtimer_.lock();
    if (ptimers_.size() != 0) {
        utils::PointerList<ITimerPosixListener *>::iterator ii = ptimers_.begin();
        while (ii != ptimers_.end()) {
            ITimerPosixListener * ptimer = *ii;
            temp2 << " - " << ptimer->name();
            ii++;
        }
    }
    mtimer_.unlock();
    logger().debug() << temp2.str() << logs::end;
}

//deprecated
void ActionManagerTimer::debugTimers() {
    //().debug() << "Print Defined timers" << logs::end;
    std::ostringstream temp;
        temp << "Print Defined timers :";
    mtimer_.lock();
    if (timers_.size() != 0) {
        utils::PointerList<ITimerListener *>::iterator i = timers_.begin();
        while (i != timers_.end()) {
            ITimerListener * timer = *i;
            temp << " - " << timer->name();
            i++;
        }
    }
    mtimer_.unlock();
    logger().debug() << temp.str() << logs::end;
}

