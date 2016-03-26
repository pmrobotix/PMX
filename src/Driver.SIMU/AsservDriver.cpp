#include "AsservDriver.hpp"

#include "../Log/Logger.hpp"

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
	currentLeftCounter_ = 0;
	currentRightCounter_ = 0;
	rightCounter_ = 0;
	leftCounter_ = 0;
	chrono_.start();
}

AsservDriver::~AsservDriver()
{
	twLeft_.join();
	twRight_.join();
}

void AsservDriver::computeCounterL()
{
	double deltaT_ms = chrono_.getElapsedTimeInMilliSec() - tLeft_ms_;
	currentLeftCounter_ = (deltaT_ms * leftPower_ / 1000.0);
}


void AsservDriver::computeCounterR()
{
	double deltaT_ms = chrono_.getElapsedTimeInMilliSec() - tRight_ms_;
	currentRightCounter_ = (deltaT_ms * rightPower_ / 1000.0);
}

void AsservDriver::setMotorLeftPosition(int power, long internal_ticks)
{
	if (twLeft_.joinable())
			twLeft_.join();

	computeCounterL();
	leftPower_ = power;
	tLeft_ms_ = chrono_.getElapsedTimeInMilliSec();


	AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
	twLeft_ = w_->positionLeftThread("setMotorLeftPosition", internal_ticks);
}

void AsservDriver::setMotorRightPosition(int power, long internal_ticks)
{
	computeCounterR();
	rightPower_ = power;
	tRight_ms_ = chrono_.getElapsedTimeInMilliSec();

	//todo arreter le motor une fois le nb de ticks
}

void AsservDriver::setMotorLeftPower(int power, int timems) //in ticks per sec
{
	if (twLeft_.joinable())
		twLeft_.join();
	computeCounterL();
	leftPower_ = power;
	tLeft_ms_ = chrono_.getElapsedTimeInMilliSec();

	if (timems > 0)
	{
		AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
		twLeft_ = w_->memberLeftThread("setMotorLeftPower", timems);
	}
	else
	{
		//forever until stop
	}
}
void AsservDriver::setMotorRightPower(int power, int timems)
{
	if (twRight_.joinable())
		twRight_.join();
	computeCounterR();
	rightPower_ = power;
	tRight_ms_ = chrono_.getElapsedTimeInMilliSec();

	if (timems > 0) //TODO
	{
		AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
		twRight_ = w_->memberRightThread("setMotorRightPower", timems);
	}
	else
	{
		//forever until stop
	}
}

long AsservDriver::getLeftExternalEncoder()
{
	computeCounterL();
	return leftCounter_ + currentLeftCounter_;
}
long AsservDriver::getRightExternalEncoder()
{
	computeCounterR();
	return rightCounter_ + currentRightCounter_;
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
	currentRightCounter_ = 0;
	currentLeftCounter_ = 0;
}

void AsservDriver::stopMotorLeft()
{
	computeCounterL();
	leftCounter_ += currentLeftCounter_;
	currentLeftCounter_ = 0;
	leftPower_ = 0;
	logger().debug() << "stopMotorLeft" << logs::end;
}
void AsservDriver::stopMotorRight()
{
	computeCounterR();
	rightCounter_ += currentRightCounter_;
	currentRightCounter_ = 0;
	rightPower_ = 0;
	logger().debug() << "stopMotorLeft" << logs::end;
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
