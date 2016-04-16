/*!
 * \file
 * \brief Implémentation de la classe ActionManagerTimer.
 */

#include "ActionManagerTimer.hpp"



ActionManagerTimer::ActionManagerTimer()
		: stop_(false)
{
}

void ActionManagerTimer::execute()
{
	logger().info() << "ActionManagerTimer is started stop_=" << stop_ << logs::end;

	int size = 0;
	long tps = 0;
	long starttime = 0;

	chronoTimer_.start();
	while (!stop_)
	{
		//on traite les timers

		mtimer_.lock();
		size = timers_.size();
		if (size > 0)
		{
			starttime = chronoTimer_.getElapsedTimeInMicroSec();

			utils::PointerList<ITimerListener *>::iterator i = timers_.begin();
			while (i != timers_.end())
			{
				ITimerListener * timer = *i;

				tps = starttime - timer->getLastTime();
//				logger().debug() <<  " size=" << size << " - " << timer->info()
//						<< " tps=" << tps
//						<< " starttime=" << starttime
//						<< " timer->lastTime()=" << timer->getLastTime()
//						<< logs::end;
				if (tps >= (timer->timeSpan() * 1000))
				{
					timer->setLastTime(starttime);
					timer->onTimer(chronoTimer_);
				}
				i++;
			}
		}
		mtimer_.unlock();

		//on traite les actions
		maction_.lock();
		size = actions_.size();

		//logger().debug() << "size : " << size << logs::end;
		if (size > 0)
		{

			IAction * action = actions_.front();
			if (action == NULL)
			{
				logger().error("action is NULL");
			}
			actions_.pop_front();
			maction_.unlock();

			bool persist = action->execute();

			maction_.lock();
			if (persist)
			{
				actions_.push_back(action);
			}
			else
			{
				delete action;
			}

		}

		maction_.unlock();
	}

	stop_ = false;
	logger().debug("ActionManagerTimer is stopped");
}

void ActionManagerTimer::debugActions()
{
	maction_.lock();
	//logger().info() << "Defined actions" << logs::end;
	utils::PointerList<IAction *>::iterator i = actions_.begin();
	while (i != actions_.end())
	{
		IAction * action = *i;
		logger().debug() << " - " << action->info() << logs::end;
		i++;
	}
	maction_.unlock();
	//logger().info() << "End of defined actions" << logs::end;
}
