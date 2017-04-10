/*!
 * \file
 * \brief Définition de la classe GpioPCA9555.
 * Program for the 12C PCA9555 Board part number DFR0013 IIC TO GPIO module from dfrobot.com
 * 16 outputs that I used to drive this relay board made in Bulgaria
 * http://www.denkovi.com/product/21/16-relay-board-for-your-pic-avr-project-12v.html
 */

#ifndef OPOS6UL_GPIO_PCA9555_HPP
#define OPOS6UL_GPIO_PCA9555_HPP

#include <as_devices/cpp/as_i2c.hpp>

#include "../Log/LoggerFactory.hpp"

//  with no jumpers the full address is   1 0 0 1 1 1    1 0 0 A2 A1 A0  0x27 is the default address for the DFR0013 board with no jumpers.
// 0x27 is default address for the DFR0013 board with no jumpers.
// 0x20 is address for the DFR0013 board with all jumpers.
#define GPIOBOARD_PCA9555		 0x24

// COMMAND BYTE TO REGISTER RELATIONSHIP FROM PCA9555 DATA SHEET
// At reset, the device's ports are inputs with a high value resistor pull-ups to VDD
// If relays turning on during power up are a problem. Add a pull down resistor to each relay transistor base.
#define IN_P0 			0x00 // Read Input port0
#define IN_P1 			0x01 // Read Input port1
#define OUT_P0 		0x02 // Write Output port0
#define OUT_P1 		0x03 // Write Output port1
#define INV_P0 			0x04 // Input Port Polarity Inversion port0 if B11111111 is written input polarity is inverted
#define INV_P1 			0x05 // Input Port Polarity Inversion port1 if B11111111 is written input polarity is inverted
#define CONFIG_P0 	0x06 // Configuration port0 configures the direction of the I/O pins 0 is output 1 is input
#define CONFIG_P1 	0x07 // Configuration port1 configures the direction of the I/O pins 0 is output 1 is input

#define PAUSE 200000 //us

/*!
 * \brief Implementation for the 12C PCA9555 Board part number DFR0013 IIC TO GPIO module from dfrobot.com.
 */
class GpioPCA9555
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref GpioPCA9555 (OPOS6UL).
	 */
	static const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("GpioPCA9555.OPO");
		return instance;
	}

	AsI2c i2cGB_;

	bool connected_;

	int port0Value_;
	int port1Value_;

	/*!
	 * \brief Constructeur de la classe.
	 */
	GpioPCA9555();

	void write_i2c(unsigned char command, unsigned char value);
	long read_i2c(unsigned char command);

public:

	static GpioPCA9555 & instance()
	{
		static GpioPCA9555 instance;
		return instance;
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~GpioPCA9555()
	{
	}

	void begin();

	bool isConnected()
	{
		return connected_;
	}

	int getValueP1(int pin);
	void setValueP0(int port, int pin, int value);
	void setOnP0(int pin);
	void setOffP0(int pin);
	void setup();

};

#endif
