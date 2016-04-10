/*************************************************** 
 This is a library for the Adafruit RGB 16x2 LCD Shield
 Pick one up at the Adafruit shop!
 ---------> http://http://www.adafruit.com/products/714

 The shield uses I2C to communicate, 2 pins are required to
 interface
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries.
 BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_RGBLCDShield.hpp"

#include <unistd.h>

#include "../Log/Exception.hpp"
#include "../Log/Logger.hpp"



/*
 #include <Wire.h>
 #ifdef __AVR__
 #define WIRE Wire
 #else // Arduino Due
 #define WIRE Wire1
 #endif

 #if ARDUINO >= 100
 #include "Arduino.h"
 #else
 #include "WProgram.h"
 #endif*/

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// RGBLCDShield constructor is called).
Adafruit_RGBLCDShield::Adafruit_RGBLCDShield()
{

	_displaycontrol = 0;
	_displaymode = 0;
	_numlines = 0;
	_currline = 0;
	_i2cAddr = 0;

	connected_ = 0;

	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

	// the I/O expander pinout
	_rs_pin = 15;
	_rw_pin = 14;
	_enable_pin = 13;
	_data_pins[0] = 12;  // really d4
	_data_pins[1] = 11;  // really d5
	_data_pins[2] = 10;  // really d6
	_data_pins[3] = 9;  // really d7

	_button_pins[0] = 0;
	_button_pins[1] = 1;
	_button_pins[2] = 2;
	_button_pins[3] = 3;
	_button_pins[4] = 4;


	begin(16, 2);

}

void Adafruit_RGBLCDShield::begin(uint8_t, uint8_t lines, uint8_t dotsize) //cols, lines, dotsize
{

	//mutex_.lock();
	try
	{
		_i2c.begin();
		connected_ = true;

		// check if i2c
		//if (_i2cAddr != 255)
		//{
		//_i2c.begin(_i2cAddr);
		//WIRE.begin();
		//_i2c.begin();

		_i2c.pinMode(8, OUTPUT);
		_i2c.pinMode(6, OUTPUT);
		_i2c.pinMode(7, OUTPUT);
		setBacklight(0x7);

		if (_rw_pin)
			_i2c.pinMode(_rw_pin, OUTPUT);

		_i2c.pinMode(_rs_pin, OUTPUT);
		_i2c.pinMode(_enable_pin, OUTPUT);
		for (uint8_t i = 0; i < 4; i++)
			_i2c.pinMode(_data_pins[i], OUTPUT);

		for (uint8_t i = 0; i < 5; i++)
		{
			_i2c.pinMode(_button_pins[i], INPUT);
			_i2c.pullUp(_button_pins[i], 1);
		}
		//}

		if (lines > 1)
		{
			_displayfunction |= LCD_2LINE;
		}
		_numlines = lines;
		_currline = 0;

		// for some 1 line displays you can select a 10 pixel high font
		if ((dotsize != 0) && (lines == 1))
		{
			_displayfunction |= LCD_5x10DOTS;
		}

		// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
		// according to datasheet, we need at least 40ms after power rises above 2.7V
		// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
		usleep(50000);

		// Now we pull both RS and R/W low to begin commands
		_digitalWrite(_rs_pin, 0);  //LOW
		_digitalWrite(_enable_pin, 0);  //LOW
		if (_rw_pin != 255)
		{
			_digitalWrite(_rw_pin, 0);  //LOW
		}

		//put the LCD into 4 bit or 8 bit mode
		if (!(_displayfunction & LCD_8BITMODE))
		{
			// this is according to the hitachi HD44780 datasheet
			// figure 24, pg 46

			// we start in 8bit mode, try to set 4 bit mode
			write4bits(0x03);
			usleep(4500); // wait min 4.1ms

			// second try
			write4bits(0x03);
			usleep(4500); // wait min 4.1ms

			// third go!
			write4bits(0x03);
			usleep(150);

			// finally, set to 8-bit interface
			write4bits(0x02);
		}
		else
		{
			// this is according to the hitachi HD44780 datasheet
			// page 45 figure 23

			// Send function set command sequence
			command(LCD_FUNCTIONSET | _displayfunction);
			usleep(4500);  // wait more than 4.1ms

			// second try
			command(LCD_FUNCTIONSET | _displayfunction);
			usleep(150);

			// third go
			command(LCD_FUNCTIONSET | _displayfunction);
		}

		// finally, set # lines, font size, etc.
		command(LCD_FUNCTIONSET | _displayfunction);

		// turn the display on with no cursor or blinking default
		_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
		display();

		// clear it off
		clear();

		// Initialize to default text direction (for romance languages)
		_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
		// set the entry mode
		command(LCD_ENTRYMODESET | _displaymode);
	} catch (logs::Exception * e)
	{

		logger().error() << "begin()::Exception - Adafruit_MCP23017 NOT CONNECTED !!! (begin test) " //<< e->what()
				<< logs::end;
	}
	//mutex_.unlock();
}

