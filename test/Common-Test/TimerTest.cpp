/*!
 * \file
 * \brief Implémentation de la classe TimerTest.
 */

#include "../../src/Common/Utils/Timer.hpp"
#include "TimerTest.hpp"

#include <unistd.h>


using namespace test;


test::TimerTest::TimerTest()
			: UnitTest("TimerTest"), timer_(100, 0, this)
	{
	}

void test::TimerTest::suite()
{

	gettime();

}

void test::TimerTest::gettime()
{


	startTimer();

	sleep(1);

	stopTimer();

	this->assert(true, "TimerTest 01");



}

void test::TimerTest::startTimer()
{
	timer_.start("TimerTest");
}

void test::TimerTest::stopTimer()
{
	timer_.stopTimer();
}

void test::TimerTest::onTimer(utils::Chronometer chrono)
{
	logger().info() << chrono.getElapsedTimeInMicroSec() << logs::end;
}

void test::TimerTest::onTimerEnd(utils::Chronometer chrono)
{

}
