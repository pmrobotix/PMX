//drivers...ARM

#include "LcdShieldDriver.hpp"

#include <stddef.h>
#include <cstdint>
#include <string>

#include "Adafruit_RGBLCDShield.hpp"

using namespace std;

ALcdShieldDriver * ALcdShieldDriver::create(std::string botId)
{
//	static LcdShieldDriver *instance = new LcdShieldDriver();
//	return instance;

    return new LcdShieldDriver();
}

LcdShieldDriver::LcdShieldDriver()
{
    Adafruit_RGBLCDShield::instance().clear();
}

LcdShieldDriver::~LcdShieldDriver()
{
}

void LcdShieldDriver::clear()
{
    Adafruit_RGBLCDShield::instance().clear();
}

void LcdShieldDriver::home()
{
    Adafruit_RGBLCDShield::instance().home();
}

void LcdShieldDriver::setBacklightOn()
{
    Adafruit_RGBLCDShield::instance().setBacklight(LCD_ON);
}

void LcdShieldDriver::setBacklightOff()
{
    Adafruit_RGBLCDShield::instance().setBacklight(LCD_OFF);
}

void LcdShieldDriver::setCursor(uint8_t col, uint8_t row)
{
    Adafruit_RGBLCDShield::instance().setCursor(col, row);
}

size_t LcdShieldDriver::write(uint8_t value)
{
    size_t s = Adafruit_RGBLCDShield::instance().write__(value);
    return s;
}



void LcdShieldDriver::print_content_string(std::string str, int row, int col)
{

}

void LcdShieldDriver::print_content_integer(int value, int row, int col)
{

}
