/*
 * BotManager.hpp
 *
 *  Created on: 31 janv. 2016
 *      Author: pmx
 */

#ifndef SIMULAUNCHER_MAIN_BOTMANAGER_HPP_
#define SIMULAUNCHER_MAIN_BOTMANAGER_HPP_

#include "../Log/LoggerFactory.hpp"

struct SDL_Thread;

class BotManager
{

private:
	SDL_Thread* idthread;
	SDL_Thread* idthread1;
	SDL_Thread* idthreadLedBarLegoEV3;
	SDL_Thread* idthreadLedBarAPF;

	bool start_;
	bool stop_;

public:

	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("BotManager");
		return instance;
	}

	BotManager();

	~BotManager();

	void reset();

	void launchLedBarTest();

	void launchRobotThreads();

	void start(bool s)
	{
		start_ = s;
	}

	bool start()
	{
		return start_;
	}

	void stop(bool s)
	{
		stop_ = s;
	}

	bool stop()
	{
		return stop_;
	}
};

#endif /* SIMULAUNCHER_MAIN_BOTMANAGER_HPP_ */
