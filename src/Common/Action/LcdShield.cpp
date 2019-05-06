#include "LcdShield.hpp"

#include "../Action.Driver/ALcdShieldDriver.hpp"

using namespace std;

LcdShield::LcdShield(std::string botId, Actions & actions) :
        AActionsElement(actions), botId_(botId)
{
    lcdshielddriver = ALcdShieldDriver::create(botId);
}

LcdShield::~LcdShield()
{
    delete lcdshielddriver;
}

void LcdShield::clear()
{
    lcdshielddriver->clear();
}

void LcdShield::home()
{
    lcdshielddriver->home();
}
void LcdShield::setBacklightOn()
{
    lcdshielddriver->setBacklightOn();
}
void LcdShield::setBacklightOff()
{
    lcdshielddriver->setBacklightOff();
}

void LcdShield::setCursor(uint8_t col, uint8_t row)
{
    lcdshielddriver->setCursor(col, row);
}

void LcdShield::init()
{
    clear();
    setBacklightOn();
}

void LcdShield::reset()
{
    clear();
    setCursor(0, 0);
}

//for Print
size_t LcdShield::write(uint8_t value)
{
    return lcdshielddriver->write(value);
}

void LcdShield::display_content_string(std::string str, int row, int col)
{
    lcdshielddriver->print_content_string(str, row, col);
}

void LcdShield::display_content_integer(int value, int row, int col=1)
{
    lcdshielddriver->print_content_integer(value, row, col);
}
