/*!
 * \file
 * \brief Implémentation de la classe LedDriverTest.
 */

#include "LedDriverTest.hpp"

void test::LedDriverTest::suite()
{
	this->testSet();

}

void test::LedDriverTest::testSet()
{
	logger().debug() << "Starting..." << logs::end;

	this->assert(true, "OK");
}

