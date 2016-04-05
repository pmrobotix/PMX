#include "AsservDriver.hpp"

#include <cmath>

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

	wantedRightSpeed_ = 0.0;
	wantedLeftSpeed_ = 0.0;

	currentLeftCounter_ = 0.0;
	currentRightCounter_ = 0.0;

	rightCounter_ = 0.0;
	leftCounter_ = 0.0;

	chrono_.start();

}

AsservDriver::~AsservDriver()
{
	if (twLeft_.joinable())
		twLeft_.join();
//	if (twRight_.joinable())
//		twRight_.join();
}

float AsservDriver::convertPowerToSpeed(int power)
{

	if (power < 15 && power > -15)
		return 0.0;

	float speed = ((float) power * 0.25f) / 860.0f; //vitesse max = 250mm/s pour 860 de power
	return speed;

}

void AsservDriver::computeCounterL()
{
	// MAJ real speed
	leftSpeed_ = (leftSpeed_ + wantedLeftSpeed_) / 2;
	if (std::abs(leftSpeed_ - wantedLeftSpeed_) < 0.0001)
		leftSpeed_ = 0.0;
	//leftSpeed_ = wantedLeftSpeed_;

	// utilise la real speed
	float deltaT_ms = chrono_.getElapsedTimeInMilliSec() - tLeft_ms_;
	float currentLeftMeters = (deltaT_ms * leftSpeed_) / 1000.0f; //1mm = 1tick //TODO ramener la config de l'asserv !!
	mutexL_.lock();
	currentLeftCounter_ = (currentLeftMeters * 1000.0f); //conversion 1 meter = 1000ticks
	leftCounter_ += currentLeftCounter_;
	mutexL_.unlock();

//	logger().debug() << "computeCounterL "
//			<< " d_ms="
//			<< deltaT_ms
//			<< " leftSpeed_="
//			<< leftSpeed_
//			<< " wantedLSpeed_="
//			<< wantedLeftSpeed_
//			<< " LCounter_="
//			<< leftCounter_
//			<< " currentLCounter_="
//			<< currentLeftCounter_
//			<< logs::end;
//
//	loggerM().debug() << "computeCounterL "
//			<< " leftCounter_="
//			<< leftCounter_
//			<< " currentLeftCounter_="
//			<< currentLeftCounter_
//			<< " currentMeters="
//			<< currentLeftMeters
//			<< logs::end;

}

void AsservDriver::computeCounterR()
{

	rightSpeed_ = (rightSpeed_ + wantedRightSpeed_) / 2;

	float deltaT_ms = chrono_.getElapsedTimeInMilliSec() - tRight_ms_;
	float currentRightMeters = (deltaT_ms * rightSpeed_) / 1000.0f;
	/*
	 //si a=1 alors uniquement wanted speed
	 float a=deltaT_ms/10.0f; //10ms de latence moteur
	 if(a>1)
	 a=1;
	 float b=1-a;
	 rightSpeed_=(rightSpeed_*b+ wantedRightSpeed_*a);
	 */

	mutexR_.lock();
	//1mm = 1tick //TODO ramener la config de l'asserv !!
	currentRightCounter_ = (currentRightMeters * 1000.0f); //conversion 1 meter = 1000ticks
	rightCounter_ += currentRightCounter_;
	mutexR_.unlock();

//	logger().debug() << "computeCounterR "
//			<< " deltaT_ms="
//			<< deltaT_ms
//			<< " rightSpeed_="
//			<< rightSpeed_
//			<< " wantedRightSpeed_="
//			<< wantedRightSpeed_
//			<< " rightCounter_="
//			<< rightCounter_
//			<< " currentRightCounter_="
//			<< currentRightCounter_
//			<< logs::end;
//
//	loggerM().debug() << "computeCounterR "
//			<< " rightCounter_="
//			<< rightCounter_
//			<< " currentRightCounter_="
//			<< currentRightCounter_
//			<< " currentMeters="
//			<< currentRightMeters
//			<< logs::end;

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
	twLeft_ = w_->positionLeftThread("setMotorLeftPosition", leftCounter_ + (ticksToDo * sens));
}

