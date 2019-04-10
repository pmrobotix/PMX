/*!
 * \file
 * \brief Implémentation de la classe AsservDriverTest.
 */

#include "AsservDriverTest.hpp"

#include <sys/types.h>
#include <unistd.h>

#include "../../src/Common/Utils/Chronometer.hpp"
#include "../../src/Log/Logger.hpp"

void test::AsservDriverTest::suite()
{
    //this->test();
    testcodeurs();
}

void test::AsservDriverTest::test()
{
    logger().info() << "AsservDriverTest::test() Go" << logs::end;

    long left = 0;
    long currentL = 0;
    long currentR = 0;
    long right = 0;
    asservdriver_->resetExternalEncoders();
    utils::Chronometer chrono("AsservDriverTest");
    chrono.start();

    asservdriver_->setMotorLeftPower(20, 0); //percent
    asservdriver_->setMotorRightPower(100, 0); //percent

    while (1) {

        currentL = asservdriver_->getLeftExternalEncoder();
        currentR = asservdriver_->getRightExternalEncoder();
        if ((currentR != right) || (currentL != left)) {
            logger().info() << "currentL = " << currentL << "   currentR = " << currentR << logs::end;
            left = currentL;
            right = currentR;
        }

        if (chrono.getElapsedTimeInSec() >= 1)
            break;
    }

    asservdriver_->stopMotorLeft();
    asservdriver_->stopMotorRight();
    sleep(1);
    asservdriver_->setMotorLeftPower(200, 1000); //1050 max
    asservdriver_->setMotorRightPower(1049, 1000); //1050 max
    sleep(3);
    asservdriver_->stopMotorLeft();
    asservdriver_->stopMotorRight();

    this->assert(true, "OK");
}

void test::AsservDriverTest::testcodeurs()
{
    logger().info() << "testcodeurs()..." << logs::end;
    int32_t lastCountD = 0, countD = 0;
    int32_t lastCountG = 0, countG = 0;

    utils::Chronometer chrono("AsservDriverTest::testcodeurs()");

    while (1) {
        chrono.start();

        long long t0 = chrono.getElapsedTimeInMicroSec();
        int32_t tempD = asservdriver_->getRightExternalEncoder();

        long long t1 = chrono.getElapsedTimeInMicroSec();

        int32_t tempG = asservdriver_->getLeftExternalEncoder();

        long long t2 = chrono.getElapsedTimeInMicroSec();

        countD = tempD - lastCountD;
        lastCountD = tempD;

        countG = tempG - lastCountG;
        lastCountG = tempG;

        long long t3 = chrono.getElapsedTimeInMicroSec();

        logger().info() << "t1=" << t1 - t0 << " t2-t1=" << t2 - t1 << " t3-t2=" << t3 - t2 << " countD=" << countD
                << " countG=" << countG << logs::end;

        sleep(1);
    }

}
