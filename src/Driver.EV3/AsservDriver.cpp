//drivers...EV3

#include "AsservDriver.hpp"

#include <cerrno>
#include <cstring>

#include "../Log/Logger.hpp"

using namespace std;
using namespace ev3dev;

AAsservDriver * AAsservDriver::create()
{
	static AsservDriver *instance = new AsservDriver();
	return instance;
}

void AsservDriver::reset()
{
	if (_motor_left.connected())
		_motor_left.reset();

	if (_motor_right.connected())
		_motor_right.reset();
}

AsservDriver::AsservDriver()
{
	logger().debug() << "AsservDriver()" << logs::end;

	motor mright = motor(OUTPUT_A);
	if (mright.connected())
	{
		_motor_right = OUTPUT_A;
		logger().info() << "EV3 Motor (RIGHT) - "
				<< mright.address()
				<< " connected (CountPerRot:"
				<< mright.count_per_rot()
				<< " DriverName:"
				<< mright.driver_name()
				<< " Polarity:"
				<< mright.polarity()
				<< " EncoderPolarity:"
				<< mright.encoder_polarity()
				<< ")"
				<< logs::end;
	}
	else
	{
		_motor_right = OUTPUT_A;
		logger().error() << "ERROR OUTPUT_A - Motor (RIGHT) " << "not connected !!" << logs::end;
	}

	motor mleft = motor(OUTPUT_D);
	if (mleft.connected())
	{
		_motor_left = OUTPUT_D;
		logger().info() << "EV3 Motor (LEFT) - "
				<< mleft.address()
				<< " connected (CountPerRot:"
				<< mleft.count_per_rot()
				<< " DriverName:"
				<< mleft.driver_name()
				<< " Polarity:"
				<< mleft.polarity()
				<< " EncoderPolarity:"
				<< mleft.encoder_polarity()
				<< ")"
				<< logs::end;

	}
	else
	{
		_motor_left = OUTPUT_D;
		logger().error() << "ERROR OUTPUT_D - Motor (LEFT) " << "not connected !!" << logs::end;
	}

	if (_motor_right.connected()) //if both motors are connected, then initialize each motor.
	{
		_motor_right.reset();
		_motor_right.set_stop_command(motor::stop_command_brake);
		enableRightHardRegulation(true);
	}

	if (_motor_left.connected()) //if both motors are connected, then initialize each motor.
	{
		_motor_left.reset();
		_motor_left.set_stop_command(motor::stop_command_brake);
		enableLeftHardRegulation(true);
	}
}

void AsservDriver::setMotorLeftPosition(long ticks, int power)
{
	//set_duty_cycle_sp -100 to 100
	//_motor_left.set_position_sp(ticks).set_duty_cycle_sp(50).run_to_rel_pos();

	/*
	 if (connectedLeft_)
	 {
	 logger().debug() << "ticks=" << ticks << " power=" << power << logs::end;

	 enableLeftHardRegulation(true);
	 _motor_left.set_stop_mode(motor::stop_mode_brake);
	 _motor_left.set_position_mode(motor::position_mode_absolute);
	 _motor_left.set_run_mode(motor::run_mode_position);

	 _motor_left.set_position_sp(ticks);
	 _motor_left.set_pulses_per_second_sp(power);
	 _motor_left.set_ramp_up_sp(0);
	 _motor_left.set_ramp_down_sp(0);
	 _motor_left.start();
	 }
	 else
	 {
	 logger().error() << "Left motor not connected !" << logs::end;
	 }*/
}

void AsservDriver::setMotorRightPosition(long ticks, int power)
{/*
 if (connectedRight_)
 {
 enableRightHardRegulation(true);
 _motor_right.set_stop_mode(motor::stop_mode_brake);
 _motor_right.set_position_mode(motor::position_mode_absolute);
 _motor_right.set_run_mode(motor::run_mode_position);

 _motor_right.set_position_sp(ticks);
 _motor_right.set_pulses_per_second_sp(power);
 _motor_right.set_ramp_up_sp(0);
 _motor_right.set_ramp_down_sp(0);
 _motor_right.start();
 }
 else
 {
 logger().error() << "Right motor not connected !" << logs::end;
 }*/
}

