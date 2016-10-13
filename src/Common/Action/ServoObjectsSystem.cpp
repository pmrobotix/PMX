#include <unistd.h>

#include "../../Log/Logger.hpp"
#include "../Action.Driver/AServoDriver.hpp"
#include "Actions.hpp"
#include "ServoObjectsSystem.hpp"

using namespace std;

ServoObjectsSystem::ServoObjectsSystem(Actions & actions)
		: AActionsElement(actions)
{
	servodriver = AServoDriver::create();
	releaseAll();
}

ServoObjectsSystem::~ServoObjectsSystem()
{
	servodriver->setPosition(SERVO_LEFT, 0);
	servodriver->setPosition(SERVO_CENTRE, 0);
	servodriver->setPosition(SERVO_RIGHT, 0);
	releaseAll();
}

void ServoObjectsSystem::leftDeploy(double percent, bool keep)
{
	leftHold();
	servodriver->setPosition(SERVO_LEFT, percent); // percentage
	if (!keep)
	{
		sleep(2);
		leftRelease();
	}
}
void ServoObjectsSystem::leftRelease()
{
	servodriver->release(SERVO_LEFT);
}
void ServoObjectsSystem::leftHold()
{
	servodriver->hold(SERVO_LEFT);
}

void ServoObjectsSystem::centreDeploy(double percent, bool keep)
{
	centreHold();
	servodriver->setPosition(SERVO_CENTRE, percent); // percentage
	if (!keep)
	{
		sleep(2);
		centreRelease();
	}
}
void ServoObjectsSystem::centreRelease()
{
	servodriver->release(SERVO_CENTRE);
}
void ServoObjectsSystem::centreHold()
{
	servodriver->hold(SERVO_CENTRE);
}

void ServoObjectsSystem::rightDeploy(double percent, bool keep)
{
	rightHold();
	servodriver->setPosition(SERVO_RIGHT, percent); // percentage
	if (!keep)
	{
		sleep(2);
		rightRelease();
	}
}
void ServoObjectsSystem::rightRelease()
{
	servodriver->release(SERVO_RIGHT);
}
void ServoObjectsSystem::rightHold()
{
	servodriver->hold(SERVO_RIGHT);
}

void ServoObjectsSystem::releaseAll()
{
	leftRelease();
	centreRelease();
	rightRelease();
}
void ServoObjectsSystem::holdAll()
{
	leftHold();
	centreHold();
	rightHold();
}
