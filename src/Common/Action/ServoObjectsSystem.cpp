#include <unistd.h>

#include "../../Log/Logger.hpp"
#include "../Action.Driver/AServoDriver.hpp"
#include "Actions.hpp"
#include "ServoObjectsSystem.hpp"

using namespace std;

ServoObjectsSystem::ServoObjectsSystem(Actions & actions) :
		AActionsElement(actions)
{
	servodriver = AServoDriver::create();
	releaseAll();
}

ServoObjectsSystem::~ServoObjectsSystem()
{
	servodriver->setPosition(LEFT_CARPET, 0);
	servodriver->setPosition(RIGHT_CARPET, 0);
	releaseAll();
}

void ServoObjectsSystem::leftDeploy(int debug)
{
	holdLeft();
	servodriver->setPosition(LEFT_CARPET, 0);
	usleep(400000);
	servodriver->setPosition(LEFT_CARPET, 75);
	sleep(1);
	if (!debug)
	{
		servodriver->setPosition(LEFT_CARPET, 0);
		sleep(1);
	}
	releaseLeft();
}

void ServoObjectsSystem::rightDeploy(int debug)
{
	holdRight();
	servodriver->setPosition(RIGHT_CARPET, 0);
	usleep(400000);
	servodriver->setPosition(RIGHT_CARPET, 70);
	sleep(1);
	if (!debug)
	{
		servodriver->setPosition(RIGHT_CARPET, 0);
		sleep(1);
	}
	releaseRight();
}

void ServoObjectsSystem::releaseLeft()
{
	servodriver->release(LEFT_CARPET);
}

void ServoObjectsSystem::releaseRight()
{
	servodriver->release(RIGHT_CARPET);
}

void ServoObjectsSystem::holdLeft()
{
	servodriver->hold(LEFT_CARPET);
}
void ServoObjectsSystem::holdRight()
{
	servodriver->hold(RIGHT_CARPET);
}

void ServoObjectsSystem::releaseAll()
{
	releaseLeft();
	releaseRight();
}
void ServoObjectsSystem::holdAll()
{
	holdLeft();
	holdRight();
}