/********** high level commands, for the user! */
void Adafruit_RGBLCDShield::clear()
{
	if (!connected_)
	{
		logger().error() << "clear() : Adafruit_RGBLCDShield NOT CONNECTED !" << logs::end;
		return;
	}
	mutex_.lock();
	command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
	usleep(2000);  // this command takes a long time!
	mutex_.unlock();
}

void Adafruit_RGBLCDShield::home()
{
	if (!connected_)
	{
		logger().error() << "home() : Adafruit_RGBLCDShield NOT CONNECTED !" << logs::end;
		return;
	}
	mutex_.lock();
	command(LCD_RETURNHOME);  // set cursor position to zero
	usleep(2000);  // this command takes a long time!
	mutex_.unlock();
}

void Adafruit_RGBLCDShield::setCursor(uint8_t col, uint8_t row)
{
	if (!connected_)
	{
		logger().error() << "setCursor() : Adafruit_RGBLCDShield NOT CONNECTED !" << logs::end;
		return;
	}
	mutex_.lock();
	int row_offsets[] =
	{ 0x00, 0x40, 0x14, 0x54 };
	if (row > _numlines)
	{
		row = _numlines - 1;    // we count rows starting w/0
	}

	command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
	mutex_.unlock();
}

// Turn the display on/off (quickly)
void Adafruit_RGBLCDShield::noDisplay()
{
	mutex_.lock();
	_displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
	mutex_.unlock();
}
void Adafruit_RGBLCDShield::display()
{
	mutex_.lock();
	_displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
	mutex_.unlock();
}

// Turns the underline cursor on/off
void Adafruit_RGBLCDShield::noCursor()
{
	mutex_.lock();
	_displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
	mutex_.unlock();
}
void Adafruit_RGBLCDShield::cursor()
{
	mutex_.lock();
	_displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
	mutex_.unlock();
}

// Turn on and off the blinking cursor
void Adafruit_RGBLCDShield::noBlink()
{
	mutex_.lock();
	_displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
	mutex_.unlock();
}
void Adafruit_RGBLCDShield::blink()
{
	mutex_.lock();
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
	mutex_.unlock();
}

// These commands scroll the display without changing the RAM
void Adafruit_RGBLCDShield::scrollDisplayLeft(void)
{
	if (!connected_)
	{
		logger().error() << "scrollDisplayLeft() : Adafruit_RGBLCDShield NOT CONNECTED !" << logs::end;
		return;
	}
	mutex_.lock();
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
	mutex_.unlock();
}
void Adafruit_RGBLCDShield::scrollDisplayRight(void)
{
	if (!connected_)
	{
		logger().error() << "scrollDisplayLeft() : Adafruit_RGBLCDShield NOT CONNECTED !" << logs::end;
		return;
	}
	mutex_.lock();
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
	mutex_.unlock();
}

// This is for text that flows Left to Right
void Adafruit_RGBLCDShield::leftToRight(void)
{
	mutex_.lock();
	_displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
	mutex_.unlock();
}

// This is for text that flows Right to Left
void Adafruit_RGBLCDShield::rightToLeft(void)
{
	mutex_.lock();
	_displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
	mutex_.unlock();
}

// This will 'right justify' text from the cursor
void Adafruit_RGBLCDShield::autoscroll(void)
{
	if (!connected_)
	{
		logger().error() << "autoscroll() : Adafruit_RGBLCDShield NOT CONNECTED !" << logs::end;
		return;
	}
	mutex_.lock();
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
	mutex_.unlock();
}

