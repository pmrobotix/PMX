//drivers...SIMU

#include "LcdShieldDriver.hpp"

#include <stddef.h>
#include <cstdint>
#include <string>

using namespace std;

ALcdShieldDriver * ALcdShieldDriver::create(std::string botId)
{
//	static LcdShieldDriver *instance = new LcdShieldDriver();
//	return instance;

	return new LcdShieldDriver();
}

LcdShieldDriver::LcdShieldDriver()
{

}

LcdShieldDriver::~LcdShieldDriver()
{
}

void LcdShieldDriver::clear()
{

}

void LcdShieldDriver::home()
{

}

void LcdShieldDriver::setBacklightOn()
{

}

void LcdShieldDriver::setBacklightOff()
{

}

void LcdShieldDriver::setCursor(uint8_t col, uint8_t row)
{

}

size_t LcdShieldDriver::write(uint8_t value)
{
	return 1;
}


void LcdShieldDriver::print_content_string(std::string str, int row, int col=1)
{

}

void LcdShieldDriver::print_content_integer(int value, int row, int col=1)
{

}
