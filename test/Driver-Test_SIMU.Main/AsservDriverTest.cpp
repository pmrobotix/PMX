/*!
 * \file
 * \brief Implémentation de la classe AsservDriverTest.
 */

#include "AsservDriverTest.hpp"

#include <unistd.h>

#include "../../src/Common/Utils/Chronometer.hpp"
#include "../../src/Log/Logger.hpp"

void test::AsservDriverTest::suite()
{
	this->testSet();

}

void test::AsservDriverTest::testSet()
{
	logger().debug() << "Starting..." << logs::end;

	asservdriver->motion_ActivateManager(true);

	int power = 800;
	int timems = 3000;
	utils::Chronometer chrono;

	asservdriver->resetEncoders();
	long left = asservdriver->getLeftInternalEncoder();
	long right = asservdriver->getRightInternalEncoder();

	asservdriver->setMotorLeftPower(power, timems);
	asservdriver->setMotorRightPower(power, timems);
	chrono.start();
	while (chrono.getElapsedTimeInMilliSec() < timems + 1000)
	{
		left = asservdriver->getLeftInternalEncoder();
		right = asservdriver->getRightInternalEncoder();
		logger().debug() << "left="
				<< left
				<< " right="
				<< right
				<< " timems="
				<< chrono.getElapsedTimeInMilliSec()
				<< logs::end;
		usleep(90000);
	}

	asservdriver->setMotorLeftPosition(power, 300);
	asservdriver->setMotorRightPosition(power, 300);
	chrono.start();
	while (chrono.getElapsedTimeInMilliSec() < timems + 2000)
	{
		left = asservdriver->getLeftInternalEncoder();
		right = asservdriver->getRightInternalEncoder();
		logger().debug() << "left="
				<< left
				<< " right="
				<< right
				<< " timems="
				<< chrono.getElapsedTimeInMilliSec()
				<< logs::end;
		usleep(90000);
	}

	this->assert(true, "OK");
}

