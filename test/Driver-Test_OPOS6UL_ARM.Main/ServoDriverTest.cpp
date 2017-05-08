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

	servodriver->hold(SERVO_FUNNY_ACTION);

	this->assert(true, "OK");
}