void AsservDriver::setMotorRightPosition(int power, long ticksToDo)
{
//	if (twRight_.joinable())
//		twRight_.join();

	int sens = 0;
	if (power < 0)
		sens = -1;
	else if (power > 0)
		sens = 1;

	computeCounterR();
	mutexR_.lock();
	wantedRightSpeed_ = convertPowerToSpeed(power);
	tRight_ms_ = chrono_.getElapsedTimeInMilliSec();
	mutexR_.unlock();
	computeCounterR();
//	AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
//	twRight_ = w_->positionRightThread("setMotorRightPosition", rightCounter_ + (ticksToDo * sens));
}

void AsservDriver::setMotorLeftPower(int power, int time_ms) //in ticks per sec
{

	if (twLeft_.joinable())
		twLeft_.join();

	computeCounterL();
	mutexL_.lock();
	wantedLeftSpeed_ = convertPowerToSpeed(power);
	tLeft_ms_ = chrono_.getElapsedTimeInMilliSec();
	mutexL_.unlock();
	//logger().debug() << "setMotorLeftPower power=" << power << " leftSpeed_=" << leftSpeed_ << logs::end;
	computeCounterL();

	if (time_ms > 0)
	{
		AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
		twLeft_ = w_->memberLeftThread("setMotorLeftPower", time_ms);
	}
}
void AsservDriver::setMotorRightPower(int power, int time_ms)
{
//	if (twRight_.joinable())
//		twRight_.join();
	computeCounterR();
	mutexR_.lock();
	wantedRightSpeed_ = convertPowerToSpeed(power);
	tRight_ms_ = chrono_.getElapsedTimeInMilliSec();
	mutexR_.unlock();
	computeCounterR();
//	if (time_ms > 0)
//	{
//		AsservDriverWrapper *w_ = new AsservDriverWrapper(this);
//		twRight_ = w_->memberRightThread("setMotorRightPower", time_ms);
//	}
}

long AsservDriver::getLeftExternalEncoder()
{
	computeCounterL();
	//logger().debug() << "getLeftExternalEncoder=" << leftCounter_ + currentLeftCounter_ << logs::end;

	return (long) leftCounter_;
}
long AsservDriver::getRightExternalEncoder()
{
	computeCounterR();
	//logger().debug() << "getRightExternalEncoder=" << rightCounter_ + currentRightCounter_ << logs::end;

	return (long) rightCounter_;
}

//+/- 2,147,483,648
long AsservDriver::getLeftInternalEncoder()
{
	return (long) getLeftExternalEncoder();
}

//+/- 2,147,483,648
long AsservDriver::getRightInternalEncoder()
{
	return (long) getRightExternalEncoder();
}

void AsservDriver::resetEncoder()
{
	mutexL_.lock();
	leftCounter_ = 0.0;
	currentLeftCounter_ = 0.0;

	mutexL_.unlock();

	mutexR_.lock();
	rightCounter_ = 0.0;
	currentRightCounter_ = 0.0;

	mutexR_.unlock();
}

void AsservDriver::stopMotorLeft()
{
	computeCounterL();
	mutexL_.lock();
	currentLeftCounter_ = 0;
	//leftSpeed_ = 0;
	wantedLeftSpeed_ = 0.0;
	mutexL_.unlock();
	computeCounterL();
	logger().debug() << "stopMotorLeft !!!!!" << logs::end; //TODO plante
}

void AsservDriver::stopMotorRight()
{
	computeCounterR();
	mutexR_.lock();
	currentRightCounter_ = 0;
	//rightSpeed_ = 0;
	wantedRightSpeed_ = 0.0;
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
