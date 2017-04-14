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

	asservdriver->setMotorLeftPower(10,10);
	//usleep(2000);
	//asservdriver->setMotorLeftPower(0,0);
	this->assert(true, "OK");
}

