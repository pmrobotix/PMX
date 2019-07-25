/*!
 * \file
 * \brief Implémentation de la classe SensorDriverTest.
 */

#include "SensorDriverTest.hpp"

#include <unistd.h>
#include <cstdint>

void test::SensorDriverTest::suite()
{
	this->firstTest();

}

void test::SensorDriverTest::firstTest()
{

	sensordriver->frontLeft();

	this->assert(true, "OK");
}

