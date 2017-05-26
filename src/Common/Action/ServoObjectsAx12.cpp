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
//0 => hold
//>0 => wait sec keeping force
//-1 => wait moving and hold
void ServoObjectsAx12::deploy(int servo, int pos, int keep_millisec)
{
	hold(servo);
	servodriver->setPosition(servo, pos); // percentage
	if (keep_millisec > 0)
	{
		usleep(keep_millisec*1000.0);
		release(servo);
	}
	else if (keep_millisec == -1)
	{
		int b= 0;
		/*
		while (int r = servodriver->getMoving(servo) >= 1)
		{
			if (r < 0)
			{
				break;
			}

			usleep(10000);
		}*/
		int p = servodriver->getPos(servo);
		while ( !((p <= pos + 10) && (p >= pos - 10)))
		{
			p = servodriver->getPos(servo);
			printf ("\n p=%d b=%d\n", p, b);
			b++;
			if (b>1000) break;
			//usleep(100000);
		}

	}
	hold(servo);
}

void ServoObjectsAx12::turn(int servo, int speed, int keep_millisec)
{
	hold(servo);
	servodriver->turn(servo, speed); // percentage
	if (keep_millisec != 0)
	{
		usleep(keep_millisec * 1000.0);
		release(servo);
	}
	hold(servo);
}

void ServoObjectsAx12::setSpeed(int servo, int speed)
{
	servodriver->setRate(servo, speed);
}

void ServoObjectsAx12::setSpeedAll(int speed)
{
	for (int fooInt = 0; fooInt < 52; fooInt++)
	{
		if (fooInt != 12)
		setSpeed(fooInt, speed);
	}
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
	for (int fooInt = 0; fooInt < 52; fooInt++)
	{
		release(fooInt);
	}

}
void ServoObjectsAx12::holdAll()
{
	for (int fooInt = 0; fooInt < 52; fooInt++)
	{
		hold(fooInt);
	}

}

