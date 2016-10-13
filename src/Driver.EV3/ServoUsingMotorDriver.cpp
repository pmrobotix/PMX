//drivers...EV3

#include "ServoUsingMotorDriver.hpp"

#include <string>

#include "../Log/Logger.hpp"

using namespace std;
using namespace ev3dev;

AServoUsingMotorDriver * AServoUsingMotorDriver::create()
{
	static ServoUsingMotorDriver *instance = new ServoUsingMotorDriver();
	return instance;
}

ServoUsingMotorDriver::ServoUsingMotorDriver()
		: connected_(0)
{
	logger().debug() << "ServoDeviceDriver()" << logs::end;

	motor m = motor(OUTPUT_C);
	if (m.connected())
	{
		_servo_device = OUTPUT_C;
		logger().info() << "EV3 Motor as SERVO - "
				<< m.address()
				<< " connected (CountPerRot:"
				<< m.count_per_rot()
				<< " DriverName:"
				<< m.driver_name()
				<< " Polarity:"
				<< m.polarity()
				<< " EncoderPolarity:"
				<< m.encoder_polarity()
				<< ")"
				<< logs::end;
		connected_ = 1;
	}
	else
	{
		_servo_device = OUTPUT_C;
		logger().error() << "ERROR OUTPUT_C - Motor as SERVO " << "not connected !!" << logs::end;
	}

	if (connected_ == 1)
	{
		_servo_device.reset();
		_servo_device.set_stop_command(motor::stop_command_brake);
		enableHardRegulation(false);
	}
}

void ServoUsingMotorDriver::setMotorPosition(int power, int ticks, int ramptimems)
{
	if (_servo_device.connected())
	{
		_servo_device.set_ramp_down_sp(ramptimems);
		_servo_device.set_ramp_up_sp(ramptimems);
		if (_servo_device.speed_regulation_enabled() == "on") //speed_sp
		{
			power = limit(power, MAXVALUE_speed_sp); //real MAX speed of EV3
			_servo_device.set_position_sp(ticks).set_speed_sp(power).run_to_rel_pos();
		}
		else if (_servo_device.speed_regulation_enabled() == "off") //duty_cycle_sp
		{
			power = limit(power, MAXVALUE_duty_cycle_sp);
			_servo_device.set_position_sp(ticks).set_duty_cycle_sp(power).run_to_rel_pos();
		}
		logger().error() << "ticks = " << ticks << logs::end;
	}
}


long ServoUsingMotorDriver::getInternalEncoder()
{
	if (_servo_device.connected())
	{
		return _servo_device.position();
	}
	else
		return 0;
}

void ServoUsingMotorDriver::stopMotor()
{

	if (_servo_device.connected())
	{
		_servo_device.stop();
	}
}

void ServoUsingMotorDriver::resetEncoder(int pos)
{
	if (_servo_device.connected())
	{
		_servo_device.set_position(pos);
	}
}

int ServoUsingMotorDriver::getMotorCurrent()
{
	return 0;
}

void ServoUsingMotorDriver::enableHardRegulation(bool enable)
{
	if (_servo_device.connected())
	{
		if (enable)
		{
			_servo_device.set_speed_regulation_enabled("on"); //speed_sp
		}
		else
		{
			_servo_device.set_speed_regulation_enabled("off"); //duty_cycle_sp
		}
	}
}

int ServoUsingMotorDriver::limit(int power, int max)
{
	if ((power < -max))
	{
		logger().error() << "ERROR Motor power "
				<< power
				<< " exceeded minimum "
				<< -max
				<< "!!"
				<< logs::end;
		power = -max;
	}
	else if (power > max)
	{
		logger().error() << "ERROR Motor power "
				<< power
				<< "exceeded maximum "
				<< max
				<< "!!"
				<< logs::end;
		power = max;
	}
	return power;
}
