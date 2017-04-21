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
		i2c_CCAx12Adc_(1), connected_(true)
{
	begin();
}

void CCAx12Adc::begin()
{
	//open i2c and setslave
	i2c_CCAx12Adc_.setSlaveAddr(AX12ADC_ADDR);

	int present7 = 0;
	//present7 = pingAX(7);
	setLedOff(3);
while(1)
	{

		//writeAXData(7, P_GOAL_POSITION, 1000);
		int d1 = readAXData(7, P_ID);
		//int d1 = getADC(1);
		printf("d1: %d\n", d1);
		//usleep(300000);

		/*setLedOn(3);
		 usleep(300000);
		 setLedOff(3);
		 usleep(300000);*/

	}
	exit(0);

	present7 = pingAX(7);
	int present180 = pingAX(180);

	logger().error() << "present180 :" << present180 << " present7 :" << present7 << logs::end;
	/*
	 long r = write_i2c(CONFIG_P0, 0x00); //defines all pins on Port0 are outputs
	 if (r < 0)
	 {
	 connected_ = false;
	 logger().error() << "setup() : GpioBoard NOT CONNECTED !" << logs::end;
	 return;
	 }
	 else
	 {
	 write_i2c(OUT_P0, 0x00); //clears all relays
	 write_i2c(CONFIG_P1, 0xFF); //defines all pins on Port1 are inputs
	 write_i2c(IN_P1, 0x00); //clears all relays
	 usleep(PAUSE);
	 }
	 */

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
	//wiringPiI2CWrite(fd, CMD_SET_LED_ON);
	//wiringPiI2CWrite(fd, led);
	write_i2c(CMD_SET_LED_ON, led);
}

// Set the led off
// @param led :  1 - 10
void CCAx12Adc::setLedOff(int led)
{
	//wiringPiI2CWrite(fd, CMD_SET_LED_OFF);
	//wiringPiI2CWrite(fd, led);
	write_i2c(CMD_SET_LED_OFF, led);
}

// Get the ADC value (12bits)
// @param ADC :  0 - 9
// @returns value : 0 - 4095
int CCAx12Adc::getADC(int adc)
{
	//wiringPiI2CWrite(fd, CMD_GET_ADC);
	//wiringPiI2CWrite(fd, adc);
	////write_i2c(CMD_GET_ADC, adc);
	//int low = wiringPiI2CRead(fd);
	//int high = wiringPiI2CRead(fd);
	unsigned char bytes[4];
	//read_i2c_nbytes(bytes,2);
	write_readI2c(CMD_GET_ADC, adc, 4, bytes);
	int low = bytes[0];
	int low2 = bytes[1];
	int high = bytes[2];
	int high2 = bytes[3];
	printf("low:%d, low2:%d high:%d high2:%d\n", low, high, low2, high2);
	return high * 256 + low - 128;
}

// Ping a Dynamixel Servo
// @param id : the id of the dynamixel
// @returns 1 is servo is found
int CCAx12Adc::pingAX(int id)
{
	struct timeval t1, t2, t3;
	double elapsedTime1, elapsedTime2;
	// start timer
	gettimeofday(&t1, NULL);
	//wiringPiI2CWrite(fd, CMD_PING_AX);
	//wiringPiI2CWrite(fd, id);
	////write_i2c(CMD_PING_AX, id);

	gettimeofday(&t2, NULL);
	//int d2 = wiringPiI2CRead(fd);
	unsigned char bytes[1];
	//read_i2c_nbytes(bytes,1);
	write_readI2c(CMD_PING_AX, id, 1, bytes);
	int d2 = bytes[0];
	// stop timer
	gettimeofday(&t3, NULL);

	// compute and print the elapsed time in millisec
	elapsedTime1 = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
	elapsedTime1 += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
	elapsedTime2 = (t3.tv_sec - t2.tv_sec) * 1000.0;      // sec to ms
	elapsedTime2 += (t3.tv_usec - t2.tv_usec) / 1000.0;   // us to ms

	printf("pingAX %d read1: %d w: %lf ms  r: %lf ms\n ", id, d2, elapsedTime1, elapsedTime2);
	return d2;
}

// Read data (8bits or 16bits) at a specified address
// @param id : the id of the dynamixel
// @param address : address of the data
// @returns the data
int CCAx12Adc::readAXData(int id, int address)
{

	//int size = getAddressSize(address);
	//wiringPiI2CWrite(fd, CMD_READ_AX);
	//wiringPiI2CWrite(fd, id);
	//wiringPiI2CWrite(fd, address);
	//int low = wiringPiI2CRead(fd);
	//int high = wiringPiI2CRead(fd);

	unsigned char bytes[4];
	write3_readI2c(CMD_READ_AX, id, address, 4, bytes);
	int low = bytes[0];
	int high = bytes[1];

	printf("readAXData low:%d high:%d %d %d\n", low, high, bytes[2], bytes[3]);
	return high * 256 + low;

}

