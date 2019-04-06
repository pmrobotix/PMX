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
    this->test();

}

void test::AsservDriverTest::test()
{
    logger().info() << "AsservDriverTest::test() Go" << logs::end;

    long left = 0;
    long currentL = 0;
    long currentR = 0;
    long right = 0;
    asservdriver_->resetExternalEncoders();
    utils::Chronometer chrono;
    chrono.start();

    asservdriver_->setMotorLeftPower(20, 0); //percent
    asservdriver_->setMotorRightPower(100, 0);//percent

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
    asservdriver_->setMotorLeftPower(200, 1000);//1050 max
    asservdriver_->setMotorRightPower(1049, 1000); //1050 max
    sleep(3);
    asservdriver_->stopMotorLeft();
    asservdriver_->stopMotorRight();

    this->assert(true, "OK");
}

