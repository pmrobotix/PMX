/*!
 * \file
 * \brief Implémentation de la classe TimerFactoryTest.
 */

#include "TimerFactoryTest.hpp"

#include <unistd.h>

#include "../../src/Common/Action/TimerPosix.hpp"
#include "../../src/Common/Utils/Chronometer.hpp"
#include "../../src/Log/Logger.hpp"
#include "../../src/Thread/Thread.hpp"

using namespace utils;

void test::TimerFactoryTest::suite()
{
    this->test();

}

void test::TimerFactoryTest::test()
{
    logger().info() << "TimerFactoryTest::test()..." << logs::end;


    utils::Chronometer chrono("TimerFactoryTest");
    chrono.start();

    logger().info() << "create timers " << chrono.getElapsedTimeInMicroSec()<< logs::end;
    TimerPosix *timer1 = new TimerPosix("timer1");
    TimerPosix *timer2 = new TimerPosix("timer2");
    TimerPosix *timer3 = new TimerPosix("timer3");

    sleep(1);
    logger().info() << "start timers"  << chrono.getElapsedTimeInMicroSec()<< logs::end;
    timer1->start();
    utils::sleep_for_secs(3);

    timer2->start();
    utils::sleep_for_millis(200);
    timer3->start();
    logger().info() << "wait timers..."  << chrono.getElapsedTimeInMicroSec()<< logs::end;

    //utils::sleep_for_secs(10);
    while (true){
        utils::sleep_for_micros(200000);

        logger().info() << " - "  << chrono.getElapsedTimeInMicroSec()<< logs::end;
        if (chrono.getElapsedTimeInSec()> 10)
            break;
    }

}
