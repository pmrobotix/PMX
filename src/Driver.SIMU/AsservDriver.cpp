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
	tLeft_ms_ = 0.0;
	tRight_ms_ = 0.0;

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
	float speed = ((float) power * 0.3f) / 128.0f; //vitesse max = 300mm/s
	return speed;

}

void AsservDriver::computeCounterL()
{
	float deltaT_ms = chrono_.getElapsedTimeInMilliSec() - tLeft_ms_;
	float currentLeftMeters = (deltaT_ms * leftSpeed_) / 1000.0f;
	mutexL_.lock();
	//1mm = 1tick //TODO ramener la config de l'asserv !!
	currentLeftCounter_ = (long) (currentLeftMeters * 1000.0f); //conversion 1 meter = 1000ticks
	mutexL_.unlock();

	//logger().debug() << "computeCounterL " << " deltaT_ms=" << deltaT_ms << " leftSpeed_=" << leftSpeed_ << logs::end;

	loggerM().debug() << "computeCounterL "
			<< " leftCounter_="
			<< leftCounter_
			<< " currentLeftCounter_="
			<< currentLeftCounter_
			<< " currentMeters="
			<< currentLeftMeters
			<< logs::end;

}

void AsservDriver::computeCounterR()
{

	float deltaT_ms = chrono_.getElapsedTimeInMilliSec() - tRight_ms_;
	float currentRightMeters = (deltaT_ms * rightSpeed_) / 1000.0f;
	mutexR_.lock();
	//1mm = 1tick //TODO ramener la config de l'asserv !!
	currentRightCounter_ = (long) (currentRightMeters * 1000.0f); //conversion 1 meter = 1000ticks
	mutexR_.unlock();

	loggerM().debug() << "computeCounterR "
			<< " rightCounter_="
			<< rightCounter_
			<< " currentRightCounter_="
			<< currentRightCounter_
			<< " currentMeters="
			<< currentRightMeters
			<< logs::end;

}

void AsservDriver::setMotorLeftPosition(int power, long ticksToDo)
{
	if (twLeft_.joinable())
		twLeft_.join();

	int sens = 0;
	if (power < 0)
		sens = -1;
	else if (power > 0)
		sens = 1;

	computeCounterL();
	mutexL_.lock();
	leftSpeed_ = convertPowerToSpeed(power);
	tLeft_ms_ = chrono_.getElapsedTimeInMilliSec();
	mutexL_.unlock();

	logger().debug() << "setMotorLeftPosition    power=" << power << " leftSpeed_=" << leftSpeed_ << logs::end;

	AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
	twLeft_ = w_->positionLeftThread("setMotorLeftPosition", leftCounter_ + currentLeftCounter_ + (ticksToDo * sens));
}

void AsservDriver::setMotorRightPosition(int power, long ticksToDo)
{
	if (twRight_.joinable())
		twRight_.join();

	int sens = 0;
	if (power < 0)
		sens = -1;
	else if (power > 0)
		sens = 1;

	computeCounterR();
	mutexR_.lock();
	rightSpeed_ = convertPowerToSpeed(power);
	tRight_ms_ = chrono_.getElapsedTimeInMilliSec();
	mutexR_.unlock();
	computeCounterR();
	AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
	twRight_ = w_->positionRightThread("setMotorRightPosition",
			rightCounter_ + currentRightCounter_ + (ticksToDo * sens));
}

void AsservDriver::setMotorLeftPower(int power, int time_ms) //in ticks per sec
{
	if (twLeft_.joinable())
		twLeft_.join();

	computeCounterL();
	mutexL_.lock();
	leftSpeed_ = convertPowerToSpeed(power);
	tLeft_ms_ = chrono_.getElapsedTimeInMilliSec();
	mutexL_.unlock();
	logger().debug() << "setMotorLeftPower power=" << power << " leftSpeed_=" << leftSpeed_ << logs::end;
	computeCounterL();

	AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
	twLeft_ = w_->memberLeftThread("setMotorLeftPower", time_ms);
}
void AsservDriver::setMotorRightPower(int power, int time_ms)
{
	if (twRight_.joinable())
		twRight_.join();
	computeCounterR();
	mutexR_.lock();
	rightSpeed_ = convertPowerToSpeed(power);
	tRight_ms_ = chrono_.getElapsedTimeInMilliSec();
	mutexR_.unlock();
	computeCounterL();

	AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
	twRight_ = w_->memberRightThread("setMotorRightPower", time_ms);

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
	mutexL_.unlock();

	mutexR_.lock();
	rightCounter_ = 0;
	currentRightCounter_ = 0;
	mutexR_.unlock();
}

void AsservDriver::stopMotorLeft()
{
	computeCounterL();
	mutexL_.lock();
	leftCounter_ += currentLeftCounter_;
	currentLeftCounter_ = 0;
	leftSpeed_ = 0;
	mutexL_.unlock();
	computeCounterL();
	logger().debug() << "stopMotorLeft !!!!!" << logs::end;
}

void AsservDriver::stopMotorRight()
{
	computeCounterR();
	mutexR_.lock();
	rightCounter_ += currentRightCounter_;
	currentRightCounter_ = 0;
	rightSpeed_ = 0;
	mutexR_.unlock();
	computeCounterR();
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
