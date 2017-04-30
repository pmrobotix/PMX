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
		: connected_(0)
{
	logger().error() << "ServoDriver::ServoDriver() CCAx12Adc::instance().begin();" << logs::end;

	//CCAx12Adc::instance().begin();

}

void ServoDriver::hold(ServoLabel servo)
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

void ServoDriver::setPosition(ServoLabel servo, double percent)
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

void ServoDriver::release(ServoLabel servo)
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

void ServoDriver::setRate(ServoLabel servo, int millisec)
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
