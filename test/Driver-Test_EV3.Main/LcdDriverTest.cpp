/*!
 * \file
 * \brief Implémentation de la classe LcdDriverTest.
 */

#include "LcdDriverTest.hpp"

#include <unistd.h>

#include "../../src/Log/Logger.hpp"

void test::LcdDriverTest::suite()
{
    this->testLCD();

}

void test::LcdDriverTest::testLCD()
{

    this->assert(true, "OK");
}

