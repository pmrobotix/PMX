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
	leddriver->setBit(0, LED_GREEN);
	leddriver->setBit(1, LED_ORANGE);
	usleep(1000000);
	leddriver->setBit(0, LED_OFF);
	leddriver->setBit(1, LED_OFF);

	this->assert(true, "OK");
}

