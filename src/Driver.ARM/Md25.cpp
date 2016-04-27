/*!
 * \file
 * \brief Implémentation de la classe Md25.
 */

#include "Md25.hpp"

#include <unistd.h>

#include "../Log/Exception.hpp"
#include "../Log/Logger.hpp"

Md25::Md25()
		: connected_(false), current_mode_(MD25_MODE_1)
{
}

void Md25::begin()
{
	try
	{
		//open i2c and setslave
		//md25_i2c_.open(MD25_DEFAULT_ADDRESS);
		md25_i2c_.setSlaveAddr(MD25_DEFAULT_ADDRESS);

		//test getSoftware
		int soft = getSoftwareVersion();
		logger().info() << "soft=" << soft << logs::end;
		if (soft == 3)
		{
			connected_ = true;

			//SETUP MD25
			setup();
		}
		else
		{
			logger().error() << "init() : MD25 is now OFF, Software=" << (int) soft << " not eq 3 !" << logs::end;
		}
	} catch (logs::Exception * e)
	{

		logger().error() << "init()::Exception - Md25 NOT CONNECTED !!! (getSoftwareVersion test) " //<< e->what()
				<< logs::end;
	}
}

void Md25::setup()
{
	setMode(MD25_MODE_0); //mode0 => 0-128-255   ;  mode1 => -128+127
	setAccelerationRate(10); //Acc:1,2,3,5,10
	setCommand(MD25_ENABLE_SPEED_REGULATION);
	setCommand(MD25_RESET_ENCODERS);
}

void Md25::resetEncoders()
{
	setCommand(MD25_RESET_ENCODERS);
}

int Md25::getSoftwareVersion(void)
{
	long val = 0;
	mutex_.lock();
	val = read_i2c(MD25_SOFTWAREVER_REG);
	mutex_.unlock();
	return val;
}

float Md25::getBatteryVolts(void)
{
	return (float) getValue(MD25_VOLTAGE_REG) / 10.0;
}

int Md25::getAccelerationRate()
{
	return getValue(MD25_ACCELRATE_REG);
}

int Md25::getMotor1Speed(void)
{
	return getValue(MD25_SPEED1_REG);
}

int Md25::getMotor2Speed(void)
{
	return getValue(MD25_SPEED2_REG);
}

int Md25::getMotor1Current(void)
{
	return getValue(MD25_CURRENT1_REG);
}

int Md25::getMotor2Current(void)
{
	return getValue(MD25_CURRENT2_REG);
}

int Md25::getMode(void)
{
	return getValue(MD25_MODE_REG);
}

long Md25::getValue(unsigned char reg)
{
	if (!connected_)
	{
		logger().error() << "Md25::getCommand : return 0; MD25 NOT CONNECTED !" << logs::end;
		return 0;
	}
	long val = 0;
	try
	{
		mutex_.lock();
		val = read_i2c(reg);
		mutex_.unlock();
	} catch (logs::Exception * e)
	{
		logger().error() << "Exception getCommand: " << e->what() << logs::end;
	}
	return val;
}

void Md25::getEncoder(long *pvalue, unsigned char MD25Register) //TODO void
{
	if (!connected_)
	{
		logger().error() << "getEncoder : return 0 : MD25 NOT CONNECTED !" << logs::end;
		return;
	}
	unsigned char encoder2 = 0;
	unsigned char encoder5 = 0;
	unsigned char encoder4 = 0;
	unsigned char encoder3 = 0;
	/*
	 mutex_.lock();
	 encoder2 = read_i2c(MD25Register);
	 encoder3 = read_i2c(MD25Register + 1);
	 encoder4 = read_i2c(MD25Register + 2);
	 encoder5 = read_i2c(MD25Register + 3);
	 mutex_.unlock();
	 *pvalue = (encoder2 << 24) + (encoder3 << 16) + (encoder4 << 8) + encoder5;
	 */

	long buff[4];
	read_i2c_4Bytes(MD25Register, buff);

	encoder2 = buff[0];
	encoder3 = buff[1];
	encoder4 = buff[2];
	encoder5 = buff[3];

	*pvalue = (encoder2 << 24) + (encoder3 << 16) + (encoder4 << 8) + encoder5;

	/*
	 logger().debug() << "getEncoder: MD25Register=" << reinterpret_cast<void*>(MD25Register) << " "
	 << reinterpret_cast<void*>(encoder5) << " " << reinterpret_cast<void*>(encoder4) << " "
	 << reinterpret_cast<void*>(encoder3) << " " << reinterpret_cast<void*>(encoder2) << " " << (long) *pvalue
	 << utils::end;
	 */

}

