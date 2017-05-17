/*!
 * \file
 * \brief Implémentation de la classe CCAx12Adc.
 */

#include "CCAx12Adc.hpp"

#include <sys/time.h>
#include <unistd.h>
#include <cstdio>

#include "../Log/Logger.hpp"

CCAx12Adc::CCAx12Adc() :
		i2c_CCAx12Adc_(1)
		, connected_(false)
{
	begin();
}

void CCAx12Adc::begin()
{
	connected_ = false;

	//open i2c and setslave
	i2c_CCAx12Adc_.setSlaveAddr(AX12ADC_ADDR);

	int present = pingAX(6);
	if (present)
	{
		connected_ = true;
	}

}

int CCAx12Adc::getAddressSize(int address)
{
	switch (address)
	{
	case P_MODEL_NUMBER:
	case P_CW_ANGLE_LIMIT:
	case P_CCW_ANGLE_LIMIT:
	case P_MAX_TORQUE:
	case P_DOWN_CALIBRATION:
	case P_UP_CALIBRATION:
	case P_GOAL_POSITION:
	case P_GOAL_SPEED:
	case P_TORQUE_LIMIT:
	case P_PRESENT_POSITION:
	case P_PRESENT_SPEED:
	case P_PRESENT_LOAD:
	case P_PUNCH:
		return 2;
	default:
		return 1;
	}
}

// Set the led on
// @param led :  1 - 10
void CCAx12Adc::setLedOn(int led)
{
	if (!connected_)
	{
		logger().error() << "CCAx12Adc::setLedOn() : BOARD NOT CONNECTED !" << logs::end;
		return;
	}
	//wiringPiI2CWrite(fd, CMD_SET_LED_ON);
	//wiringPiI2CWrite(fd, led);
	//write_i2c(CMD_SET_LED_ON, led);
	write(CMD_SET_LED_ON);
	write(led);
}

// Set the led off
// @param led :  1 - 10
void CCAx12Adc::setLedOff(int led)
{
	if (!connected_)
	{
		logger().error() << "CCAx12Adc::setLedOff() : BOARD NOT CONNECTED !" << logs::end;
		return;
	}
	//wiringPiI2CWrite(fd, CMD_SET_LED_OFF);
	//wiringPiI2CWrite(fd, led);
	//write_i2c(CMD_SET_LED_OFF, led);
	write(CMD_SET_LED_OFF);
	write(led);

}

// Get the ADC value (12bits)
// @param ADC :  0 - 9
// @returns value : 0 - 4095
int CCAx12Adc::getADC(int adc)
{
	if (!connected_)
	{
		logger().error() << "CCAx12Adc::getADC() : BOARD NOT CONNECTED !" << logs::end;
		return -1;
	}
	if (adc >= 9 || adc < 0)
	{
		logger().error() << "CCAx12Adc::getADC() : bad number adc !" << logs::end;
		return -2;
	}

	write(CMD_GET_ADC);
	write(adc);
	int low = read();
	int high = read();
	if (low < 0 || high < 0) return -3;

	//printf("low:%d, high:%d \n", low, high);
	return high * 256 + low;
}

// Ping a Dynamixel Servo
// @param id : the id of the dynamixel
// @returns 1 is servo is found
int CCAx12Adc::pingAX(int id)
{
	if (!connected_)
	{
		logger().error() << "CCAx12Adc::pingAX() : BOARD NOT CONNECTED !" << logs::end;
		return -1;
	}

	write(CMD_PING_AX);
	write(id);
	int err = read();
	if (err == 252) return -1;
	return err;
}

// Read data (8bits or 16bits) at a specified address
// @param id : the id of the dynamixel
// @param address : address of the data
// @returns the data
int CCAx12Adc::readAXData(int id, int address)
{
	if (!connected_)
	{
		logger().error() << "CCAx12Adc::readAXData() : BOARD NOT CONNECTED !" << logs::end;
		return -1;
	}
	int size = getAddressSize(address);

	int err = 0;
	err |= write(CMD_READ_AX);
	err |= write(id);
	err |= write(address);
	if (err == -1) return -1;
	int low = read();
	if (low == 252) return -2;
	if (size > 1)
	{
		int high = read();
		//int high = bytes[1];
		if (low == 252 && high == 252) return -3;
		return high * 256 + low;
	}
	else
	{
		if (low == 252) return -1;
		return low;
	}

}

// Write data (8bits or 16bits) at a specified address
// @param id : the id of the dynamixel
// @param address : address of the data
int CCAx12Adc::writeAXData(int id, int address, int data)
{
	if (!connected_)
	{
		logger().error() << "CCAx12Adc::writeAXData() : BOARD NOT CONNECTED !" << logs::end;
		return -1;
	}

	int size = getAddressSize(address);

	int err = 0;

	err |= write(CMD_WRITE_AX);
	err |= write(id);
	err |= write(address);
	if (err == -1) return -1;

	uint8_t xlow = data & 0xff;
	err |= write(xlow);
	if (err == -1) return -2;

	if (size == 2)
	{
		uint8_t xhigh = (data >> 8);
		err |= write(xhigh);
		if (err == -1) return -3;
	}

	int low = read();
	if (low == 252) return -2;

	return low;
}

int CCAx12Adc::write_i2c(unsigned char command, unsigned char value)
{
	return i2c_CCAx12Adc_.writeRegByte(command, value);
}

int CCAx12Adc::write(unsigned char command)
{

	if (i2c_CCAx12Adc_.write(&command, 1))
	{
		printf("write > cmd=%d > error!\n", command);
		return -1;
	}
	return 0;
}

int CCAx12Adc::read()
{

	unsigned char a[1];
	a[0] = 0;
	if (i2c_CCAx12Adc_.read(a, 1) < 0)
	{
		printf("write_readI2c  > read  error!\n");
		return -1;
	}

	return a[0];
}

