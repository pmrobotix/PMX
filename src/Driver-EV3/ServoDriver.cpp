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

ServoDriver::ServoDriver() :
		connected_(0)
{
	logger().debug() << "ServoDriver()" << logs::end;

	sv1_ = servo_motor("in4:i2c88:sv1"); //funny action
	//sv1_.set_polarity(sv1_.polarity_inversed);
	sv2_ = servo_motor("in4:i2c88:sv2"); //pince puissance
	sv3_ = servo_motor("in4:i2c88:sv3");
	sv4_ = servo_motor("in4:i2c88:sv4");
	sv5_ = servo_motor("in4:i2c88:sv5");
	sv6_ = servo_motor("in4:i2c88:sv6");

	sv7_ = servo_motor("in4:i2c88:sv7");
	sv8_ = servo_motor("in4:i2c88:sv8");

}

void ServoDriver::hold(ServoLabel servo) // 1 à 8
{
	switch (servo)
	{
	case SERVO_3:
		sv3_.set_command(servo_motor::command_run);
		break;
	case SERVO_7:
		sv7_.set_command(servo_motor::command_run);
		break;
	case SERVO_6:
		sv6_.set_command(servo_motor::command_run);
		break;
	case SERVO_5:
		sv5_.set_command(servo_motor::command_run);
		break;

	case SERVO_PINCE:
		sv4_.set_command(servo_motor::command_run);
		break;

	case SERVO_BASCULE:
		sv8_.set_command(servo_motor::command_run);
		break;

	case SERVO_HERCULE:
		sv2_.set_command(servo_motor::command_run);
		break;

	case SERVO_FUNNY_ACTION:
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

	case SERVO_FUNNY_ACTION:
		sv1_.set_position_sp(pos); //(-100% to 100%)
		break;

	case SERVO_HERCULE:
		sv2_.set_position_sp(pos); //(-100% to 100%)
		break;

	case SERVO_3:
		sv3_.set_position_sp(pos); //(-100% to 100%)
		break;

	case SERVO_PINCE:
		sv4_.set_position_sp(pos); //(-100% to 100%)
		break;
	case SERVO_5:
		sv5_.set_position_sp(pos); //(-100% to 100%)
		break;
	case SERVO_6:
		sv6_.set_position_sp(pos); //(-100% to 100%)
		break;
	case SERVO_7:
		sv7_.set_position_sp(pos); //(-100% to 100%)
		break;
	case SERVO_BASCULE:
		sv8_.set_position_sp(pos); //(-100% to 100%)
		break;

	default:
		break;
	}
}

void ServoDriver::release(ServoLabel servo)
{
	switch (servo)
	{

	case SERVO_FUNNY_ACTION:
		sv1_.set_command(servo_motor::command_float);
		break;

	case SERVO_HERCULE:
		sv2_.set_command(servo_motor::command_float);
		break;

	case SERVO_3:
		sv3_.set_command(servo_motor::command_float);
		break;

	case SERVO_PINCE:
		sv4_.set_command(servo_motor::command_float);
		break;

	case SERVO_5:
		sv5_.set_command(servo_motor::command_float);
		break;
	case SERVO_6:
		sv6_.set_command(servo_motor::command_float);
		break;
	case SERVO_7:
		sv7_.set_command(servo_motor::command_float);
		break;
	case SERVO_BASCULE:
		sv8_.set_command(servo_motor::command_float);
		break;

	default:
		break;
	}
}

void ServoDriver::setRate(ServoLabel servo, int millisec)
{
	switch (servo)
	{

	default:
		break;
	}
}