long Md25::ensureGetEncoder(long, unsigned char MD25Register)
{
	if (!connected_)
	{
		logger().error() << "ensureGetEncoder : return 0 : MD25 NOT CONNECTED !" << logs::end;
		return 0.0;
	}
	long value = 0;

	try
	{
		getEncoder(&value, MD25Register);

	} catch (logs::Exception * e)
	{
		logger().error() << "1=>ensureGetEncoder::Exception on getEncoder :" << e->what() << logs::end;

		try
		{
			getEncoder(&value, MD25Register);

		} catch (logs::Exception * e)
		{
			logger().error() << "2=>ensureGetEncoder::Exception on getEncoder :" << e->what() << logs::end;
			try
			{
				getEncoder(&value, MD25Register);

			} catch (logs::Exception * e)
			{
				logger().error() << "3=>ensureGetEncoder::Exception on getEncoder :" << e->what() << logs::end;
				getEncoder(&value, MD25Register);
			}
		}
	}

	return value;
}

void Md25::setMode(unsigned char mode)
{
	if (!connected_)
	{
		logger().error() << "setMode : MD25 NOT CONNECTED !" << logs::end;
		return;
	}
	try
	{
		mutex_.lock();
		write_i2c(MD25_MODE_REG, mode);
		mutex_.unlock();
		current_mode_ = mode;
	} catch (logs::Exception * e)
	{
		logger().error() << "Exception getEncoder: " << e->what() << logs::end;
	}
}

void Md25::setAccelerationRate(unsigned char rate)
{
	if (!connected_)
	{
		logger().error() << "setAccelerationRate : MD25 NOT CONNECTED !" << logs::end;
		return;
	}
	try
	{
		mutex_.lock();
		write_i2c(MD25_ACCELRATE_REG, rate);
		mutex_.unlock();
	} catch (logs::Exception * e)
	{
		logger().error() << "Exception setAccelerationRate: " << e->what() << logs::end;
	}
}

void Md25::setCommand(unsigned char command)
{
	if (!connected_)
	{
		logger().error() << "setCommand : MD25 NOT CONNECTED !" << logs::end;
		return;
	}
	try
	{
		mutex_.lock();
		write_i2c(MD25_CMD_REG, command);
		mutex_.unlock();
	} catch (logs::Exception * e)
	{
		logger().error() << "Exception setCommand: " << e->what() << logs::end;
	}
}

void Md25::setSpeedRegisters(int speed_1, int speed_2)
{
	if (!connected_)
	{
		logger().error() << "setSpeedRegisters : MD25 NOT CONNECTED !" << logs::end;
		return;
	}
	ensureSetSpeed(speed_1, MD25_SPEED1_REG);
	ensureSetSpeed(speed_2, MD25_SPEED2_REG);
}

void Md25::ensureSetSpeed(int speed, unsigned char reg)
{
	if (!connected_)
	{
		logger().error() << "ensureSetSpeed : MD25 NOT CONNECTED !" << logs::end;
		return;
	}
	try
	{
		setSpeedReg(speed, reg);
	} catch (logs::Exception * e)
	{
		logger().error() << "1=>Exception ensureSetSpeed: setSpeedReg :" << e->what() << logs::end;
		try
		{
			setSpeedReg(speed, reg);
		} catch (logs::Exception * e)
		{
			logger().error() << "2=>Exception ensureSetSpeed: setSpeedReg :" << e->what() << logs::end;
			try
			{
				setSpeedReg(speed, reg);
			} catch (logs::Exception * e)
			{
				logger().error() << "3=>Exception ensureSetSpeed: setSpeedReg :" << e->what() << logs::end;
				setSpeedReg(speed, reg);
			}
		}
	}
}

