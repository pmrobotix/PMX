#include <unistd.h>

#include "../../Log/Logger.hpp"
#include "../Action.Driver/AServoDriver.hpp"
#include "Actions.hpp"
#include "ServoObjectsAx12.hpp"

using namespace std;

ServoObjectsAx12::ServoObjectsAx12(Actions & actions) :
		AActionsElement(actions)
{
	servodriver = AServoDriver::create();
	//releaseAll(); //TODO supprimer ici car carte ax12 pas forcement connecté...
}

ServoObjectsAx12::~ServoObjectsAx12()
{
	for (int fooInt = 0; fooInt < 254; fooInt++)
	{
		servodriver->setPosition(fooInt, 0);
		release(fooInt);
	}
}

void ServoObjectsAx12::deploy(int servo, int pos, int keep_sec)
{
	hold(servo);
	servodriver->setPosition(servo, pos); // percentage
	if (keep_sec != 0)
	{
		sleep(keep_sec);
		release(servo);
	}
	hold(servo);
}

void ServoObjectsAx12::turn(int servo, int speed, int keep_sec)
{
	hold(servo);
	servodriver->turn(servo, speed); // percentage
	if (keep_sec != 0)
	{
		sleep(keep_sec);
		release(servo);
	}
	hold(servo);
}

void ServoObjectsAx12::setSpeed(int servo, int speed)
{
	servodriver->setRate(servo, speed);
}

void ServoObjectsAx12::release(int servo)
{
	servodriver->release(servo);
}

void ServoObjectsAx12::hold(int servo)
{
	servodriver->hold(servo);
}

void ServoObjectsAx12::releaseAll()
{
	for (int fooInt = 0; fooInt < 254; fooInt++)
	{
		release(fooInt);
	}

}
void ServoObjectsAx12::holdAll()
{
	for (int fooInt = 0; fooInt < 254; fooInt++)
	{
		hold(fooInt);
	}

}