// This will 'left justify' text from the cursor
void Adafruit_RGBLCDShield::noAutoscroll(void)
{
	if (!connected_)
	{
		logger().error() << "noAutoscroll() : Adafruit_RGBLCDShield NOT CONNECTED !" << logs::end;
		return;
	}
	mutex_.lock();
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
	mutex_.unlock();
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void Adafruit_RGBLCDShield::createChar(uint8_t location, uint8_t charmap[])
{
	if (!connected_)
	{
		logger().error() << "createChar() : Adafruit_RGBLCDShield NOT CONNECTED !" << logs::end;
		return;
	}
	mutex_.lock();
	location &= 0x7; // we only have 8 locations 0-7
	command(LCD_SETCGRAMADDR | (location << 3));
	for (int i = 0; i < 8; i++)
	{
		write__(charmap[i]);
	}
	command(LCD_SETDDRAMADDR);  // unfortunately resets the location to 0,0
	mutex_.unlock();
}





/*********** mid level commands, for sending data/cmds */

inline void Adafruit_RGBLCDShield::command(uint8_t value)
{
	send(value, 0);
}
/*
 #if ARDUINO >= 100
 inline size_t Adafruit_RGBLCDShield::write(uint8_t value) {
 send(value, HIGH);
 return 1;
 }
 #else*/
size_t Adafruit_RGBLCDShield::write__(uint8_t value)
{
	if (!connected_)
	{
		logger().error() << "write() : Adafruit_RGBLCDShield NOT CONNECTED !" << logs::end;
		return 0;
	}
	send(value, 1); //HIGH
	return 1;
}
//#endif

/************ low level data pushing commands **********/

// little wrapper for i/o writes
void Adafruit_RGBLCDShield::_digitalWrite(uint8_t p, uint8_t d)
{
	if (!connected_)
	{
		logger().error() << "_digitalWrite() : Adafruit_RGBLCDShield NOT CONNECTED !" << logs::end;
		return;
	}
	/*
	 if (_i2cAddr != 255)
	 {
	 // an i2c command
	 _i2c.digitalWrite(p, d);
	 }
	 else
	 {
	 // straightup IO
	 digitalWrite(p, d);
	 }*/
	_i2c.digitalWrite(p, d);
}

// Allows to set the backlight, if the LCD backpack is used
void Adafruit_RGBLCDShield::setBacklight(uint8_t status)
{
	if (!connected_)
	{
		logger().error() << "setBacklight() : Adafruit_RGBLCDShield NOT CONNECTED !" << logs::end;
		return;
	}
	mutex_.lock();
	// check if i2c or SPI
	_i2c.digitalWrite(8, ~(status >> 2) & 0x1);
	_i2c.digitalWrite(7, ~(status >> 1) & 0x1);
	_i2c.digitalWrite(6, ~status & 0x1);
	mutex_.unlock();
}

// little wrapper for i/o directions
void Adafruit_RGBLCDShield::_pinMode(uint8_t p, uint8_t d)
{
	if (!connected_)
	{
		logger().error() << "_pinMode() : Adafruit_RGBLCDShield NOT CONNECTED !" << logs::end;
		return;
	}
	/*
	 if (_i2cAddr != 255)
	 {
	 // an i2c command
	 _i2c.pinMode(p, d);
	 }
	 else
	 {
	 // straightup IO
	 pinMode(p, d);
	 }*/
	_i2c.pinMode(p, d);
}

// write either command or data, with automatic 4/8-bit selection
void Adafruit_RGBLCDShield::send(uint8_t value, uint8_t mode)
{
	_digitalWrite(_rs_pin, mode);

	// if there is a RW pin indicated, set it low to Write
	if (_rw_pin != 255)
	{
		_digitalWrite(_rw_pin, 0);    //LOW
	}

	if (_displayfunction & LCD_8BITMODE)
	{
		write8bits(value);
	}
	else
	{
		write4bits(value >> 4);
		write4bits(value);
	}
}

void Adafruit_RGBLCDShield::pulseEnable(void)
{
	mutex_.lock();
	_digitalWrite(_enable_pin, 0);    //LOW
	usleep(1);
	_digitalWrite(_enable_pin, 1);    //HIGH
	usleep(1);    // enable pulse must be >450ns
	_digitalWrite(_enable_pin, 0);    //LOW
	usleep(100);   // commands need > 37us to settle
	mutex_.unlock();
}

void Adafruit_RGBLCDShield::write4bits(uint8_t value)
{
	if (_i2cAddr != 255)
	{
		uint16_t out = 0;

		out = _i2c.readGPIOAB();

		// speed up for i2c since its sluggish
		for (int i = 0; i < 4; i++)
		{
			out &= ~(1 << _data_pins[i]);
			out |= ((value >> i) & 0x1) << _data_pins[i];
		}

		// make sure enable is low
		out &= ~(1 << _enable_pin);

		_i2c.writeGPIOAB(out);

		// pulse enable
		usleep(1);
		out |= (1 << _enable_pin);
		_i2c.writeGPIOAB(out);
		usleep(1);
		out &= ~(1 << _enable_pin);
		_i2c.writeGPIOAB(out);
		usleep(100);

	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			_pinMode(_data_pins[i], OUTPUT);
			_digitalWrite(_data_pins[i], (value >> i) & 0x01);
		}
		pulseEnable();
	}
}

void Adafruit_RGBLCDShield::write8bits(uint8_t value)
{
	for (int i = 0; i < 8; i++)
	{
		_pinMode(_data_pins[i], OUTPUT);
		_digitalWrite(_data_pins[i], (value >> i) & 0x01);
	}

	pulseEnable();
}

uint8_t Adafruit_RGBLCDShield::readButtons(void)
{
	if (!connected_)
	{
		logger().error() << "readButtons() : Adafruit_RGBLCDShield NOT CONNECTED !" << logs::end;
		return 0;
	}
	mutex_.lock();
	uint8_t reply = 0x1F;

	for (uint8_t i = 0; i < 5; i++)
	{
		reply &= ~((_i2c.digitalRead(_button_pins[i])) << i);
	}
	mutex_.unlock();
	return reply;
}
