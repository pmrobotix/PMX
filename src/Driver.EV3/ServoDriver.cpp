//drivers...EV3

#include "ServoDriver.hpp"

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
	sv8_ = servo_motor("in4:i2c88:sv8");
	sv8_.set_polarity(sv8_.polarity_inversed);

}

void ServoDriver::hold(int servoId) // 1 à 8
{
	switch (servoId)
	{
	case 1:
		sv1_.set_command(servo_motor::command_run);
		break;
	case 8:
		sv8_.set_command(servo_motor::command_run);
		break;
	default:
		break;
	}
}

void ServoDriver::setPosition(int servoId, int pos)
{
	switch (servoId)
	{
	case 1:
		sv1_.set_position_sp(pos);
		break;
	case 8:
		sv8_.set_position_sp(pos);
		break;
	default:
		break;
	}
}

void ServoDriver::release(int servoId)
{
	switch (servoId)
	{
	case 1:
		sv1_.set_command(servo_motor::command_float);
		break;
	case 8:
		sv8_.set_command(servo_motor::command_float);
		break;
	default:
		break;
	}
}
