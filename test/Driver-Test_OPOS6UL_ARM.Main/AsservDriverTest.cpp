/*!
 * \file
 * \brief Implémentation de la classe AsservDriverTest.
 */

#include "AsservDriverTest.hpp"

#include <unistd.h>
#include <cstdint>

void test::AsservDriverTest::suite()
{
	this->firstTest();

}

void test::AsservDriverTest::firstTest()
{


	this->assert(true, "OK");
}

