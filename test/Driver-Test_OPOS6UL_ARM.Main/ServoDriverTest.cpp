/*!
 * \file
 * \brief Implémentation de la classe ServoDriverTest.
 */

#include "ServoDriverTest.hpp"


void test::ServoDriverTest::suite()
{
	this->firstTest();

}

void test::ServoDriverTest::firstTest()
{

	servodriver_->hold(0);

	this->assert(true, "OK");
}

