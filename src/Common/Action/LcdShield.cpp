#include "LcdShield.hpp"

#include "../Interface.Driver/ALcdShieldDriver.hpp"

using namespace std;

LcdShield::LcdShield(std::string botId, Actions & actions) :
        AActionsElement(actions), botId_(botId)
{
    lcdshielddriver_ = ALcdShieldDriver::create(botId);
    if(!(lcdshielddriver_->is_connected()))
    {
        logger().debug() << "lcdshielddriver_->is_connected() FALSE !" << logs::end;
    }
}

LcdShield::~LcdShield()
{
    delete lcdshielddriver_;
}

void LcdShield::clear()
{
    lcdshielddriver_->clear();
}

void LcdShield::home()
{
    lcdshielddriver_->home();
}
void LcdShield::setBacklightOn()
{
    lcdshielddriver_->setBacklightOn();
}
void LcdShield::setBacklightOff()
{
    lcdshielddriver_->setBacklightOff();
}

void LcdShield::setCursor(uint8_t col, uint8_t row)
{
    lcdshielddriver_->setCursor(col, row);
}

bool LcdShield::is_connected()
{
    return lcdshielddriver_->is_connected();
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
    return lcdshielddriver_->write(value);
}

void LcdShield::display_content_string(std::string str, int row, int col)
{
    lcdshielddriver_->print_content_string(str, row, col);
}

void LcdShield::display_content_integer(int value, int row, int col)
{
    lcdshielddriver_->print_content_integer(value, row, col);
}
