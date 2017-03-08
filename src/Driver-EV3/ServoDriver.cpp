//drivers...EV3

#include "ServoDriver.hpp"

#include <string>

#include "../Log/Logger.hpp"

using namespace std;
using namespace ev3dev;

AServoDriver * AServoDriver::create()
{
	static ServoDriver *instance = new ServoDriver();
	return instance;
}

ServoDriver::ServoDriver()
		: connected_(0)
{
	logger().debug() << "ServoDriver()" << logs::end;

	sv1_ = servo_motor("in4:i2c88:sv1");
	sv7_ = servo_motor("in4:i2c88:sv7");
	sv8_ = servo_motor("in4:i2c88:sv8");
	sv8_.set_polarity(sv8_.polarity_inversed);

}

void ServoDriver::hold(ServoLabel servo) // 1 à 8
{
	switch (servo)
	{
	case SERVO_LEFT:
		sv7_.set_command(servo_motor::command_run);
		break;

	case SERVO_RIGHT:
		sv8_.set_command(servo_motor::command_run);
		break;

	case SERVO_CENTRE:
		sv1_.set_command(servo_motor::command_run);
		break;

	default:
		break;
	}
}

void ServoDriver::setPosition(ServoLabel servo, double pos)
{
	switch (servo)
	{
	case SERVO_LEFT:
		sv7_.set_position_sp(pos); //(-100% to 100%)
		break;

	case SERVO_RIGHT:
		sv8_.set_position_sp(pos); //(-100% to 100%)
		break;

	case SERVO_CENTRE:
		sv1_.set_position_sp(pos); //(-100% to 100%)
		break;

	default:
		break;
	}
}

void ServoDriver::release(ServoLabel servo)
{
	switch (servo)
	{
	case SERVO_LEFT:
		sv7_.set_command(servo_motor::command_float);
		break;

	case SERVO_RIGHT:
		sv8_.set_command(servo_motor::command_float);
		break;

	case SERVO_CENTRE:
		sv1_.set_command(servo_motor::command_float);
		break;
	default:
		break;
	}
}

void ServoDriver::setRate(ServoLabel servo, int millisec)
{
	switch (servo)
	{
	case SERVO_LEFT:
		break;

	case SERVO_RIGHT:
		break;

	case SERVO_CENTRE:
		break;

	default:
		break;
	}
}
