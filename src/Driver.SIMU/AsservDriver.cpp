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
	leftSpeed_ = 0.0; // m/s
	rightSpeed_ = 0.0;
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

float AsservDriver::convertPowerToSpeed(int power)
{
	//30cm/s= 0.300m/s => 128 de power
	float speed = power * 0.3 / 128.0; //vitesse max = 300mm/s
	return speed;

}

void AsservDriver::computeCounterL()
{
	float deltaT_ms = chrono_.getElapsedTimeInMilliSec() - tLeft_ms_;
	float meters = (float) (deltaT_ms * leftSpeed_ / 1000.0);

	mutexL_.lock();
	//1mm = 1tick //TODO ramener la config de l'asserv !!
	currentLeftCounter_ = (long) (meters * 1000.0); //conversion 1 meter = 1000ticks

	logger().debug() << "computeCounterL "
			<< " leftCounter_="
			<< leftCounter_
			<< " currentLeftCounter_="
			<< currentLeftCounter_
			<< " currentMeters="
			<< meters
			<< logs::end;
	mutexL_.unlock();
}

void AsservDriver::computeCounterR()
{
	float deltaT_ms = chrono_.getElapsedTimeInMilliSec() - tRight_ms_;
	currentRightCounter_ = (deltaT_ms * rightSpeed_ / 1000.0);
}

void AsservDriver::setMotorLeftPosition(int power, long ticksToDo)
{
	if (twLeft_.joinable())
		twLeft_.join();

	computeCounterL();
	mutexL_.lock();
	leftSpeed_ = convertPowerToSpeed(power);
	mutexL_.unlock();
	tLeft_ms_ = chrono_.getElapsedTimeInMilliSec();

	AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
	twLeft_ = w_->positionLeftThread("setMotorLeftPosition", leftCounter_ + currentLeftCounter_ + ticksToDo);
}

void AsservDriver::setMotorRightPosition(int power, long internal_ticks)
{
	computeCounterR();
	rightSpeed_ = convertPowerToSpeed(power);
	tRight_ms_ = chrono_.getElapsedTimeInMilliSec();

	//todo arreter le motor une fois le nb de ticks
}

void AsservDriver::setMotorLeftPower(int power, int timems) //in ticks per sec
{
	if (twLeft_.joinable())
		twLeft_.join();
	computeCounterL();
	mutexL_.lock();
	leftSpeed_ = convertPowerToSpeed(power);
	mutexL_.unlock();
	tLeft_ms_ = chrono_.getElapsedTimeInMilliSec();

	logger().debug() << "setMotorLeftPower power=" << power << " leftSpeed_=" << leftSpeed_ << logs::end;

	if (timems > 0) //stop using thread
	{
		AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
		twLeft_ = w_->memberLeftThread("setMotorLeftPower", timems);
	}
}
void AsservDriver::setMotorRightPower(int power, int timems)
{
	if (twRight_.joinable())
		twRight_.join();
	computeCounterR();
	rightSpeed_ = convertPowerToSpeed(power);
	tRight_ms_ = chrono_.getElapsedTimeInMilliSec();

	if (timems > 0) //stop using thread
	{
		AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
		twRight_ = w_->memberRightThread("setMotorRightPower", timems);
	}
}

long AsservDriver::getLeftExternalEncoder()
{
	computeCounterL();
	//logger().debug() << "getLeftExternalEncoder=" << leftCounter_ + currentLeftCounter_ << logs::end;

	return leftCounter_ + currentLeftCounter_;
}
long AsservDriver::getRightExternalEncoder()
{
	computeCounterR();
	//logger().debug() << "getRightExternalEncoder=" << rightCounter_ + currentRightCounter_ << logs::end;

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
	mutexL_.lock();
	leftCounter_ = 0;
	currentLeftCounter_ = 0;
	mutexL_.lock();

	rightCounter_ = 0;
	currentRightCounter_ = 0;
}

void AsservDriver::stopMotorLeft()
{
	computeCounterL();
	mutexL_.lock();
	leftCounter_ += currentLeftCounter_;
	currentLeftCounter_ = 0;
	leftSpeed_ = 0;
	mutexL_.unlock();
	logger().debug() << "stopMotorLeft !!!!!" << logs::end;
}
void AsservDriver::stopMotorRight()
{
	computeCounterR();
	rightCounter_ += currentRightCounter_;
	currentRightCounter_ = 0;
	rightSpeed_ = 0;
	logger().debug() << "stopMotorRight !!!!!" << logs::end;
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
