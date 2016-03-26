#include "MotorControl.hpp"

#include "../Asserv.Driver/AAsservDriver.hpp"

using namespace std;

MotorControl::MotorControl(Asserv & asserv) :
		AAsservElement(asserv)
{
	asservdriver = AAsservDriver::create();

	asservdriver->enableHardRegulation(true);
}

MotorControl::~MotorControl()
{
}

void MotorControl::runMotorLeft(int power, int timems)
{
	asservdriver->setMotorLeftPower(power, timems);
}

void MotorControl::runMotorRight(int power, int timems)
{
	asservdriver->setMotorRightPower(power, timems);
}

void MotorControl::stopMotors()
{
	asservdriver->stopMotorLeft();
	asservdriver->stopMotorRight();
}


void MotorControl::setMotorLeftPosition(int power, long ticks)
{
	asservdriver->setMotorLeftPosition(power, ticks);
}

void MotorControl::setMotorRightPosition(int power, long ticks)
{
	asservdriver->setMotorRightPosition(power, ticks);
}


