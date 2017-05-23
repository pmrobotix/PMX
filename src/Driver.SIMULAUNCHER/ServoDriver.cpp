//drivers...SIMU

#include "ServoDriver.hpp"

#include "../Log/Logger.hpp"

using namespace std;

AServoDriver * AServoDriver::create()
{
	static ServoDriver *instance = new ServoDriver();
	return instance;
}

ServoDriver::ServoDriver()
		: connected_(0)
{
	logger().debug() << "ServoDriver()" << logs::end;

}

void ServoDriver::hold(int servo) // 1 à 8
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

void ServoDriver::setPosition(int servo, int percent)
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

void ServoDriver::release(int servo)
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