// Write data (8bits or 16bits) at a specified address
// @param id : the id of the dynamixel
// @param address : address of the data
int CCAx12Adc::writeAXData(int id, int address, int data)
{
	/*
	 int size = getAddressSize(address);
	 wiringPiI2CWrite(fd, CMD_WRITE_AX);
	 wiringPiI2CWrite(fd, id);
	 wiringPiI2CWrite(fd, address);
	 if (size == 1)
	 {
	 wiringPiI2CWrite(fd, data);
	 }
	 else
	 {
	 uint8_t xlow = data & 0xff;
	 uint8_t xhigh = (data >> 8);
	 wiringPiI2CWrite(fd, xlow);
	 wiringPiI2CWrite(fd, xhigh);
	 }
	 int error = wiringPiI2CRead(fd);
	 return error;*/
}

int CCAx12Adc::write3_readI2c(unsigned char command, unsigned char value, unsigned char addr,
		unsigned char nbBytes2Read, unsigned char* data)
{
	/*
	 if (i2c_CCAx12Adc_.writeRegByte(command, value) < 0)
	 {
	 printf("write3_readI2c > writeRegByte > %c %d > error!\n", command, value);
	 return -1;
	 }

	 //Read the data back from the slave
	 if (i2c_CCAx12Adc_.readReg(addr, data, nbBytes2Read) < 0)
	 {
	 printf("write3_readI2c > readReg > %d %d %d > error!\n", addr, data, nbBytes2Read);
	 return -1;
	 }*/
	unsigned char writedata[3];

	writedata[0] = command;
	writedata[1] = command;
	if (i2c_CCAx12Adc_.write(writedata, 2) < 0)
	{
		printf("write3_readI2c > write > %d %d %d> error!\n", command, value, addr);
		return -1;
	}
	writedata[0] = value;
	writedata[1] = value;
	if (i2c_CCAx12Adc_.write(writedata, 2) < 0)
	{
		printf("write3_readI2c > write > %d %d %d> error!\n", command, value, addr);
		return -1;
	}

	writedata[0] = addr;
	writedata[1] = addr;
	if (i2c_CCAx12Adc_.write(writedata, 2) < 0)
	{
		printf("write3_readI2c > write > %d %d %d> error!\n", command, value, addr);
		return -1;
	}

	printf("write_readI2c > write 3bytes OK !\n");
	//Read the data back from the slave
	if (i2c_CCAx12Adc_.read(data, nbBytes2Read) < 0)
	{
		printf("write_readI2c  > read > nb=%d > error!\n", nbBytes2Read);
		return -2;
	}
	printf("write_readI2c  > read OK !\n");

	return 0;
}
int CCAx12Adc::write_readI2c(unsigned char command, unsigned char value, unsigned char nbBytes2Read,
		unsigned char* data)
{
	if (i2c_CCAx12Adc_.writeRegByte(command, value) < 0)
	{
		printf("write_readI2c > writeRegByte > cmd=%d val=%d > error!\n", command, value);
		return -1;
	}
	printf("write_readI2c  > writeRegByte OK !\n");

	//Read the data back from the slave
	if (i2c_CCAx12Adc_.read(data, nbBytes2Read) < 0)
	{
		printf("write_readI2c  > read > cmd=%d nb=%d > error!\n", command, nbBytes2Read);
		return -2;
	}
	printf("write_readI2c  > read OK !\n");
	return 0;
}

int CCAx12Adc::write_i2c(unsigned char command, unsigned char value)
{
	return i2c_CCAx12Adc_.writeRegByte(command, value);
}
/*
 int CCAx12Adc::read_i2c(unsigned char command)
 {
 return i2c_CCAx12Adc_.readRegByte(command);
 }

 int CCAx12Adc::read_i2c_nbytes(unsigned char *data, int size)
 {
 if (i2c_CCAx12Adc_.read(data, size) < 0)
 {
 printf("read_i2c_2bytes > read > data=%d size=%d > error!\n", data[0], size);
 return -1;
 }
 else
 return 0;
 }*/
/*
 int CCAx12Adc::read_i2c_2bytes(unsigned char command)
 {
 unsigned char data[2];
 long r = i2c_CCAx12Adc_.readReg(command, data, 2);
 if (r < 0) logger().error() << "ERROR read_i2c_2bytes*() " << logs::end;
 unsigned char high = data[0];
 unsigned char low = data[1];

 logger().error() << "high: " << (int) high << " \tlow: " << (int) low << logs::end;
 int distance = ((high * 16 + low) / 16) * 10 / (int) std::pow(2, shift_); // Calculate the range in CM

 return distance;
 }*/