//regulation enabled  => power in ticks per second -860 / +860
//regulation disabled => power in percentage -100 / +100
void AsservDriver::setMotorLeftPower(int power, int timems)
{
	if (_motor_left.connected())
	{
		//with time
		if (_motor_left.speed_regulation_enabled() == "on") //speed_sp
		{
			limit(power, 860);

			//Writing sets the target speed in tacho counts per second used when `speed_regulation`
			// is on. Reading returns the current value.
			_motor_left.set_speed_sp(power);

			logger().debug() << "LEFT current motor speed in ticks per second:"
					<< _motor_left.speed()
					<< " count_per_rot:"
					<< _motor_left.count_per_rot()
					<< logs::end;
		}
		else if (_motor_left.speed_regulation_enabled() == "off") //duty_cycle_sp
		{
			limit(power, 100);

			//Units are in percent. Valid values are -100 to 100. A negative value causes
			// the motor to rotate in reverse. This value is only used when `speed_regulation`
			// is off.
			_motor_left.set_duty_cycle_sp(power);
		}

		if (timems > 0)
		{
			_motor_left.set_time_sp(timems);
			_motor_left.run_timed();
		}
		else
		{
			//forever
			_motor_left.run_forever();
		}
	}
	else
	{
		logger().error() << "LEFT motor not connected !" << logs::end;
	}
}

int AsservDriver::limit(int power, int max)
{
	if ((power < -max))
	{
		logger().info() << "ERROR Motor power " << power << " exceeded minimum " << -max << "!!" << logs::end;
		power = -max;
	}
	else if (power > max)
	{
		logger().info() << "ERROR Motor power " << power << "exceeded maximum " << max << "!!" << logs::end;
		power = max;
	}
	return power;
}

void AsservDriver::setMotorRightPower(int power, int timems)
{
	if (_motor_right.connected())
	{
		//with time
		if (_motor_right.speed_regulation_enabled() == "on")			//speed_sp
		{
			//TODO limit 860 ticks per second
			limit(power, 860);

			//Writing sets the target speed in tacho counts per second used when `speed_regulation`
			// is on. Reading returns the current value.
			_motor_right.set_speed_sp(power);

			logger().debug() << "RIGHT current motor speed in ticks per second:"
					<< _motor_right.speed()
					<< " count_per_rot:"
					<< _motor_right.count_per_rot()
					<< logs::end;
		}
		else if (_motor_right.speed_regulation_enabled() == "off") //duty_cycle_sp
		{
			//TODO limit percentage
			limit(power, 100);

			//Units are in percent. Valid values are -100 to 100. A negative value causes
			// the motor to rotate in reverse. This value is only used when `speed_regulation`
			// is off.
			_motor_right.set_duty_cycle_sp(power);
		}

		if (timems > 0)
		{
			_motor_right.set_time_sp(timems);
			_motor_right.run_timed();
		}
		else
		{
			//forever
			_motor_right.run_forever();
		}
	}
	else
	{
		logger().error() << "RIGHT motor not connected !" << logs::end;
	}

}

long AsservDriver::getLeftExternalEncoder()
{
	return 0; //TODO getLeftExternalEncoder
}
long AsservDriver::getRightExternalEncoder()
{
	return 0; //TODO getRightExternalEncoder
}

long AsservDriver::getLeftInternalEncoder()
{
	if (_motor_left.connected())
	{
		//+/- 2,147,483,648
		return _motor_left.position();
	}
	else
		return 0;

}
long AsservDriver::getRightInternalEncoder()
{
	if (_motor_right.connected())
	{
		return _motor_right.position();
	}
	else
		return 0;
}

void AsservDriver::stopMotorLeft()
{
	setMotorLeftPower(0, 0);
	if (_motor_left.connected())
	{
		_motor_left.stop();
	}
	logger().error() << "stopMotorLeft" << logs::end;

}
void AsservDriver::stopMotorRight()
{

	setMotorRightPower(0, 0);
	if (_motor_right.connected())
	{
		_motor_right.stop();
	}
	logger().error() << "stopMotorRight" << logs::end;
}

void AsservDriver::resetEncoder()
{
	if (_motor_left.connected())
	{
		_motor_left.set_position(0);
	}
	if (_motor_right.connected())
	{
		_motor_right.set_position(0);
	}
}

int AsservDriver::getMotorLeftCurrent()
{
	return 0;
}
int AsservDriver::getMotorRightCurrent()
{
	return 0;
}

void AsservDriver::enableLeftHardRegulation(bool enable)
{
	if (_motor_left.connected())
	{
		if (enable)
		{

			_motor_left.set_speed_regulation_enabled("on"); //speed_sp
		}
		else
		{
			_motor_left.set_speed_regulation_enabled("off"); //duty_cycle_sp
		}
	}
}

void AsservDriver::enableRightHardRegulation(bool enable)
{
	if (_motor_right.connected())
	{
		if (enable)
		{
			_motor_right.set_speed_regulation_enabled("on"); //speed_sp
		}
		else
		{
			_motor_right.set_speed_regulation_enabled("off"); //duty_cycle_sp
		}
	}
}

void AsservDriver::enableHardRegulation(bool enable)
{
	enableLeftHardRegulation(enable);
	enableRightHardRegulation(enable);
}
