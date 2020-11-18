/*!
 * drivers...OPO
 */

#include "LedDriver.hpp"

#include <unistd.h>
#include <string>

#include "../Log/Logger.hpp"

ALedDriver * ALedDriver::create(std::string, int nb)
{
    return new LedDriver(nb);
}

LedDriver::LedDriver(int nb)
{
    //GPIO1_2 = (0 x 32) + 2 = 2

    nb_ = 8; //Force number of leds.

    // OPOS6UL: ((Bank number - 1) x 32) + pin number
    //Example : GPIO1_2 = (0 x 32) + 2 = 2
    //GPIO5_8 = (4 x 32) + 8 = 136
    //GPIO5_7 = (4 x 32) + 7 = 135
    //GPIO5_6 = (4 x 32) + 6 = 134
    //GPIO5_5 = (4 x 32) + 5 = 133
    //GPIO5_4 = (4 x 32) + 4 = 132
    //GPIO5_3 = (4 x 32) + 3 = 131
    //GPIO5_2 = (4 x 32) + 2 = 130
    //GPIO5_1 = (4 x 32) + 1 = 129
    gpio[0] = new AsGpio(136);
    gpio[1] = new AsGpio(135);
    gpio[2] = new AsGpio(134);
    gpio[3] = new AsGpio(133);
    gpio[4] = new AsGpio(132);
    gpio[5] = new AsGpio(131);
    gpio[6] = new AsGpio(130);
    gpio[7] = new AsGpio(129);

    for (int i = 0; i < 8; i++) {
        if (gpio[i] != NULL) {
            gpio[i]->setPinDirection((char *)"out");
            gpio[i]->setIrqMode((char *)"none");
            gpio[i]->setPinValue(0);
        }
    }
}

LedDriver::~LedDriver()
{
}

void LedDriver::setBit(int index, LedColor color)
{
    int c = 0;
    if (color != LED_OFF)
        c = 1;
    gpio[index]->setPinValue(c);
}

void LedDriver::setBytes(uint hex, LedColor color)
{
    for (int i = 0; i < nb_; i++) {
        if (((hex >> i) & 0x01) == 1) {
            setBit(i, color);
        } else {
            setBit(i, LED_OFF);
        }
    }
}
