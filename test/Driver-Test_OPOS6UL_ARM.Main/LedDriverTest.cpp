/*!
 * \file
 * \brief Implémentation de la classe LedDriverTest.
 */

#include "LedDriverTest.hpp"

#include <unistd.h>
#include <cstdint>

void test::LedDriverTest::suite()
{
    this->firstTest();

}

void test::LedDriverTest::firstTest()
{

    for (int i = 0; i < 8; i++) {
        for (int n = 0; n < 2; n++) {
            leddriver->setBit(i, LED_GREEN);
            usleep(50000);
            leddriver->setBit(i, LED_OFF);
            usleep(50000);
        }
    }

    for (int n = 0; n < 4; n++) {
        leddriver->setBytes(0xAA, LED_GREEN);
        usleep(100000);
        leddriver->setBytes(0x55, LED_GREEN);
        usleep(100000);
    }
    leddriver->setBytes(0x00, LED_OFF);

    this->assert(true, "OK");
}

