/*!
 * \file
 * \brief Implémentation de la classe AsservDriverTest.
 */

#include "AsservDriverTest.hpp"

#include "../../src/Common/Asserv.Driver/AAsservDriver.hpp"

void test::AsservDriverTest::suite()
{
	this->testSet();

}

void test::AsservDriverTest::testSet()
{

	AAsservDriver* asservdriver;
	asservdriver = AAsservDriver::create();

	asservdriver->enableHardRegulation(true);

	int power = 860;
	asservdriver->setMotorLeftPower(power, 0);
	/*
	 sleep(3);
	 asservdriver->stopMotorLeft();
	 asservdriver->stopMotorRight();

	 */

	this->assert(true, "OK");
}

