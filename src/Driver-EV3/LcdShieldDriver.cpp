//drivers...EV3

#include "LcdShieldDriver.hpp"

#include <unistd.h>
#include <string>

#include "../Log/Logger.hpp"
#include "Scaffolding.hpp"

using namespace std;

ALcdShieldDriver * ALcdShieldDriver::create(std::string botId)
{
//	static LcdShieldDriver *instance = new LcdShieldDriver();
//	return instance;

    return new LcdShieldDriver();
}

LcdShieldDriver::LcdShieldDriver()
{

    //sudo systemctl stop brickman
    //sudo systemctl start brickman
    lcd l;
    logger().debug() << "Resolution is " << l.resolution_x() << " x " << l.resolution_y() << ", " << l.bits_per_pixel()
            << " bit(s) per pixel" << "Frame buffer size is " << l.frame_buffer_size() << " byte, " << "line length is "
            << l.line_length() << " byte" << logs::end;

    l.fill(0xFF);
    sleep(1);
    l.fill(0x00);

    prog_init();

    prog_clearscreen();
    prog_display_string("TEST PMX");
    prog_display_integer(10);
    sleep(1);
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

