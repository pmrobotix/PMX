//drivers...SIMU

#include "ServoDriver.hpp"

#include <cmath>

#include "../Log/Logger.hpp"

using namespace std;

AServoDriver * AServoDriver::create()
{
	static ServoDriver *instance = new ServoDriver();
	return instance;
}

ServoDriver::ServoDriver()
		: connected_(0), left_(4), right_(0), centre_(1)
{
	logger().debug() << "ServoDriver()" << logs::end;

	right_.isInverted();

	//TODO set default rate ?
}

void ServoDriver::hold(int servo)
{
	switch (servo)
	{
	/*
	case SERVO_LEFT:
		left_.setServoEnable(1);
		break;

	case SERVO_RIGHT:
		right_.setServoEnable(1);
		break;

	case SERVO_CENTRE:
		centre_.setServoEnable(1);
		break;
*/
	default:
		break;
	}
}

void ServoDriver::setPosition(int servo, int percent)
{
	if (percent >= 100)
		percent = 100;
	if (percent <= -100)
		percent = -100;
	//   0   -> 4096
	//-100.0%-> 100.0% percent

	double pos = round((percent) * 20.48) + 2048;
	//double pos = percent;

//

	logger().error() << "servo" << servo << " pos=" << (int) pos << logs::end;
	switch (servo)
	{
	/*
	case SERVO_LEFT:
		left_.setServoPosition((int) pos);
		break;

	case SERVO_RIGHT:
		right_.setServoPosition((int) pos);
		break;

	case SERVO_CENTRE:
		centre_.setServoPosition((int) pos);
		break;
*/
	default:
		break;
	}
}

void ServoDriver::release(int servo)
{
	switch (servo)
	{
	/*
	case SERVO_LEFT:
		left_.setServoEnable(0);
		break;

	case SERVO_RIGHT:
		right_.setServoEnable(0);
		break;

	case SERVO_CENTRE:
		centre_.setServoEnable(0);
		break;
*/
	default:
		break;
	}
}

void ServoDriver::setRate(int servo, int millisec)
{
	switch (servo)
	{
	/*
	case SERVO_LEFT:
		break;

	case SERVO_RIGHT:
		break;

	case SERVO_CENTRE:
		break;
*/
	default:
		break;
	}
}


void ServoDriver::turn(int servo, int speed)
{

}

int ServoDriver::getMoving(int servo){

}

int ServoDriver::getPos(int servo){

}

int ServoDriver::ping(int){

}
