/*!
 * \file
 * \brief Implémentation de la classe ActionManagerTimer.
 */

#include "ActionManagerTimer.hpp"

#include <unistd.h>

ActionManagerTimer::ActionManagerTimer() :
        stop_(false), pause_(false), chronoTimer_("ActionManagerTimer")
{
}

void ActionManagerTimer::execute()
{
    logger().debug() << "ActionManagerTimer is started stop_=" << stop_ << logs::end;

    int sizeT = 0;
    int sizeA = 0;
    long tps = 0;
    long starttime = 0;

    chronoTimer_.start();
    while (!stop_) {
        /*while (pause_)
         {
         //this->yield();
         usleep(10);
         }*/

        //on traite les timers
        mtimer_.lock();

        sizeT = timers_.size();
        if (sizeT > 0) {
            starttime = chronoTimer_.getElapsedTimeInMicroSec();
            utils::PointerList<ITimerListener *>::iterator i = timers_.begin();
            while (i != timers_.end() && !stop_) {
                ITimerListener * timer = *i;
                tps = starttime - timer->getLastTime();
//				logger().debug() <<  " size=" << size << " - " << timer->info()
//						<< " tps=" << tps
//						<< " starttime=" << starttime
//						<< " timer->lastTime()=" << timer->getLastTime()
//						<< logs::end;
                if (tps >= (timer->timeSpan() * 1000)) {
                    timer->setLastTime(starttime);
                    timer->onTimer(chronoTimer_);
                }
                i++;
            }
        }
        mtimer_.unlock();
        /*while (pause_)
         {
         //this->yield();
         usleep(10);
         }*/

        //on traite les actions
        maction_.lock();
        sizeA = actions_.size();

        if (sizeA > 0 && !stop_) {
            //logger().debug() << "sizeA = " << sizeA << logs::end;
            IAction * action = actions_.front();
            if (action == NULL) {
                logger().error("action is NULL");
            }
            actions_.pop_front();
            maction_.unlock();

            bool persist = action->execute();

            maction_.lock();
            if (persist) {
                actions_.push_back(action);
            }

        }
        maction_.unlock();
    }
    stop_ = false; //on reinitialise le stop.
    logger().debug("ActionManagerTimer is stopped and finished");
}

void ActionManagerTimer::debugActions()
{
    maction_.lock();
    logger().debug() << "Defined actions" << logs::end;
    utils::PointerList<IAction *>::iterator i = actions_.begin();
    while (i != actions_.end()) {
        IAction * action = *i;
        logger().debug() << " - " << action->info() << logs::end;
        i++;
    }
    maction_.unlock();
    logger().debug() << "End of defined actions" << logs::end;
}

void ActionManagerTimer::debugTimers()
{
    mtimer_.lock();
    logger().debug() << "Defined timers" << logs::end;
    utils::PointerList<ITimerListener *>::iterator i = timers_.begin();
    while (i != timers_.end()) {
        ITimerListener * timer = *i;
        logger().debug() << " - " << timer->info() << logs::end;
        i++;
    }
    mtimer_.unlock();
    logger().debug() << "End of defined timers" << logs::end;
}

