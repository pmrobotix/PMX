/*
 * BotManager.cpp
 *
 *  Created on: 31 janv. 2016
 *      Author: pmx
 */

#include "BotManager.hpp"

#include <SDL2/SDL_thread.h>
#include <unistd.h>
#include <cstdio>

#include "../Bot-APF9328/APF9328LedBarTest.hpp"
#include "../Bot-APF9328/APF9328RobotExtended.hpp"
#include "../Bot-LegoEV3/L_LedBarTest.hpp"
#include "../Bot-LegoEV3/LegoEV3RobotExtended.hpp"
#include "../Common/ConsoleManager.hpp"
#include "../Common/Robot.hpp"
#include "../Log/Logger.hpp"
#include "SDLTool.hpp"

int threadLegoEV3RobotExtended(void* data)
{
	BotManager* botm = (BotManager*) data;

	LegoEV3RobotExtended &robotlegoev3 = LegoEV3RobotExtended::instance();

	botm->logger().debug() << "Starting threadLegoEV3RobotExtended... " << botm->start() << logs::end;

	while (!botm->start())
	{
		usleep(100000);
	}

	botm->logger().debug() << "Running threadLegoEV3RobotExtended " << botm->start() << logs::end;

	//add specific tests for this robot
	//robotlegoev3.getConsoleManager().add(new LegoEV3LedBarTest());
	/*
	 Arguments &args = robotlegoev3.getArgs();

	 args['c'].set(true);
	 args['c']["color"].empty(); // = "green";

	 args['s'].set(true);
	 args["type"] = "tt";*/

	char *ptest[4];
	ptest[0] = "LegoEV3";
	ptest[1] = "m";
	ptest[2] = "-c";
	ptest[3] = "green";
	ptest[4] = "-s";
	robotlegoev3.parseConsoleArgs(4, ptest);

	//launch automate
	robotlegoev3.begin(0, 0);

	while (!botm->stop())
	{
		//DO something
		usleep(100000);
	}
	botm->logger().debug() << "End threadLegoEV3RobotExtended " << logs::end;

	return 0;
}

int threadAPF9328RobotExtended(void* data)
{
	BotManager* botm = (BotManager*) data;

	APF9328RobotExtended &robotapf = APF9328RobotExtended::instance();

	botm->logger().info() << "Starting threadAPF9328RobotExtended... " << botm->start() << logs::end;
	while (!botm->start())
	{
		usleep(100000);
	}

	while (!botm->stop())
	{
		//DO something
		usleep(100000);
	}
	botm->logger().debug() << "End threadAPF9328RobotExtended " << logs::end;
	return 0;
}

int threadLedBarLegoEV3(void* data)
{
	BotManager* botm = (BotManager*) data;
	LegoEV3RobotExtended &robotlegoev3 = LegoEV3RobotExtended::instance();

	//add specific tests for this robot
	robotlegoev3.getConsoleManager().add(new L_LedBarTest());

	/*
	 Arguments &args = robotlegoev3.getArgs();
	 args["type"] = "t";
	 args['n'].set(true);
	 args['n']["num"] = "1"; //LegoEV3LedBarTest

	 args['c'].set(true);
	 args['c']["color"] = "green";

	 args['s'].set(true);
	 */
//		char *ptest[7];
//		ptest[0] = "LegoEV3";
//		ptest[1] = "t";
//		ptest[2] = "-n";
//		ptest[3] = "1";
//		ptest[4] = "-s";
//		ptest[5] = "-c";
//		ptest[6] = "green";
	char *ptest[] =
	{ "LegoEV3", "t", "-n", "1", "-s", "-c", "green", NULL };
	robotlegoev3.parseConsoleArgs(7, ptest);

	//launch automate
	robotlegoev3.begin(7, ptest);

	return 0;
}

int threadLedBarAPF(void* data)
{

	BotManager* botm = (BotManager*) data;
	APF9328RobotExtended &robotapf = APF9328RobotExtended::instance();

	robotapf.getConsoleManager().add(new APF9328LedBarTest());

	//		 Arguments &args = robotapf.getArgs();
	//		 args["type"] = "t";
	//		 args['n'].set(true);
	//		 args['n']["num"] = "1"; //APF9328LedBarTest
	//
	//		 args['c'].set(true);
	//		 args['c']["color"] = "green";
	//
	//		 args['s'].set(true);

	char *ptest[] =
	{ "APF9328", "t", "-n", "1", "-s", "-c", "green", NULL };
	robotapf.parseConsoleArgs(7, ptest);
	//launch automate
	robotapf.begin(7, ptest);

	return 0;
}

void BotManager::reset()
{
	printf("reset\n");
}

BotManager::BotManager()
{
	logger().debug() << "BotManager" << logs::end;

	start_ = false;
	stop_ = false;

	idthread = NULL;
	idthread1 = NULL;
	idthreadLedBarLegoEV3 = NULL;
	idthreadLedBarAPF = NULL;
}

BotManager::~BotManager()
{
	SDLTool::checkThread(__PRETTY_FUNCTION__);
	SDL_WaitThread(idthread, NULL);
	SDL_WaitThread(idthread1, NULL);
	SDL_WaitThread(idthreadLedBarLegoEV3, NULL);
	SDL_WaitThread(idthreadLedBarAPF, NULL);
}

void BotManager::launchRobotThreads()
{
	SDLTool::checkThread(__PRETTY_FUNCTION__);
//Run the threads with different robots
	idthread = SDL_CreateThread(&threadLegoEV3RobotExtended, "threadLegoEV3RobotExtended", (void*) this);

	idthread1 = SDL_CreateThread(&threadAPF9328RobotExtended, "threadAPF9328RobotExtended", (void*) this);
}

void BotManager::launchLedBarTest()
{
	SDLTool::checkThread(__PRETTY_FUNCTION__);
	idthreadLedBarLegoEV3 = SDL_CreateThread(&threadLedBarLegoEV3, "threadLedBarLegoEV3", (void*) this);

	idthreadLedBarAPF = SDL_CreateThread(&threadLedBarAPF, "threadLedBarAPF", (void*) this);

}
