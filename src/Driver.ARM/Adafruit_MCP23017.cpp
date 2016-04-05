/*************************************************** 
 This is a library for the MCP23017 i2c port expander

 These displays use I2C to communicate, 2 pins are required to
 interface
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries.
 BSD license, all text above must be included in any redistribution
 ****************************************************/

//#include <Wire.h>
//#include <avr/pgmspace.h>
#include "Adafruit_MCP23017.hpp"

#include <stdlib.h>



/*
 #ifdef __AVR__
 #define WIRE Wire
 #else // Arduino Due
 #define WIRE Wire1
 #endif*/
/*
 #if ARDUINO >= 100
 #include "Arduino.h"
 #else
 #include "WProgram.h"
 #endif*/
/*
// minihelper
static inline void wiresend(uint8_t x)
{

	 #if ARDUINO >= 100
	 WIRE.write((uint8_t)x);
	 #else
	 WIRE.send(x);
	 #endif
}

static inline uint8_t wirerecv(void)
{

	 #if ARDUINO >= 100
	 return WIRE.read();
	 #else
	 return WIRE.receive();
	 #endif
}
*/
////////////////////////////////////////////////////////////////////////////////


void Adafruit_MCP23017::begin(void)
{


	//try
	//{
		//open i2c and setslave
		MCP_i2c_.open(MCP23017_ADDRESS);

		//setup
		write_i2c(MCP23017_IODIRA, 0xFF); // all inputs on port A
		write_i2c(MCP23017_IODIRB, 0xFF); // all inputs on port B

	/*} catch (utils::Exception * e)
	{

		logger().error() << "begin()::Exception - Adafruit_MCP23017 NOT CONNECTED !!! (... test) " //<< e->what()
				<< utils::end;
	}*/
}

void Adafruit_MCP23017::pinMode(uint8_t p, uint8_t d)
{
	uint8_t iodir;
	uint8_t iodiraddr;

	// only 16 bits!
	if (p > 15)
		return;

	if (p < 8)
		iodiraddr = MCP23017_IODIRA;
	else
	{
		iodiraddr = MCP23017_IODIRB;
		p -= 8;
	}

	// read the current IODIR
	/*
	 WIRE.beginTransmission(MCP23017_ADDRESS | i2caddr);
	 wiresend(iodiraddr);
	 WIRE.endTransmission();
	 WIRE.requestFrom(MCP23017_ADDRESS | i2caddr, 1);
	 iodir = wirerecv();
	 */
	iodir = read_i2c(iodiraddr);

	// set the pin and direction
	if (d == INPUT)
	{
		iodir |= 1 << p;
	}
	else
	{
		iodir &= ~(1 << p);
	}

	// write the new IODIR
	/*
	 WIRE.beginTransmission(MCP23017_ADDRESS | i2caddr);
	 wiresend(iodiraddr);
	 wiresend(iodir);
	 WIRE.endTransmission();*/
	write_i2c(iodiraddr, iodir);
}

uint16_t Adafruit_MCP23017::readGPIOAB()
{
	uint16_t ba = 0;
	uint8_t a;

	// read the current GPIO output latches
	/*
	 WIRE.beginTransmission(MCP23017_ADDRESS | i2caddr);
	 wiresend(MCP23017_GPIOA);
	 WIRE.endTransmission();

	 WIRE.requestFrom(MCP23017_ADDRESS | i2caddr, 2);
	 a = wirerecv();
	 ba = wirerecv();
	 ba <<= 8;
	 ba |= a;
	 */

	a = read_i2c(MCP23017_GPIOA); // TODO use readI2c_2Byte ??
	ba = read_i2c(MCP23017_GPIOA + 1);
	ba <<= 8;
	ba |= a;

	return ba;
}

void Adafruit_MCP23017::writeGPIOAB(uint16_t ba)
{
	/*
	 WIRE.beginTransmission(MCP23017_ADDRESS | i2caddr);
	 wiresend(MCP23017_GPIOA);
	 wiresend(ba & 0xFF);
	 wiresend(ba >> 8);
	 WIRE.endTransmission();*/

	char buf[3];
	buf[0] = MCP23017_GPIOA;
	buf[1] = ba & 0xFF;
	buf[2] = ba >> 8;
	writeI2c_3Bytes(buf);

}

