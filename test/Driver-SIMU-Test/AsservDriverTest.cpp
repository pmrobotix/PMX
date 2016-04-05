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

	asservdriver->enableHardRegulation(true);

	int power = 30;
	int timems = 3000;
	utils::Chronometer chrono;

	asservdriver->resetEncoder();
	long left = asservdriver->getLeftInternalEncoder();
	//long lastL = left;

	asservdriver->setMotorLeftPower(power, timems);
	chrono.start();
	while (chrono.getElapsedTimeInMilliSec() < timems + 2000)
	{
		left = asservdriver->getLeftInternalEncoder();
		logger().debug() << "left=" << left << " timems=" << chrono.getElapsedTimeInMilliSec() << logs::end;
	//	lastL = left;
		usleep(9000);
	}

	this->assert(true, "OK");
}

