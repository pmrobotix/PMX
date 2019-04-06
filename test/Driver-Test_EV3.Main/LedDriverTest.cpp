/*!
 * \file
 * \brief Implémentation de la classe LedDriverTest.
 */

#include "LedDriverTest.hpp"

#include <unistd.h>

void test::LedDriverTest::suite()
{
    this->testSet();

}

void test::LedDriverTest::testSet()
{

    leddriver_->setBit(0, LED_GREEN);
    leddriver_->setBit(1, LED_YELLOW);
    usleep(500000);
    leddriver_->setBit(0, LED_BLACK);
    leddriver_->setBit(1, LED_BLACK);
    usleep(500000);
    leddriver_->setBit(0, LED_ORANGE);
    leddriver_->setBit(1, LED_AMBER);
    usleep(500000);
    leddriver_->setBit(0, LED_OFF);
    leddriver_->setBit(1, LED_OFF);

    this->assert(true, "OK");
}