void Adafruit_MCP23017::digitalWrite(uint8_t p, uint8_t d)
{
	uint8_t gpio;
	uint8_t gpioaddr, olataddr;

	// only 16 bits!
	if (p > 15)
		return;

	if (p < 8)
	{
		olataddr = MCP23017_OLATA;
		gpioaddr = MCP23017_GPIOA;
	}
	else
	{
		olataddr = MCP23017_OLATB;
		gpioaddr = MCP23017_GPIOB;
		p -= 8;
	}

	// read the current GPIO output latches
	/*
	 WIRE.beginTransmission(MCP23017_ADDRESS | i2caddr);
	 wiresend(olataddr);
	 WIRE.endTransmission();

	 WIRE.requestFrom(MCP23017_ADDRESS | i2caddr, 1);
	 gpio = wirerecv();*/

	gpio = read_i2c(olataddr);

	// set the pin and direction
	if (d == 1) //HIGH
	{
		gpio |= 1 << p;
	}
	else
	{
		gpio &= ~(1 << p);
	}

	// write the new GPIO
	write_i2c(gpioaddr, gpio);
	/*
	 WIRE.beginTransmission(MCP23017_ADDRESS | i2caddr);
	 wiresend(gpioaddr);
	 wiresend(gpio);
	 WIRE.endTransmission();*/
}

void Adafruit_MCP23017::pullUp(uint8_t p, uint8_t d)
{
	uint8_t gppu;
	uint8_t gppuaddr;

	// only 16 bits!
	if (p > 15)
		return;

	if (p < 8)
		gppuaddr = MCP23017_GPPUA;
	else
	{
		gppuaddr = MCP23017_GPPUB;
		p -= 8;
	}

	// read the current pullup resistor set
	/*
	 WIRE.beginTransmission(MCP23017_ADDRESS | i2caddr);
	 wiresend(gppuaddr);
	 WIRE.endTransmission();

	 WIRE.requestFrom(MCP23017_ADDRESS | i2caddr, 1);
	 gppu = wirerecv();*/

	gppu = read_i2c(gppuaddr);

	// set the pin and direction
	if (d == 1) //HIGH
	{
		gppu |= 1 << p;
	}
	else
	{
		gppu &= ~(1 << p);
	}

	// write the new GPIO
	write_i2c(gppuaddr, gppu);
	/*
	 WIRE.beginTransmission(MCP23017_ADDRESS | i2caddr);
	 wiresend(gppuaddr);
	 wiresend(gppu);
	 WIRE.endTransmission();*/
}

uint8_t Adafruit_MCP23017::digitalRead(uint8_t p)
{
	uint8_t gpioaddr;

	// only 16 bits!
	if (p > 15)
		return 0;

	if (p < 8)
		gpioaddr = MCP23017_GPIOA;
	else
	{
		gpioaddr = MCP23017_GPIOB;
		p -= 8;
	}

	// read the current GPIO
	/*
	 WIRE.beginTransmission(MCP23017_ADDRESS | i2caddr);
	 wiresend(gpioaddr);
	 WIRE.endTransmission();

	 WIRE.requestFrom(MCP23017_ADDRESS | i2caddr, 1);
	 return (wirerecv() >> p) & 0x1;*/
	int ret = read_i2c(gpioaddr);
	return (ret >> p) & 0x1;

}

void Adafruit_MCP23017::write_i2c(unsigned char command, unsigned char value)
{
	MCP_i2c_.writeRegValue(MCP23017_ADDRESS, command, value);
}

int Adafruit_MCP23017::read_i2c(unsigned char command)
{
	unsigned char receivedVal = 0;
	MCP_i2c_.readRegValue(MCP23017_ADDRESS, command, &receivedVal);
	return receivedVal;
}

void Adafruit_MCP23017::writeI2c_3Bytes(const char *buf)
{
	MCP_i2c_.writeI2cSize(MCP23017_ADDRESS, buf, 3);
}

char* Adafruit_MCP23017::readI2c_2Byte()
{
	//char buf[2];
	char *buf = (char *)calloc(2, sizeof(char));
	MCP_i2c_.readI2cSize(MCP23017_ADDRESS, buf, 2);
	return buf;
}
