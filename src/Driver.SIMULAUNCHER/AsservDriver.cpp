//drivers...SIMU

#include "AsservDriver.hpp"

using namespace std;

AAsservDriver * AAsservDriver::create()
{
	static AsservDriver *instance = new AsservDriver();
	return instance;
}

AsservDriver::AsservDriver()
{
	tLeft_ms_ = 0;
	tRight_ms_ = 0;
	leftPower_ = 0;
	rightPower_ = 0;

	rightCounter_ = 0;
	leftCounter_ = 0;
	chrono_.start();
}

AsservDriver::~AsservDriver()
{
}

void AsservDriver::computeCounterL()
{
	long deltaT_ms = chrono_.getElapsedTimeInMilliSec() - tLeft_ms_;
	leftCounter_ += (deltaT_ms * leftPower_) / 25;
}

void AsservDriver::computeCounterR()
{
	long deltaT_ms = chrono_.getElapsedTimeInMilliSec() - tRight_ms_;
	rightCounter_ += (deltaT_ms * rightPower_) / 25;
}

void AsservDriver::setMotorLeftPosition(long internal_ticks, int power) //TODO quelle est la référence pour les ticks internal ? or external ?
{
	computeCounterL();
	leftPower_ = power;
	tLeft_ms_ = chrono_.getElapsedTimeInMilliSec();

	//todo arreter le motor une fois le nb de ticks
}

void AsservDriver::setMotorRightPosition(long internal_ticks, int power)
{

}

void AsservDriver::setMotorLeftPower(int power, int timems)
{
	computeCounterL();
	leftPower_ = power;
	tLeft_ms_ = chrono_.getElapsedTimeInMilliSec();

	if (timems > 0)
	{

	}
	else
	{

	}

}
void AsservDriver::setMotorRightPower(int power, int timems)
{
	computeCounterR();
	rightPower_ = power;
	tRight_ms_ = chrono_.getElapsedTimeInMilliSec();

	if (timems > 0) //TODO
	{

	}
	else
	{

	}
}

long AsservDriver::getLeftExternalEncoder()
{
	computeCounterL();
	return leftCounter_;
}
long AsservDriver::getRightExternalEncoder()
{
	computeCounterR();
	return rightCounter_;
}

//+/- 2,147,483,648
long AsservDriver::getLeftInternalEncoder()
{
	return getLeftExternalEncoder();
}

//+/- 2,147,483,648
long AsservDriver::getRightInternalEncoder()
{
	return getRightExternalEncoder();
}

void AsservDriver::resetEncoder()
{
	rightCounter_ = 0;
	leftCounter_ = 0;
}

void AsservDriver::stopMotorLeft()
{
	leftPower_ = 0;
}
void AsservDriver::stopMotorRight()
{
	rightPower_ = 0;
}

int AsservDriver::getMotorLeftCurrent()
{
	return 0;
}
int AsservDriver::getMotorRightCurrent()
{
	return 0;
}

void AsservDriver::enableHardRegulation(bool enable)
{

}
