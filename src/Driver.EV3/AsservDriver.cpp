//drivers...EV3

#include "AsservDriver.hpp"

#include <unistd.h>
#include <ctime>
#include <iostream>
#include <string>

#include "../Common/Utils/Exception.hpp"

using namespace std;
using namespace ev3dev;

AAsservDriver * AAsservDriver::create()
{
	static AsservDriver *instance = new AsservDriver();
	return instance;
}

AsservDriver::AsservDriver() :
		connectedLeft_(0), connectedRight_(0)
{
	logger().debug() << "AsservDriver()" << logs::end;

	//TEST Motors if connected
	motor m = motor(OUTPUT_A);
	if (m.connected())
	{
		if (m.port_name() == OUTPUT_A)
		{
			_motor_right = OUTPUT_A;
			logger().debug() << m.type() << " motor on port " << m.port_name()
					<< " - Connected (RIGHT)" << logs::end;
			connectedRight_++;
		}
	}
	else
	{
		logger().error() << " Motor A " << " not connected !!" << logs::end;
	}

	motor m2 = motor(OUTPUT_D);
	if (m2.connected())
	{
		if (m2.port_name() == OUTPUT_D)
		{
			_motor_left = OUTPUT_D;
			logger().debug() << m2.type() << " motor on port " << m2.port_name()
					<< " - Connected (LEFT)" << logs::end;
			connectedLeft_++;
		}
	}
	else
	{
		logger().error() << " Motor D " << " not connected !!" << logs::end;
	}

	if (connectedRight_) //if both motors are connected, then initialize each motor.
	{
		_motor_right.reset();
		_motor_right.set_position_mode(motor::position_mode_absolute);
		_motor_right.set_stop_mode(motor::stop_mode_brake);

		enableRightHardRegulation(true);
	}
	if (connectedLeft_) //if both motors are connected, then initialize each motor.
	{
		_motor_left.reset();
		_motor_left.set_position_mode(motor::position_mode_absolute);
		_motor_left.set_stop_mode(motor::stop_mode_brake);

		enableLeftHardRegulation(true);
	}

}

void AsservDriver::setMotorLeftPosition(long ticks, int power) // TODO faire un retour d'error de not connected
{
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
}

void AsservDriver::setMotorRightPosition(long ticks, int power)
{
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
}

// -900 < power < +900
void AsservDriver::setMotorLeftPower(int power, int timems)
{
	if (connectedLeft_)
	{
		if ((power < -900))
		{
			cout << "ERROR setMotorLeftPower exceed -900!" << endl;
			power = -900;
		}
		else if (power > 900)
		{
			cout << "ERROR setMotorLeftPower exceed +900!" << endl;
			power = 900;
		}
		else
		{
			if (timems > 0)
			{
				_motor_left.set_run_mode(motor::run_mode_time);
				_motor_left.set_time_sp(timems);
				usleep(timems * 1000);
			}
			else
			{
				_motor_left.set_run_mode(motor::run_mode_forever);
			}
			if (_motor_left.regulation_mode() == motor::mode_on)
			{
				//avec regulation
				//TODO verif de -900 à +900
				_motor_left.set_pulses_per_second_sp(power);
			}
			else
			{
				//sans regulation
				//TODO verif de -100 à +100
				_motor_left.set_duty_cycle_sp(power / 9);
			}

			_motor_left.start();
		}
	}
}
void AsservDriver::setMotorRightPower(int power, int timems)
{
	if (connectedRight_)
	{
		if ((power < -900))
		{
			cout << "ERROR setMotorRightPower exceed -900!" << endl;
			power = -900;
		}
		else if (power > 900)
		{
			cout << "ERROR setMotorRightPower exceed +900!" << endl;
			power = 900;
		}
		else
		{
			if (timems > 0)
			{
				_motor_right.set_run_mode(motor::run_mode_time);
				_motor_right.set_time_sp(timems);
				usleep(timems * 1000);
			}
			else
			{
				_motor_right.set_run_mode(motor::run_mode_forever);
			}
			if (_motor_right.regulation_mode() == motor::mode_on)
			{
				//avec regulation
				_motor_right.set_pulses_per_second_sp(power);
			}
			else
			{
				//sans regulation
				//TODO verif de -100 à +100
				_motor_right.set_duty_cycle_sp(power);
			}
			_motor_right.start();
		}
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
	if (connectedLeft_)
	{
		//+/- 2,147,483,648
		return _motor_left.position();
	}
	else
		return 0;

}
long AsservDriver::getRightInternalEncoder()
{
	if (connectedRight_)
	{
		return _motor_right.position();
	}
	else
		return 0;
}

void AsservDriver::stopMotorLeft()
{

	setMotorLeftPower(0, 0);
	_motor_left.stop();

}
void AsservDriver::stopMotorRight()
{

	setMotorRightPower(0, 0);
	_motor_right.stop();
}

void AsservDriver::resetEncoder()
{
	_motor_left.set_position(0);
	_motor_right.set_position(0);

	//_motor_left.reset();
	//_motor_right.reset();

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
	if (enable)
	{
		_motor_left.set_regulation_mode(motor::mode_on);
	}
	else
	{
		_motor_left.set_regulation_mode(motor::mode_off);
	}
}

void AsservDriver::enableRightHardRegulation(bool enable)
{
	if (enable)
	{
		_motor_right.set_regulation_mode(motor::mode_on);
	}
	else
	{
		_motor_right.set_regulation_mode(motor::mode_off);
	}
}

void AsservDriver::enableHardRegulation(bool enable)
{
	enableLeftHardRegulation(enable);
	enableRightHardRegulation(enable);
}