void Md25::setSpeedReg(int speed, unsigned char reg)
{
	if (!connected_)
	{
		logger().error() << "setSpeedReg : return 0 : MD25 NOT CONNECTED !" << logs::end;
		return;
	}
	if ((speed < 0 || speed > 255) && (current_mode_ == MD25_MODE_0 || current_mode_ == MD25_MODE_2))
	{
		logger().error() << "setSpeedReg ERROR : MODE="
				<< current_mode_
				<< " speed="
				<< speed
				<< " ; must 0 < speed < 255 !"
				<< logs::end;
		return;
	}
	if ((speed < -128 || speed > 127) && (current_mode_ == MD25_MODE_1 || current_mode_ == MD25_MODE_3))
	{
		logger().error() << "setSpeedReg ERROR : MODE="
				<< current_mode_
				<< " speed="
				<< speed
				<< " ; must -128 < speed < 127 !"
				<< logs::end;
		return;
	}
	mutex_.lock();
	write_i2c(reg, speed);
	mutex_.unlock();
	//Compare with the read register ; Not possible using (current_mode_ == MD25_MODE_1) because negative values
	unsigned char reading = getValue(reg);
	if (speed != (int)reading)
	{
		logger().error() << "=> ERROR COMPARAISON setSpeedReg"
				<< (int) reg
				<< " : write != read :"
				<< (int) speed
				<< " / "
				<< (int) reading
				<< logs::end;
	}
}

void Md25::stopMotor(unsigned char reg)
{
	if (!connected_)
	{
		logger().error() << "stopMotor : return 0 : MD25 NOT CONNECTED !" << logs::end;
		return;
	}
	switch (current_mode_)
	{
	case MD25_MODE_0:
	case MD25_MODE_2:
		ensureSetSpeed(128, reg);
		break;
	case MD25_MODE_1:
	case MD25_MODE_3:
		ensureSetSpeed(0, reg);
		break;
	}
}

void Md25::stopMotors(void)
{
	if (!connected_)
	{
		logger().error() << "stopMotors : return 0 : MD25 NOT CONNECTED !" << logs::end;
		return;
	}

	stopMotor(MD25_SPEED1_REG);
	stopMotor(MD25_SPEED2_REG);
}

void Md25::write_i2c(unsigned char command, unsigned char value)
{
	//md25_i2c_.writeRegValue(MD25_DEFAULT_ADDRESS, command, value);
	md25_i2c_.writeRegByte(command, value);
}

long Md25::read_i2c(unsigned char command)
{
	long receivedVal = 0;
	//md25_i2c_.readRegValue(MD25_DEFAULT_ADDRESS, command, &receivedVal);
	receivedVal = (long) md25_i2c_.readRegByte(command);
	return receivedVal;
}

//TODO ne marche pas
void Md25::read_i2c_4Bytes(unsigned char reg, long *data)
{
	try
	{

		mutex_.lock();
		//byte par byte
		data[0] = read_i2c(reg);
		data[1] = read_i2c(reg + 1);
		data[2] = read_i2c(reg + 2);
		data[3] = read_i2c(reg + 3);

		//or directly 4 bytes

//		md25_i2c_.readReg(reg, data, 4);
//		usleep(1000);
		mutex_.unlock();

		logger().debug() << "getEncoder: reg="
				<< reinterpret_cast<void*>(reg)
				<< " data[0]="
				<< reinterpret_cast<void*>(data[0])
				<< " data[1] ="
				<< reinterpret_cast<void*>(data[1])
				<< " data[2]"
				<< reinterpret_cast<void*>(data[2])
				<< " data[3]"
				<< reinterpret_cast<void*>(data[3])

				<< logs::end;

	} catch (logs::Exception * e)
	{

		logger().error() << "read_i2c_4Bytes ERROR readReg :" << e->what() << logs::end;

	}

	//int ret = md25_i2c_.readReg_nValue(reg, data, 4);

}

