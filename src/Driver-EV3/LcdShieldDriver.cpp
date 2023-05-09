//drivers...EV3

#include "LcdShieldDriver.hpp"

#include "../Log/Logger.hpp"
#include "Scaffolding.hpp"
#include "ev3dev.h"

using namespace std;
using namespace ev3dev;

ALcdShieldDriver * ALcdShieldDriver::create(std::string botId)
{
    return new LcdShieldDriver();
}

LcdShieldDriver::LcdShieldDriver()
{

    //sudo systemctl stop brickman
    //sudo systemctl start brickman

    //fprintf(stderr, "is_FBConsole=%d\n", is_FBConsole);


    lcd l;
    logger().debug() << "Resolution is " << l.resolution_x() << " x " << l.resolution_y() << ", " << l.bits_per_pixel()
            << " bit(s) per pixel" << "Frame buffer size is " << l.frame_buffer_size() << " byte, " << "line length is "
            << l.line_length() << " byte" << logs::end;

    l.fill(0xFF);
    /*sleep(1);
    l.fill(0x00);
    sleep(1);
    l.fill(0xFF);
    //sleep(1);*/

    prog_init();
    prog_clearscreen();
}

LcdShieldDriver::~LcdShieldDriver()
{
    prog_exit();
}

bool LcdShieldDriver::is_connected()
{
    return true;
}
void LcdShieldDriver::clear()
{
    prog_clearscreen();
}

void LcdShieldDriver::home()
{
    prog_set_cursorpos(1, 1);
}

void LcdShieldDriver::setBacklightOn()
{
    prog_clearscreen();
}

void LcdShieldDriver::setBacklightOff()
{
    prog_clearscreen();
}

void LcdShieldDriver::setCursor(uint8_t col, uint8_t row)
{
    prog_set_cursorpos(col, row);
}

size_t LcdShieldDriver::write(uint8_t value)
{
    return 0;
}


void LcdShieldDriver::print_content_string(std::string str, int row, int col)
{
    prog_contentX(str.c_str(), row, col);
}

void LcdShieldDriver::print_content_integer(int value, int row, int col)
{
    prog_NumberX(value, row, col);
}

