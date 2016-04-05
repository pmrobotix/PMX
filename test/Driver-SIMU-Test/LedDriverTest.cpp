/*!
 * \file
 * \brief Implémentation de la classe ActionManagerTest.
 */

#include "LedDriverTest.hpp"

#include "../../src/Common/Action/ActionManager.hpp"

void test::LedDriverTest::suite()
{
	this->testSet();

}

void test::LedDriverTest::testSet()
{
	logger().debug() << "Starting..." << logs::end;

	this->assert(true, "OK");
}

